/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: dup2.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-05-08 08:24:08  naniwa
 * minor fix
 *
 * Revision 1.3  2000/05/06 03:54:25  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.2  2000/02/04 15:16:29  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_dup2_f -ファイル記述子の複製
 */
W
psc_dup2_f (struct posix_request *req)
{
  W		errno;
  struct file	*fp, *fp2;


  /* プロセスからファイル構造体へのポインタを取り出す
   */
  errno = proc_get_file (req->procid, req->param.par_dup2.fileid1, &fp);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }


  if (fp->f_inode == NULL)
    {
      /* 複製するファイル記述子の番号がおかしい。
       * (ファイルをオープンしていない)
       */
      put_response (req, EP_BADF, -1, 0, 0);
      return (FAIL);
    }

  errno = proc_get_file (req->procid, req->param.par_dup2.fileid2, &fp2);
  if (errno) {
    put_response (req, errno, -1, 0, 0);
    return (FAIL);
  }
  if (fp2->f_inode != NULL) {
    /* 既に open されている file id だった */
    errno = fs_close_file (fp2->f_inode);
    if (errno) {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }
    fp2->f_inode = NULL;
  }
  fp->f_inode->i_refcount++;
  errno = proc_set_file(req->procid, req->param.par_dup2.fileid2,
			fp->f_omode, fp->f_inode);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, req->param.par_dup2.fileid2, 0, 0);
  return (SUCCESS);
}  
