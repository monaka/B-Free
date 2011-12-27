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
#IDENT=-DI386 -DUSE_LOWLIB
#DEBUG=-DTSKSW_DEBUG

#PATH=.:/bin:/usr/bin:../../tool/kp
CPP=/lib/cpp
ROOT=../../../../kernel
CFLAGS=-I. -I$(ROOT) -I$(ROOT)/ITRON -I$(ROOT)/ITRON/kernlib -I$(ROOT)/ITRON/h -I$(ROOT)/ITRON/i386 -I$(ROOT)/POSIX/libc/others $(IDENT)
CC=gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x00001000
ENTRY=_main
KERNLIBS=$(ROOT)/POSIX/libc/libc.a #$(ROOT)/ITRON/kernlib/libkernel.a
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

OBJS= frtm.o stack.o interp.o funcs.o dict.o demos.o fgraph.o posix.o debug.o
IOBJS= 

all: frtm

frtm: $(OBJS) $(IOBJS) $(LIBS) $(KERNLIBS)
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(IOBJS) $(LIBS) $(KERNLIBS)

frtm.o: frtm.c frtm.h errmes.h
stack.o: stack.c frtm.h
interp.o: interp.c frtm.h funcs.h posix.h
funcs.o: funcs.c frtm.h
dict.o: dict.c frtm.h
demos.o: demos.c frtm.h
fgraph.o: fgraph.c frtm.h
posix.o: frtm.h
debug.o: frtm.h

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

install_3rd:
	./install_3rd.sh
clean:
	rm -f $(OBJS)
	rm -f frtm
