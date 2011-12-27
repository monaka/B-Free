/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/startup.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static unsigned char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/startup.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * POSIX 環境のための startup 関数の定義。
 */

#include <stdio.h>


/*
 * プログラムのエントリ関数。
 *
 * プログラムは main() ではなく、この __entry() 関数によって実行を開始する。
 *
 * この関数では次の処理を行う。
 *
 * 1) standard I/O 関数のための初期化
 * 2) main() 関数の呼び出し
 * 3) プログラムの終了のための後始末
 *
 *
 * POSIX プロセスの実行を開始するときには、スタックは次のようになっている。
 *
 *  	+----------------------+ <-- return address.
 *	|    Argument Count    | 
 *	+----------------------+
 *	|    Argument Vector   |
 *	+----------------------+
 *	|    Environment       |
 *	+----------------------+
 *	|                      |
 *      //   引数の文字列     //
 *	|    テーブル          |
 *	+----------------------+
 *	|                      |
 *	//   環境変数          //
 *	|      の収納領域      | <- user_stack_top (引数)
 *	+----------------------+
 *
 * entry 関数は、Argument Count, Argument Vector, Environment の3つの情報を
 * main 関数にそのまま引き渡す。 
 * 
 */
__entry (int ac, char **av, char *ae)
{
  int	errcode;

  setup_standard_io ();		/* stdin, stdout, stderr の初期化。 */
				/* setup_standard_io は、libc.a にある */
				/* 関数。*/

  errcode = main (ac, av, ae);	/* ユーザ関数 main の実行。このとき、 */
				/* 返ってきた値をプログラムの exit に */
				/* する。 */

  exit (errcode);		/* エラーコードを呼び出し元に返す */
}
