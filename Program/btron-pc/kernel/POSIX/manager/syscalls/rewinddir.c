/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: rewinddir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_rewinddir_f - $B%G%#%l%/%H%j$NFI$_<h$j$N$?$a$N%+%l%s%H%]%$%s%?$r(B
 *		     $B:G=i$KLa$9!#(B
 */
W
psc_rewinddir_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp;

  errno = proc_get_file (req->procid, req->param.par_read.fileid, &fp);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }
  else if (fp == 0)
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }
  else if (fp->f_inode == 0)
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  /* $BBP>]%U%!%$%k$,%Q%$%W$@$C$?!"%(%i!<$K$9$k(B
   */     
  if (fp->f_flag & F_PIPE)
    {
      /* $B%Q%$%W$NFI$_=q$-(B */
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  /* $BBP>]%U%!%$%k$,%G%#%l%/%H%j$G$O$J$+$C$?$N$G%(%i!<$K$9$k(B
   */
  if ((fp->f_inode->i_mode & FS_FMT_DIR) == 0)
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  errno = proc_rewind_file (req->procid, req->param.par_rewinddir.fileid);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
