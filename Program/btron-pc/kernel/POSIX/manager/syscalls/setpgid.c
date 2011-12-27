/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: setpgid.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_setpgid_f - プロセスの実効グループ ID を設定する
 */
W
psc_setpgid_f (struct posix_request *req)
{
  W	uid;
  W	errno;

  errno = proc_get_euid (req->procid, &uid);
  if (errno)
    {
      put_response (req, errno, 0, -1, 0);
      return (FAIL);
    }

  if (uid != 0)
    {
      put_response (req, EP_PERM, errno, -1, 0);
      return (FAIL);
    }

  errno = proc_set_gid (req->procid, req->param.par_setpgid.gid);
  if (errno)
    {
      put_response (req, errno, 0, -1, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, 0, 0, 0);
  return (FAIL);
}  
