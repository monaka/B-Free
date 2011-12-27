/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/entry.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/entry.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: entry.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-11-11 13:36:05  night
 * IBM PC 版への最初の登録
 *
 * ----------------
 *
 * Revision 1.3  1995/09/21  15:52:56  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:51:57  night
 * 註釈の変更
 *
 * Revision 1.1  1995/02/20  15:16:41  night
 * はじめての登録
 *
 *
 */

#include "lowlib.h"


/*
 * POSIX システムコールのエントリポイント
 *
 * 引数：
 *	sysno	システムコール番号
 *	uargp	システムコールの引数(群)を示すポインタ
 *	errnop	errno を入れるためのポインタ
 *
 *
 * 返り値：
 *	システムコールの結果
 *
 * 機能：
 *	この関数は、POSIX システムコールの入り口となる。
 *	引数 stack_top はユーザプログラムのスタックを指す。
 *	stack_top の先は、次のような構造をもつ。
 *
 *
 *	ユーザプログラムがトラップをかけるときには、
 *	LOWLIB に処理を指示するために、各レジスタに次の値を入れておく。 
 *	ITRON カーネル (すなわち、トラップがかかったときに本当に一番最
 *	初に処理する部分) は、AX, BX, CX, DX の各レジスタの内容は関知
 *	していない。 単に ITRON システムコールによってあらかじめ指定さ
 *	れていたトラップハンドラをコールするだけである。
 *
 *	AX	システムコール番号
 *	BX	システムコールの引数を指すポインタ
 *	CX	errno の値を入れるための領域を示すポインタ
 *	DX	unused (未使用)
 *
 *	システムコールが終了したときには、これらのレジスタには別の値が
 *	入る。
 *
 *	AX	システムコールの実行結果(返り値) ...... errno の値では
 *		ない。
 *	BX	不定
 *	CX	不定
 *	DX	不定
 *
 *	システムコールの処理が終了すると、ITRON カーネルのトラップの入口
 *	に戻る。そのとき、AX レジスタ(トラップの処理結果)の値だけをセッ
 *	トする。
 *
 *	なお、ユーザプログラムが、POSIX システムコールを呼ぶとき(トラッ
 *	プを実行したとき)には タスクは、切りかわらない。そのため、引数
 *	にポインタがあった場合、そのままアクセスできる。特別な処理は必
 *	要がない(もちろん、システムコールの処理を別の仮想空間が異なっ
 *	ているタスクが行う場合には、実際の物理アドレスが異なってくるため、
 *	特別な処理が必要になる。
 *
 */
int
posix_entry (int sysno, void *uargp, int *errnop)
{
  int	result;

  /*
   * システムコール番号のチェック。
   * もし、システムコール番号が
   */
  if ((sysno < 0) || (sysno > nsyscall))
    {
      *errnop = EINVAL;
      return (0);
    }

  /*
   * POSIX API を担当する各関数を呼び出す。
   */
  result = (int)(syscalls[sysno].func)(uargp);
  
  /*
   * システムコールの処理が終了した。
   *
   * 大域変数 errno にはシステムコールの結果 (エラー番号)が入っている
   * それをユーザに渡すために、ポインタ errnop が示す領域に値を入れる。
   * (LOWLIB は各プロセスにひとつひとつ別個にデータ領域をもつ。そのた
   * め、errno の値も各プロセスにひとつずつ存在している。だから、errno
   * は、プロセスごとに別の値をもつことができる)
   */
  *errnop = errno;
  return (result);
}

