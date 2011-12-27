/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* posix_syscall.h - POSIX 環境マネージャ用のヘッダファイル
 *		     (システムコール関連の定義)
 *
 * Note:
 *	PSC (and psc) = Posix System Call
 *
 */

#ifndef __POSIX_SYSCALL_H__
#define __POSIX_SYSCALL_H__	1


#define NR_POSIX_SYSCALL	80

/* =================== POSIX システムコール番号の定義 =============== */
#define PSC_NOACTION     0
#define PSC_ACCESS       1
#define PSC_CHDIR        2
#define PSC_CHMOD        3
#define PSC_CHOWN        4
#define PSC_CLOSE        5
#define PSC_CREAT        6
#define PSC_DUP          7
#define PSC_EXEC         8
#define PSC_EXIT         9
#define PSC_FCNTL        10
#define PSC_FORK         11
#define PSC_FSTAT	 12
#define PSC_GETEGID      13
#define PSC_GETEUID      14
#define PSC_GETGID       15
#define PSC_GETPGRP      16
#define PSC_GETPID       17
#define PSC_GETPPID      18
#define PSC_GETUID       19
#define PSC_LINK         20
#define PSC_LSEEK        21
#define PSC_MKDIR        22
#define PSC_OPEN         23
#define PSC_PIPE         24
#define PSC_READ         25
#define PSC_RENAME       26
#define PSC_RMDIR        27
#define PSC_SETGID       28
#define PSC_SETPGID      29
#define PSC_SETSID       30
#define PSC_SETUID       31
#define PSC_STAT         32
#define PSC_TIME         33
#define PSC_TIMES        34
#define PSC_UMASK        35
#define PSC_UNLINK       36
#define PSC_UTIME        37
#define PSC_WAITPID      38
#define PSC_WRITE        39

#define PSC_BRK		 40
#define PSC_CHROOT	 41
#define PSC_GETDENTS	 42
#define PSC_MKNOD	 43
#define PSC_MOUNT        44
#define PSC_MOUNTROOT    45
#define PSC_STATFS       46
#define PSC_STIME	 47
#define PSC_SYNC	 48
#define PSC_UMOUNT       49

/* =================== SIGNAL 関係 =============== */
#define PSC_ALARM	 50
#define PSC_KILL         51
#define PSC_PAUSE        52
#define PSC_SIGACTION    53
#define PSC_SIGRETURN	 54
#define PSC_SIGPROCMASK  55
#define PSC_SIGPENDING   56
#define PSC_SIGSUSPEND   57
#define PSC_SIGADDSET    58	/* 不要? */
#define PSC_SIGDELSET    59	/* 不要? */
#define PSC_SIGEMPTYSET  60	/* 不要? */
#define PSC_SIGFILLSET   61	/* 不要? */
#define PSC_SIGISMEMBER  62	/* 不要? */
#define PSC_SIGLONGJMP   63	/* 不要? */
#define PSC_SIGSETJMP    64	/* 不要? */

/* =================== miserous system calls  =============== */
#define PSC_MISC         65

#define PSC_MEMORY       66

#define PSC_DUP2         67	/* 不要? */
#define PSC_GETCWD       68	/* 不要? */
#define PSC_GETENV       69	/* 不要? */

#define PSC_GETGRGID     70	/* 不要? */
#define PSC_GETGRNAM     71	/* 不要? */
#define PSC_GETGROUPS    72	/* 不要? */
#define PSC_GETLOGIN     73	/* 不要? */

#define PSC_MKFIFO       74	/* 不要? */

#define PSC_REMOVE       75	/* 不要? */
#define PSC_REWIND       76	/* 不要? */
#define PSC_SLEEP        77	/* 不要? */

#define PSC_TTYNAME      78	/* 不要? */
#define PSC_TZSET        79	/* 不要? */

#define PSC_UNAME        80	/* 不要? */

/* MISC 
 */
#define M_SET_PROCINFO	1
#define M_PROC_DUMP	2
#define M_VM_DUMP	3
#define M_PRINT_FLIST	4


/* =============== 各システムコール用の 構造体定義 ================== */

/* psc_access -
 *
 */
struct psc_access
{
  W		pathlen;
  B		*path;
  W		accflag;
};


/* psc_chdir -
 *
 */
struct psc_chdir
{
  W		pathlen;
  B		*path;
};


/* psc_chmod -
 *
 */
struct psc_chmod
{
  W		pathlen;
  B		*path;
  W		mode;
};


/* psc_chown -
 *
 */
struct psc_chown
{
  W		pathlen;
  B		*path;
  W		uid;
  W		gid;
};


/* psc_close -
 *
 */
struct psc_close
{
  W		fileid;
};


/* psc_creat -
 *
 */
struct psc_creat
{
  W pathlen;
  B *path;
  W mode;
};


/* psc_dup -
 *
 */
struct psc_dup
{
  W		fileid;
};


/* psc_dup2 -
 *
 */
struct psc_dup2
{
  W		fileid1;
  W		fileid2;
};


/* psc_execve - 指定したプログラムファイルを読み込み、実行する
 *
 */
struct psc_execve
{
  UW		pathlen;
  B		*name;
  B		(*argv)[];
  B		(*envp)[];

#if 0
  FP		start_main;	/* メインタスク用のスタート関数 */
  FP		start_signal;	/* シグナルタスク用のスタート関数 */
#endif
};


/* psc_exit -
 *
 */
struct psc_exit
{
  W		evalue;
};


/* psc_fcntl -
 *
 */
struct psc_fcntl
{
  W fileid;
  W cmd;
  VP arg;
};


/* psc_fork -
 *
 */
struct psc_fork
{
  ID	main_task;
  ID	signal_task;
};



/* psc_getcwd -
 *
 */
struct psc_getcwd
{
  W		dirnamelen;
  B		*dirname;
};


/* psc_getegid -
 *
 */
struct psc_getegid
{
};


/* psc_getenv -
 *
 */
struct psc_getenv
{
};


/* psc_geteuid -
 *
 */
struct psc_geteuid
{
  /* have no value */
};


/* psc_getgid -
 *
 */
struct psc_getgid
{
  /* have no value */
};


/* psc_getgrgid -
 *
 */
struct psc_getgrgid
{
};


/* psc_getgrnam -
 *
 */
struct psc_getgrnam
{
  W grnamlen;
  B *grnam;
};


/* psc_getgroups -
 *
 */
struct psc_getgroups
{
  W grouplen;
  B *group;
};


/* psc_getlogin -
 *
 */
struct psc_getlogin
{
  W loginlen;
  B *login;
};


/* psc_getpgrp -
 *
 */
struct psc_getpgrp
{
};


/* psc_getpid -
 *
 */
struct psc_getpid
{
};


/* psc_getppid -
 *
 */
struct psc_getppid
{
};


/* psc_getuid -
 *
 */
struct psc_getuid
{
};


/* psc_kill -
 *
 */
struct psc_kill
{
  W		pid;
  W		signal;
};


/* psc_link -
 *
 */
struct psc_link
{
      W srclen;
      B *src;
      W dstlen;
      B *dst;
};


/* psc_lseek -
 *
 */
struct psc_lseek
{
  W		fileid;
  W		offset;
  W		mode;
};


/* psc_mkdir -
 *
 */
struct psc_mkdir
{
  W		pathlen;
  B		*path;
  W		mode;
};


/* psc_mkfifo -
 *
 */
struct psc_mkfifo
{
  W pathlen;
  B *path;
  W mode;
};


/* psc_open -
 *
 */
struct psc_open
{
  W		pathlen;
  B		*path;
  W		oflag;
  W		mode;	/* if oflag is O_CREATE */
};


/* psc_pause -
 *
 */
struct psc_pause
{
};


/* psc_pipe -
 *
 */
struct psc_pipe
{
  W		pipeid[2];
};


/* psc_read -
 *
 */
struct psc_read
{
  W	fileid;
  B	*buf;
  W	length;
};


/* psc_remove -
 *
 */
struct psc_remove
{
  W		pathlen;
  B		*path;
};


/* psc_rename -
 *
 */
struct psc_rename
{
  W		oldpathlen;
  B		*oldpath;
  W		newpathlen;
  B		*newpath;
};


/* psc_rewind -
 *
 */
struct psc_rewind
{
  W		fileid;
};


/* psc_rmdir -
 *
 */
struct psc_rmdir
{
  W		pathlen;
  B		*path;
};


/* psc_setgid -
 *
 */
struct psc_setgid
{
  UW		gid;
};


/* psc_setpgid -
 *
 */
struct psc_setpgid
{
  UW		gid;
};


/* psc_setsid -
 *
 */
struct psc_setsid
{
  W sid;
};


/* psc_setuid -
 *
 */
struct psc_setuid
{
  UW		uid;
};


/* psc_sigaction -
 *
 */
struct psc_sigaction
{
  int signo;
  struct sigaction *action;
  struct sigaction *oldaction;
};


/* psc_sigaddset -
 *
 */
struct psc_sigaddset
{
};


/* psc_sigdelset -
 *
 */
struct psc_sigdelset
{
};


/* psc_sigemptyset -
 *
 */
struct psc_sigemptyset
{
};


/* psc_sigfillset -
 *
 */
struct psc_sigfillset
{
};


/* psc_sigismember -
 *
 */
struct psc_sigismember
{
};


/* psc_siglongjmp -
 *
 */
struct psc_siglongjmp
{
};


/* psc_sigpending -
 *
 */
struct psc_sigpending
{
};


/* psc_sigprocmask -
 *
 */
struct psc_sigprocmask
{
};


/* psc_sigsetjmp -
 *
 */
struct psc_sigsetjmp
{
};


/* psc_sigsuspend -
 *
 */
struct psc_sigsuspend
{
};


/* psc_sleep -
 *
 */
struct psc_sleep
{
  W second;
};


/* psc_stat -
 *
 */
struct psc_stat
{
  W		fileid;
  struct stat	*st;
};


/* psc_time -
 *
 */
struct psc_time
{
  /* time_t *tm */
  W *tm;
};


/* psc_times -
 *
 */
struct psc_times
{
  struct tms *buf;
};


/* psc_ttyname -
 *
 */
struct psc_ttyname
{
  W namelen;
  B *name;
};


/* psc_tzset -
 *
 */
struct psc_tzset
{
};


/* psc_umask -
 *
 */
struct psc_umask
{
  W	umask;
};


/* psc_uname -
 *
 */
struct psc_uname
{
  struct utsname	*uname;
};


/* psc_unlink -
 *
 */
struct psc_unlink
{
  W		pathlen;
  B		*path;
};


/* psc_utime -
 *
 */
struct psc_utime
{
      W pathlen;
      B *path;
      struct utimbuf *buf;
};


/* psc_waitpid -
 *
 */
struct psc_waitpid
{
  W pid;
  W *statloc;
  W opts;
};


/* psc_write -
 *
 */
struct psc_write
{
  W	fileid;
  B	*buf;
  W	length;
};


/* psc_mount -
 *
 */
struct psc_mount
{
  B	devnamelen;    		/* マウントするデバイス名の長さ */
  B	*devname;		/* マウントするデバイス名の長さ */
  W	dirnamelen;		/* マウントするディレクトリ名の長さ */
  B	*dirname;		/* マウントするディレクトリ名 */
  W	fstype;			/* ファイルシステムのタイプ */
  W	option;			/* オプション */
};


/* psc_umount -
 *
 */
struct psc_umount
{
  W	dirnamelen;
  B	*dirname;
  W	option;
};


/* psc_memory -
 *
 */
struct psc_memory
{
  W	command;
  UW	vaddr;
  UW	paddr;
};


/* psc_mountroot -
 *
 */
struct psc_mountroot
{
  ID	device;
  W	fstype;
  W	option;
};


/* psc_statfs -
 *
 */
struct psc_statfs
{
  ID		device;
  struct statfs	*fsp;
};


/* psc_getdents -
 *
 */
struct psc_getdents
{
  UW fileid;
  VP buf;
  UW length;
};

struct procinfo
{
  ID			proc_maintask;
  ID			proc_signal_handler;

  W			proc_uid;
  W			proc_gid;
  W			proc_euid;
  W			proc_egid;

  W			proc_umask;

  UW			proc_pid;		/* my process ID */
  UW			proc_ppid;		/* parent process ID */

  UW			proc_access;
};

/* psc_set_procinfo
 *
 */
struct psc_misc
{
  W		cmd;
  W		length;
  union
    {
      struct procinfo	set_procinfo;
      W			procid;
    } arg;
};

/* =========================== 構造体定義 =========================== */


/* POSIX マネージャへの要求メッセージの構造体 */
struct posix_request
{
  ID	receive_port;		/* 要求受信用のポート	*/
  ID	caller;			/* 呼び出し元のタスク ID */
  ID	procid;			/* 呼び出し元のプロセス ID */
  W	msg_length;		/* メッセージの長さ	*/
  W	operation;		/* 要求番号(システムコールに対応)	*/

  union {
    struct psc_access		par_access;
    struct psc_chdir		par_chdir;
    struct psc_chmod		par_chmod;
    struct psc_chown		par_chown;
    struct psc_close		par_close;
    struct psc_creat		par_creat;
    struct psc_dup		par_dup;
    struct psc_dup2		par_dup2;
    struct psc_execve		par_execve;
    struct psc_exit		par_exit;
    struct psc_fcntl		par_fcntl;
    struct psc_fork		par_fork;
    struct psc_getcwd		par_getcwd;
    struct psc_getegid		par_getegid;
    struct psc_getenv		par_getenv;
    struct psc_geteuid		par_geteuid;
    struct psc_getgid		par_getgid;
    struct psc_getgrgid		par_getgrgid;
    struct psc_getgrnam		par_getgrnam;
    struct psc_getgroups	par_getgroups;
    struct psc_getlogin		par_getlogin;
    struct psc_getpgrp		par_getpgrp;
    struct psc_getpid		par_getpid;
    struct psc_getppid		par_getppid;
    struct psc_getuid		par_getuid;
    struct psc_kill		par_kill;
    struct psc_link		par_link;
    struct psc_lseek		par_lseek;
    struct psc_mkdir		par_mkdir;
    struct psc_mkfifo		par_mkfifo;
    struct psc_open		par_open;
    struct psc_pause		par_pause;
    struct psc_pipe		par_pipe;
    struct psc_read		par_read;
    struct psc_remove		par_remove;
    struct psc_rename		par_rename;
    struct psc_rewind		par_rewind;
    struct psc_rmdir		par_rmdir;
    struct psc_setgid		par_setgid;
    struct psc_setpgid		par_setpgid;
    struct psc_setsid		par_setsid;
    struct psc_setuid		par_setuid;
    struct psc_sigaction	par_sigaction;
    struct psc_sigaddset	par_sigaddset;
    struct psc_sigdelset 	par_sigdelset;
    struct psc_sigemptyset	par_sigemptyset;
    struct psc_sigfillset 	par_sigfillset;
    struct psc_sigismember	par_sigismember;
    struct psc_siglongjmp	par_siglongjmp;
    struct psc_sigpending 	par_sigpending;
    struct psc_sigprocmask	par_sigprocmask;
    struct psc_sigsetjmp  	par_sigsetjmp;
    struct psc_sigsuspend	par_sigsuspend;
    struct psc_sleep 		par_sleep;
    struct psc_stat  		par_stat;
    struct psc_time  		par_time;
    struct psc_times 		par_times;
    struct psc_ttyname		par_ttyname;
    struct psc_tzset 		par_tzset;
    struct psc_umask 		par_umask;
    struct psc_uname 		par_uname;
    struct psc_unlink     	par_unlink;
    struct psc_utime 		par_utime;
    struct psc_waitpid   	par_waitpid;
    struct psc_write 		par_write;
    struct psc_mount		par_mount;
    struct psc_umount		par_umount;
    struct psc_memory		par_memory;
    struct psc_mountroot	par_mountroot;
    struct psc_statfs		par_statfs;
    struct psc_misc		par_misc;
    struct psc_getdents		par_getdents;
  } param;
};



struct posix_response
{
  ID	receive_port;		/* 要求受信用のポート	*/
  W	msg_length;		/* メッセージの長さ	*/
  W	operation;		/* 要求番号(システムコールに対応)	*/
  W	errno;			/* エラー番号 */
  W	status;			/* ステータス(リターン値) */

  W	ret1;			/* サブに使用するリターン値 */
  W	ret2;			/* サブに使用するリターン値 */
};


struct posix_syscall
{
  B	*name;
  W	callno;
  W	(*syscall)(struct posix_request *);
};


extern struct posix_syscall	syscall_table[];


extern W	psc_noaction_f (struct posix_request *);
extern W	psc_access_f (struct posix_request *);
extern W	psc_chdir_f (struct posix_request *);
extern W	psc_chmod_f (struct posix_request *);
extern W	psc_chown_f (struct posix_request *);
extern W	psc_close_f (struct posix_request *);
extern W	psc_creat_f (struct posix_request *);
extern W	psc_dup_f (struct posix_request *);
extern W	psc_dup2_f (struct posix_request *);
extern W	psc_exec_f (struct posix_request *);
extern W	psc_exit_f (struct posix_request *);
extern W	psc_fcntl_f (struct posix_request *);
extern W	psc_fork_f (struct posix_request *);
extern W	psc_getcwd_f (struct posix_request *);
extern W	psc_getegid_f (struct posix_request *);
extern W	psc_getenv_f (struct posix_request *);
extern W	psc_geteuid_f (struct posix_request *);
extern W	psc_getgid_f (struct posix_request *);
extern W	psc_getgrgid_f (struct posix_request *);
extern W	psc_getgrnam_f (struct posix_request *);
extern W	psc_getgroups_f (struct posix_request *);
extern W	psc_getlogin_f (struct posix_request *);
extern W	psc_getpgrp_f (struct posix_request *);
extern W	psc_getpid_f (struct posix_request *);
extern W	psc_getppid_f (struct posix_request *);
extern W	psc_getuid_f (struct posix_request *);
extern W	psc_kill_f (struct posix_request *);
extern W	psc_link_f (struct posix_request *);
extern W	psc_lseek_f (struct posix_request *);
extern W	psc_mkdir_f (struct posix_request *);
extern W	psc_mkfifo_f (struct posix_request *);
extern W	psc_open_f (struct posix_request *);
extern W	psc_pause_f (struct posix_request *);
extern W	psc_pipe_f (struct posix_request *);
extern W	psc_read_f (struct posix_request *);
extern W	psc_remove_f (struct posix_request *);
extern W	psc_rename_f (struct posix_request *);
extern W	psc_rewind_f (struct posix_request *);
extern W	psc_rmdir_f (struct posix_request *);
extern W	psc_setgid_f (struct posix_request *);
extern W	psc_setpgid_f (struct posix_request *);
extern W	psc_setsid_f (struct posix_request *);
extern W	psc_setuid_f (struct posix_request *);
extern W	psc_sigaction_f (struct posix_request *);
extern W	psc_sigaddset_f (struct posix_request *);
extern W	psc_sigdelset_f (struct posix_request *);
extern W	psc_sigemptyset_f (struct posix_request *);
extern W	psc_sigfillset_f (struct posix_request *);
extern W	psc_sigismember_f (struct posix_request *);
extern W	psc_siglongjmp_f (struct posix_request *);
extern W	psc_sigpending_f (struct posix_request *);
extern W	psc_sigprocmask_f (struct posix_request *);
extern W	psc_sigsetjmp_f (struct posix_request *);
extern W	psc_sigsuspend_f (struct posix_request *);
extern W	psc_sleep_f (struct posix_request *);
extern W	psc_stat_f (struct posix_request *);
extern W	psc_time_f (struct posix_request *);
extern W	psc_times_f (struct posix_request *);
extern W	psc_ttyname_f (struct posix_request *);
extern W	psc_tzset_f (struct posix_request *);
extern W	psc_umask_f (struct posix_request *);
extern W	psc_uname_f (struct posix_request *);
extern W	psc_unlink_f (struct posix_request *);
extern W	psc_utime_f (struct posix_request *);
extern W	psc_waitpid_f (struct posix_request *);
extern W	psc_write_f (struct posix_request *);
extern W	psc_mount_f (struct posix_request *);
extern W	psc_umount_f (struct posix_request *);
extern W	psc_memory_f (struct posix_request *);
extern W	psc_mountroot_f (struct posix_request *);
extern W	psc_statfs_f (struct posix_request *);
extern W	psc_misc_f (struct posix_request *);
extern W	psc_getdents_f (struct posix_request *);

#endif /* #define __POSIX_SYSCALL_H__ */
