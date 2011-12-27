# $Revision: 1.1 $
#
#

#
# コンパイルフラグ一覧
#
#

IDENT=
DEBUG=

#PATH=.:/bin:/usr/bin:../../tool/kp
CPP=/lib/cpp
ROOT=../..
#CFLAGS=-Wall -I. -I$(ROOT) -I$(ROOT)/ITRON -I$(ROOT)/ITRON/kernlib -I$(ROOT)/ITRON/h -I$(ROOT)/ITRON/i386 -I$(ROOT)/POSIX/libc/others $(IDENT)
CFLAGS=-Wall -I. -I$(ROOT) -I$(ROOT)/ITRON -I$(ROOT)/ITRON/h -I$(ROOT)/ITRON/i386 -I$(ROOT)/POSIX/libc/others $(IDENT)
CC=gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x00001000
ENTRY=_main
#KERNLIBS=$(ROOT)/POSIX/libc/libc.a $(ROOT)/ITRON/kernlib/libkernel.a 
KERNLIBS=$(ROOT)/POSIX/libc/libc.a
#LD=/usr/local/bin/ld
#AS=/usr/local/bin/as
LD=/usr/bin/ld
AS=/usr/bin/as

#%.o: %.c
#	kp -input-code euc -input $< -output kptmp.c
#	$(CC) $(CFLAGS) -c kptmp.c -o $@
#	rm -f kptmp.c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

OBJS= test.o

all: test

test: $(OBJS) $(LIBS) $(KERNLIBS)
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(LIBS) $(KERNLIBS)

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f test
	rm -f $(OBJS)
