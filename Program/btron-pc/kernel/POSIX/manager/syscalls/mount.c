/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: mount.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-03-24 05:13:19  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.2  1999/03/24 05:12:52  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_mount_f - ファイルシステムをマウントする
 *
 * 引数:
 *	devnamelen	マウントするデバイス名の長さ
 *	devname		マウントするデバイス名
 *	dirnamelen	マウントするディレクトリ名の長さ
 *	dirname		マウントするディレクトリ名
 *	fstype		ファイルシステムのタイプ
 *	option		オプション
 *
 * Note:
 *	このシステムコールは、POSIX では定義されていない。
 *
 */
W
psc_mount_f (struct posix_request *req)
{
  W		errno;
#ifdef USE_ALLOCA
  B		*dirname, *devname;
#else
  B		dirname[MAX_NAMELEN];
  B		devname[MAX_NAMELEN];
#endif
  struct inode	*mountpoint, *device;
  struct access_info	acc;

  /* デバイスのオープン
   */
#ifdef USE_ALLOCA
  devname = (B *)alloca (sizeof (B) * (req->param.par_mount.devnamelen + 1));
  if (devname == NULL)
    {
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif

  vget_reg (req->caller,
	    req->param.par_mount.devname,
	    req->param.par_mount.devnamelen,
	    devname);
  devname[(W)(req->param.par_mount.devnamelen)] = '\0';
  acc.uid = 0;
  acc.gid = 0;
  if (fs_open_file (devname, O_RDWR, 0, &acc, rootfile, &device) == FAIL)
    {
      put_response (req, EP_NOENT, -1, 0, 0);
      return (FAIL);
    }
  
  /* マウントポイントのオープン
   */
#ifdef USE_ALLOCA
  dirname = (B *)alloca (sizeof (B) * (req->param.par_mount.dirnamelen + 1));
  if (dirname == NULL)
    {
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif
  vget_reg (req->caller,
	    req->param.par_mount.dirname,
	    req->param.par_mount.dirnamelen,
	    dirname);
  dirname[req->param.par_mount.dirnamelen] = '\0';
  acc.uid = 0;
  acc.gid = 0;
  if (fs_open_file (dirname, O_RDWR, 0, &acc, rootfile, &mountpoint) == FAIL)
    {
      put_response (req, EP_NOENT, -1, 0, 0);
      fs_close_file (device);
      return (FAIL);
    }

  if (mountpoint->i_refcount > 1)
    {
      put_response (req, EP_BUSY, -1, 0, 0);
      fs_close_file (device);
      fs_close_file (mountpoint);
      return (FAIL);
    }

  errno = mount_fs (device,
		    mountpoint,
		    req->param.par_mount.fstype,
		    req->param.par_mount.option);
  if (errno == EP_OK)
    {
      put_response (req, EP_OK, 0, 0, 0);
      fs_close_file (device);
      fs_close_file (mountpoint);
      return (SUCCESS);
    }

  put_response (req, errno, -1, 0, 0);
  fs_close_file (device);
  fs_close_file (mountpoint);
  return (FAIL);
}  
