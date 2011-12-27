/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_fs.h - SFS のための定義
 *
 *
 * $Log: sfs_fs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.11  1999-05-28 15:48:37  naniwa
 * sfs ver 1.1
 *
 * Revision 1.10  1997/07/03 14:24:36  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.9  1996/11/18  13:46:11  night
 * ファイルの中身を作成。
 *
 * Revision 1.8  1996/11/16  12:54:47  night
 * SFS_FMT_REG、SFS_FMT_DIR、SFS_FMT_DEV マクロの値を変更した。
 *
 * Revision 1.7  1996/11/14  13:18:33  night
 * 二重間接ブロックのための定義を追加。
 *
 * Revision 1.6  1996/11/13  12:54:00  night
 * sfs_inode 構造体の形式の変更。
 *
 * Revision 1.5  1996/11/12  11:32:25  night
 * SFS_DIRECT_BLOCK_ENTRY マクロの値を 100 から 53 に変更した。
 * (sfs_inode 構造体の大きさを 256 バイトに合わせるため)
 *
 * Revision 1.4  1996/11/10  11:56:20  night
 * sfs_inode 構造体にブロック管理用の領域を追加。
 * (sfs_i_direct 要素)
 *
 * Revision 1.3  1996/11/08  11:06:18  night
 * SFS ファイルシステムの各種構造体を定義した。
 *
 * Revision 1.2  1996/11/07  21:12:15  night
 * 文字コードを EUC に変更した。
 *
 * Revision 1.1  1996/11/07  12:50:56  night
 * 最初の登録
 *
 */


#ifndef __SFS_FS_H__
#define __SFS_FS_H__	1

#include "../../../ITRON/h/types.h"


#define SFS_MAGIC			0x12345678
#define SFS_MAXNAMELEN			14
#define SFS_VERSION_HI			1
#define SFS_VERSION_LO			1
#define SFS_DIRECT_BLOCK_ENTRY		70

#define SFS_INDIRECT_BLOCK_ENTRY	20
#define SFS_DINDIRECT_BLOCK_ENTRY 	26
#define SFS_TINDIRECT_BLOCK_ENTRY 	1
#define SFS_INDIRECT_BLOCK		128


#define SFS_FMT_REG	(0010000)
#define SFS_FMT_DIR	(0020000)
#define SFS_FMT_DEV	(0040000)


struct sfs_superblock
{
  UW	sfs_magic;
  H	sfs_version_hi;
  H	sfs_version_lo;
  UW	sfs_mountcount;
  H	sfs_blocksize;		/* ブロックサイズ */

  UW	sfs_nblock;
  UW	sfs_freeblock;

  UW	sfs_bitmapsize;

  UW	sfs_ninode;
  UW	sfs_freeinode;

  UW	sfs_isearch;		/* この番号以下の inode は使用中 */
  UW	sfs_bsearch;		/* この番号以下の block は使用中 */

  UW	sfs_datablock;		/* データ領域の開始位置。
				 * inode は、スーパーブ
				 * ロックの直後になるので、
				 * 特に記録しない。*/
};



struct sfs_inode
{
  UW	sfs_i_index;	/* SFS の場合、inode は 1 からはじまる */
  UW	sfs_i_nlink;
  UW	sfs_i_size;
  UW	sfs_i_size_blk;
  
  UW	sfs_i_perm;
  UW	sfs_i_uid;
  UW	sfs_i_gid;
  UW	sfs_i_dev;	/* not used */

  UW	sfs_i_atime;
  UW	sfs_i_ctime;
  UW	sfs_i_mtime;

  UW	sfs_i_direct[SFS_DIRECT_BLOCK_ENTRY];
  UW	sfs_i_indirect[SFS_INDIRECT_BLOCK_ENTRY];
  UW	sfs_i_dindirect[SFS_DINDIRECT_BLOCK_ENTRY];
  UW	sfs_i_tindirect[SFS_TINDIRECT_BLOCK_ENTRY]; /* reservation only */
};


struct sfs_indirect
{
  UW	sfs_in_block[SFS_INDIRECT_BLOCK];
};


struct sfs_dir
{
  UW	sfs_d_index;		/* inode 番号 */
  B	sfs_d_name[SFS_MAXNAMELEN];
  B	pad[2];			/* padding */
};


extern W	sfs_mount ();
extern W	sfs_mountroot ();
extern W	sfs_umount ();
extern W	sfs_statfs ();
extern W	sfs_syncfs ();
extern W	sfs_get_inode ();
extern W	sfs_put_inode ();

extern struct fsops	sfs_fsops;
extern struct iops	sfs_iops;

#endif /* __SFS_FS_H__ */
