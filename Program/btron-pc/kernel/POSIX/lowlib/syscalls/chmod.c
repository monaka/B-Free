/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/chmod.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/chmod.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: chmod.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-03-21 00:57:11  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.3  1999/02/19 10:04:29  monaka
 * Addition or fix for parameter structure.
 *
 * Revision 1.2  1999/02/19 08:59:08  monaka
 * added the parameter structure on.
 *
 * Revision 1.1  1997/08/31 13:10:42  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_chmod (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_chmod *args = (struct psc_chmod *)argp;

  req.param.par_chmod.pathlen = args->pathlen;
  req.param.par_chmod.path = args->path;
  req.param.par_chmod.mode = args->mode;

  error = _make_connection(PSC_CHMOD, &req, &res);
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
