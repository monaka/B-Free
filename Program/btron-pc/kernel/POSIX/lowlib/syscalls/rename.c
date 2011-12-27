/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/rename.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/rename.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: rename.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-03-21 00:57:59  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.3  1999/02/19 10:04:49  monaka
 * Addition or fix for parameter structure.
 *
 * Revision 1.2  1999/02/19 08:59:37  monaka
 * added the parameter structure on.
 *
 * Revision 1.1  1997/08/31 13:10:47  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_rename (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_rename *args = (struct psc_rename *)argp;

  req.param.par_rename.oldpathlen = args->oldpathlen;
  req.param.par_rename.oldpath = args->oldpath;
  req.param.par_rename.newpathlen = args->newpathlen;
  req.param.par_rename.newpath = args->newpath;

  error = _make_connection(PSC_RENAME, &req, &res);
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
