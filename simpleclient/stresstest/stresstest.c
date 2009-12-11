/*
* stresstest: simple program for applying IAX2 protocol stress to asterisk.
*
* Copyrights:
* Copyright (C) 2007, Wimba, Inc.
*
* Contributors:
* Mihai Balea <mihai AT hates DOT ms>
* Peter Grayson <jpgrayson@gmail.com>
*
* This program is free software, distributed under the terms of
* the GNU Lesser (Library) General Public License.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "iaxclient.h"
#include "file.h"

#ifdef WIN32
// Only under windows...
#undef main
#endif

#define MAX_CALLS 1

#define TEST_OK            0
#define TEST_NO_CONNECTION -1
#define TEST_NO_MEDIA      -2
#define TEST_UNKNOWN_ERROR -99

static const int format =
		IAXC_FORMAT_H263 |
		IAXC_FORMAT_H263_PLUS |
		IAXC_FORMAT_H264 |
		IAXC_FORMAT_MPEG4 |
		IAXC_FORMAT_THEORA;
static int formatp = IAXC_FORMAT_THEORA;
static int framerate = 15;
static int bitrate = 200000;
static int width = 320;
static int height = 240;
static int fragsize = 1400;

static int call_established = 0;
static int running = 0;

static int send_video = 1;
static int send_audio = 1;
static int print_netstats = 0;
static int call_timeout_ms = 0;
static int connect_timeout_ms = 5000;
static int video_frames_count = 0;
static int audio_frames_count = 0;

static struct timeval start_time;

static FILE * log_file = 0;

#ifdef __GNUC__
void mylog(const char * fmt, ...) __attribute__ ((format (printf, 1, 2)));
#endif

void mylog(const char * fmt, ...)
{
	va_list ap;
	time_t t;
	struct tm * tmp;
	char str[1024];
	char time_str[1024];

	t = time(0);
	tmp = localtime(&t);

	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tmp);
	snprintf(str, sizeof(str), "stresstest: %s: %s", time_str, fmt);

	va_start(ap, fmt);
	vfprintf(log_file ? log_file : stderr, str, ap);
	va_end(ap);
}

/* routine used to shutdown and close nicely.*/
void hangup_and_exit(int code)
{
	mylog("Dump call\n");
	iaxc_dump_call();
	mylog("Sleep for 500 msec\n");
	iaxc_millisleep(500);
	mylog("Stop processing thread\n");
	iaxc_stop_processing_thread();
	mylog("Calling iaxc_shutdown...\n");
	iaxc_shutdown();
	mylog("Exiting with code %d\n", code);
	exit(code);
}

void signal_handler(int signum)
{
	if ( signum == SIGTERM || signum == SIGINT )
	{
		running = 0;
	}
}

void fatal_error(char *err)
{
	mylog("FATAL ERROR: %s\n", err);
	exit(TEST_UNKNOWN_ERROR);
}

int levels_callback(float input, float output) {
	//mylog("Input level: %f\nOutput level: %f\n", input, output);
	return 1;
}

int netstat_callback(struct iaxc_ev_netstats n) {
	static int i;

	if ( !print_netstats )
		return 0;

	if ( i++ % 25 == 0 )
		mylog("RTT\t"
		"Rjit\tRlos%%\tRlosC\tRpkts\tRdel\tRdrop\tRooo\t"
		"Ljit\tLlos%%\tLlosC\tLpkts\tLdel\tLdrop\tLooo\n");

	mylog("%d\t"
		"%d\t%d\t%d\t%d\t%d\t%d\t%d\t"
		"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",

		n.rtt,

		n.remote.jitter,
		n.remote.losspct,
		n.remote.losscnt,
		n.remote.packets,
		n.remote.delay,
		n.remote.dropped,
		n.remote.ooo,

		n.local.jitter,
		n.local.losspct,
		n.local.losscnt,
		n.local.packets,
		n.local.delay,
		n.local.dropped,
		n.local.ooo
		);

	return 0;
}

void process_text_message(char *message)
{
	unsigned int prefs;

	if ( strncmp(message, "CONTROL:", strlen("CONTROL:")) == 0 )
	{
		message += strlen("CONTROL:");
		if ( strcmp(message, "STOPVIDEO") == 0 )
		{
			// Stop sending video
			prefs = iaxc_get_video_prefs();
			prefs = prefs | IAXC_VIDEO_PREF_SEND_DISABLE ;
			iaxc_set_video_prefs(prefs);
		} else if ( strcmp(message, "STARTVIDEO") == 0 )
		{
			// Start sending video
			prefs = iaxc_get_video_prefs();
			prefs = prefs & ~IAXC_VIDEO_PREF_SEND_DISABLE ;
			iaxc_set_video_prefs(prefs);
		}
	} else
		mylog("Text message received: %s\n", message);
}

void usage()
{
	printf("Usage: stresstest <options>\n\n"
		"available options:\n"
		"  -F <codec> <framerate> <bitrate> <width> <height> <fragsize> set video parameters\n"
		"  -o <filename> media file to run\n"
		"  -v stop sending video\n"
		"  -a stop sending audio\n"
		"  -l run file in a loop\n"
		"  -n dump periodic netstats to log file\n"
		"  -t <TIMEOUT> terminate call after TIMEOUT seconds\n"
		"  -c <TIMEOUT> try connecting for TIMEOUT seconds (default 5)\n"
		"  -L <FILE> log to FILE\n"
		"\n"
		);
	exit(1);
}

int test_mode_state_callback(struct iaxc_ev_call_state s)
{
	mylog("Call #%d state %d\n", s.callNo, s.state);

	if ( s.state & IAXC_CALL_STATE_COMPLETE )
	{
		mylog("Call answered\n");
		call_established = 1;
	}
	if (s.state == IAXC_CALL_STATE_FREE)
	{
		mylog("Call terminated\n");
		running = 0;
	}

	return 0;
}

int test_mode_callback(iaxc_event e)
{
	switch ( e.type )
	{
	case IAXC_EVENT_LEVELS:
		return levels_callback(e.ev.levels.input, e.ev.levels.output);
	case IAXC_EVENT_NETSTAT:
		return netstat_callback(e.ev.netstats);
	case IAXC_EVENT_TEXT:
		process_text_message(e.ev.text.message);
		break;
	case IAXC_EVENT_STATE:
		return test_mode_state_callback(e.ev.call);
	case IAXC_EVENT_VIDEO:
		video_frames_count++;
		break;
	case IAXC_EVENT_AUDIO:
		audio_frames_count++;
		break;
	default:
		break;
	}

	return 0;
}

long msecdiff(struct timeval *t0, struct timeval *t1)
{
	return (t1->tv_sec - t0->tv_sec) * 1000L + (t1->tv_usec - t0->tv_usec) / 1000L;
}

int main(int argc, char **argv)
{
	int i;
	char *dest = NULL;
	char *ogg_file = NULL;
	int loop = 0;

	/* install signal handler to catch CRTL-Cs */
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	/* Parse command line */
	for ( i = 1; i < argc; i++)
	{
		if ( argv[i][0] == '-' )
		{
			switch ( argv[i][1] )
			{
			case 'F': /* set video params */
				if ( i+6 >= argc )
					usage();
				formatp = 1 << atoi(argv[++i]);
				framerate = atoi(argv[++i]);
				bitrate = atoi(argv[++i]);
				width = atoi(argv[++i]);
				height = atoi(argv[++i]);
				fragsize = atoi(argv[++i]);
				break;
			case 'o':
				if ( i+1 >= argc )
					usage();
				ogg_file = argv[++i];
				break;
			case 'v':
				send_video = 0;
				break;
			case 'a':
				send_audio = 0;
				break;
			case 'l':
				loop = 1;
				break;
			case 'n':
				print_netstats = 1;
				break;
			case 't':
				if ( i+1 >= argc )
					usage();
				call_timeout_ms = 1000 * atoi(argv[++i]);
				break;
			case 'c':
				if ( i+1 >= argc )
					usage();
				connect_timeout_ms = 1000 * atoi(argv[++i]);
				break;
			case 'L':
				if ( i+1 >= argc )
					usage();
				if ( !(log_file = fopen(argv[++i], "w")) )
				{
					mylog("failed to open log \"%s\"\n",
							argv[i]);
					exit(1);
				}
				break;
			default:
				usage();
			}
		}
		else
		{
			dest = argv[i];
		}
	}

	if ( dest == NULL )
	{
		mylog("No destination, quitting\n");
		return -1;
	}

	if ( ogg_file )
		load_ogg_file(ogg_file);
	else
		mylog("No media file, running dry\n");

	// Get start time for timeouts
	gettimeofday(&start_time, NULL);

	// Initialize iaxclient
	iaxc_video_format_set(formatp, format, framerate, bitrate,
			width, height, fragsize);
	iaxc_set_test_mode(1);

	if (iaxc_initialize(MAX_CALLS))
		fatal_error("cannot initialize iaxclient!");

	iaxc_set_formats(IAXC_FORMAT_SPEEX, IAXC_FORMAT_SPEEX);
	iaxc_video_bypass_jitter(0);
	iaxc_set_audio_prefs(IAXC_AUDIO_PREF_RECV_REMOTE_ENCODED);
	iaxc_set_video_prefs(IAXC_VIDEO_PREF_RECV_REMOTE_ENCODED);
	iaxc_set_event_callback(test_mode_callback);

	// Crank the engine
	if ( iaxc_start_processing_thread() < 0 )
		fatal_error("failed iaxc_start_processing_thread()\n");

	// Dial out
	if ( iaxc_call(dest) < 0 )
		fatal_error("failed iaxc_call()");

	// Wait for the call to be established;
	while ( !call_established && running )
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		if ( connect_timeout_ms > 0 &&
				msecdiff(&start_time, &now) > connect_timeout_ms )
			hangup_and_exit(TEST_NO_CONNECTION);
		iaxc_millisleep(5);
	}

	running = 1;
	while ( running )
	{
		struct timeval now;

		// We only need this if we actually want to send something
		if ( ogg_file && ( send_audio || send_video ) )
		{
			ogg_packet *op;

			op = get_next_audio_op();
			if ( !loop && audio_is_eos() )
				break;
			if ( send_audio && op != NULL && op->bytes > 0 )
				iaxc_push_audio(op->packet, op->bytes,
						SPEEX_SAMPLING_RATE *
						SPEEX_FRAME_DURATION / 1000);

			op = get_next_video_op();
			if ( !loop && video_is_eos() )
				break;
			if ( send_video && op != NULL && op->bytes > 0 )
				iaxc_push_video(op->packet, op->bytes, 1);
		}

		// Tight spinloops are bad, mmmkay?
		iaxc_millisleep(5);

		// Exit after a positive timeout
		gettimeofday(&now, NULL);
		if ( call_timeout_ms > 0 &&
				msecdiff(&start_time, &now) > call_timeout_ms )
			running = 0;
	}

	mylog("Received %d audio frames and %d video frames\n",
			audio_frames_count, video_frames_count);

	if ( audio_frames_count == 0 && video_frames_count == 0 )
		hangup_and_exit(TEST_NO_MEDIA);
	else
		hangup_and_exit(TEST_OK);
	return 0;
}
