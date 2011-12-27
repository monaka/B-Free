/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: global.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1998-02-25 12:47:52  night
 * プロセス毎の固有データ情報を、それぞれ大域変数で固定的にもつのではなく、
 * lowlib_data というポインタで管理するようにした。
 * lowlib_data の値は、マクロ LOWLIB_DATA で指定したアドレス (0x7fff0000)
 * に設定される。このアドレスは、プロセス毎に違った物理ページにマッピング
 * される。仮想アドレスは固定だが、実際の領域はプロセス毎に違った場所に存
 * 在している。LOWLIB が管理するプロセス毎の情報はこの領域に入れるように
 * する。
 *
 * Revision 1.2  1997/08/31 13:13:42  night
 * とりあえず、OS ファイルに入れるところまでできた。
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC 版への最初の登録
 *
 * ----------------
 *
 * Revision 1.4  1995/09/21  15:52:56  night
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

#include "lowlib.h"


struct lowlib_data	*lowlib_data = (struct lowlib_data *)LOWLIB_DATA;

ID	posix_manager = 0;

int	errno;
