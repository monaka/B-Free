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

OBJS= test-app.o
IOBJS= 
TARGET=test-app
all: $(TARGET)

test-app: $(OBJS) $(IOBJS) $(LIBS) $(KERNLIBS)
	$(LD) -static -o $@ -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(IOBJS) $(LIBS) $(KERNLIBS)

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

install_3rd:
	./install_3rd.sh
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
