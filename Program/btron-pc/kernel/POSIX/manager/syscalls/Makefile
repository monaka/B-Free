# $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/syscalls/Makefile,v 1.1 2011/12/27 17:13:35 liu1 Exp $
#
#
# $Log: Makefile,v $
# Revision 1.1  2011/12/27 17:13:35  liu1
# Initial Version.
#
# Revision 1.8  2000-06-01 08:47:48  naniwa
# to implement getdents
#
# Revision 1.7  2000/02/16 08:19:25  naniwa
# to reorder POSIX system calls
#
# Revision 1.6  2000/01/15 15:30:52  naniwa
# minor fix
#
# Revision 1.5  1999/05/30 04:06:39  naniwa
# minor fix
#
# Revision 1.4  1999/04/18 18:44:06  monaka
# Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
#
# Revision 1.3  1999/04/13 04:15:30  monaka
# MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
#
# Revision 1.2  1999/03/24 08:00:23  monaka
# Modified include pathes (POSIX manager should be independ from ITRON.)
#
# Revision 1.1  1999/03/21 14:01:51  monaka
# They are separated from syscall.c. Still no major changes available.
#
#
#
#
#


#DEBUG=-DEXEC_DEBUG -DDEBUG
IDENT=$(DEBUG) -DKERNEL
INC=-I.. -I../../../ITRON -I../../../ITRON/h
CFLAGS = -Wall $(INC) $(IDENT)
CC=gcc

OBJS =	access.o \
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
	time.o \
	times.o \
	ttyname.o \
	tzset.o \
	umask.o \
	umount.o \
	uname.o \
	unlink.o \
	utime.o \
	waitpid.o \
	write.o


all: libsyscall_mgr.a


libsyscall_mgr.a: $(OBJS)
	ar rv libsyscall_mgr.a $(OBJS)
	ranlib libsyscall_mgr.a


clean:
	rm -f $(OBJS) libsyscall_mgr.a
