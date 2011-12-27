/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: global.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:52:56  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/03/18  14:25:49  night
 * 次の大域変数の宣言を追加。
 *
 * main_task	ユーザプログラムのコードを実行するタスク
 * signal_task	シグナルの受信処理を行うタスク
 * fifo_task	パイプを使うときに使用するタスク
 * alarm_task	alarm システムコール用のタスク
 * efile		実行ファイルを指しているメッセージポート
 * 		コード部のページインのときに使用する
 *
 * Revision 1.2  1995/02/20  15:22:12  night
 * #include <sys/types.h> のインクルード文を追加。
 * #include <posix.h> のインクルード文を削除した。
 *
 * Revision 1.1  1995/02/20  15:16:42  night
 * はじめての登録
 *
 *
 */

#include <sys/types.h>
#include "global.h"


ID	main_task;	/* ユーザプログラムのコードを実行するタスク */
ID	signal_task;	/* シグナルの受信処理を行うタスク */
ID	fifo_task;	/* パイプを使うときに使用するタスク */
ID	alarm_task;	/* alarm システムコール用のタスク */
  
ID	efile;		/* 実行ファイルを指しているメッセージポート 
			 * コード部のページインのときに使用する。
			 */

