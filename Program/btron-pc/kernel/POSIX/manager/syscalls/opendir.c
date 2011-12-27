/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: opendir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-05-06 03:54:29  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.3  2000/02/04 15:16:32  naniwa
 * minor fix
 *
 * Revision 1.2  1999/03/24 03:54:54  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_opendir_f - ディレクトリのオープン
 */
W
psc_opendir_f (struct posix_request *req)
{
#if USE_ALLOCA
  B		*pathname;
#else
  B		pathname[MAX_NAMELEN];
#endif
  W		fileid;
  W		errno;
  struct inode	*startip;
  struct inode	*newip;
  struct access_info	acc;


  errno = proc_alloc_fileid (req->procid, &fileid);
  if (errno)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }

#if USE_ALLOCA
  pathname = (B *)alloca (req->param.par_opendir.pathlen + 1);
  if (pathname == NULL)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

#if 0
  printk ("bzero (pathname = 0x%x, req->param.par_open.pathlen + 1 = %d)\n",
	  pathname, req->param.par_open.pathlen + 1);
  bzero (pathname, req->param.par_open.pathlen + 1);
#endif
#ifdef DEBUG
  printk ("vget_reg (req->caller = %d, req->param.par_opendir.path = 0x%x, req->param.par_opendir.pathlen = %d, pathname = 0x%x)\n",
	  req->caller, req->param.par_opendir.path, req->param.par_opendir.pathlen + 1, pathname);
#endif

#ifdef DEBUG
  printk ("pathname: 0x%x\n", pathname);
#endif
  errno = vget_reg (req->caller, req->param.par_opendir.path,
		    req->param.par_opendir.pathlen + 1, pathname);
  if (errno)
    {
      /* パス名のコピーエラー */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, 0, 0, 0);
      else
	put_response (req, EP_FAULT, 0, 0, 0);
	
      return (FAIL);
    }

#ifdef DEBUG
  printk ("pathname: 0x%x\n", pathname);
#endif
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
#ifdef DEBUG
      printk ("startip = rootfile = 0x%x\n", rootfile);
#endif      
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

  errno = fs_open_file (pathname,
			O_RDWR,		/* ディレクトリを書き換える可 */
					/* 能性があるので、O_RDWR を指 */
					/* 定する */
			0,
			&acc,
			startip, 
			&newip);
  if (errno)
    {
      /* ファイルがオープンできない */
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  
  if ((newip->i_mode & FS_FMT_DIR) == 0)
    {
      /* ファイルは、ディレクトリではなかった。
       * エラーとする( root ユーザの場合には、
       * 成功でもよい？)
       */
      fs_close_file (newip);
      put_response (req, EP_NOTDIR, -1, 0, 0);
      return (FAIL);
    }

  proc_set_file (req->procid, fileid, O_RDWR, newip);
  put_response (req, EP_OK, fileid, 0, 0);
  return (SUCCESS);
}  
