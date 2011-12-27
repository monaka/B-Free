/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: rmdir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-05-06 03:54:32  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_rmdir_f - ディレクトリを削除する
 */
W
psc_rmdir_f (struct posix_request *req)
{
#ifdef USE_ALLOCA
  B		*pathname;
#else
  B		pathname[MAX_NAMELEN];
#endif
  W		errno;
  struct inode	*startip;
  struct access_info	acc;

#ifdef USE_ALLCOA
  pathname = alloca (req->param.par_rmdir.pathlen + 1);
  if (pathname == NULL)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

  errno = vget_reg (req->caller, req->param.par_rmdir.path,
		    req->param.par_rmdir.pathlen + 1, pathname);
  if (errno)
    {
      /* パス名のコピーエラー */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, -1, 0, 0);
      else
	put_response (req, EP_FAULT, -1, 0, 0);
	
      return (FAIL);
    }


  if (*pathname != '/')
    {
      errno = proc_get_cwd (req->procid, &startip);
      if (errno)
	{
	  put_response (req, errno, -1, 0, 0);
	  return (FAIL);
	}
    }
  else
    {
      startip = rootfile;
    }
  errno = proc_get_euid (req->procid, &(acc.uid));
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  errno = proc_get_egid (req->procid, &(acc.gid));
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  errno = fs_remove_dir (startip,
			 pathname,
			 &acc);
  if (errno)
    {
      /* ファイルがオープンできない */
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }
  
  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
