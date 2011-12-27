/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: uname.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_uname_f -
 */
W
psc_uname_f (struct posix_request *req)
{
  W	errno;

  errno = vput_reg (req->caller,
		    req->param.par_uname.uname,
		    sizeof (struct utsname),
		    (VP)&system_name);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
