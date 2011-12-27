/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: filesystem.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */

/*
 * ファイルシステムの管理を行うための関数群
 *
 * 以下の関数を定義している。
 *
 * init_filesystem ...... ファイルシステム処理のための初期化
 * attach ............... ファイルシステムのマウント
 * dettach .............. ファイルシステムのアンマウント
 * get_root ............. ファイルシステムの root ディレクトリの取得
 * lookup ............... 指定したファイル情報を取得する
 * get_attribute ........ ファイルの情報を取得
 * get_directory_list ... ディレクトリ内のエントリ一覧を取得する
 * read ................. ファイルの内容の読み込み
 * write ................ ファイルの内容の書き込み
 * truncate ............. ファイルサイズの縮退
 * make_directory ....... ディレクトリの作成
 * remove_directory ..... ディレクトリの削除
 *
 */

/*
 * $Log: filesystem.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-07-25 16:02:00  night
 * IBM PC 版用への最初の登録
 *
 */

#include "init.h"


extern struct file_ops	dosfs_ops;

struct filesystem_entry filesystem_table[] =
{
  { "dosfs", &dosfs_ops},	/* DOS ファイルシステムのためのエントリ */
};

W	nfilesystem;


/*
 * ファイルシステムの管理用データ
 */
struct filesystem	*root_fs;
struct filedata		*rootdir;
struct filedata		*current_dir;


/* init_filesystem ファイルシステム処理のための初期化
 *
 * root_device	root ファイルシステムとなるデバイス
 * type		ファイルシステムの種類
 * root		(返り値) ファイルシステムの root ディレクトリの情報
 */
ER
init_filesystem (ID root_device, UW part, B *type)
{
  W	i;
  ER	error;

  nfilesystem = sizeof (filesystem_table) / sizeof (struct filesystem_entry);
  for (i = 0; i < nfilesystem; i++)
    {
      if (strcmp (type, filesystem_table[i].fsname) == 0)
	{
	  break;
	}
    }
  if (i >= nfilesystem)
    {
      return (E_OBJ);	/* 引数に該当するファイルシステムはない */
    }

  
  root_fs = (struct filesystem *)malloc (sizeof (struct filesystem));
  root_fs->fs_type = &(filesystem_table[i]);
  root_fs->mount_file = NULL;
  root_fs->private = NULL;

  error = FOP_ATTACH (root_fs, root_device, part);
  root_fs->device = root_device;
  root_fs->dd = part;

  if (error != E_OK)
    {
      return (error);
    }

/*
  rootdir = (struct filedata *)malloc (sizeof (struct filedata));
  if (rootdir == NULL)
    {
      free (root_fs);
      root_fs = NULL;
      return (E_NOMEM);
    }
*/

  error = FOP_GETROOT(root_fs, &rootdir);
  if (error != E_OK)
    {
      printf ("init_filesystem: cannot getrootdir\n");
      return (error);
    }

/*  printf ("rootdir = 0x%x\n", rootdir); */
  current_dir = rootdir;
  return (E_OK);
}

 
ER
attach (struct filedata *parent, ID device, UW dd)
{
  ER error;

  error = FOP_ATTACH (parent->fs, device, dd);
  return (error);
}


/* get_root ............. ファイルシステムの root ディレクトリの取得
 *
 *
 */
ER
get_root (struct filesystem *fs, struct filedata **fp)
{
  return (FOP_GETROOT (fs, fp));
}


B *
fentry_copy (B *fname, B *bp)
{
  while (*fname == '/')
    fname++;

  while ((*fname != '/') && (*fname != '\0'))
    *bp++ = *fname++;

  *bp = '\0';
  return (fname);
}

/* lookup ............... 指定したファイル情報を取得する
 *
 *
 */
ER
lookup (struct filedata *parent, B *fname, struct filedata **ret_fp)
{
  struct filedata *fp;
  B		  bp[MAXFILENAME];
  ER		  error;
  struct filedata	*tmp;

  if (*fname == '\0')
    {
      (*ret_fp) = parent;
      return (E_OK);
    }

  tmp = parent;
  while (*fname != '\0')
    {
      fname = fentry_copy (fname, bp);
      printf ("name: %s\n", fname);	/* */

      printf ("lookup: call lookup for dos. parent = 0x%x, bp = 0x%x, fp = 0x%x\n",
	      parent, bp, &fp);
      error = FOP_LOOKUP (parent, bp, &fp);
      if (error != E_OK)
	{
	  return (error);
	}

      if (parent != tmp)
	free (parent);
      parent = fp;
    }
  
  (*ret_fp) = fp;
  printf ("lookup: return\n");	/* */
  return (E_OK);
}


/* get_attribute ........ ファイルの情報を取得
 *
 *
 */
ER
get_attribute (struct filedata *fp, struct fstat *ret_st)
{
#ifdef notdef
  printf ("get_attribute: %s, %d\n", __FILE__, __LINE__);
  printf ("0x%x, 0x%x\n", fp, ret_st);
  printf ("fp = 0x%x\n", fp);
  printf ("fp->fs = 0x%x\n", fp->fs);
  printf ("fp->fs->fs_type = 0x%x\n", fp->fs->fs_type);
  printf ("fp->fs->fs_type->fop = 0x%x\n", fp->fs->fs_type->fop);
  printf ("fp->fs->fs_type->fop->getattr = 0x%x\n", fp->fs->fs_type->fop->getattr);
#endif
  return (FOP_GETATTR (fp, ret_st));
}


/* get_directory_list ... ディレクトリ内のエントリ一覧を取得する
 *
 *
 */
ER
get_directory_list (struct filedata *dir, struct directory *ret)
{
  return (FOP_GETDIRECTORY (dir, ret));
}


/* truncate ............. ファイルサイズの縮退
 *
 *
 */

/* read ................. ファイルの内容の読み込み
 *
 *
 */
ER
read (struct filedata *fp, W offset, W size, B *buf)
{
  return (FOP_READ (fp, offset, size, buf));
}


/* write ................ ファイルの内容の書き込み
 *
 *
 */
ER
write (struct filedata *fp, W offset, W size, B *buf)
{
  return (FOP_WRITE (fp, offset, size, buf));
}


/* make_directory ....... ディレクトリの作成
 *
 *
 */

/* remove_directory ..... ディレクトリの削除
 *
 *
 */

