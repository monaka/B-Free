/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/api.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/api.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: api.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  2000-06-01 08:44:40  naniwa
 * to implement getdents
 *
 * Revision 1.5  2000/02/16 08:17:04  naniwa
 * to reorder POSIX system calls
 *
 * Revision 1.4  1999/03/21 00:59:46  monaka
 * Function watipid was renamed to waitpid.
 *
 * Revision 1.3  1999/02/24 13:13:12  night
 * Changed function name for sigsuspend.
 *
 * Revision 1.2  1997/08/31 13:13:42  night
 * とりあえず、OS ファイルに入れるところまでできた。
 *
 * Revision 1.1  1996/11/11  13:36:05  night
 * IBM PC 版への最初の登録
 *
 * ----------------
 *
 *
 */

/*
 *	POSIX Envinronment systemcall table.
 */

#include "lowlib.h"


#define DEF_SYSCALL(x)		{ (int (*)())psys_ ## x, #x }
#define DEF_NOSYSCALL		{ nodef, "nosyscall" }

/*
 * 何もしない、システムコールの定義 (ENOSYS エラーを返す)。
 */
static int
nodef () 
{
  return (ENOSYS); 
}


struct syscall_entry syscalls[] =
{
  DEF_SYSCALL (noaction),	/*  0 */
  DEF_SYSCALL (access),		/*  1 */
  DEF_SYSCALL (chdir),		/*  2 */
  DEF_SYSCALL (chmod),		/*  3 */
  DEF_SYSCALL (chown),		/*  4 */
  DEF_SYSCALL (close),		/*  5 */
  DEF_SYSCALL (creat),		/*  6 */
  DEF_SYSCALL (dup),		/*  7 */
  DEF_SYSCALL (exec),		/*  8 */
  DEF_SYSCALL (exit),		/*  9 */
  DEF_SYSCALL (fcntl),		/* 10 */
  DEF_SYSCALL (fork),		/* 11 */
  DEF_SYSCALL (noaction),	/* 12 */ /* fstat */
  DEF_SYSCALL (getegid),	/* 13 */
  DEF_SYSCALL (geteuid),	/* 14 */
  DEF_SYSCALL (getgid),		/* 15 */
  DEF_SYSCALL (getpgrp),	/* 16 */
  DEF_SYSCALL (getpid),		/* 17 */
  DEF_SYSCALL (getppid),	/* 18 */
  DEF_SYSCALL (getuid),		/* 19 */
  DEF_SYSCALL (link),		/* 20 */
  DEF_SYSCALL (lseek),		/* 21 */
  DEF_SYSCALL (mkdir),		/* 22 */
  DEF_SYSCALL (open),		/* 23 */
  DEF_SYSCALL (pipe),		/* 24 */
  DEF_SYSCALL (read),		/* 25 */
  DEF_SYSCALL (rename),		/* 26 */
  DEF_SYSCALL (rmdir),		/* 27 */
  DEF_SYSCALL (setgid),		/* 28 */
  DEF_SYSCALL (setpgid),	/* 29 */
  DEF_SYSCALL (setsid),		/* 30 */
  DEF_SYSCALL (setuid),		/* 31 */
  DEF_SYSCALL (stat),		/* 32 */
  DEF_SYSCALL (time),		/* 33 */
  DEF_SYSCALL (times),		/* 34 */
  DEF_SYSCALL (umask),		/* 35 */
  DEF_SYSCALL (unlink),		/* 36 */
  DEF_SYSCALL (utime),		/* 37 */
  DEF_SYSCALL (waitpid),	/* 38 */
  DEF_SYSCALL (write),		/* 39 */
  DEF_SYSCALL (noaction),	/* 40 */ /* BRK */
  DEF_SYSCALL (noaction),	/* 41 */ /* CHROOT */
  DEF_SYSCALL (getdents),	/* 42 */
  DEF_SYSCALL (noaction),	/* 43 */ /* MKNOD */
  DEF_SYSCALL (mount),		/* 44 */
  DEF_SYSCALL (mountroot),	/* 45 */
  DEF_SYSCALL (statfs),		/* 46 */
  DEF_SYSCALL (noaction),	/* 47 */ /* STIME */
  DEF_SYSCALL (noaction),	/* 48 */ /* SYNC */
  DEF_SYSCALL (umount),		/* 49 */
  DEF_SYSCALL (noaction),	/* 50 */ /* ALARM */
  DEF_SYSCALL (kill),		/* 51 */
  DEF_SYSCALL (pause),		/* 52 */
  DEF_SYSCALL (sigaction),	/* 53 */
  DEF_SYSCALL (noaction),	/* 54 */ /* SIGRETURN */
  DEF_SYSCALL (sigprocmask),	/* 55 */
  DEF_SYSCALL (sigpending),	/* 56 */
  DEF_SYSCALL (sigsuspend),	/* 57 */
  DEF_SYSCALL (sigaddset),	/* 58 */
  DEF_SYSCALL (sigdelset),	/* 59 */
  DEF_SYSCALL (sigemptyset),	/* 60 */
  DEF_SYSCALL (sigfillset),	/* 61 */
  DEF_SYSCALL (sigismember),	/* 62 */
  DEF_SYSCALL (siglongjmp),	/* 63 */
  DEF_SYSCALL (sigsetjmp),	/* 64 */
  DEF_SYSCALL (misc),		/* 65 */
  DEF_SYSCALL (memory),		/* 66 */
  DEF_SYSCALL (dup2),		/* 67 */
  DEF_SYSCALL (getcwd),		/* 68 */
  DEF_SYSCALL (getenv),		/* 69 */
  DEF_SYSCALL (getgrgid),	/* 70 */
  DEF_SYSCALL (getgrnam),	/* 71 */
  DEF_SYSCALL (getgroups),	/* 72 */
  DEF_SYSCALL (getlogin),	/* 73 */
  DEF_SYSCALL (mkfifo),		/* 74 */
  DEF_SYSCALL (remove),		/* 75 */
  DEF_SYSCALL (rewind),		/* 76 */
  DEF_SYSCALL (sleep),		/* 77 */
  DEF_SYSCALL (ttyname),	/* 78 */
  DEF_SYSCALL (tzset),		/* 79*/
  DEF_SYSCALL (uname),		/* 80 */
};


int	nsyscall = (sizeof (syscalls) / sizeof (syscalls[0]));
