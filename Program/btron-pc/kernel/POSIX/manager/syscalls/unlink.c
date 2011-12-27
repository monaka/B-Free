/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: unlink.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-05-06 03:54:33  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.3  2000/02/04 15:16:35  naniwa
 * minor fix
 *
 * Revision 1.2  1999/12/26 11:19:12  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_unlink_f - ファイルを削除する
 *
 * 引数:
 * 	req->param.par_unlink.path	削除するファイルのパス名
 *	req->param.par_unlink.pathlen	パス名の長さ
 *
 */
W
psc_unlink_f (struct posix_request *req)
{
#ifdef USE_ALLOCA
  B			*pathname;
#else
  B			pathname[MAX_NAMELEN];
#endif
  W			errno;
  struct inode		*startip;
  struct access_info	acc;


  /* パス名を呼び出し元のプロセス(タスク)から
   * 取り出す。
   * 呼び出し元のタスク ID は、メッセージパラメータの
   * 中に入っている。
   */
#ifdef USE_ALLOCA
  pathname = alloca (req->param.par_unlink.pathlen + 1);
  if (pathname == NULL)
    {
      /* メモリ取得エラー */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif
#if 0
  bzero (pathname, req->param.par_open.pathlen + 1);
#endif
  errno = vget_reg (req->caller,
		    req->param.par_unlink.path,
		    req->param.par_unlink.pathlen + 1,
		    pathname);
  if (errno)
    {
      /* パス名のコピーエラー */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, 0, 0, 0);
      else
	put_response (req, EP_FAULT, 0, 0, 0);
	
      return (FAIL);
    }


  /* ファイルが相対パスで指定されているかどうかを
   * チェックする。
   * (パス名が '/' ではじまっていない場合には、
   * 相対パスと見なす)
   * 相対パスではじまっている場合には、
   * プロセスのカレントディレクトリを取り出して、
   * そこをファイルのパスの最初のディレクトリと
   * する。
   */
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


  /* プロセスのユーザ ID とグループ ID の
   * 取り出し。
   * この情報に基づいて、ファイルを削除できるかどうかを
   * 決定する。
   */
  errno = proc_get_euid (req->procid, &(acc.uid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  errno = proc_get_egid (req->procid, &(acc.gid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = fs_remove_file (startip,
			  pathname,
			  &acc);
  if (errno)
    {
      /* ファイルがオープンできない */
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  
  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
} 
