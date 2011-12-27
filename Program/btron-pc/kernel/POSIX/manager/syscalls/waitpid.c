/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: waitpid.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-01-21 14:29:54  naniwa
 * fixed response to parent process
 *
 * Revision 1.3  1999/07/23 14:40:53  naniwa
 * minor fix
 *
 * Revision 1.2  1999/07/21 15:12:39  naniwa
 * implemented
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_waitpid_f (struct posix_request *req)
{
  W i;
  W mypid, pid, children, exst;
  struct proc *procp;

  pid = req->param.par_waitpid.pid;
  mypid = req->procid;
  if (pid == 0) pid = (-proc_table[mypid].proc_pgid);

  /* プロセステーブルを走査して子プロセスを調査 */
  children = 0;
  for(i = 1; i < MAX_PROCESS; ++i) {
    proc_get_procp(i, &procp);
    if (procp->proc_status == PS_DORMANT) continue;
    if (procp->proc_ppid == mypid) {
      if (pid > 0 && pid != procp->proc_pid) continue;
      if (pid < -1 && pid != -procp->proc_pgid) continue;
      children++;
      if (procp->proc_status == PS_ZOMBIE) {
	/* 子プロセスの情報をクリアし，親プロセスに返事を送る */
	exst = (procp->proc_exst << 8);
	put_response (req, EP_OK, i, exst, 0);
	
	/* 親プロセスの状態変更 */
	proc_get_procp(mypid, &procp);
	procp->proc_status = PS_RUN;

	/* 子プロセスのエントリーの開放 */
	proc_exit(i);
	
	return(SUCCESS);
      }
    }
  }
  if (children > 0) {
    /* 対応する子プロセスはあったが，まだ終了していなかった */
    if (req->param.par_waitpid.opts & WNOHANG) {
      /* 親に返事を送る必要がある */
      put_response (req, EP_OK, 0, 0, 0);
      return (SUCCESS);
    }
    /* 親プロセスの状態を変更し，返事を送らずにシステムコールを終了 */
    proc_get_procp(mypid, &procp);
    procp->proc_status = PS_WAIT;
    procp->proc_wpid = pid;
    procp->proc_rvpt = req->receive_port;
    return (SUCCESS);
  }
  else {
    /* エラーを返す */
    put_response (req, EP_CHILD, 0, 0, 0);
    return (FAIL);
  }
}  
