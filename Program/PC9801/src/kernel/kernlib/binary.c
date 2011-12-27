/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/binary.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/binary.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";


/*
 * $Log: binary.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:39  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/01/14  14:26:18  night
 * 最初の登録。
 * bzero(), bcopy() を作成した。
 *
 *
 */

/*
 * バイナリ関係の操作関数。
 *
 * 以下の関数を提供する。
 *
 *	bzero	領域を 0 クリアする。
 *	bcopy	領域のコピー
 *	
 *
 */


/*
 * ヘッダファイルの読み込み
 * ヘッダファイルのパスは、Makefile に指定してある。
 */
#include <types.h>


/*
 * 指定した領域を 0 クリアする。
 *
 * 注意：領域のアドレスはチェックしていないので、
 *	 メモリフォールトが生じる可能性がある。
 *
 * この関数は特に値は返さない。
 */
void
bzero (B *bufp, W size)
     /*
      * bufp	0 クリアする領域の先頭アドレス。
      * size	領域の大きさ。
      */
{
  while (size-- > 0)
    {
      *bufp++ = 0;
    }
}


/*
 * 領域のコピー
 *
 */
void
bcopy (B *buf1, B *buf2, W size)
{
  W	counter;	/* コピーしている位置を記憶するためのカウンタ */

  /*
   * 1バイトずつコピーしていく。
   */
  for (counter = 0; counter < size; counter++)
    {
      buf2[counter] = buf1[counter];
    }
}


