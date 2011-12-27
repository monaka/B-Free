/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: closedir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-04 15:16:26  naniwa
 * minor fix
 *
 * Revision 1.2  1999/07/10 10:45:35  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_closedir_f (struct posix_request *req)
{
  struct file	*fp;
  W		err;

  err = proc_get_file (req->procid, req->param.par_closedir.fileid, &fp);
  if (err)
    {
      put_response (req, err, -1, 0, 0);
      return (FAIL);
    }

  if (fp->f_inode == NULL) {
    put_response (req, EP_BADF, -1, 0, 0);
    return(FAIL);
  }

  err = fs_close_file (fp->f_inode);
  if (err)
    {
      put_response (req, err, -1, 0, 0);
      return (FAIL);
    }

  fp->f_inode = NULL;
  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
