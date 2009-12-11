/*
* vtestcall: make a single video test call with IAXCLIENT
*
* IAX Support for talking to Asterisk and other Gnophone clients
*
* Copyright (C) 1999, Linux Support Services, Inc.
*
* Mark Spencer <markster@linux-support.net>
* Stefano Falsetto <falsetto@gnu.org>
* Mihai Balea <mihai AT hates DOT ms>
*
* This program is free software, distributed under the terms of
* the GNU Lesser (Library) General Public License
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "iaxclient.h"

#include <SDL.h>

#ifdef WIN32
// Only under windows...
#undef main
#endif

#define MAX_CALLS 6

static int video = 0;

//int format = IAXC_FORMAT_THEORA | IAXC_FORMAT_SPEEX;
int format = IAXC_FORMAT_H263 | IAXC_FORMAT_H263_PLUS | IAXC_FORMAT_H264 | IAXC_FORMAT_MPEG4 | IAXC_FORMAT_THEORA;
int formatp = IAXC_FORMAT_THEORA;
int framerate = 15;
int bitrate = 200000;
int width = 320;
int height = 240;
int fragsize = 1400;

int preview = 1;

int call_established = 0;

// Forward declaration
int display_video(struct iaxc_ev_video v, int remote);
void process_text_message(char *message);

char caption[80] = "";

SDL_mutex *mut;

// Audio-cosmetic...
struct iaxc_sound sound_ringOUT, sound_ringIN;

/* routine called at exit to shutdown audio I/O and close nicely.
NOTE: If all this isnt done, the system doesnt not handle this
cleanly and has to be rebooted. What a pile of doo doo!! */
void killem(void)
{
	fprintf(stderr,"Calling iaxc_shutdown...");
	iaxc_shutdown();
	fprintf(stderr,"Done.\nCallig SDL_Quit...\n");
	SDL_DestroyMutex(mut);
	SDL_Quit();
	fprintf(stderr,"Done\nProgram terminated correctly.\n");
	exit(0);
}

void signal_handler(int signum)
{
	if ( signum == SIGTERM || signum == SIGINT ) 
	{
		killem();
		exit(0);
	}
}

void fatal_error(char *err) {
	killem();
	fprintf(stderr, "FATAL ERROR: %s\n", err);
	exit(1);
}

void mysleep(void)
{
	iaxc_millisleep(10);
}

int levels_callback(float input, float output) {
	//fprintf(stderr,"Input level: %f\nOutput level: %f\n",input,output);
	return 1;
}

int netstat_callback(struct iaxc_ev_netstats n) {
	static int i;
	if(i++%25 == 0)
		fprintf(stderr, "RTT\t"
		"Rjit\tRlos%%\tRlosC\tRpkts\tRdel\tRdrop\tRooo\t"
		"Ljit\tLlos%%\tLlosC\tLpkts\tLdel\tLdrop\tLooo\n");

	fprintf(stderr, "%d\t"
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

void hangup_and_exit(void)
{
	iaxc_dump_call();
	fprintf(stderr,"Dumped call\n");
	iaxc_millisleep(1000);
	fprintf(stderr,"Sleeped for 1000 msec\n");
	iaxc_stop_processing_thread();
	fprintf(stderr,"Stopped processing thread\n");
	killem();
}


void my_safe_caption(char* c) 
{
	if(SDL_mutexP(mut)==-1) 
	{
		fprintf(stderr, "Couldn't lock mutex");
		exit(-1);
	}

	SDL_WM_SetCaption(c, NULL);

	if(SDL_mutexV(mut)==-1) 
	{
		fprintf(stderr, "Couldn't unlock mutex");
		exit(-1);
	}

}

int state_callback(struct iaxc_ev_call_state s)
{
	printf("------------------------------------------------------------\n");
	printf("Call #%d state report\n",s.callNo);
	printf("State\t\t: \t %d\n",s.state);
	printf("Format\t\t: \t %d\n",s.format);
	printf("Video format\t: \t %d\n",s.vformat);
	printf("Remote\t\t: \t %s\n",s.remote);
	printf("Remote name\t: \t %s\n",s.remote_name);
	printf("Local\t\t: \t %s\n",s.local);
	printf("Local context\t: \t %s\n",s.local_context);
	printf("------------------------------------------------------------\n");

	// Finished the phase of handshaking for the call in entry
	if (s.state == (IAXC_CALL_STATE_ACTIVE | IAXC_CALL_STATE_COMPLETE)) 
	{
		iaxc_stop_sound(sound_ringIN.id);
		call_established = 1;
	}
	if (!(s.state & (IAXC_CALL_STATE_BUSY|IAXC_CALL_STATE_TRANSFER))) 
	{
		iaxc_stop_sound(sound_ringOUT.id);
	}
	if (s.state == (IAXC_CALL_STATE_ACTIVE|IAXC_CALL_STATE_RINGING)) 
	{
		//iaxc_play_sound(&sound_ringIN, 1 /* ring device */ );
		//to_answer=s.callNo;
		fprintf(stderr,"Auto-Answering to caller %s on line %d...\n",s.remote,s.callNo);
		sprintf(caption,"Auto-Answering to caller %s on line %d...\n",s.remote,s.callNo);
		my_safe_caption(caption);
		fprintf(stderr, "Mihai: answer call vformat=0x%x\n", s.vformat);
		//iaxc_unquelch(s.callNo);
		iaxc_millisleep(1000);
		iaxc_answer_call(s.callNo);
		iaxc_select_call(s.callNo);
		call_established = 1;
		//iaxc_millisleep(1000);
		return 0;
	}
	if (s.state == IAXC_CALL_STATE_FREE) 
	{
		fprintf(stderr,"Disconnect from other end\n");
		hangup_and_exit();
	}

	return 0;
}

int iaxc_callback(iaxc_event e)
{
	//printf("Received iaxc event type 0x%x\n", e.type);
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
		return state_callback(e.ev.call);
	case IAXC_EVENT_VIDEO:
		if ( !video )
			return 0;

		if ( !e.ev.video.encoded )
			return display_video(e.ev.video,
					e.ev.video.source == IAXC_SOURCE_REMOTE);
		else
			fprintf(stderr, "We cannot handle encoded video in callbacks yet\n");
		break;

	case IAXC_EVENT_AUDIO:
		fprintf(stderr, "Got %s %s audio\n",
				e.ev.audio.source == IAXC_SOURCE_REMOTE ? "remote" : "local",
				e.ev.audio.encoded ? "encoded" : "raw");
		break;
	case IAXC_EVENT_VIDCAP_ERROR:
		fprintf(stderr, "\nVIDEO CAPTURE DEVICE ERROR\n");
		break;
	default:
		break;
	}

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
		fprintf(stderr, "Text message received: %s\n", message);
}

void list_devices()
{
	struct iaxc_audio_device *devs;
	int nDevs, input, output,ring;
	int i;

	int vinput;
	int nVdevs;
	struct iaxc_video_device *vDevs;

	iaxc_audio_devices_get(&devs,&nDevs, &input, &output, &ring);
	printf("\nThere are %d audio devices:\n", nDevs);
	for(i=0;i<nDevs;i++) 
	{
		fprintf(stderr, "AUDIO DEVICE ID = %d   NAME = %s   CAPS=%lx\n",
				devs[i].devID, devs[i].name, devs[i].capabilities);
		iaxc_audio_devices_set(input,output,ring);
	}

	iaxc_video_devices_get(&vDevs, &nVdevs, &vinput);
	printf("\nThere are %d video capture devices:\n", nVdevs);
	for ( i = 0; i < nVdevs; i++ )
	{
		printf("VIDEO DEVICE ID = %d   NAME = %s\n",
				vDevs[i].id, vDevs[i].name);
	}
	printf("Currently selected VIDEO device id: %d\n", vinput);

}

void usage()
{ 
	printf(
		"\n"
		"Usage is: tescall <options>\n\n"
		"available options:\n"
		"-F <codec,framerate,bitrate,width,height,fragsize> set video parameters\n"
		"-r <user password host> register to server host, with credentials user and password\n"
		"-s set silence threshold\n"
		"\n"
		);

	exit(1);
}

static SDL_Surface *window;
SDL_Overlay *rolay;
SDL_Overlay *lolay;
unsigned char	*remote_data = NULL;
unsigned char	*local_data = NULL;

int init_sdl() 
{
	int InitCode;

#ifdef LINUX
	InitCode=SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_EVENTTHREAD;;
#else
	InitCode=SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE;
#endif

	if( SDL_Init(InitCode) < 0 ) 
	{
		fprintf(stderr, "could not init SDL: %s\n", SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);
	
	// XXX check these flags, do right thing with format, watch changes, etc.
	window = SDL_SetVideoMode(width,height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_RESIZABLE);
	if( !window ) 
	{
		fprintf(stderr, "could not open one of the output windows\n");
		return -1;
	}

	rolay = SDL_CreateYUVOverlay(width, height, SDL_IYUV_OVERLAY, window);
	if ( !rolay )
	{
		fprintf(stderr, "could not create Remote video overlay");
		return -1;
	}
	lolay = SDL_CreateYUVOverlay(width, height, SDL_IYUV_OVERLAY, window); 
	if( !lolay ) 
	{
		fprintf(stderr, "could not create Local video overlay\n");
		return -1;
	}
	
	// Ignore most events
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
	mut=SDL_CreateMutex();

	return 0;
}

int display_video(struct iaxc_ev_video v, int remote) 
{
	SDL_Rect Rrect = {v.width/2, v.height/2, v.width/2, v.height/2};
	SDL_Rect rect  = {0, 0, v.width, v.height};

	if( SDL_mutexP(mut) == -1 ) 
	{
		fprintf(stderr, "Couldn't lock mutex");
		exit(-1);
	}
	
	if ( v.size <= 0 )
	{
		fprintf(stderr, "WARNING: size %d in callback\n", v.size);
		return 0;
	}
	
	if ( !remote ) 
	{
		// Local video
		// Make a local copy of the video data
		if ( local_data != NULL ) 
		{
			free(local_data);
			local_data = NULL;
		}
		local_data = (unsigned char *)malloc(v.size);
		memcpy(local_data, v.data, v.size);

		SDL_LockYUVOverlay(lolay);
		lolay->pixels[0] = local_data;
		lolay->pixels[1] = local_data  + (v.width * v.height);
		lolay->pixels[2] = local_data  + (v.width * v.height * 5 / 4 );
		SDL_UnlockYUVOverlay(lolay);

		if (remote_data != NULL)
		{
			SDL_DisplayYUVOverlay(rolay, &rect);
			if (preview)
				SDL_DisplayYUVOverlay(lolay, &Rrect);
		}
		else
			SDL_DisplayYUVOverlay(lolay, &Rrect);

	} else 
	{
		// Remote video
		// Make a local copy of the video data
		if ( remote_data != NULL ) 
		{
			free(remote_data);
			remote_data = NULL;
		}
		remote_data = (unsigned char *)malloc(v.size);
		memcpy(remote_data, v.data, v.size);
		
		if ( v.callNo < 0 || v.callNo > MAX_CALLS ) return -1; // Sanity check
		SDL_LockYUVOverlay(rolay);
		rolay->pixels[0] = remote_data;
		rolay->pixels[1] = remote_data + (v.width * v.height);
		rolay->pixels[2] = remote_data + (v.width * v.height * 5/4);
		SDL_UnlockYUVOverlay(rolay);
		SDL_DisplayYUVOverlay(rolay, &rect);

		// Display the current local frame as well, to minimize flicker
		if (preview)
			SDL_DisplayYUVOverlay(lolay, &Rrect);
	}

	if( SDL_mutexV(mut) == -1 ) 
	{
		fprintf(stderr, "Couldn't unlock mutex");
		exit(-1);
	}
	return 0;
}

int main(int argc, char **argv)
{
	FILE	*f = stdout;
	char	c;
	int	i; //,OutLoop=0;
	char	mydest[80], *dest = NULL;
	double	silence_threshold = -99;
	int	jbypass = 0;
	int callNo = 0;
	
	/* install signal handler to catch CRTL-Cs */
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
	/* Parse command line */
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] == '-') 
		{
			switch(argv[i][1])
			{
			case 'F': /* set video formats */
				{
					formatp = 1<<atoi(argv[++i]);
					framerate = atoi(argv[++i]);
					bitrate = atoi(argv[++i]);
					width = atoi(argv[++i]);
					height = atoi(argv[++i]);
					fragsize = atoi(argv[++i]);
				}
				break;
			case 'P': /* */
				preview = 0;
				break;
			case 's':
				if(i+1 >= argc) usage();
				silence_threshold = atof(argv[++i]);
				break;
			case 'r':
				if(i+3 >= argc) usage();
				iaxc_register(argv[i+1],argv[i+2],argv[i+3]);
				i+=3;
				break;
			default:
				usage();
			}
		} else 
			dest=argv[i];
	}
	
	if ( width<128 || height<96 ) 
	{
		fprintf(stderr,"Frame is too small\n");
		usage();
	}
	
	if ( width%2 || height%2 ) 
	{
		fprintf(stderr,"Frame width and height must be multiple of 2\n");
		usage();
	}	

	// To receive calls...
	iaxc_video_format_set(formatp, format, framerate, bitrate, width, height, fragsize);

	fprintf(stderr,">>> Forcing video mode!\n");
	video=1;

	if ( video ) 
	{
		if( init_sdl() ) 
		{
			fprintf(stderr, "could not init SDL\n");
			return -1;
		}
	}

	if (iaxc_initialize(MAX_CALLS)) fatal_error("cannot initialize iaxclient!");
	
	// ERA DEFAULT: 
	iaxc_set_formats(IAXC_FORMAT_SPEEX, IAXC_FORMAT_SPEEX);
	//iaxc_set_formats(IAXC_FORMAT_ALAW,IAXC_FORMAT_ULAW|IAXC_FORMAT_GSM);
	iaxc_set_silence_threshold(1.0);
	iaxc_set_audio_output(0);
	iaxc_set_filters(/*IAXC_FILTER_AGC|*/IAXC_FILTER_DENOISE|IAXC_FILTER_CN/*|IAXC_FILTER_ECHO*/);

	list_devices();

	iaxc_set_event_callback(iaxc_callback); 
	
	// Video should go through the jitter buffer
	iaxc_video_bypass_jitter(0);
	
	// Set what kind of video data we want to receive through callbacks
	// See iaxclient.h for possible values
	iaxc_set_video_prefs(IAXC_VIDEO_PREF_RECV_LOCAL_RAW |
			IAXC_VIDEO_PREF_RECV_REMOTE_RAW);

	// Set what kind of audio data we want to receive through callbacks
	// See iaxclient.h for possible values
	//iaxc_set_audio_prefs(IAXC_AUDIO_PREF_RECV_LOCAL_RAW | IAXC_AUDIO_PREF_RECV_REMOTE_RAW);

	fprintf(f, "\n\
			   vtestcall accepts keyboard input while it's running.\n\
			   You must hit 'enter' for your keypresses to be recognized,\n\
			   although you can type more than one key on a line:\n\
			   \n\
			   s: switch video capture devices \n\
			   b: bypass video jitter stuff\n\
			   r: reject incoming call\n\
			   d: dial\n\
			   c: set caller id info\n\
			   w: video window toggle\n\
			   0-9 * or #: dial those DTMF digits\n\
			   q: drop the call and hangup\n");
	
	printf("Starting processing thread...\n");
	iaxc_start_processing_thread();

	if ( dest != NULL )
	{
		sprintf(caption, "Calling to %s", dest);
		fprintf(stderr, "Calling to %s\n", dest);
		my_safe_caption(caption);
		callNo = iaxc_call(dest);
		if (callNo <= 0)
			iaxc_select_call(callNo);
		else
			fprintf(stderr, "Failed to make call to '%s'", dest);
	}

	/* process keyboard input received by the video window */
	printf("ready for keyboard input\n");
	for(;;) {
		SDL_Event event;
		while (1) {
			SDL_WaitEvent(&event);
			if(event.type == SDL_QUIT) {
				break;
			} else if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
				case SDLK_a:
					printf("Answering to incoming call...\n");
					break;
				case SDLK_b:
					jbypass=(jbypass+1) % 2;
					printf("New bypass video jitter buffer mode=%d\n",jbypass);
					iaxc_video_bypass_jitter(jbypass);
					break;
				case SDLK_q:
					printf("Hanging up and exiting\n");
					hangup_and_exit();
					break;
				case SDLK_r:
					printf("Rejecting incoming call...\n");
					iaxc_reject_call();
					iaxc_dump_call();
					break;
				case SDLK_h:
					printf("Hanging up selected call...\n");
					iaxc_dump_call();
					break;
				case SDLK_d:
					printf("Select type of call (Audio only or audio/Video): ");
					fflush(stdin);
					// TODO: Better control on inserted strings
					fscanf(stdin,"%s",mydest);

					fprintf(stderr,"format_set a %d,%d,%d,%d,%d,%d,%d\n",formatp, format, framerate, bitrate, width, height, fragsize);
					iaxc_video_format_set(formatp, format, framerate, bitrate, width, height, fragsize);

					/*
					if (iaxc_initialize(Vmode|Amode,MAX_CALLS))
					fatal_error("cannot initialize iaxclient!");
					*/
					printf("Insert address to dial to in `%s' type call: ",mydest[0]=='A'?"A only":"A/V");
					fscanf(stdin,"%s",mydest);
					fprintf(stderr,"\n --> Calling to %s\n",mydest);
					sprintf(caption,"Calling to %s",mydest);
					my_safe_caption(caption);
					//iaxc_play_sound(&sound_ringOUT, 1 /* ring device */ );
					// Force to have a A or A/V call
					iaxc_call_ex(mydest, NULL, NULL, (mydest[0]!='A'));
					break;
				case SDLK_c:
					{
						char myCIDname[80];
						char myCIDnumber[80];

						printf("Insert caller name: ");
						fscanf(stdin,"%s",myCIDname);
						printf("Insert caller ID number: ");
						fscanf(stdin,"%s",myCIDnumber);
						iaxc_set_callerid(myCIDname,myCIDnumber);
					}
					break;
				case SDLK_s:
					{
						int input;
						int ndevs;
						struct iaxc_video_device *vDevs;
						int newVideoDevId;

						iaxc_video_devices_get(&vDevs, &ndevs, &input);

						printf("There are %d video capture devices:\n", ndevs);
						for ( i = 0; i < ndevs; i++ )
						{
							printf("VIDEO DEVICE ID = %d   NAME = %s\n", vDevs[i].id, vDevs[i].name);
						}
						printf("Currently selected device id: %d\n", input);

						printf("Select video capture device: ");
						fflush(stdin);
						fscanf(stdin,"%d", &newVideoDevId);

						if ( iaxc_video_device_set(newVideoDevId) )
						{
							printf("Error selecting device id %d\n",
									newVideoDevId);
							break;
						}

						/* explicitly set the prefs again in case
						 * previous capture device has failed
						 */
						iaxc_set_video_prefs( iaxc_get_video_prefs() );
					}
					break;
				case SDLK_t: /* transmit-only */
					printf("transmit mode active\n");
					break;    
				case SDLK_w: /* off */
					{
						unsigned int prefs;

						prefs = iaxc_get_video_prefs();

						if ( prefs & IAXC_VIDEO_PREF_CAPTURE_DISABLE )
						{
							prefs &= ~IAXC_VIDEO_PREF_CAPTURE_DISABLE;
							printf("video switched on\n");
						}
						else
						{
							prefs |= IAXC_VIDEO_PREF_CAPTURE_DISABLE;
							printf("video switched off\n");
						}
						iaxc_set_video_prefs(prefs);
					}
					break;    
				#if 1
				case '1': case '2': case '3': case '4': case '5':
				case '6': case '7': case '8': case '9': case '0':
				case '#': case '*':
					c=event.key.keysym.sym;
					printf (">>>>>>>>>> sending %c\n", c);
					iaxc_send_dtmf(c);
					break;
				#endif
				default:
					fprintf(stderr,"Keystroke %c not handled!\n",event.key.keysym.sym);
					break;
				}
			}
		}
	}

	return 0;
}
