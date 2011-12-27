/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: exit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-07-23 14:41:10  naniwa
 * implemented
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_exit_f - プロセスを終了させる
 */
W
psc_exit_f (struct posix_request *req)
{
  struct proc *myprocp, *procp;
  struct posix_request preq;
  W mypid, wpid, exst;
  W i;
  ER errno;

  mypid = req->procid;
  errno = proc_get_procp(mypid, &myprocp);
  if (errno) return errno;
  myprocp->proc_exst = req->param.par_exit.evalue;

  errno = proc_get_procp(myprocp->proc_ppid, &procp);
  if (errno) return errno;
  wpid = procp->proc_wpid;
  if (procp->proc_status == PS_WAIT &&
      (wpid == -1 || wpid == mypid || -wpid == myprocp->proc_pgid)) {
    /* 親プロセスが自分を WAIT していればメッセージ送信 */
    procp->proc_status = PS_RUN;
    preq.receive_port = procp->proc_rvpt;
    preq.operation = PSC_WAITPID;
    exst = (myprocp->proc_exst << 8);
    put_response (&preq, EP_OK, mypid, exst, 0);

    /* エントリーの開放 */
    proc_exit(mypid);
  }
  else {
    /* そうでなければ，ZOMBIE 状態に */
    myprocp->proc_status = PS_ZOMBIE;
  }

  /* 子プロセスの親を INIT に変更 */
  for(i = 1; i < MAX_PROCESS; ++i) {
    proc_get_procp(i, &procp);
    if (procp->proc_status == PS_DORMANT) continue;
    if (procp->proc_ppid != mypid) continue;
    procp->proc_ppid = 0; /* INIT プロセスの pid は 0 */
    
    /* 子プロセスが ZOMBIE で INIT が wait していれば クリアする? */
  }

  /* POSIX の vmtree のみを開放．仮想メモリーは exd_tsk で開放 */
  destroy_proc_memory (myprocp, 0);

  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
