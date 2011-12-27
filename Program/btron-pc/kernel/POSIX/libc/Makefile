# $Revision: 1.1 $
#
#

IDENT=-DTIMER_TEST -DI386
DEBUG=-DTSKSW_DEBUG

CPP=/lib/cpp
#ROOT=../../ITRON
ROOT=../..
#CFLAGS=-b i486-linuxaout -I. -I$(ROOT) -I../kernlib -I../../ITRON/h -I../../ITRON/i386 -fno-builtin $(IDENT)
CFLAGS=-fno-builtin -I. -I$(ROOT) -I$(ROOT)/ITRON -I$(ROOT)/ITRON/kernlib -I$(ROOT)/ITRON/h $(IDENT)
CC=gcc
KP=kp -input-code euc
MKDEPEND=$(CC) -M
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

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

OTHEROBJS= others/atoi.o others/stdlib.o _main.o others/graphic.o

STRINGOBJS = string/strcat.o string/strcmp.o string/strcpy.o string/strlen.o \
	string/ctype.o

OBJS=   call_posix.o \
	native/sys_noaction.o \
	native/sys_access.o \
	native/sys_chdir.o \
	native/sys_chmod.o \
	native/sys_chown.o \
	native/sys_close.o \
	native/sys_creat.o \
	native/sys_dup.o \
	native/sys_dup2.o \
	native/sys_exec.o \
	native/sys_exit.o \
	native/sys_fcntl.o \
	native/sys_fork.o \
	native/fork.o \
	native/sys_getcwd.o \
	native/sys_getdents.o \
	native/sys_getegid.o \
	native/sys_getenv.o \
	native/sys_geteuid.o \
	native/sys_getgid.o \
	native/sys_getgrgid.o \
	native/sys_getgrnam.o \
	native/sys_getgroups.o \
	native/sys_getlogin.o \
	native/sys_getpgrp.o \
	native/sys_getpid.o \
	native/sys_getppid.o \
	native/sys_getuid.o \
	native/sys_kill.o \
	native/sys_link.o \
	native/sys_lseek.o \
	native/sys_mkdir.o \
	native/sys_mkfifo.o \
	native/sys_misc.o \
	native/sys_mountroot.o \
	native/sys_open.o \
	native/sys_pause.o \
	native/sys_pipe.o \
	native/sys_read.o \
	native/sys_remove.o \
	native/sys_rename.o \
	native/sys_rmdir.o \
	native/sys_setgid.o \
	native/sys_setpgid.o \
	native/sys_setsid.o \
	native/sys_setuid.o \
	native/sys_sigaction.o \
	native/sys_sigaddset.o \
	native/sys_sigdelset.o \
	native/sys_sigemptyset.o \
	native/sys_sigfillset.o \
	native/sys_sigismember.o \
	native/sys_siglongjmp.o \
	native/sys_sigpending.o \
	native/sys_sigprocmask.o \
	native/sys_sigsetjmp.o \
	native/sys_sigsuspend.o \
	native/sys_sleep.o \
	native/sys_fstat.o \
	native/sys_time.o \
	native/sys_times.o \
	native/sys_ttyname.o \
	native/sys_umask.o \
	native/sys_uname.o \
	native/sys_unlink.o \
	native/sys_utime.o \
	native/sys_waitpid.o \
	native/sys_write.o




all: libc.a libnative.a
	ranlib libc.a
	ranlib libnative.a

libc.a: $(OBJS) $(OTHEROBJS) $(STRINGOBJS)
	ar rv libc.a $(OBJS) $(OTHEROBJS) $(STRINGOBJS)

libnative.a: $(OBJS)
	ar rv libnative.a $(OBJS)

syscall.o: syscall.c

call_posix.o: call_posix.s




clean:
	rm -f $(OBJS)
	rm -f $(OTHEROBJS)
	rm -f $(STRINGOBJS)
	rm -f libc.a
	rm -f libnative.a
