# @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:35  liu1
# Initial Version.
#
# Revision 1.1  1995-03-04 14:36:41  night
# ╨г╫И╓непо©
#
#
#

.PHONY: clean

OBJS=	_exit.o access.o chdir.o chmod.o chown.o close.o closedir.o \
	creat.o dup.o dup2.o execl.o execle.o execlp.o execv.o \
	execve.o execvp.o fcntl.o fork.o fseek.o fstat.o getcwd.o \
	getegid.o getenv.o geteuid.o getgid.o getgrgid.o getgrnam.o \
	getgroups.o getlogin.o getpgrp.o getpid.o getppid.o getuid.o \
	kill.o link.o lseek.o mkdir.o mkfifo.o open.o opendir.o pause.o \
	pipe.o read.o readdir.o remove.o rename.o rewind.o rewinddir.o \
	rmdir.o setgid.o setpgid.o setsid.o setuid.o sigaction.o sigaddset.o \
	sigdelset.o sigemptyset.o sigfillset.o sigismember.o siglongjmp.o \
	sigpending.o sigprocmask.o sigsetjmp.o sigsuspend.o sleep.o stat.o \
	time.o times.o ttyname.o tzset.o umask.o uname.o unlink.o \
	utime.o watipid.o write.o mount.o umount.o

SRCS=	_exit.c access.c chdir.c chmod.c chown.c close.c closedir.c \
	creat.c dup.c dup2.c execl.c execle.c execlp.c execv.c \
	execve.c execvp.c fcntl.c fork.c fseek.c fstat.c getcwd.c \
	getegid.c getenv.c geteuid.c getgid.c getgrgid.c getgrnam.c \
	getgroups.c getlogin.c getpgrp.c getpid.c getppid.c getuid.c \
	kill.c link.c lseek.c mkdir.c mkfifo.c open.c opendir.c pause.c \
	pipe.c read.c readdir.c remove.c rename.c rewind.c rewinddir.c \
	rmdir.c setgid.c setpgid.c setsid.c setuid.c sigaction.c sigaddset.c \
	sigdelset.c sigemptyset.c sigfillset.c sigismember.c siglongjmp.c \
	sigpending.c sigprocmask.c sigsetjmp.c sigsuspend.c sleep.c stat.c \
	time.c times.c ttyname.c tzset.c umask.c uname.c unlink.c \
	utime.c watipid.c write.c mount.c umount.c


CFLAGS=-c -g -I../../../../include


all: libsyscall.a


libsyscall.a: $(OBJS)
	ar -r $@ $(OBJS)
	ranlib $@

clean:
	rm -f $(OBJS) libsyscall.a