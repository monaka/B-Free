/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: readdir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-02-04 15:16:34  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_readdir_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp;
  W		rlength;
  W		i, len, rest_length;
  static B	buf[PAGE_SIZE];

  errno = proc_get_file (req->procid, req->param.par_readdir.fileid, &fp);
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

  for (i = 0, rest_length = req->param.par_readdir.length;
       rest_length > 0;
       rest_length -= rlength, i += rlength) {
    /* ページ毎にファイルに読み込み */
    len = rest_length > PAGE_SIZE ? PAGE_SIZE : rest_length;
    errno = fs_read_file (fp->f_inode,
			  fp->f_offset + i,
			  buf, len, &rlength);
      if (errno) break;

      /* 呼び出したプロセスのバッファへの書き込み */
      errno = vput_reg (req->caller, req->param.par_readdir.buf + i,
			rlength, buf);
      if (errno || (rlength < len)) {
	i += rlength;
	break;
      }
  }
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
    }

  fp->f_offset += i;
  put_response (req, EP_OK, i, 0, 0);
  return (SUCCESS);
}  
