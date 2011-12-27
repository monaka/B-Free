# $Revision: 1.1 $
#
#

# -DFDDEBUG	... FDC のデバッグ用マクロ
#DEBUG= -DFDDEBUG
IDENT=-DTIMER_TEST -DI386 -DIBMPC $(DEBUG)


CPP=/lib/cpp
ROOT=../../../ITRON
#CFLAGS=-b i486-linuxaout -I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../../ITRON/kernlib -fno-builtin $(IDENT)
CFLAGS=-I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../../ITRON/kernlib -fno-builtin $(IDENT)
CC=gcc
#AS=/usr/local/bin/as
AS=/usr/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80120000
ENTRY=start
LIBS=../../../ITRON/kernlib/libkernel.a
#LD=/usr/local/bin/ld
LD=/usr/bin/ld

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS= fd.o misc.o version.o spec.o fdc.o 

all: fd765a

fd765a: $(OBJS) $(SRCS) Makefile
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(LIBS)

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS) fd765a


fd.o: fd.h
fdc.o: fd.h
