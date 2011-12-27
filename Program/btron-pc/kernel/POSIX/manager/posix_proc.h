/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* posix_proc.h - POSIX プロセス管理
 *
 *
 * $Log: posix_proc.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.16  2000-05-06 03:52:26  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.15  2000/01/15 15:29:29  naniwa
 * minor fix
 *
 * Revision 1.14  1999/07/23 14:39:01  naniwa
 * modified to implement exit
 *
 * Revision 1.13  1999/07/21 15:10:06  naniwa
 * modified to implement waitpid
 *
 * Revision 1.12  1999/03/24 04:52:09  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.11  1998/05/23 15:32:43  night
 * enum 宣言の最後の要素についていた ',' を削除した。
 * (ANSI の文法では、最後の要素に は、',' はつけられない)
 *
 * Revision 1.10  1998/01/06 16:37:11  night
 * proc 構造体にプロセスが使っているメモリ領域の情報用のエントリを追加し
 * た。
 *
 * テキスト領域
 * UW			text_start_addr;
 * UW			text_length;
 *
 * データ領域
 * UW			data_start_addr;
 * UW			data_length;
 *
 * BSS 領域
 * UW			bss_start_addr;
 * UW			bss_length;
 *
 * Revision 1.9  1997/12/17 14:16:21  night
 * 関数 fork() の引数変更に伴う、関数プロトタイプの変更。
 *
 * Revision 1.8  1997/10/24 13:59:26  night
 * enum proc_status の追加。
 * 関数定義の追加。
 *
 * Revision 1.7  1997/10/23 14:32:33  night
 * exec システムコール関係の処理の更新
 *
 * Revision 1.6  1997/05/08 15:11:30  night
 * プロセスの情報を設定する機能の追加。
 * (syscall misc の proc_set_info コマンド)
 *
 * Revision 1.5  1997/03/25 13:34:53  night
 * ELF 形式の実行ファイルへの対応
 *
 * Revision 1.4  1996/11/18  13:43:47  night
 * ファイル構造体にファイルの種類(通常ファイルかパイプか)を示す、
 * f_flag を追加。
 *
 * Revision 1.3  1996/11/17  16:55:28  night
 * 文字コードを EUC にした。
 *
 * Revision 1.2  1996/11/17  16:47:49  night
 * 以下の要素を追加
 *
 *    struct proc		*proc_next;
 *    W			proc_euid;
 *    W			proc_egid;
 *    W			proc_umask;
 *
 * Revision 1.1  1996/11/14  13:18:15  night
 * 最初の登録
 *
 *
 */

#ifndef __POSIX_PROC_H__
#define __POSIX_PROC_H__	1


#define F_FILE		0x0001
#define F_PIPE		0x0002


struct file
{
  struct proc		*f_procp;
  struct inode		*f_inode;
  W			f_flag;
  W			f_offset;	/* current offset */
  W			f_omode;
};


enum proc_status
{
  PS_DORMANT,		/* 未生成状態 */
  PS_SLEEP,		/* sleep 状態 */
  PS_WAIT,		/* wait 状態 */
  PS_RUN,		/* run 状態 */
  PS_ZOMBIE		/* zombie 状態 */
};

struct proc
{
  struct proc		*proc_next;

  enum proc_status	proc_status;		/* プロセスの状態を示す */

  ID			proc_maintask;		/* メインタスク */
  ID			proc_signal_handler;	/* シグナルハンドラタスク */

  W			proc_uid;
  W			proc_gid;
  W			proc_euid;
  W			proc_egid;

  W			proc_umask;

  struct inode		*proc_workdir;
  struct file		proc_open_file[MAX_OPEN];

  UW			proc_pid;		/* my process ID 
						 * この値が 0 のときは、このエントリは、
						 * 使っていない。
						 */
  
  UW			proc_ppid;		/* parent process ID */

  UW			proc_pgid;		/* process group ID */
  UW			proc_wpid;		/* pid parameter of waitpid */
  UW			proc_exst;		/* exit status */
  UW			proc_rvpt;		/* receive_port */
 

  struct vm_tree	*vm_tree;		/* 仮想空間の情報 
						 * 仮想ページと物理ページ/スワップ情報を管理する
						 */

  UW			text_start_addr;
  UW			text_length;

  UW			data_start_addr;
  UW			data_length;

  UW			bss_start_addr;
  UW			bss_length;
};



#ifdef KERNEL

/* process.c */

extern struct proc	proc_table[MAX_PROCESS];

extern W		init_process (void);
extern W		proc_set_info (struct posix_request *req);
extern W		proc_get_uid (W procid, W *uid);
extern W		proc_get_euid (W procid, W *uid);
extern W		proc_set_euid (W procid, W uid);
extern W		proc_get_gid (W procid, W *gid);
extern W		proc_get_egid (W procid, W *gid);
extern W		proc_get_pid (W procid, W *pid);
extern W		proc_get_ppid (W procid, W *ppid);
extern W		proc_set_egid (W procid, W gid);
extern W		proc_set_gid (W procid, W gid);
extern W		proc_alloc_fileid (W procid, W *retval);
extern W		proc_get_cwd (W procid, struct inode **cwd);
extern W		proc_set_cwd (W procid, struct inode *cwd);
extern W		proc_get_file (W procid, W fileid, struct file **fp);
extern W		proc_set_file (W procid, W fileid, W flag, struct inode *ip);
extern W		proc_rewind_file (W procid, W fileid);
extern W		proc_get_cwd (W procid, struct inode **cwd);
extern W		proc_set_umask (W procid, W umask);
extern W		proc_get_umask (W procid, W *umask);
extern W		proc_renew_task (W procid, FP main_funcp, FP signal_funcp, ID *new_main_task, ID *new_signal_task);
extern W		proc_dump (struct posix_request *req);
extern W		proc_get_procp (W procid, struct proc **procp);
extern W		proc_destroy_memory (W procid);
extern W		proc_exit (W procid);
extern W		proc_vm_dump (struct posix_request *req);
extern struct vm_tree	*proc_get_vmtree (W procid);
extern W		proc_alloc_proc (struct proc **procp);



/* exec.c */
extern W		exec_program (struct posix_request *req, W procid, B *pathname);


/* fork.c */
extern W		fork (struct proc *parent, W *childid, ID main_task, ID signal_task);
extern W		proc_duplicate (struct proc *source, struct proc *destination);

/* filesystem.c */
extern W		open_special_dev(struct proc *procp);

#endif

#endif /* __POSIX_PROC_H__ */
