
EXEEXT=.exe

CFLAGS=-I../../lib -I../../lib/libiax2/src -I../../lib/gsm/inc -Iinclude

OBJS=	simpleiax.o

LIBS=	../../lib/libiaxclient.a


all: simpleiax$(EXEEXT)

../../lib/libiaxclient.a:
	(cd ../../lib; make libiaxclient.a)

clean:
	rm -f *.o simpleiax.exe

simpleiax.exe: $(OBJS) $(LIBS)
	gcc $(OBJS) $(LIBS) -lwinmm -lwsock32 -o simpleiax$(EXEEXT)

call8065.exe: call8065.o $(LIBS)
	gcc call8065.o $(LIBS) -lwinmm -lwsock32 -o call8065$(EXEEXT)
