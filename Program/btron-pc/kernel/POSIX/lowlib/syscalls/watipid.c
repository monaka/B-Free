/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/watipid.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/watipid.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: watipid.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-07-21 15:07:19  naniwa
 * implemented
 *
 * Revision 1.2  1999/03/21 00:58:31  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.1  1997/08/31 13:10:51  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_waitpid (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_waitpid *args = (struct psc_waitpid *)argp;

  req.param.par_waitpid.pid = args->pid;
  req.param.par_waitpid.statloc = args->statloc;
  req.param.par_waitpid.opts = args->opts;

  error = _make_connection(PSC_WAITPID, &req, &res);
  if (error != E_OK)
    {
      /* What should I do? */
    }

  else if (res.errno)
    {
      ERRNO = res.errno;
      return (-1);
    }

  if (args->statloc != NULL) *(args->statloc) = res.ret1;
  return (res.status);
}
