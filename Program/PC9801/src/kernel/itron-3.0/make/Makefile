# $Revision: 1.1 $
#
#

IDENT=-DTIMER_TEST -DI386 -DAUTO_START
DEBUG=-DTSKSW_DEBUG

PATH=.:/bin:/usr/bin:../../../tool/kp
CPP=/lib/cpp
LD=/usr/local/bin/ld		# ZMAGIC タイプの実行ファイルを作ることができる
                                # 古い ld を使うこと
#LD=/usr/bin/ld			# システムに古い実行ファイルを作ることができる 
				# ld がある場合にはこちらを使う。
#LD=../../../../bin/ld
AS=/usr/local/bin/as
CFLAGS=-I. -I../h -I../i386 -I../pc9801 -b i486-linuxaout -fno-builtin $(IDENT)
CC=/usr/bin/gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
STARTADDR=0x80020000
ENTRY=startup

%.o: %.c
	kp -input-code euc -input $< -output kptmp.c
	$(CC) $(CFLAGS) -c kptmp.c -o $@
	rm -f kptmp.c

%.o: %.s
	$(CPP) $(ASFLAGS) $*.s | $(AS) -o $*.o

#OBJS=startup.o main.o memory.o task.o version.o error.o dummy.o printf.o 
#OBJS=startup.o main.o task.o version.o error.o dummy.o printf.o \
#     console.o io.o interrupt.o fault.o locore.o syscall.o misc.o timer.o \
#     virtual_memory.o itron_memory.o pmemory.o 

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
	keyboard.o \
	device.o \
	echo.o \
	dbg_functions.o \
	system.o \
	debug.o 


#SRCS=main.c memory.c task.c version.c error.c dummy.c printf.c \
#     console.c fault.c syscall.c misc.c timer.c \
#     virtual_memory.c itron_memory.c pmemory.c 

#ASRCS=startup.s io.s interrupt.s locore.s

all: itron.image

itron.image: $(OBJS)
	$(LD) -z -o itron.image -e ${ENTRY} -Ttext $(STARTADDR) $(OBJS) ../../kernlib/port_manager.o ../../kernlib/message_port.o ../../kernlib/sys_errno.o

tags:
	etags ../*/*.[hcs]

depend:
	$(MKDEPEND) -x c $(SRCS) > .depend

clean:
	rm -f $(OBJS)

include CFILES
include SFILES
