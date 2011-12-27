/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/mm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/mm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: mm.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:57  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/21  13:19:21  night
 * コメント追加
 *
 * Revision 1.1  1995/03/04  14:23:12  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX メモリ管理マネージャ (MM) と通信を行うための関数群
 *
 * 基本的にメモリ管理マネージャが処理するパケットの種類分だけ関数が存
 * 在している。
 *
 * このファイルの中で定義する関数は以下のとおり：
 *
 * vm_create	指定したタスクの仮想空間を生成する。
 * vm_remove	指定したタスクの仮想空間を削除する。
 * vm_write	仮想空間に情報を書き込む
 * vm_read	仮想空間の内容を読みこむ
 * vm_stat	仮想空間についての情報を取得する
 *
 */


#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include "funcs.h"

/*
 * 仮想メモリ空間の領域を生成するための関数
 *
 *
 */
int
vm_create (ID task, void *start, unsigned int size, int attr)
     /* task	仮想空間タスク ID */
     /* start	仮想空間のスタートアドレス */
     /* size	仮想空間の大きさ */
     /* attr	仮想空間の属性 */
{
  struct mm_arg		arg;
  struct mm_res		res;
  ID			sendport;
  ID			recvport;
  ER			error;

  /*
   * 送信先のメッセージポートの取得および返信メッセージの受信用のメッ
   * セージポートの作成。
   */
  recvport = get_port (sizeof (struct mm_res), 
		       sizeof (struct mm_res));
  if (recvport == 0)
    {
      return (E_NOMEM);
    }
  if (find_port (MEMORY_MANAGER_NAME, &sendport) != E_OK)
    {
      return (E_NOMEM);
    }

  /*
   * メモリマネージャに送るパケットを作成する。
   */
  arg.a.mm_create_arg.taskid = task;
  arg.a.mm_create_arg.start = start;
  arg.a.mm_create_arg.size = size;
  arg.a.mm_create_arg.attr = attr;
  arg.recvport = recvport;

  /*
   * メモリマネージャに対して、仮想空間を生成する要求を送る。
   */
  error = snd_mbf (sendport, sizeof (arg), &arg);
  if (error != E_OK)
    {
      /* エラーになってしまった。 */
      return (error);
    }

  /*
   * メモリマネージャからの返信メッセージを受信する。
   */
  error = rcv_mbf (recvport, sizeof (res));
  if (error != E_OK)
    {
      /* エラーになってしまった。 */
      return (error);
    }

  return (E_OK);
}

/*
 * 仮想空間を削除する要求をメモリマネージャに送る。
 *
 */
int
vm_delete (ID task, void *start)
     /* task
      * start
      */
{

}

       
