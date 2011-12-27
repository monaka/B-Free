/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/getdents.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/getdents.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: getdents.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  2000-06-01 08:45:29  naniwa
 * first version
 *
 *
 */


#include "../lowlib.h"

int
psys_getdents (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_getdents *args = (struct psc_getdents *)argp;

  req.param.par_getdents.fileid = args->fileid;
  req.param.par_getdents.buf = args->buf;
  req.param.par_getdents.length = args->length;

  error = _make_connection(PSC_GETDENTS, &req, &res);
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
