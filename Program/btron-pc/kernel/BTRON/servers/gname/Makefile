# $Revision: 1.1 $
#
#

DEBUG=-DDEBUG
IDENT=-DI386 -DIBMPC

CPP=/lib/cpp
ROOT=../../../ITRON
#CFLAGS=-b i486-linuxaout -I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../kernlib -fno-builtin $(IDENT)
CFLAGS=-I. -I$(ROOT) -I$(ROOT)/h -I$(ROOT)/i386 -I../../kernlib -fno-builtin $(IDENT) $(DEBUG)
CC=gcc
AS=/usr/local/bin/as
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=%%address%%
ENTRY=start
LIBS=../../kernlib/libkernel.a
#LD=/usr/local/bin/ld
LD=/usr/bin/ld

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS=gname.o misc.o
SRCS=gname.c misc.c

all: gname

gname: $(OBJS)
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(LIBS)

tags:
	etags *.[hcs]

depend:
	$(MKDEPEND)  $(CFLAGS) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS) gname
