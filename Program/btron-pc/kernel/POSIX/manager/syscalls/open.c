/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: open.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-06-23 09:18:38  naniwa
 * to support O_APPEND
 *
 * Revision 1.4  2000/05/20 11:57:16  naniwa
 * minor fix
 *
 * Revision 1.3  2000/05/06 03:54:27  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.2  2000/02/04 15:16:31  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_open_f - ファイルのオープン
 */
W
psc_open_f (struct posix_request *req)
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
      put_response (req, EP_NOMEM, -1, 0, 0);
      return (FAIL);
    }

#ifdef USE_ALLOCA
  pathname = alloca (req->param.par_open.pathlen + 1);
  if (pathname == NULL)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, -1, 0, 0);
      return (FAIL);
    }
#endif
#if 0
  bzero (pathname, req->param.par_open.pathlen + 1);
#endif
  /* パス名をユーザプロセスから POSIX サーバのメモリ空間へコピーする。
   */
  errno = vget_reg (req->caller, req->param.par_open.path,
		    req->param.par_open.pathlen + 1, pathname);
  if (errno)
    {
      /* パス名のコピーエラー */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, -1, 0, 0);
      else
	put_response (req, EP_FAULT, -1, 0, 0);
	
      return (FAIL);
    }

#ifdef notdef
  printf ("psc_open_f: open file path = %s\n",
	  pathname);
#endif
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

  errno = proc_get_umask (req->procid, &umask);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  errno = fs_open_file (pathname,
			req->param.par_open.oflag,
			req->param.par_open.mode & (~umask),
			&acc,
			startip, 
			&newip);
  if (errno)
    {
#ifdef notdef
      printf ("open systemcall: Not found entry.\n");
#endif
      /* ファイルがオープンできない */
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }
  
  if (newip->i_mode & FS_FMT_DIR)
    {
      W	uid, euid;

      /* ファイルは、ディレクトリだった
       * エラーとする
       */

      /* root ユーザの場合には、
       * 成功でもよい
       */
#ifdef notdef
      if (proc_get_uid (req->procid, &uid))
	{
	  put_response (req, EP_INVAL, -1, 0, 0);
	  return (FAIL);
	}
      if (proc_get_euid (req->procid, &euid))
	{
	  put_response (req, EP_INVAL, -1, 0, 0);
	  return (FAIL);
	}
      if ((uid != 0) && (euid != 0))
	{
	  fs_close_file (newip);
	  put_response (req, EP_ISDIR, -1, 0, 0);
	  return (FAIL);
	}
#else
      if (acc.uid != SU_UID) {
	fs_close_file (newip);
	put_response (req, EP_ACCESS, -1, 0, 0);
	return (FAIL);
      }
#endif
      if (req->param.par_open.oflag != O_RDONLY) {
	fs_close_file (newip);
	put_response (req, EP_ISDIR, -1, 0, 0);
	return (FAIL);
      }
    }

  if (proc_set_file (req->procid, fileid,
		     req->param.par_open.oflag, newip))
    {
      put_response (req, EP_INVAL, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, fileid, 0, 0);
  return (SUCCESS);
}  
