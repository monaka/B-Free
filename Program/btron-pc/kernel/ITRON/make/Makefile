# $Revision: 1.1 $
#
#
COMMON=../../../include


#DEBUG=-DTSKSW_DEBUG
DEBUG=-DKEYBOARD_DEBUG
#IDENT=-DTIMER_TEST -DI386 -DAUTO_START -DIBMPC $(DEBUG)
IDENT=-DTIMER_TEST -DI386 -DIBMPC -DAUTO_START -DPORTMANAGER -DKERNLIB -DITRON $(DEBUG)

CPP=/lib/cpp
#LD=/usr/local/bin/ld		# ZMAGIC タイプの実行ファイルを作ることができる
#                                # 古い ld を使うこと
LD=/usr/bin/ld			# システムに古い実行ファイルを作ることができる 
				# ld がある場合にはこちらを使う。
#LD=../../../../bin/ld
#AS=/usr/local/bin/as-new
AS=/usr/bin/as
CFLAGS=-I. -I../h -I../i386 -I../ibmpc -I$(COMMON) -Wall -fno-builtin $(IDENT)
CC=/usr/bin/gcc
MKDEPEND=$(CC) -M
STARTADDR=0x80020000
ENTRY=startup

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

OBJS= \
	startup.o \
	devconfig.o \
	console.o \
	dummy.o \
	error.o \
	itron_memory.o \
	main.o \
	misc.o \
	kalloc.o \
	pmemory.o \
	printf.o \
	syscall.o \
	syscall-if.o \
	task.o \
	time.o \
	version.o \
	virtual_memory.o \
	semaphore.o \
	tss.o \
	fault.o \
	timer.o \
	io.o \
	gdt.o \
	task2.o \
	message.o \
	eventflag.o \
	locore.o \
	interrupt.o  \
	device.o \
	keyboard.o \
	echo.o \
	dbg_functions.o \
	system.o \
	lowlib.o \
	debug.o 

OTHEROBJ=../kernlib/port_manager.o ../kernlib/message_port.o \
           ../kernlib/sys_errno.o

SRCS=main.c memory.c task.c time.c version.c error.c dummy.c printf.c \
     console.c fault.c syscall.c misc.c timer.c \
     virtual_memory.c itron_memory.c pmemory.c \
     startup.S

#ASRCS=startup.s io.s interrupt.s locore.s

all: itron.image

itron.image: $(OBJS) libkernel.a port_manager
	$(LD) -static -o itron.image -e ${ENTRY} -Ttext=$(STARTADDR) $(OBJS) $(OTHEROBJ)

libkernel.a:
	make -C ../kernlib

port_manager:
	make -C ../servers

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS)
	rm -f itron.image
	make -C ../kernlib clean
	make -C ../servers clean

include CFILES
include SFILES
