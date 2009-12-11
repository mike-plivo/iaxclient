
CFLAGS=-I../../lib -I../../lib/libiax/src -I../../lib/gsm/inc -I../../lib/portaudio/pa_common -I../../lib/portaudio/pablio -Iinclude

#debug
CFLAGS:= $(CFLAGS) -g

OBJS=	iax2slin.o

LIBS=	../../lib/libiaxclient.a

EXEEXT=

# glean the platform
# We will define this as either Linux, Win32, or MacOSX
ifneq (,$(findstring Linux,$(shell uname)))
OSTYPE=LINUX
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
SYSLIBS=-lpthread -lm
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

PROGS= iax2slin$(EXEEXT) 

all: makelib $(PROGS)

clean: 
	rm -f $(OBJS) $(PROGS)

makelib:
	(cd ../../lib; $(MAKE) libiaxclient.a)

../../lib/libiaxclient.a: 
	(cd ../../lib; $(MAKE) libiaxclient.a)

iax2slin$(EXEEXT): $(OBJS) $(LIBS)
	gcc $(OBJS) $(LIBS) $(SYSLIBS) -o iax2slin$(EXEEXT)


