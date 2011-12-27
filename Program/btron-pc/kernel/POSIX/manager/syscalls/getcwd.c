/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: getcwd.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_getcwd_f - $B%W%m%;%9$N%+%l%s%H%G%#%l%/%H%j$N<h$j=P$7(B
 */
W
psc_getcwd_f (struct posix_request *req)
{
  struct inode	*cwd;
#ifdef USE_ALLOCA
  B		*tmppath;
#else
  B		tmppath[MAX_NAMELEN];
#endif
  W		errno;


  /* $B%W%m%;%9$N8=:_$N%+%l%s%H%G%#%l%/%H%j$r(B
   * $B<h$j=P$9!#(B
   * $B%Q%9L>$N7A$G%W%m%0%i%`$KJV$9$,!"(B
   * POSIX $B%^%M!<%8%cFb$K$O%Q%9L>$N7A$G$O!"(B
   * $B5-21$7$F$$$J$$(B(inode $B$X$N%]%$%s%?$G5-21$7$F$$$k(B)$B!#(B
   * $B$=$N$?$a!"(Binode $B$+$i%Q%9L>$r<h$j=P$9$h$&$K$9$k!#(B
   */
  errno = proc_get_cwd (req->procid, &cwd);
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

#ifdef USE_ALLOCA
  tmppath = alloca (req->param.par_getcwd.dirnamelen);
  if (tmppath == NULL)
    {
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

  /* inode $B$+$i%Q%9L>(B($BJ8;zNs(B)$B$X$HJQ49$9$k(B
   */
  errno = fs_convert_path (cwd, tmppath, req->param.par_getcwd.dirnamelen);
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = vput_reg (req->caller,
		    req->param.par_getcwd.dirname,
		    strlen (tmppath) + 1,
		    tmppath);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
