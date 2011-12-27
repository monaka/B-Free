/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: write.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-05-06 03:54:34  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.4  2000/02/20 09:39:44  naniwa
 * to use MAX_BODY_SIZE
 *
 * Revision 1.3  2000/02/04 15:16:36  naniwa
 * minor fix
 *
 * Revision 1.2  1999/03/24 03:54:55  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_write_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp;
  W		rlength;
  W		i, len;
  static B	buf[MAX_BODY_SIZE];
  W		rest_length;
#ifdef DEBUG
  W		j;
#endif

  errno = proc_get_file (req->procid, req->param.par_write.fileid, &fp);
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

  if (fp->f_omode == O_RDONLY) {
      put_response (req, EP_BADF, -1, 0, 0);
      return (FAIL);
  }

  if (fp->f_flag & F_PIPE)
    {
      /* パイプの読み書き */
      put_response (req, EP_NOSUP, -1, 0, 0);
      return (FAIL);
    }

#ifdef debug
  printk ("posix:write: inode = 0x%x, offset = %d, buf = 0x%x, length = %d\n",
	  fp->f_inode, fp->f_offset, req->param.par_write.buf, req->param.par_read.length);
#endif

  for (i = 0, rest_length = req->param.par_write.length;
       rest_length > 0; rest_length -= rlength, i += rlength)
    {
#ifdef DEBUG
      printk ("vget_reg (caller = %d, src addr = 0x%x, size = %d, dst = 0x%x\n",
	      req->caller, req->param.par_write.buf + i,
	      rest_length > MAX_BODY_SIZE ? MAX_BODY_SIZE : rest_length, buf);
#endif
      len = rest_length > MAX_BODY_SIZE ? MAX_BODY_SIZE : rest_length;
      errno = vget_reg (req->caller, req->param.par_write.buf + i, len, buf);
      if (errno)
	{
	  break;
	}

#ifdef DEBUG
      printk ("writedata length = %d\n", len);
      for (j = 0;
	   j < ((rest_length > MAX_BODY_SIZE) ? MAX_BODY_SIZE : rest_length);
	   j++)
	{
	  char	print_buffer[2];

	  print_buffer[0] = buf[j];
	  print_buffer[1] = '\0';
	  printk ("%s", print_buffer);
	}
      printk ("\n");
#endif

      errno = fs_write_file (fp->f_inode,
			     fp->f_offset + i,
			     buf, len, &rlength);
      if (errno || (rlength < len))
	{
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
