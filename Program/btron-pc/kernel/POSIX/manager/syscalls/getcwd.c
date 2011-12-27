/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: getcwd.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-21 14:01:51  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_getcwd_f - プロセスのカレントディレクトリの取り出し
 */
W
psc_getcwd_f (struct posix_request *req)
{
  struct inode	*cwd;
#ifdef USE_ALLOCA
  B		*tmppath;
#else
  B		tmppath[MAX_NAMELEN];
#endif
  W		errno;


  /* プロセスの現在のカレントディレクトリを
   * 取り出す。
   * パス名の形でプログラムに返すが、
   * POSIX マネージャ内にはパス名の形では、
   * 記憶していない(inode へのポインタで記憶している)。
   * そのため、inode からパス名を取り出すようにする。
   */
  errno = proc_get_cwd (req->procid, &cwd);
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

#ifdef USE_ALLOCA
  tmppath = alloca (req->param.par_getcwd.dirnamelen);
  if (tmppath == NULL)
    {
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

  /* inode からパス名(文字列)へと変換する
   */
  errno = fs_convert_path (cwd, tmppath, req->param.par_getcwd.dirnamelen);
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = vput_reg (req->caller,
		    req->param.par_getcwd.dirname,
		    strlen (tmppath) + 1,
		    tmppath);
  if (errno)
    {
      put_response (req, errno, -1, 0, 0);
      return (FAIL);
    }

  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
}  
