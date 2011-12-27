/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/access.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/access.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: access.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1999-03-21 00:57:09  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.4  1999/03/16 16:26:34  monaka
 * Added some code from BTRON/init (but it runs under POSIX). May be these code will work but I don't test well.
 *
 * Revision 1.3  1999/02/19 10:04:27  monaka
 * Addition or fix for parameter structure.
 *
 * Revision 1.2  1999/02/19 08:59:06  monaka
 * added the parameter structure on.
 *
 * Revision 1.1  1997/08/31 13:10:42  night
 * �ǽ����Ͽ
 *
 *
 *
 */


#include "../lowlib.h"


int
psys_access (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  struct psc_access *args = (struct psc_access *)argp;

  req.param.par_access.path = args->path;
  req.param.par_access.pathlen = args->pathlen;
  req.param.par_access.accflag = args->accflag;

  error = _make_connection(PSC_ACCESS, &req, &res);
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
