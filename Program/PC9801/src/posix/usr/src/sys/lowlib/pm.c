/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/pm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/pm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: pm.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:57  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/03/04  14:23:12  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX プロセス管理マネージャ (PM) と通信を行うための関数群
 *
 * 基本的にプロセス管理マネージャが処理するパケットの種類分だけ関数が存
 * 在している。
 *
 * このファイルの中で定義する関数は以下のとおり：
 *
 * proc_create
 * proc_atatch_task
 * proc_detach_task
 * proc_set_signal_port
 * proc_send_signal
 *
 */


#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include <server/proc.h>
#include "funcs.h"

/*
 * シグナルを受信するためのポートをプロセスマネージャに登録する。
 *
 */
int
proc_set_signal_port (pid_t pid, ID sigport)
{

  /*
   * プロセスマネージャに送るパケットを作成する。
   */

  /*
   * プロセスマネージャに対して、仮想空間を生成する要求を送る。
   */
  
}

