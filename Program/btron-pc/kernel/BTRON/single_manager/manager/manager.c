/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/single_manager/manager/manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */

static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/single_manager/manager/manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: manager.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1998-11-10 14:57:43  night
 * main 関数の名前を startup に変更。
 * 註釈の追加。
 *
 * Revision 1.1  1998/10/23 17:18:40  night
 * 最初の登録
 *
 *
 */




/* このファイルは、BTRON single manager のメイン処理モジュールが
 * 入っている。
 */


#include "manager.h"

/* B-Free BTRON single manager のメイン処理関数
 *
 * 
 */
void
startup ()
{
  /* マネージャの資源の初期化
   */
  

  /* マネージャが要求を受信するためのポートを作成
   */


  /* ポートをポートマネージャに登録。登録名は、 "BTRON/manager"
   */


  /* マネージャ内の各モジュールの初期化
   */


  /* 処理ループ: ポートにデータが入ったかどうかをチェックし続ける
   */
  for (;;)
    {
    }

  /* ここには来ない */
}
