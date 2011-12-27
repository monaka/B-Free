# $Revision: 1.1 $
#
#

IDENT=-DTIMER_TEST -DI386
DEBUG=-DTSKSW_DEBUG

PATH=.:/bin:/usr/bin:../../../tool/kp
CPP=/lib/cpp
ROOT=../../itron-3.0
CFLAGS=-b i486-linuxaout -I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../kernlib -fno-builtin $(IDENT)
CC=gcc
AS=/usr/local/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80120000
ENTRY=_start
LIBS=../../kernlib/libkernel.a
LD=/usr/local/bin/ld

%.o: %.c
	kp -input-code euc -input $< -output kptmp.c
	$(CC) $(CFLAGS) -c kptmp.c -o $@
	rm -f kptmp.c

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS= fd.o misc.o version.o spec.o fdc.o 

all: fd765a

fd765a: $(OBJS)
	$(LD) -static -z -o $@ -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS)


fd.o: fd.h