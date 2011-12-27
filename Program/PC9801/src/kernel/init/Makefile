# $Revision: 1.1 $
#
#

IDENT=-DTIMER_TEST -DI386
DEBUG=-DTSKSW_DEBUG

PATH=.:/bin:/usr/bin:../../tool/kp
CPP=/lib/cpp
ROOT=../itron-3.0
CFLAGS=-b i486-linuxaout -I. -I$(ROOT) -I../kernlib -I../itron-3.0/h -I../itron-3.0/i386 -fno-builtin $(IDENT)
CC=gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x00001000
ENTRY=_start
LIBS=dosfs/libdosfs.a lib/libs.a ../kernlib/libkernel.a
LD=/usr/local/bin/ld

%.o: %.c
	kp -input-code euc -input $< -output kptmp.c
	$(CC) $(CFLAGS) -c kptmp.c -o $@
	rm -f kptmp.c

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS= start.o main.o device.o command.o

all: init

init: $(OBJS) lib/libs.a dosfs/libdosfs.a
	$(LD) -static -z -o $@ -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

main.s: main.c
	cc $(CFLAGS) -S main.c

main.o: init.h main.c

device.o: init.h device.c

start.o: init.h start.c

lib.o: init.h lib.c


tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS)


