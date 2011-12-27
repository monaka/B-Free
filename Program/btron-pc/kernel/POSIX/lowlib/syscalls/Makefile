# $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:35  liu1
# Initial Version.
#
# Revision 1.9  2000-06-01 08:45:11  naniwa
# to implement getdents
#
# Revision 1.8  2000/05/25 08:03:31  naniwa
# minor change
#
# Revision 1.7  2000/02/16 08:17:29  naniwa
# to reorder POSIX system calls
#
# Revision 1.6  1999/11/15 13:43:09  naniwa
# minor fix
#
# Revision 1.5  1999/11/10 10:47:24  naniwa
# to implement execve
#
# Revision 1.4  1999/04/18 18:43:55  monaka
# Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
#
# Revision 1.3  1999/03/21 00:54:38  monaka
# Added warning rule and rule for _make_connection.c
#
# Revision 1.2  1999/02/19 08:54:26  monaka
# rule about sitsuspend deleted.
#
# Revision 1.1  1997/08/31 13:10:42  night
# ╨г╫И╓непо©
#
#
#
#

#ROOT=../../../ITRON
ROOT=../../..
CFLAGS = -Wall -I../../libc/include -I../../libc/include/sys -I$(ROOT) -I$(ROOT)/ITRON -I$(ROOT)/ITRON/h -I$(ROOT)/ITRON/kernlib $(IDENT)


OBJS =	_make_connection.o \
	access.o \
	chdir.o \
	chmod.o \
	chown.o \
	close.o \
	creat.o \
	dup.o \
	dup2.o \
	exec.o \
	exit.o \
	fcntl.o \
	fork.o \
	getcwd.o \
	getdents.o \
	getegid.o \
	getenv.o \
	geteuid.o \
	getgid.o \
	getgrgid.o \
	getgrnam.o \
	getgroups.o \
	getlogin.o \
	getpgrp.o \
	getpid.o \
	getppid.o \
	getuid.o \
	kill.o \
	link.o \
	lseek.o \
	memory.o \
	misc.o \
	mkdir.o \
	mkfifo.o \
	mount.o \
	mountroot.o \
	noaction.o \
	open.o \
	pause.o \
	pipe.o \
	read.o \
	remove.o \
	rename.o \
	rewind.o \
	rmdir.o \
	setgid.o \
	setpgid.o \
	setsid.o \
	setuid.o \
	sigaction.o \
	sigaddset.o \
	sigdelset.o \
	sigemptyset.o \
	sigfillset.o \
	sigismember.o \
	siglongjmp.o \
	sigpending.o \
	sigprocmask.o \
	sigsetjmp.o \
	sigsuspend.o \
	sleep.o \
	stat.o \
	statfs.o \
	template.o \
	time.o \
	times.o \
	ttyname.o \
	tzset.o \
	umask.o \
	umount.o \
	uname.o \
	unlink.o \
	utime.o \
	watipid.o \
	write.o


all: libsyscall.a


libsyscall.a: $(OBJS)
	ar rv libsyscall.a $(OBJS)
	ranlib libsyscall.a


clean:
	rm -f $(OBJS)
	rm -f libsyscall.a
