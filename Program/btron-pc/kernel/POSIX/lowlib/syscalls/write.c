/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/write.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/write.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: write.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-03-21 00:58:32  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.3  1999/03/16 16:26:39  monaka
 * Added some code from BTRON/init (but it runs under POSIX). May be these code will work but I don't test well.
 *
 * Revision 1.2  1999/02/19 08:59:50  monaka
 * added the parameter structure on.
 *
 * Revision 1.1  1997/08/31 13:10:51  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_write (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  struct psc_write *args = (struct psc_write *)argp;

  req.param.par_write.fileid = args->fileid;
  req.param.par_write.buf = args->buf;
  req.param.par_write.length = args->length;

  error = _make_connection(PSC_WRITE, &req, &res);
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
