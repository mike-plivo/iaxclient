/*
* testcall: make a single test call with IAXCLIENT
*
* IAX Support for talking to Asterisk and other Gnophone clients
*
* Copyright (C) 1999, Linux Support Services, Inc.
*
* Mark Spencer <markster@linux-support.net>
* Stefano Falsetto <falsetto@gnu.org>
*
* This program is free software, distributed under the terms of
* the GNU Lesser (Library) General Public License
*/

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>
//#include <streams.h>

#include "iaxclient_lib.h"

#ifdef WIN32
// Only under windows...
#undef main
#endif

#include "video_grab.h"

int 	format, framerate, bitrate, width=320, height=240, fragsize=0;
char	*ofile = NULL;
FILE	*f = NULL;
int	nframes = 0, frame_count = 0;
long	start_time = 0;

char caption[80] = "";

SDL_mutex *mut;

static void *grabber;

long get_time_milli()
{
	struct timeval	time;

	if ( gettimeofday(&time, NULL) < 0 ) fprintf(stderr, "Cannot get system time");
	return time.tv_sec * 1000 + time.tv_usec / 1000;
}

/* routine called at exit to shutdown audio I/O and close nicely.
NOTE: If all this isnt done, the system doesnt not handle this
cleanly and has to be rebooted. What a pile of doo doo!! */
void killem(void)
{
	long	end_time;
	double	seconds;

	grabber_destroy(grabber);
	SDL_Quit();
	if ( f ) fclose(f);
	end_time = get_time_milli();
	seconds = (end_time - start_time) * 1.0 / 1000;
	printf("Grabbed %d frames in %.3f seconds, framerate = %.1f fps\n", frame_count, seconds, frame_count / seconds);
	exit(1);
}

void fatal_error(char *err) {
	killem();
	fprintf(stderr, "FATAL ERROR: %s\n", err);
	exit(1);
}

static SDL_Surface *window;
SDL_Overlay *videoolay;

int init_sdl()
{
	int InitCode;
	
	// Initialize SDL
#ifdef LINUX
	InitCode=SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_EVENTTHREAD;
#else
	InitCode=SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE;
#endif
	
	if(SDL_Init(InitCode) < 0)
	{
		fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
		return -1;
	}
	atexit(SDL_Quit);	
	
	// Setup window
	window = SDL_SetVideoMode(width,height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_RESIZABLE);
	if(window == NULL) 
	{
		fprintf(stderr, "Could not open window\n");
		return -1;
	}
	
	// Create YUV overlay
	videoolay = SDL_CreateYUVOverlay(width, height, SDL_IYUV_OVERLAY, window);
	if (videoolay == NULL )
	{
		fprintf(stderr, "Could not create YUV video overlay\n");
		return -1;
	}
	
	// Ignore most events
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
	
	// Create a mutex to guar access to the window
	mut=SDL_CreateMutex();
	
	return 0;
}

int display_video(void * buffer, long size)
{
	SDL_Rect rect  = {0, 0, width, height};
	
	// Lock mutex
	if(SDL_mutexP(mut)==-1) fatal_error("Couldn't lock mutex");
	
	SDL_LockYUVOverlay(videoolay);
	videoolay->pixels[0] = (Uint8 *)buffer;
	videoolay->pixels[1] = (Uint8 *)buffer  + (width * height);
	videoolay->pixels[2] = (Uint8 *)buffer  + (width * height * 5 / 4 );
	SDL_UnlockYUVOverlay(videoolay);
	SDL_DisplayYUVOverlay(videoolay, &rect);	
	
	// Unlock mutex
	if(SDL_mutexV(mut)==-1) fatal_error("Couldn't unlock mutex");
	
	return 0;
}

void grabber_callback(void *gc_data, double sampleTime, void *buffer, long size)
{
	frame_count++;
	printf("Frame %d\r", frame_count);
	fflush(stdout);
	
	if ( display_video(buffer, size) ) printf("Grabber_callback: cannot display video\n");
	if ( f != NULL )
	{
		if ( fwrite(buffer, 1, size, f) < size ) fatal_error("Error writing data to file\n");
		if ( --nframes <= 0 )
		{
			printf("Frame limit reached, exiting\n");
			killem();
		}
	}
}

int parse_args(int argc, char **argv)
{
	int	i;
	
	for ( i=0 ; i<argc ; i++ )
	{
		if ( strcmp(argv[i], "-w") == 0 )
		{
			width = atoi(argv[++i]);
		} else if ( strcmp(argv[i], "-h") == 0 )
		{
			height = atoi(argv[++i]);
		} else if ( strcmp(argv[i], "-of") == 0 )
		{
			ofile = argv[++i];
		} else if ( strcmp(argv[i], "-nf") == 0 )
		{
			nframes = atoi(argv[++i]);
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	void	*grabber;

	// Parse command line arguments (so far just -w and -h for width and height)
	parse_args(argc, argv);
	
	// Open output file (if requested)
	if ( ofile != NULL )
	{
		f = fopen(ofile, "w+");
		if ( f == NULL )
		{
			fprintf(stderr, "Cannot open output file %s\n", ofile);
			exit(1);
		}
		if ( nframes <= 0 )
		{
			fprintf(stderr, "Cannot save to a file without a frame limit\n");
			exit(1);
		}
	}
	// Initialize SDL
	if(init_sdl()) 
	{
		fprintf(stderr, "could not init SDL\n");
		return -1;
	}
		
	// Initialize video
	grabber = grabber_init(grabber_callback, NULL, width, height);
	if ( grabber == NULL ) 
	{
		printf("Cannot initialize grabber\n");
		return -1;
	} else
	{
		printf("Grabber initialized\n");
	}
	
	start_time = get_time_milli();

	// SDL main loop
	while ( 69 ) 
	{
		SDL_Event event;
		while (1) {
			SDL_WaitEvent(&event);
			if(event.type == SDL_QUIT) 
			{
				break;
			} else if(event.type == SDL_KEYDOWN) 
			{
				switch(event.key.keysym.sym) 
				{
				case SDLK_q:
					killem();
				default:
					fprintf(stderr,"Keystroke %c not handled!\n",event.key.keysym.sym);
					break;
				}
			}
		}
	}

	return 0;
}
