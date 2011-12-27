/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: getdents.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  2000-06-01 08:48:06  naniwa
 * first version
 *
 *
 */

#include "posix.h"

W
psc_getdents_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp;
  W		rlength;
  W		len, flen;

  errno = proc_get_file (req->procid, req->param.par_getdents.fileid, &fp);
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

  /* 対象ファイルがパイプだったり、
   * ディレクトリ以外の場合には、エラーにする
   */     
  if (fp->f_flag & F_PIPE)
    {
      /* パイプの読み書き */
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }
  
  if ((fp->f_inode->i_mode & FS_FMT_DIR) == 0)
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  errno = fs_getdents(fp->f_inode, req->caller, fp->f_offset,
		      req->param.par_getdents.buf,
		      req->param.par_getdents.length, &len, &flen);

  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
    }

  fp->f_offset += flen;
  put_response (req, EP_OK, len, 0, 0);
  return (SUCCESS);
}  
