/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/pm.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: pm.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:44  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:28:32  night
 * <sys/errno.h> のインクルード文の追加。
 *
 * Revision 1.1  1995/03/18  14:02:37  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX 環境
 * プロセスマネージャで使用する構造体や定数の定義
 *
 */


#ifndef __PM_H__
#define __PM_H__	1

#include <sys/types.h>
#include <sys/errno.h>
#include <server/proc.h>

/*
 * プロセス構造体
 * プロセスマネージャが管理するプロセスひとつひとつについて
 * 管理する情報。
 *
 */
struct process
{
  struct process	*prev;
  struct process	*next;

  enum proc_status	status;	/* プロセスの状態を示す */
  pid_t			pid;	/* プロセス ID */

  uid_t	uid;		/* プロセスが属する所有者 */
  gid_t	gid;		/* プロセスが属するグループ */

  ID	main_task;	/* ユーザプログラムのコードを実行するタスク */
  ID	signal_task;	/* シグナルの受信処理を行うタスク */
  ID	fifo_task;	/* パイプを使うときに使用するタスク */
  ID	alarm_task;	/* alarm システムコール用のタスク */
  
  ID	efile;		/* 実行ファイルを指しているメッセージポート 
			 * コード部のページインのときに使用する。
			 */

  
};

typedef struct process	process_t;


#endif /* __PM_H__ */
