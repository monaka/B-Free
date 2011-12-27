/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/funcs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: funcs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:52:56  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/03/18  14:24:16  night
 * システムコール関数のプロトタイプ宣言の変更。
 * 元は、引数をシステムコールの引数のとおりに書いていた。
 * それを、引数はすべて void * に変更した。
 *
 * Revision 1.2  1995/02/20  15:20:18  night
 * RCS の Log マクロを入れる註釈部を追加。
 *
 *
 */

/*
 * システムコール処理関数のプロトタイプ宣言
 *
 */

#ifndef __FUNCS_H__
#define __FUNCS_H__	1

#include <sys/types.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/times.h>
#include <sys/utsname.h>
#include <sys/utime.h>

/*
 * システムコール処理用関数の定義。
 */
extern int	psys_access (void *argp);
extern int	psys_chdir (void *argp);
extern int	psys_chmod (void *argp);
extern int	psys_chown (void *argp);
extern int	psys_close (void *argp);
extern int	psys_closedir (void *argp);
extern int	psys_creat (void *argp);
extern int	psys_dup (void *argp);
extern int	psys_dup2 (void *argp);
extern int	psys_execl (void *argp);
extern int	psys_execle (void *argp);
extern int	psys_execlp (void *argp);
extern int	psys_execv (void *argp);
extern int	psys_execve (void *argp);
extern int	psys_execvp (void *argp);
extern void	psys__exit (void *argp);
extern int	psys_fcntl (void *argp);
extern pid_t	psys_fork (void *argp);
extern int	psys_fstat (void *argp);
extern char	*psys_getcwd (void *argp);
extern gid_t	psys_getegid (void *argp);
extern char	*psys_getenv (void *argp);
extern uid_t	psys_geteuid (void *argp);
extern gid_t	psys_getgid (void *argp);
extern struct group *psys_getgrgid (void *argp);
extern struct group *psys_getgrnam (void *argp);
extern int	psys_getgroups (void *argp);
extern char	*psys_getlogin (void *argp);
extern pid_t	psys_getpgrp (void *argp);
extern pid_t	psys_getpid (void *argp);
extern pid_t	psys_getppid (void *argp);
extern uid_t	psys_getuid (void *argp);
extern int	psys_kill (void *argp);
extern int	psys_link (void *argp);
extern off_t	psys_lseek (void *argp);
extern int	psys_mkdir (void *argp);
extern int	psys_mkfifo (void *argp);
extern int	psys_open (void *argp);
extern DIR	*psys_opendir (void *argp);
extern int	psys_pause (void *argp);
extern int	psys_pipe (void *argp);
extern ssize_t	psys_read (void *argp);
extern struct dirent   *psys_readdir (void *argp);
extern int	psys_rename (void *argp);
extern void	psys_rewinddir (void *argp);
extern int	psys_rmdir (void *argp);
extern int	psys_setgid (void *argp);
extern int	psys_setpgid (void *argp);
extern pid_t	psys_setsid (void *argp);
extern int	psys_setuid (void *argp);
extern int	psys_sigaction (void *argp);
extern int	psys_sigaddset (void *argp);
extern int	psys_sigdelset (void *argp);
extern int	psys_sigemptyset (void *argp);
extern int	psys_sigfillset (void *argp);
extern int	psys_sigismember (void *argp);
extern void	psys_siglongjmp (void *argp);
extern int	psys_sigpending (void *argp);
extern int	psys_sigprocmask (void *argp);
extern int	psys_sigsetjmp (void *argp);
extern int	psys_sigsuspend (void *argp);
extern unsigned	int    psys_sleep (void *argp);
extern int	psys_stat (void *argp);
extern time_t	psys_time (void *argp);
extern clock_t	psys_times (void *argp);
extern char	*psys_ttyname (void *argp);
extern void	psys_tzset (void *argp);
extern mode_t	psys_umask (void *argp);
extern int	psys_uname (void *argp);
extern int	psys_unlink (void *argp);
extern int	psys_utime (void *argp);
extern int	psys_watipid (void *argp);
extern ssize_t	psys_write (void *argp);

/*
 * POSIX ではインプリメント依存となっているシステムコール
 */
extern int	psys_mount ();
extern int	psys_umount ();
extern void	*psys_grow_heap ();

/*
 * lowlib.c
 */
extern void	posix_exit (void);

#endif /* __FUNCS_H__ */
