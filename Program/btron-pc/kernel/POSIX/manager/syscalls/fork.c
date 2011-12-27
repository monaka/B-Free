/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: fork.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-01-26 08:26:50  naniwa
 * changed to EUC code
 *
 * Revision 1.2  1999/03/24 03:54:52  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_fork_f - 新しいプロセスを作成する
 */
W
psc_fork_f (struct posix_request *req)
{
  struct proc *procp;
  W	       errno;
  W	       childid;

  errno = proc_get_procp (req->procid, &procp);		/* 親プロセスの情報の取りだし */
  if (errno)
    {
      printk ("posix: invalid process id (%d)\n", req->procid);
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

#ifdef DEBUG
  printk ("psc_fork_f(): proc = 0x%x, proc->vm_tree = 0x%x\n", procp, procp->vm_tree);
#endif

  errno = fork (procp, &childid, req->param.par_fork.main_task, req->param.par_fork.signal_task);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, childid, 0, 0);	/* 親プロセスに対して応答 */
  return (SUCCESS);
}  
