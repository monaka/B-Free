/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/mountroot.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/mountroot.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: mountroot.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-03-21 00:57:50  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.2  1999/03/15 08:38:06  monaka
 * Code added.
 *
 * Revision 1.1  1997/08/31 13:10:46  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_mountroot (void *argp)
{
  ER	error;
  struct posix_request	req;
  struct posix_response res;

  struct psc_mountroot *args = (struct psc_mountroot *)argp;

  req.param.par_mountroot.device = args->device;
  req.param.par_mountroot.fstype = 1;
  req.param.par_mountroot.option = 0;

  error = _make_connection(PSC_MOUNTROOT, &req, &res);
  if (error != E_OK)
    {
      /* What should I do? */
    }

  else if (res.errno)
    {
      ERRNO = res.errno;
      return (-1);
    }

  return (res.status);
}
