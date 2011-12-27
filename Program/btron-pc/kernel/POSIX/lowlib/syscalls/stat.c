/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/stat.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/stat.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: stat.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-03-21 00:58:19  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.2  1999/03/16 16:26:38  monaka
 * Added some code from BTRON/init (but it runs under POSIX). May be these code will work but I don't test well.
 *
 * Revision 1.1  1997/08/31 13:10:50  night
 * �ǽ����Ͽ
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_stat (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  struct psc_stat *args = (struct psc_stat *)argp;

  req.param.par_stat.fileid = args->fileid;
  req.param.par_stat.st = args->st;

  error = _make_connection(PSC_STAT, &req, &res);
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
