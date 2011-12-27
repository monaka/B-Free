/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: fork.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  2000-04-03 14:33:04  naniwa
 * to call timer handler in task
 *
 * Revision 1.6  1999/11/10 10:47:02  naniwa
 * minor fix
 *
 * Revision 1.5  1999/07/30 08:25:34  naniwa
 * implemented
 *
 * Revision 1.4  1999/03/21 00:57:28  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.3  1998/02/25 12:51:34  night
 * プロセス毎の固有データ情報を、それぞれ大域変数で固定的にもつのではなく、
 * lowlib_data というポインタで管理するようにした。
 * この変更に伴い、プロセス毎に違った情報を設定/参照する処理は、ポインタ
 * lowlib_data を介して行うように変更した。
 *
 * lowlib_data の値は、マクロ LOWLIB_DATA で指定したアドレス (0x7fff0000)
 * に設定される。このアドレスは、プロセス毎に違った物理ページにマッピング
 * される。仮想アドレスは固定だが、実際の領域はプロセス毎に違った場所に存
 * 在している。LOWLIB が管理するプロセス毎の情報はこの領域に入れるように
 * する。
 *
 * Revision 1.2  1997/12/17 14:15:02  night
 * psys_fork(fork システムコールの処理関数) の中身を作った。
 *
 * Revision 1.1  1997/08/31 13:10:44  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"


/* fork 用エントリールーチン */
int fork_entry()
{
  ena_int();
  return(0);
}

/* fork システムコールの処理
 *
 */
int
psys_fork (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  ID			child_main;
  ID			child_signal;
  ID			myself;
  T_CTSK		task_info;
  W			size;
  struct arg {int esp; int ebp;} *args;

  args = (struct arg *) argp;

  /* 子プロセスのタスク生成
   * 実行開始アドレスは、fork_entry に設定しておく。
   *
   */

  /* 子プロセスの main task の生成 */
  bzero (&task_info, sizeof (task_info));
  task_info.tskatr = TA_HLNG;
  task_info.startaddr = (VP)fork_entry;
  task_info.itskpri = 10; /* POSIX_MIN_PRIORITY */
  task_info.stksz = 1024 * 12; /* stack size は 12KB で良いか? */
  error = vcre_tsk(&task_info, &child_main);
  if (error != E_OK) return (error);

  /* 子プロセスの signal task の生成 */
  /* 現時点では生成しない */
  child_signal = 0;

  /* 子プロセスの UNIQ PAGE の取得 */

  /* POSIX manager の呼び出し 
   *
   * 引数を設定して、POSIX manager にメッセージを送る。
   */
  req.param.par_fork.main_task = child_main;
  req.param.par_fork.signal_task = child_signal;

  error = _make_connection(PSC_FORK, &req, &res);
  if (error != E_OK)
    {
      /* What should I do? */
      del_tsk(child_main);
      ERRNO = error;
      return (-1);
    }
  else if (res.errno)
    {
      del_tsk(child_main);
      ERRNO = res.errno;
      return (-1);
    }

  /* stack のコピー */
  get_tid (&myself);
  vcpy_stk(myself, args->esp, args->ebp, child_main);

  /* 子プロセスを有効にする */
  sta_tsk (child_main, 0);
  return (res.status);
}
