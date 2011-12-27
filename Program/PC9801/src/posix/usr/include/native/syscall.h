/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/native/syscall.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: syscall.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:55  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:47:51  night
 * 最初の登録
 *
 *
 */

#ifndef __NATIVE_SYSCALL_H__
#define __NATIVE_SYSCALL_H__		1

#define	PSYS_ACCESS		1
#define PSYS_ALARM		2
#define	PSYS_CHDIR		3
#define	PSYS_CHMOD		4
#define	PSYS_CHOWN		5
#define	PSYS_CLOSE		6
#define	PSYS_CLOSEDIR	   	7
#define	PSYS_CREAT		8
#define	PSYS_DUP		9
#define	PSYS_DUP2		10
#define	PSYS_EXECL		11
#define	PSYS_EXECLE		12
#define	PSYS_EXECLP		13
#define	PSYS_EXECV		14
#define	PSYS_EXECVE		15
#define	PSYS_EXECVP		16
#define	PSYS__EXIT		17
#define	PSYS_FCNTL		18
#define	PSYS_FORK		19
#define	PSYS_FSTAT		20
#define	PSYS_GETCWD		21
#define	PSYS_GETEGID	  	22
#define	PSYS_GETENV		23
#define	PSYS_GETEUID	  	24
#define	PSYS_GETGID		25
#define	PSYS_GETGRGID	  	26
#define	PSYS_GETGRNAM	  	27
#define	PSYS_GETGROUPS		28
#define	PSYS_GETLOGIN	  	29
#define	PSYS_GETPGRP	  	30
#define	PSYS_GETPID		31
#define	PSYS_GETPPID	  	32
#define	PSYS_GETUID		33
#define	PSYS_KILL		34
#define	PSYS_LINK		35
#define	PSYS_LSEEK		36
#define	PSYS_MKDIR		37
#define	PSYS_MKFIFO		38
#define	PSYS_OPEN		39
#define	PSYS_OPENDIR	  	40
#define	PSYS_PAUSE		41
#define	PSYS_PIPE		42
#define	PSYS_READ		43
#define	PSYS_READDIR	  	44
#define	PSYS_RENAME		45
#define	PSYS_REWINDDIR	  	46
#define	PSYS_RMDIR		47
#define	PSYS_SETGID		48
#define	PSYS_SETPGID	  	49
#define	PSYS_SETSID		50
#define	PSYS_SETUID		51
#define	PSYS_SIGACTION	  	52
#define	PSYS_SIGADDSET	  	53
#define	PSYS_SIGDELSET	  	54
#define	PSYS_SIGEMPTYSET	55
#define	PSYS_SIGFILLSET	  	56
#define	PSYS_SIGISMEMBER	57
#define	PSYS_SIGLONGJMP	  	58
#define	PSYS_SIGPENDING	  	59
#define	PSYS_SIGPROCMASK	60
#define	PSYS_SIGSETJMP	  	61
#define	PSYS_SITSUSPEND	  	62
#define	PSYS_SLEEP		63
#define	PSYS_STAT		64
#define	PSYS_TIME		65
#define	PSYS_TIMES		66
#define	PSYS_TTYNAME	  	67
#define	PSYS_TZSET		68
#define	PSYS_UMASK		69
#define	PSYS_UNAME		70
#define	PSYS_UNLINK		71
#define	PSYS_UTIME		72
#define	PSYS_WATIPID	  	73
#define	PSYS_WRITE		74

#define	PSYS_MOUNT		101
#define	PSYS_UMOUNT		102


#endif /* __NATIVE_SYSCALL_H__ */

