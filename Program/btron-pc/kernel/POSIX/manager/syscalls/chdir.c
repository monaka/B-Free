/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: chdir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-05-20 12:39:21  naniwa
 * to check as a directry
 *
 * Revision 1.2  2000/05/20 11:57:17  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_chdir_f (struct posix_request *req)
{
  struct inode		*oldip;
#ifdef USE_ALLOCA
  B			*path;
#else
  B			path[MAX_NAMELEN];
#endif
  struct inode		*startip;
  struct inode		*ipp;
  struct access_info	acc;
  W			err;

#ifdef USE_ALLOCA
  path = alloca (req->param.par_chmod.pathlen + 1);
  if (path == 0)
    {
      put_response (req, EP_NOMEM, -1, 0, 0);
      return (FAIL);
    }
#endif
#if 0
  bzero (path, req->param.par_chmod.pathlen + 1);
#endif

  if (vget_reg (req->caller, req->param.par_chmod.path,
		req->param.par_chmod.pathlen + 1, path))
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  err = proc_get_cwd (req->procid, &oldip);
  if (err)
    {
      put_response (req, err, -1, 0, 0);
      return (FAIL);
    }
      
  if (*path == '/')
    {
      /* 絶対パスによる指定 */
      startip = rootfile;
    }
  else
    {
#ifdef notdef
      if (proc_get_cwd (req->procid, &startip))
	{
	  put_response (req, EP_INVAL, -1, 0, 0);
	  return (FAIL);
	}
#else
      startip = oldip;
#endif
    }


  if (proc_get_euid (req->procid, &acc.uid))
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }
      
  if (proc_get_egid (req->procid, &acc.gid))
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  err = fs_lookup (startip, path, O_RDONLY, 0, &acc, &ipp);
  if (err)
    {
      put_response (req, err, -1, 0, 0);
      return (FAIL);
    }
      
  if ((ipp->i_mode & FS_FMT_DIR) == 0)
    {
      /* ファイルは、ディレクトリではなかった。
       * エラーとする
       * 
       */
      fs_close_file (ipp);
      put_response (req, EP_NOTDIR, -1, 0, 0);
      return (FAIL);
    }

  err = permit(ipp, &acc, X_BIT);
  if (err) {
    put_response (req, err, -1, 0, 0);
    return (FAIL);
  }

  err = proc_set_cwd (req->procid, ipp);
  if (err)
    {
      put_response (req, err, -1, 0, 0);
      return (FAIL);
    }

  dealloc_inode (oldip);
  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
