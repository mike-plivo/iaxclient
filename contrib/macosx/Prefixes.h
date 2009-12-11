
#define MACOSX 
#define LIBIAX 

//#define CODEC_ILBC 0
//#define SPAN_EC 0
#define SPEEX_EC 1



// ***************************************************************************
//		BEGIN CONFIGURABLE VARIABLES SECTION
// ***************************************************************************

// Choose wheter to compile iaxclient with ffmpeg library support or not
#define USE_FFMPEG 0
// Please fill in the path to ffmpeg's libavcodec source dir
//FFMPEG_SRCDIR=

// Mats hack for MacOSX 10.2.8; see jitterbuf.c
#define HAVE_NOT_VA_ARGS
// speex_echo.h :  use __restrict instead
#define HAVE_NOT_RESTRICT_KEYWORD

