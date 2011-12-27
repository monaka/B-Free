/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/global.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: global.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-02-16 08:16:47  naniwa
 * not to depend on sys/types.h
 *
 * Revision 1.3  1998/02/25 12:48:46  night
 * プロセス毎の固有データ情報を、それぞれ大域変数で固定的にもつのではなく、
 * lowlib_data というポインタで管理するようにした。
 * それに伴い、これまであったプロセス固有のデータを示す大域変数を削除した。
 *
 * lowlib_data の値は、マクロ LOWLIB_DATA で指定したアドレス (0x7fff0000)
 * に設定される。このアドレスは、プロセス毎に違った物理ページにマッピング
 * される。仮想アドレスは固定だが、実際の領域はプロセス毎に違った場所に存
 * 在している。LOWLIB が管理するプロセス毎の情報はこの領域に入れるように
 * する。
 *
 * Revision 1.2  1997/08/31 13:13:43  night
 * とりあえず、OS ファイルに入れるところまでできた。
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC 版への最初の登録
 *
 * ----------------
 *
 * Revision 1.3  1995/09/21  15:52:57  night
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
#if 0
#include <sys/types.h>
#endif

extern struct lowlib_data	*lowlib_data;
extern ID			posix_manager;

extern struct syscall_entry	syscalls[];
extern int			nsyscall;


#endif /* __GLOBAL_H__ */
