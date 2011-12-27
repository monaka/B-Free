/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: read.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  2000-05-07 10:24:49  naniwa
 * minor fix
 *
 * Revision 1.7  2000/05/06 03:54:30  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.6  2000/02/27 15:35:46  naniwa
 * to work as multi task OS
 *
 * Revision 1.5  2000/02/20 09:39:40  naniwa
 * to use MAX_BODY_SIZE
 *
 * Revision 1.4  2000/02/04 15:16:33  naniwa
 * minor fix
 *
 * Revision 1.3  2000/01/21 14:26:47  naniwa
 * fixed argument of fs_read_file
 *
 * Revision 1.2  2000/01/15 15:30:53  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_read_f - ファイルからのデータの読み込み
 */
W
psc_read_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp;
  W		rlength;
  W		rest_length;
  W		i, len;
  static B	buf[MAX_BODY_SIZE];

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

  if (fp->f_omode == O_WRONLY) {
      put_response (req, EP_BADF, -1, 0, 0);
      return (FAIL);
  }

  if (fp->f_flag & F_PIPE)
    {
      /* パイプの読み書き */
      put_response (req, EP_NOSUP, -1, 0, 0);
      return (FAIL);
    }

  if ((fp->f_inode->i_mode & FS_FMT_DEV) &&
      ((fp->f_inode->i_dev & 0x80000000) == 0)) {
    /* スペシャルファイル（キャラクタデバイス）だった */

    /* デバイスに read メッセージを転送 */
    errno = sfs_trans_device(fp->f_inode->i_dev,
			     req->receive_port,
			     req->caller,
			     req->param.par_read.buf,
			     fp->f_offset,
			     req->param.par_read.length);
    if (errno == E_OK) {
      fp ->f_offset += req->param.par_read.length;
      return (SUCCESS);
    }
    return(FAIL);
  }
  
#ifdef DEBUG
  printf ("posix:read: inode = 0x%x, offset = %d, buf = 0x%x, length = %d\n",
	  fp->f_inode, fp->f_offset, req->param.par_read.buf, req->param.par_read.length);
#endif

  if (fp->f_offset >= fp->f_inode->i_size) {
    put_response(req, EP_OK, 0, 0, 0);
    return(SUCCESS);
  }
  for (i = 0, rest_length = req->param.par_read.length;
       rest_length > 0; rest_length -= rlength, i += rlength)
    {
      /* MAX_BODY_SIZE 毎にファイルに読み込み */
      len = rest_length > MAX_BODY_SIZE ? MAX_BODY_SIZE : rest_length;
      errno = fs_read_file (fp->f_inode,
			    fp->f_offset + i,
			    buf, len, &rlength);
      if (errno) break;

      /* 呼び出したプロセスのバッファへの書き込み */
      errno = vput_reg (req->caller, req->param.par_read.buf + i,
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
