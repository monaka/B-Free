# $Revision: 1.1 $
#
#

#
# コンパイルフラグ一覧
#
#

# -D DOS
# -D USE_LOWLIB		POSIX マネージャを呼び出すとき、lowlib を使用する。
#
IDENT=-DI386 -DUSE_LOWLIB
DEBUG=-DTSKSW_DEBUG

#PATH=.:/bin:/usr/bin:../../tool/kp
CPP=/lib/cpp
ROOT=../../ITRON
CFLAGS=-Wall -I. -I$(ROOT) -I../kernlib -I$(ROOT)/h -I$(ROOT)/i386 $(IDENT)
CC=gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x00001000
ENTRY=start
KERNLIBS=$(ROOT)/kernlib/libkernel.a ../../POSIX/libc/libnative.a
LIBS=libMitten/libMitten.a lib/libs.a
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

OBJS= start.o main.o device.o posix_if.o ether.o

all: init

init: $(OBJS) $(LIBS) $(KERNLIBS)
	(cd libMitten; make)
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(LIBS) $(KERNLIBS)

main.s: main.c
	cc $(CFLAGS) -S main.c

main.o: init.h main.c

device.o: init.h device.c

start.o: init.h start.c

filesystem.o: init.h filesystem.c

lib.o: init.h lib.c

lib/libs.a:
	(cd lib; make)

dosfs/libdosfs.a:
	(cd dosfs; make)

libMitten/libMitten.a:
	(cd libMitten; make)

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f init
	rm -f $(OBJS)
	rm -f $(ADDOBJS)
	(cd lib; make clean)
	(cd dosfs; make clean)
	(cd libMitten; make clean)

