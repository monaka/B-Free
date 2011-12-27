/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/pageinfo.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/pageinfo.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * メモリマネージャが管理する仮想空間を情報を整理するためのモジュール
 *
 */

#include <sys/types.h>
#include <server/memory.h>


/* 仮想空間情報の定義
 * 
 * 各仮想空間には、次の情報が入る。
 *
 * プロセス ID
 * 仮想メモリ領域の開始番地
 * 仮想メモリ領域のサイズ
 * ページアウトデバイス
 * 属性
 * 各ページごとの情報
 *
 */
struct vpage_t
{
  
};


/*
 * 仮想メモリの情報テーブルの初期化
 */
init_pageinfo (void)
{
  
}
