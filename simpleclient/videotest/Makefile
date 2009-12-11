
#FF_AMR=1
#FF_AMR_WB=1
AMR=0
AMR_WB=0
USE_FFMPEG=0

CFLAGS= -I../../lib -I../../lib/videoLib -I../../lib/libiax2/src -I../../lib/gsm/inc -I../../lib/portaudio/pa_common -I../../lib/portaudio/pablio -Iinclude `sdl-config --cflags` -I../../lib/ogg/include -I../../lib/theora/include -I../../lib/SDL/include

# -I ./SDL_image-1.2.4/

#debug
CPPFLAGS:= $(CFLAGS) -g -Wall
CFLAGS:= $(CFLAGS) -g -Wall -ldl

OBJS= videotest.o


EXEEXT=

# glean the platform
# We will define this as either Linux, Win32, or MacOSX
ifneq (,$(findstring Linux,$(shell uname)))
OSTYPE=LINUX
CFLAGS:= $(CFLAGS) -DLINUX
else
ifneq (,$(findstring Darwin,$(shell uname)))
OSTYPE=MACOSX
else
# CYGWIN reports CYGWIN_NT-5.0 under Win2K
ifneq (,$(findstring WIN,$(shell uname)))
OSTYPE=WIN32
else
ifneq (,$(findstring MINGW,$(shell uname)))
OSTYPE=WIN32
else
ifneq (,$(findstring SunOS,$(shell uname)))
OSTYPE=SOLARIS
else
ifneq (,$(findstring FreeBSD,$(shell uname)))
OSTYPEREAL=FREEBSD
else
$(warning OSTYPE cannot be detected, assuming Linux)
endif
OSTYPE=LINUX
endif
endif
endif
endif
endif

VVIDEO_LIBS= -lraw1394 -ldc1394_control -lavcodec -logg ../../lib/libtheora/lib/.libs/libtheora.a `sdl-config --libs`

LIBS:= ../../lib/libiaxclient.a

ifeq ($(OSTYPE),LINUX)
 VLIBS:= $(LIBS) -logg ../../lib/libtheora/lib/.libs/libtheora.a `sdl-config --libs`
 #DYNVLIBS:= -L../../lib -L /home/jcarminati/iaxclientNewAvecVideo/lib/ ogg.a theora.a -lSDL `sdl-config --libs` -liaxclient
 DYNVLIBS:= ../../lib/libiaxclient.a -L../../lib -L./SDL_image-1.2.4/.libs/ `sdl-config --libs` -logg ../../lib/libtheora/lib/.libs/libtheora.a
 CFLAGS:= $(CFLAGS) `sdl-config --cflags`
endif
ifeq ($(OSTYPE),MACOSX)
 LIBS:= $(LIBS) -framework QuickTime -framework Carbon -framework AudioUnit ../../lib/libtheora/lib/.libs/libtheora.a -logg -L/sw/lib  `sdl-config --libs`
 CFLAGS:= $(CFLAGS) `sdl-config --cflags`
endif

ifeq ($(FF_AMR),1)
  CFLAGS:= $(CFLAGS) -Iffmpeg/amr_float -DFF_AMR
endif

ifeq ($(AMR),1)
  CFLAGS:= $(CFLAGS) -Iffmpeg/amr_float -DAMR
endif

ifeq ($(FF_AMR_WB),1)
  CFLAGS:= $(CFLAGS) -Iffmpeg/amrwb_float -DFF_AMR_WB
endif

ifeq ($(AMR_WB),1)
  CFLAGS:= $(CFLAGS) -Iffmpeg/amrwb_float -DAMR_WB
endif

ifeq ($(USE_FFMPEG),1)
	CFLAGS:= $(CFLAGS) -DUSE_FFMPEG
endif

ifeq ($(OSTYPEREAL),FREEBSD)
MAKE=gmake
else
MAKE=make
endif

ifeq ($(OSTYPE),WIN32)
EXEEXT=.exe
SYSLIBS=-lwinmm -lwsock32
endif

ifeq ($(OSTYPE),LINUX)
CFLAGS:= $(CFLAGS) -DPOSIXSLEEP
ifeq ($(OSTYPEREAL),FREEBSD)
SYSLIBS=-lc_r -lm
else
SYSLIBS=-lpthread -lm -lasound
endif
endif

ifeq ($(OSTYPE),MACOSX)
CFLAGS:= $(CFLAGS) -DPOSIXSLEEP
SYSLIBS=-framework CoreAudio -framework AudioToolbox
endif

ifeq ($(OSTYPE),SOLARIS)
CFLAGS:= $(CFLAGS) -DPOSIXSLEEP
SYSLIBS= -lsocket -lnsl -lpthread -lrt -lresolv -lm
MAKE=gmake
endif

PROGS= #videotest$(EXEEXT) #recvdata$(EXEEXT)
VPROGS= videotest$(EXEEXT) #recvdata$(EXEEXT)

DYNLIB = -L../../lib

all: makelib $(PROGS) $(VPROGS) $(SINGLE)

clean:
	rm -f $(OBJS) $(PROGS) $(VPROGS) $(SINGLE)

distclean: clean
	(cd ../../lib; $(MAKE) clean)

makelib:
	(cd ../../lib; $(MAKE))

videotest$(EXEEXT): makelib
	c++ videotest.c $(CFLAGS) $(CPPFLAGS) $(DYNVLIBS) $(LIBS) $(SYSLIBS) -o videotest$(EXEEXT)

