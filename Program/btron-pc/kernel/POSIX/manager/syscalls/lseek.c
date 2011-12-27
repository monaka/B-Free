/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: lseek.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-04 15:15:10  naniwa
 * corrected log comment
 *
 * Revision 1.2  2000/02/01 08:12:30  naniwa
 * In this time, lseek can not move beyond EOF
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_lseek_f (struct posix_request *req)
{
  struct file	*fp;
  W		errno;

  errno = proc_get_file (req->procid, req->param.par_lseek.fileid, &fp);
  if (errno)
    {
      put_response (req, errno, errno, -1, 0);    
      return (FAIL);
    }

  switch (req->param.par_lseek.mode)
    {
    case SEEK_SET:
      fp->f_offset = req->param.par_lseek.offset;
      break;

    case SEEK_CUR:
      fp->f_offset += req->param.par_lseek.offset;
      break;

    case SEEK_END:
      fp->f_offset = fp->f_inode->i_size + req->param.par_lseek.offset;
      break;

    default:
      put_response (req, EP_INVAL, 0, -1, 0);
      return (FAIL);
    }

  if (fp->f_offset < 0)
    {
      fp->f_offset = 0;
    }
  else if (fp->f_offset > fp->f_inode->i_size) {
    /* EOF を越えた読み書きはまだサポートされていない */
    fp->f_offset = fp->f_inode->i_size;
  }

  put_response (req, EP_OK, 0, 0, 0);

  return (SUCCESS);
}  
