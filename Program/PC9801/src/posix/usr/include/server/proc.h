/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/server/proc.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: proc.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:59  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:47:56  night
 * 最初の登録
 *
 *
 */

#ifndef __SERVER_PM_H__
#define __SERVER_PM_H__	1

#define PROCESS_MANAGER_NAME	"posix.process_manager"

/*
 * プロセスの状態を表す enum 定義
 *
 */
enum proc_status { NONE, RUN, READY, SLEEP, WAIT };


/*
 * プロセスマネージャリクエスト
 */
enum proc_request
{
  PROC_NULL,	/* 何もしない 			*/
  PROC_GETSTAT,	/* プロセスの状態を得る		*/
  PROC_FORK,	/* 新しいプロセスを作成する	*/
};

typedef enum proc_request	proc_request_t;


/*
 * プロセスマネージャから送り返されるエラーの種類
 */
enum proc_error
{
  PROC_EOK,		/* 正常終了 */
  PROC_ENOSPT,		/* サポートしていないリクエスト */
};

typedef enum proc_error		proc_error_t;


/*
 * プロセスの状態取得のために使用するメッセージ
 *
 */
struct proc_getstat_req
{
  enum proc_status	status;	/* プロセスの状態を示す */
  pid_t			pid;	/* プロセス ID */
};


/*
 * PROC_GETSTAT の返答メッセージ
 */
struct proc_getstat_rep
{
  pid_t		pid;		/* プロセスの ID */
  uid_t		uid;		/* プロセスの所有者 ID */
  gid_t		gid;		/* プロセスのグループ ID */

  ID		main_task;	/* ユーザプログラムのコードを実行するタスク */
  ID		signal_task;	/* シグナルの受信処理を行うタスク */
  ID		fifo_task;	/* パイプを使うときに使用するタスク */
  ID		alarm_task;	/* alarm システムコール用のタスク */
};


/*
 * fork 処理を行うために使用するメッセージ
 *
 */
struct proc_fork_req
{
};


/*
 * プロセスマネージャへ送るためのメッセージ
 *
 *
 */
struct pm_msg
{
  /* この2つは、すべてのメッセージに共通 */
  proc_request_t	request;	/* リクエスト番号	*/
  ID			caller;		/* 呼び元へのポート	*/

  union
    {
      struct proc_getstat_req	proc_getstat_req;
    } body;
};

typedef struct pm_msg	pm_msg_t;


/*
 * プロセスマネージャからの返答メッセージ
 */
struct proc_reply
{
  proc_error_t	error;	/* 処理の結果のエラー番号 */

  union
    {
      struct proc_getstat_rep	proc_getstat_rep;
    } body;
};


#endif /* __SERVER_PM_H__ */
