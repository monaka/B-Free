/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/api.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/api.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: api.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:55  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:22:55  night
 * grow_heap システムコールの追加。
 *
 * Revision 1.1  1995/02/20  15:16:40  night
 * はじめての登録
 *
 *
 */

/*
 *	POSIX Envinronment systemcall table.
 */

#include <errno.h>
#include <sys/types.h>
#include "funcs.h"
#include "syscall.h"


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
  DEF_NOSYSCALL,		/*   0 */
  DEF_SYSCALL(access),		/*   1 */
  DEF_SYSCALL(chdir),		/*   2 */
  DEF_SYSCALL(chmod),		/*   3 */
  DEF_SYSCALL(chown),		/*   4 */
  DEF_SYSCALL(close),		/*   5 */
  DEF_SYSCALL(closedir),	/*   6 */
  DEF_SYSCALL(creat),		/*   7 */
  DEF_SYSCALL(dup),		/*   8 */
  DEF_SYSCALL(dup2),		/*   9 */
  DEF_SYSCALL(execl),		/*  10 */
  DEF_SYSCALL(execle),		/*  11 */
  DEF_SYSCALL(execlp),		/*  12 */
  DEF_SYSCALL(execv),		/*  13 */
  DEF_SYSCALL(execve),		/*  14 */
  DEF_SYSCALL(execvp),		/*  15 */
  DEF_SYSCALL(_exit),		/*  16 */
  DEF_SYSCALL(fcntl),		/*  17 */
  DEF_SYSCALL(fork),		/*  18 */
  DEF_SYSCALL(fstat),		/*  19 */
  DEF_SYSCALL(getcwd),		/*  20 */
  DEF_SYSCALL(getegid),		/*  21 */
  DEF_SYSCALL(getenv),		/*  22 */
  DEF_SYSCALL(geteuid),		/*  23 */
  DEF_SYSCALL(getgid),		/*  24 */
  DEF_SYSCALL(getgrgid),	/*  25 */
  DEF_SYSCALL(getgrnam),	/*  26 */
  DEF_SYSCALL(getgroups),	/*  27 */
  DEF_SYSCALL(getlogin),	/*  28 */
  DEF_SYSCALL(getpgrp),		/*  29 */
  DEF_SYSCALL(getpid),		/*  30 */
  DEF_SYSCALL(getppid),		/*  31 */
  DEF_SYSCALL(getuid),		/*  32 */
  DEF_SYSCALL(kill),		/*  33 */
  DEF_SYSCALL(link),		/*  34 */
  DEF_SYSCALL(lseek),		/*  35 */
  DEF_SYSCALL(mkdir),		/*  36 */
  DEF_SYSCALL(mkfifo),		/*  37 */
  DEF_SYSCALL(open),		/*  38 */
  DEF_SYSCALL(opendir),		/*  39 */
  DEF_SYSCALL(pause),		/*  40 */
  DEF_SYSCALL(pipe),		/*  41 */
  DEF_SYSCALL(read),		/*  42 */
  DEF_SYSCALL(readdir),		/*  43 */
  DEF_SYSCALL(rename),		/*  44 */
  DEF_SYSCALL(rewinddir),	/*  45 */
  DEF_SYSCALL(rmdir),		/*  46 */
  DEF_SYSCALL(setgid),		/*  47 */
  DEF_SYSCALL(setpgid),		/*  48 */
  DEF_SYSCALL(setsid),		/*  49 */
  DEF_SYSCALL(setuid),		/*  50 */
  DEF_SYSCALL(sigaction),	/*  51 */
  DEF_SYSCALL(sigaddset),	/*  52 */
  DEF_SYSCALL(sigdelset),	/*  53 */
  DEF_SYSCALL(sigemptyset),	/*  54 */
  DEF_SYSCALL(sigfillset),	/*  55 */
  DEF_SYSCALL(sigismember),	/*  56 */
  DEF_SYSCALL(siglongjmp),	/*  57 */
  DEF_SYSCALL(sigpending),	/*  58 */
  DEF_SYSCALL(sigprocmask),	/*  59 */
  DEF_SYSCALL(sigsetjmp),	/*  60 */
  DEF_SYSCALL(sigsuspend),	/*  61 */
  DEF_SYSCALL(sleep),		/*  62 */
  DEF_SYSCALL(stat),		/*  63 */
  DEF_SYSCALL(time),		/*  64 */
  DEF_SYSCALL(times),		/*  65 */
  DEF_SYSCALL(ttyname),		/*  66 */
  DEF_SYSCALL(tzset),		/*  67 */
  DEF_SYSCALL(umask),		/*  68 */
  DEF_SYSCALL(uname),		/*  69 */
  DEF_SYSCALL(unlink),		/*  70 */
  DEF_SYSCALL(utime),		/*  71 */
  DEF_SYSCALL(watipid),		/*  72 */
  DEF_SYSCALL(write),		/*  73 */
  DEF_NOSYSCALL,		/*  74 */
  DEF_NOSYSCALL,		/*  75 */
  DEF_NOSYSCALL,		/*  76 */
  DEF_NOSYSCALL,		/*  77 */
  DEF_NOSYSCALL,		/*  78 */
  DEF_NOSYSCALL,		/*  79 */
  DEF_NOSYSCALL,		/*  80 */
  DEF_NOSYSCALL,		/*  81 */
  DEF_NOSYSCALL,		/*  82 */
  DEF_NOSYSCALL,		/*  83 */
  DEF_NOSYSCALL,		/*  84 */
  DEF_NOSYSCALL,		/*  85 */
  DEF_NOSYSCALL,		/*  86 */
  DEF_NOSYSCALL,		/*  87 */
  DEF_NOSYSCALL,		/*  88 */
  DEF_NOSYSCALL,		/*  89 */
  DEF_NOSYSCALL,		/*  90 */
  DEF_NOSYSCALL,		/*  91 */
  DEF_NOSYSCALL,		/*  92 */
  DEF_NOSYSCALL,		/*  93 */
  DEF_NOSYSCALL,		/*  94 */
  DEF_NOSYSCALL,		/*  95 */
  DEF_NOSYSCALL,		/*  96 */
  DEF_NOSYSCALL,		/*  97 */
  DEF_NOSYSCALL,		/*  98 */
  DEF_NOSYSCALL,		/*  99 */
  DEF_NOSYSCALL,		/* 100 */
  DEF_SYSCALL(mount),		/* 101 */
  DEF_SYSCALL(umount),		/* 102 */
  DEF_SYSCALL(grow_heap),	/* 103 */
  DEF_NOSYSCALL,		/* 104 */
  DEF_NOSYSCALL,		/* 105 */
  DEF_NOSYSCALL,		/* 106 */
  DEF_NOSYSCALL,		/* 107 */
  DEF_NOSYSCALL,		/* 108 */
  DEF_NOSYSCALL,		/* 109 */
  DEF_NOSYSCALL,		/* 110 */
};


int	nsyscall = (sizeof (syscalls) / sizeof (syscalls[0]));
