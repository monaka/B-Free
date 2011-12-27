# $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:35  liu1
# Initial Version.
#
# Revision 1.4  1995-07-06 15:22:02  night
# ld コマンドを /usr/local/bin/ld.old と定義した。
#
# Revision 1.3  1995/03/18  14:22:31  night
# jmp_user.o mm.o pm.o fm.o の各ファイル用の定義の追加。
#
# Revision 1.2  1995/02/20  15:19:33  night
# 以下のファイルについての記述を追加。
#   global.[co] signal.[co]
#
#
#

.PHONY: clean SYSCALLS

LD=/usr/local/bin/ld.old

ENTRY=_lowlib_start
STARTADDR=0x88000000

%.o: %.S
	$(CPP) $(ASFLAGS) $*.S | $(AS) -o $*.o

OBJS =	lowlib.o entry.o api.o global.o signal.o jmp_user.o mm.o pm.o fm.o
SRCS =	lowlib.c entry.c api.c global.c signal.c jmp_user.S mm.c pm.c fm.c

LIBS =	../../../../../kernel/kernlib/libkernel.a syscalls/libsyscall.a

CFLAGS = -I../../../include

all: lowlib

lowlib: $(OBJS) SYSCALLS
	$(LD) -static -z -o $@ -e ${ENTRY} -T $(STARTADDR) $(OBJS) $(LIBS)

SYSCALLS:
	$(MAKE) -C syscalls

clean:
	rm -f $(OBJS) lowlib

