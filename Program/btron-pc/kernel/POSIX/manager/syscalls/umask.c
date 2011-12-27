/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: umask.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_umask_f - umask $B$N@_Dj(B
 */
W
psc_umask_f (struct posix_request *req)
{
  W	errno;
  W	old_umask;


  /* $B8E$$(B umask $B$NCM$r<h$j=P$9!#(B
   * ($B%7%9%F%`%3!<%k$NJV$jCM$H$7$F;HMQ$9$k(B)
   */
  errno = proc_get_umask (req->procid, &old_umask);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }
  
  /* $B?7$7$$(B umask $B$NCM$r@_Dj$9$k(B
   */
  errno = proc_set_umask (req->procid, req->param.par_umask.umask);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, old_umask, 0, 0);
  return (SUCCESS);
}  
