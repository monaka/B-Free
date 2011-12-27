# $Revision: 1.1 $
#
#

#DEBUG=-DDEBUG
IDENT=-DI386 -DIBMPC

CPP=/lib/cpp
ROOT=../../../ITRON
#CFLAGS=-b i486-linuxaout -I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../kernlib -fno-builtin $(IDENT)
CFLAGS=-I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I$(ROOT)/kernlib -fno-builtin $(IDENT) $(DEBUG)
CC=gcc
AS=/usr/local/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80150000
ENTRY=start
LIBS=$(ROOT)/kernlib/libkernel.a
#LD=/usr/local/bin/ld
LD=/usr/bin/ld

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS=ne2000.o ne2000low.o misc.o interrupt.o
SRCS=ne2000.c ne2000low.c misc.c interrupt.c

all: ne2000

ne2000: $(OBJS) $(SRCS) Makefile
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(LIBS)

tags:
	etags *.[hcs]

depend:
	$(MKDEPEND)  $(CFLAGS) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS) ne2000



-include .depend
