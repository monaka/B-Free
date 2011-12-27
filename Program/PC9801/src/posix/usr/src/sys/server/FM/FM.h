/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/FM.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: FM.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:53:37  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/08/12  16:26:43  night
 * #include <native/server.h> の削除。
 *
 * Revision 1.2  1995/08/05  12:12:50  night
 * サブファイルシステムのオペレーション関数の定義の追加。
 *
 * Revision 1.1  1995/03/18  14:01:11  night
 * 最初の登録
 *
 *
 */

#ifndef __POSIX_FM_H__
#define __POSIX_FM_H__	1

#include <sys/types.h>
#include <sys/config.h>
#include <sys/file.h>
#include <server/file.h>

#define PORT_MANE	"posix.manager.FM"

struct subfs_t
{
  char			*fsname;	/* ファイルシステム名 */
  struct subfs_proc	*procp;		/* サブファイルシステム用の関数 */

  struct fileid		*filelist;	/* ファイルリストへのポインタ */
};


struct subfs_proc
{
  W	(*fs_null)();
  W	(*fs_access)();
  W	(*fs_lookup)();
  W	(*fs_getattr)();
  W	(*fs_setattr)();
  W	(*fs_read)();
  W	(*fs_write)();
  W	(*fs_trunc)();
  W	(*fs_mkdir)();
  W	(*fs_rmdir)();
  W	(*fs_mknod)();
  W	(*fs_rmnod)();
  W	(*fs_link)();
  W	(*fs_remove)();
  W	(*fs_select)();
  W	(*fs_control)();
  W	(*fs_move)();

  W	(*fs_mountroot)();
  W	(*fs_mount)();
  W	(*fs_unmount)();
};


struct filelist
{
  struct filelist	*previous;
  struct filelist	*next;

  struct fileid		fid;
  
};


extern void	doit (fm_msg_t *req);


#endif /* __POSIX_FM_H__ */

