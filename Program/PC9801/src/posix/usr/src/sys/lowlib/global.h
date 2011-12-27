/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/global.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: global.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:57  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:26:24  night
 * 次の大域変数の定義を追加。
 *
 * errno		エラー番号用の変数
 * main_task	ユーザプログラムのコードを実行するタスク
 * signal_task	シグナルの受信処理を行うタスク
 * fifo_task	パイプを使うときに使用するタスク
 * alarm_task	alarm システムコール用のタスク
 * efile		実行ファイルを指しているメッセージポート
 * 		コード部のページインのときに使用する
 *
 * Revision 1.1  1995/02/21  15:10:46  night
 * はじめての登録
 *
 *
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__	1

#include <sys/types.h>

/*
 * プロセスの中でユーザプログラムを実行している主タスクのタスク ID
 */
ID	main_task;

/*
 * シグナルを処理するためのタスクのタスク ID
 */
ID	signal_task;


/*
 * LOWLIB が動いているプロセスの ID
 */
extern pid_t	my_pid;


/*
 * システムコールのエラー番号を記憶しておくための変数
 */
extern int	errno;

extern ID	main_task;	/* ユーザプログラムのコードを実行するタスク */
extern ID	signal_task;	/* シグナルの受信処理を行うタスク */
extern ID	fifo_task;	/* パイプを使うときに使用するタスク */
extern ID	alarm_task;	/* alarm システムコール用のタスク */
  
extern ID	efile;		/* 実行ファイルを指しているメッセージポート 
				 * コード部のページインのときに使用する。
				 */

#endif /* __GLOBAL_H__ */
