/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: mountroot.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_mountroot_f (struct posix_request *req)
{
  W	err;

#ifdef notdef
  printf ("mountroot: start (device = %d )\n", req->param.par_mountroot.device);
#endif  
  err = mount_root (req->param.par_mountroot.device, 
		    req->param.par_mountroot.fstype,
		    req->param.par_mountroot.option);
#ifdef notdef
  printf ("mountroot: called mount_root(), err = %d\n", err);
#endif  

  if (err)
    {
      put_response (req, err, -1, 0, 0);
      return (FAIL);
    }
  else
    {
      put_response (req, err, 0, 0, 0);
    }
  return (SUCCESS);
}
