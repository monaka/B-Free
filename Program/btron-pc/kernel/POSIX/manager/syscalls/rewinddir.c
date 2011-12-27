/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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

/* psc_rewinddir_f - ディレクトリの読み取りのためのカレントポインタを
 *		     最初に戻す。
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

  /* 対象ファイルがパイプだった、エラーにする
   */     
  if (fp->f_flag & F_PIPE)
    {
      /* パイプの読み書き */
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  /* 対象ファイルがディレクトリではなかったのでエラーにする
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
