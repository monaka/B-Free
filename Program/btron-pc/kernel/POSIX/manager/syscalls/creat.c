/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: creat.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-05-06 03:54:23  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.3  2000/02/04 15:16:27  naniwa
 * minor fix
 *
 * Revision 1.2  1999/05/30 04:09:30  naniwa
 * modified parameters passed to open system call
 *
 * Revision 1.1  1999/03/21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_creat_f - ファイルを作成する
 */
W
psc_creat_f (struct posix_request *req)
{
#ifdef USE_ALLOCA
  B		*pathname;
#else
  B		pathname[MAX_NAMELEN];
#endif
  W		fileid;
  W		errno;
  struct inode	*startip;
  struct inode	*newip;
  struct access_info	acc;
  W		umask;


  errno = proc_alloc_fileid (req->procid, &fileid);
  if (errno)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }

#ifdef USE_ALLOCA
  pathname = alloca (req->param.par_creat.pathlen);
  if (pathname == NULL)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

  errno = vget_reg (req->caller, req->param.par_creat.path,
		    req->param.par_creat.pathlen + 1, pathname);
  if (errno)
    {
      /* パス名のコピーエラー */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, 0, 0, 0);
      else
	put_response (req, EP_FAULT, 0, 0, 0);
	
      return (FAIL);
    }

  if (*pathname != '/')
    {
      errno = proc_get_cwd (req->procid, &startip);
      if (errno)
	{
	  put_response (req, errno, 0, 0, 0);
	  return (FAIL);
	}
    }
  else
    {
      startip = rootfile;
    }
  errno = proc_get_uid (req->procid, &(acc.uid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = proc_get_gid (req->procid, &(acc.gid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = proc_get_umask (req->procid, &umask);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  errno = fs_open_file (pathname,
			O_CREAT|O_WRONLY,
			req->param.par_creat.mode & (~umask),
			&acc,
			startip, 
			&newip);
  if (errno)
    {
      /* ファイルがオープンできない */
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  
  proc_set_file (req->procid, fileid, O_WRONLY, newip);
  put_response (req, EP_OK, fileid, 0, 0);
  return (SUCCESS);
}  
