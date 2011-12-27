/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* syscall.c - POSIX 環境でのシステムコール処理関数群
 *
 *
 * $Log: syscall.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.41  2000-06-01 08:46:51  naniwa
 * to implement getdents
 *
 * Revision 1.40  2000/02/16 08:18:35  naniwa
 * to reorder POSIX system calls
 *
 * Revision 1.39  1999/03/21 13:59:44  monaka
 * They are separated from syscall.c.
 *
 * Revision 1.38  1999/03/21 00:48:18  monaka
 * Major fix. Return values in some functions was modified. It should be either SUCCESS/FAIL. Some had EP_ as return value.
 *
 * Revision 1.37  1999/02/17 05:28:35  monaka
 * Changed func name and macros for sigsuspend and waitpid.
 *
 * Revision 1.36  1998/07/01 13:59:19  night
 * ifdef で指定したマクロ名が間違っていたので、正しい綴りに修正した。
 *
 * Revision 1.35  1998/06/11 15:29:59  night
 * alloca を使用しないよう変更。
 *
 * Revision 1.34  1998/02/24 14:20:29  night
 * デバッグ用プリント文の追加。
 *
 * Revision 1.33  1998/02/23 14:49:47  night
 * misc システムコールにコマンド M_VM_DUMP の処理を追加。
 *
 * Revision 1.32  1998/02/16 14:27:20  night
 * コメントの変更。
 * デバッグ用プリント文の追加。
 *
 * Revision 1.31  1998/01/06 16:41:13  night
 * exec システムコールを実行したプロセスのプロセス ID が 0 の場合には、
 * exec システムコール終了後も返答メッセージを送るようにした。
 *
 * Revision 1.30  1997/12/17 14:17:47  night
 * システムコール処理関数 psc_fork_f の中身を作成。
 *
 * Revision 1.29  1997/10/24 14:00:29  night
 * 変数の引数合わせを厳密にした。
 * 使用していない変数を削除した。
 *
 * Revision 1.28  1997/10/23 14:32:33  night
 * exec システムコール関係の処理の更新
 *
 * Revision 1.27  1997/10/22 14:58:47  night
 * 不要な関数 psc_fseek_f (), psc_fstat_f () を削除した。
 * psc_rewinddir_f () の中身を作成した。
 * psc_rmdir_f () の中で使っているパラメータの構造体が par_remove だった
 * ので、par_rmdir に変更した。
 *
 * Revision 1.26  1997/10/11 16:25:19  night
 * ファイルの write 処理の修正。
 *
 * Revision 1.25  1997/09/21 13:35:10  night
 * write システムコールのエントリ関数 (psc_write_f) にデバッグ用のプリン
 * ト文を追加。
 *
 * Revision 1.24  1997/09/09 13:50:32  night
 * POSIX のファイルへの書き込み処理(システムコールは、write) を追加。
 * これまでは、EP_NOSUP を返していた。
 *
 * Revision 1.23  1997/08/31 13:35:02  night
 * 各種システムコールの中身を作成した。
 *
 * Revision 1.22  1997/07/07 15:25:12  night
 * psc_getegid_f、psc_geteuid_f、psc_getgid_f、psc_getuid_f、psc_rewind_f
 * の各関数の中身を作成した。
 *
 * Revision 1.21  1997/07/07 15:15:41  night
 * psc_chdir_f() の中身を作成した。
 *
 * Revision 1.20  1997/07/07 14:41:57  night
 * psc_chmod_f と psc_chown_f の中身を作成。
 *
 * Revision 1.19  1997/07/07 14:28:47  night
 * close システムコール終了時に 呼び出し側に対してレスポンスを返していな
 * かったで、レスポンスを返すように修正した。
 *
 * Revision 1.18  1997/07/06 11:56:55  night
 * close システムコールの処理を追加。
 *
 * Revision 1.17  1997/07/04 15:07:39  night
 * ・スペシャルファイル - デバイスドライバポートの対応表の関連処理の追加。
 * ・ファイルの読み込み処理の改訂。
 *
 * Revision 1.16  1997/07/03 14:24:30  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.15  1997/07/02 13:26:16  night
 * statfs システムコールの中身を作成
 *
 * Revision 1.14  1997/05/14 14:10:36  night
 * open システムコール実行時にパス名を入れる領域に '\0' を入れるための 1
 * バイト文を余分に確保するように修正した。
 *
 * Revision 1.13  1997/05/12 14:31:52  night
 * misc システムコールに M_PROC_DUMP コマンドを追加。
 *
 * Revision 1.12  1997/05/08 15:11:30  night
 * プロセスの情報を設定する機能の追加。
 * (syscall misc の proc_set_info コマンド)
 *
 * Revision 1.11  1997/05/06 12:47:57  night
 * set_procinfo システムコールの追加。
 *
 * Revision 1.10  1997/04/28 15:29:24  night
 * デバッグ用の文を追加。
 *
 * Revision 1.9  1997/04/25 13:01:01  night
 * statfs システムコールの追加。
 *
 * Revision 1.8  1997/04/24 15:40:30  night
 * mountroot システムコールの実装を行った。
 *
 * Revision 1.7  1997/03/25 13:34:54  night
 * ELF 形式の実行ファイルへの対応
 *
 * Revision 1.6  1996/11/20  12:13:02  night
 * 各種システムコールの追加。
 *
 * Revision 1.5  1996/11/18  13:44:43  night
 * psc_read_f() の中身を作成。
 *
 * Revision 1.4  1996/11/14  13:18:01  night
 * ファイル処理の中身を作成(まだ途中)。
 *
 * Revision 1.3  1996/11/10  11:55:29  night
 * デバッグ文の追加。
 *
 * Revision 1.2  1996/11/08  11:04:49  night
 * 文字コードを SJIS から EUC に変更した。
 *
 * Revision 1.1  1996/11/07  12:49:19  night
 * 最初の登録。
 *
 *
 */


#include "posix.h"


struct posix_syscall	syscall_table[] =
{
  { "noaction",		PSC_NOACTION,		psc_noaction_f },
  { "access",		PSC_ACCESS,		psc_access_f },
  { "chdir",		PSC_CHDIR,		psc_chdir_f },
  { "chmod",		PSC_CHMOD,		psc_chmod_f },
  { "chown",		PSC_CHOWN,		psc_chown_f },
  { "close",		PSC_CLOSE,		psc_close_f },
  { "creat",		PSC_CREAT,		psc_creat_f },
  { "dup",		PSC_DUP,		psc_dup_f },
  { "exec",		PSC_EXEC,		psc_exec_f },
  { "exit",		PSC_EXIT,		psc_exit_f },
  { "fcntl",		PSC_FCNTL,		psc_fcntl_f },
  { "fork",		PSC_FORK,		psc_fork_f },
  { "fstat",		PSC_FSTAT,		psc_noaction_f },
  { "getegid",		PSC_GETEGID,		psc_getegid_f },
  { "geteuid",		PSC_GETEUID,		psc_geteuid_f },
  { "getgid",		PSC_GETGID,		psc_getgid_f },
  { "getpgrp",		PSC_GETPGRP,		psc_getpgrp_f },
  { "getpid",		PSC_GETPID,		psc_getpid_f },
  { "getppid",		PSC_GETPPID,		psc_getppid_f },
  { "getuid",		PSC_GETUID,		psc_getuid_f },
  { "link",		PSC_LINK,		psc_link_f },
  { "lseek",		PSC_LSEEK,		psc_lseek_f },
  { "mkdir",		PSC_MKDIR,		psc_mkdir_f },
  { "open",		PSC_OPEN,		psc_open_f },
  { "pipe",		PSC_PIPE,		psc_pipe_f },
  { "read",		PSC_READ,		psc_read_f },
  { "rename",		PSC_RENAME,		psc_rename_f },
  { "rmdir",		PSC_RMDIR,		psc_rmdir_f },
  { "setgid",		PSC_SETGID,		psc_setgid_f },
  { "setpgid",		PSC_SETPGID,		psc_setpgid_f },
  { "setsid",		PSC_SETSID,		psc_setsid_f },
  { "setuid",		PSC_SETUID,		psc_setuid_f },
  { "stat",		PSC_STAT,		psc_stat_f },
  { "time",		PSC_TIME,		psc_time_f },
  { "times",		PSC_TIMES,		psc_times_f },
  { "umask",		PSC_UMASK,		psc_umask_f },
  { "unlink",		PSC_UNLINK,		psc_unlink_f },
  { "utime",		PSC_UTIME,		psc_utime_f },
  { "waitpid",		PSC_WAITPID,		psc_waitpid_f },
  { "write",		PSC_WRITE,		psc_write_f },
  { "brk",		PSC_BRK,		psc_noaction_f },
  { "chroot", 		PSC_CHROOT,		psc_noaction_f },
  { "getdents",		PSC_GETDENTS,		psc_getdents_f },
  { "mknod", 		PSC_MKNOD,		psc_noaction_f },
  { "mount",		PSC_MOUNT,		psc_mount_f },
  { "mountroot",	PSC_MOUNTROOT,		psc_mountroot_f },
  { "statfs",		PSC_STATFS,		psc_statfs_f },
  { "stime", 		PSC_STIME,		psc_noaction_f },
  { "sync", 		PSC_SYNC, 		psc_noaction_f },
  { "umount",		PSC_UMOUNT,		psc_umount_f },
  { "alarm", 		PSC_ALARM,		psc_noaction_f },
  { "kill",		PSC_KILL,		psc_kill_f },
  { "pause",		PSC_PAUSE,		psc_pause_f },
  { "sigaction",	PSC_SIGACTION,		psc_sigaction_f },
  { "sigreturn", 	PSC_SIGRETURN,		psc_noaction_f },
  { "sigprocmask",	PSC_SIGPROCMASK,	psc_sigprocmask_f },
  { "sigpending",	PSC_SIGPENDING,		psc_sigpending_f },
  { "sigsuspend",	PSC_SIGSUSPEND,		psc_sigsuspend_f },
  { "sigaddset",	PSC_SIGADDSET,		psc_sigaddset_f },
  { "sigdelset",	PSC_SIGDELSET,		psc_sigdelset_f },
  { "sigemptyset",	PSC_SIGEMPTYSET,	psc_sigemptyset_f },
  { "sigfillset",	PSC_SIGFILLSET,		psc_sigfillset_f },
  { "sigismember",	PSC_SIGISMEMBER,	psc_sigismember_f },
  { "siglongjmp",	PSC_SIGLONGJMP,		psc_siglongjmp_f },
  { "sigsetjmp",	PSC_SIGSETJMP,		psc_sigsetjmp_f },
  { "misc",		PSC_MISC,		psc_misc_f },
  { "memory",		PSC_MEMORY,		psc_memory_f },
  { "dup2",		PSC_DUP2,		psc_dup2_f },
  { "getcwd",		PSC_GETCWD,		psc_getcwd_f },
  { "getenv",		PSC_GETENV,		psc_getenv_f },
  { "getgrgid",		PSC_GETGRGID,		psc_getgrgid_f },
  { "getgrnam",		PSC_GETGRNAM,		psc_getgrnam_f },
  { "getgroups",	PSC_GETGROUPS,		psc_getgroups_f },
  { "getlogin",		PSC_GETLOGIN,		psc_getlogin_f },
  { "mkfifo",		PSC_MKFIFO,		psc_mkfifo_f },
  { "remove",		PSC_REMOVE,		psc_remove_f },
  { "rewind",		PSC_REWIND,		psc_rewind_f },
  { "sleep",		PSC_SLEEP,		psc_sleep_f },
  { "ttyname",		PSC_TTYNAME,		psc_ttyname_f },
  { "tzset",		PSC_TZSET,		psc_tzset_f },
  { "uname",		PSC_UNAME,		psc_uname_f }
};
