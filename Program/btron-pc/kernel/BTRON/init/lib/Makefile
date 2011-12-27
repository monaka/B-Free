# $Revision: 1.1 $
#
#

IDENT=-DTIMER_TEST -DI386
DEBUG=-DTSKSW_DEBUG

CPP=/lib/cpp
ROOT=../../../ITRON
#CFLAGS=-b i486-linuxaout -m386 -DDEBUG -I -I. -I$(ROOT) -I../../kernlib -I../../../ITRON -I../../../ITRON/i386 -I../../../ITRON/h -fno-builtin
CFLAGS=-DDEBUG -I -I. -I$(ROOT) -I../../kernlib -I../../../ITRON -I../../../ITRON/i386 -I../../../ITRON/h -fno-builtin
CC=gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x00001000
ENTRY=_start
#LD=/usr/local/bin/ld
#AS=/usr/local/bin/as
LD=/usr/bin/ld
AS=/usr/bin/as

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS= string.o stdlib.o atoi.o

all: libs.a

libs.a: $(OBJS)
	ar rv libs.a $(OBJS)
	ranlib libs.a


tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS) libs.a


