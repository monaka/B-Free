# $Revision: 1.1 $
#
#

IDENT=-DI386
DEBUG=-DTSKSW_DEBUG

PATH=.:/bin:/usr/bin:../../../tool/kp
CPP=/lib/cpp
ROOT=../../itron-3.0
CFLAGS=-I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../kernlib -fno-builtin $(IDENT)
CC=gcc
AS=/usr/local/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80110000
ENTRY=_start
LIBS=../../kernlib/libkernel.a
LD=/usr/local/bin/ld.old

%.o: %.c
	kp -input-code euc -input $< -output kptmp.c
	$(CC) $(CFLAGS) -c kptmp.c -o $@
	rm -f kptmp.c

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS= main.o interrupt.o misc.o

all: keyboard

keyboard: $(OBJS)
	$(LD) -static -z -o $@ -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS)


main.o: keyboard.h