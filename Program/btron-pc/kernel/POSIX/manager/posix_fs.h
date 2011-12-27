/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* posix_fs.h - POSIX ファイルシステム
 *
 *
 * $Log: posix_fs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.21  2000-06-23 09:18:12  naniwa
 * to support O_APPEND
 *
 * Revision 1.20  2000/06/01 08:46:49  naniwa
 * to implement getdents
 *
 * Revision 1.19  2000/05/06 03:52:25  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.18  1999/05/28 15:46:43  naniwa
 * sfs ver 1.1
 *
 * Revision 1.17  1999/03/24 04:52:08  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.16  1997/10/24 13:58:39  night
 * 関数定義の追加。
 *
 * Revision 1.15  1997/10/11 16:25:19  night
 * ファイルの write 処理の修正。
 *
 * Revision 1.14  1997/09/09 13:50:32  night
 * POSIX のファイルへの書き込み処理(システムコールは、write) を追加。
 * これまでは、EP_NOSUP を返していた。
 *
 * Revision 1.13  1997/08/31 13:31:35  night
 * lseek システムコール用の定義を追加。
 *
 * Revision 1.12  1997/07/09 15:03:05  night
 * inode 構造体に i_dirty という要素を追加。
 * この要素は、構造体が変更されており、かつディスクに反映されていないこと
 * を示す。
 *
 * Revision 1.11  1997/07/04 15:07:39  night
 * ・スペシャルファイル - デバイスドライバポートの対応表の関連処理の追加。
 * ・ファイルの読み込み処理の改訂。
 *
 * Revision 1.10  1997/07/02 13:26:01  night
 * FS_MOUNTROOT マクロの引数を修正。
 *
 * Revision 1.9  1997/04/25 13:00:38  night
 * struct statfs 構造体の定義を追加。
 *
 * Revision 1.8  1997/03/25 13:34:53  night
 * ELF 形式の実行ファイルへの対応
 *
 * Revision 1.7  1996/11/20  12:10:20  night
 * FILE_UNLINK マクロ の追加。
 *
 * Revision 1.6  1996/11/18  13:43:20  night
 * inode の要素 coverfs を coverfile に変更。
 *
 * Revision 1.5  1996/11/14  13:16:54  night
 * 各種定義の追加。
 *
 * Revision 1.4  1996/11/12  11:31:42  night
 * ユーザプログラムの時には、POSIX マネージャの関数の extern 宣言を有効に
 * しないようにした。
 *
 * Revision 1.3  1996/11/08  11:04:01  night
 * O_RDONLY, FS_FMT_REG, FS_FMT_DIR, FS_FMT_DEV マクロの追加。
 *
 * Revision 1.2  1996/11/07  21:12:04  night
 * 文字コードを EUC に変更した。
 *
 * Revision 1.1  1996/11/07  12:49:19  night
 * 最初の登録
 *
 *
 */

#ifndef __POSIX_FS_H__
#define __POSIX_FS_H__	1



#ifndef O_RDONLY
#define O_RDONLY	0x00
#define O_WRONLY	0x01
#define O_RDWR		0x02
#define O_CREAT		0x08
#define O_APPEND	0x10
#endif

#define FS_FMT_REG	(0010000)
#define FS_FMT_DIR	(0020000)
#define FS_FMT_DEV	(0040000)


#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#define X_BIT		1
#define W_BIT		2
#define R_BIT		4
#define SU_UID		0

/* file system types. */

#define FS_SFS		1

#ifdef KERNEL


#define FS_MOUNTROOT(fsp,device,rootfs,rootfile)	(fsp->fs_mountroot)(device, rootfs,rootfile)

#define FILE_OPEN

#define FILE_CLOSE(ip)	(ip->i_ops->i_close (ip))

#define FILE_SYNC(ip)	(ip->i_ops->i_sync (ip))

#define FILE_LOOKUP(ip,part,oflag,mode,acc,tmpip)	\
	(ip->i_ops->i_lookup)(ip,part,oflag,mode,acc,tmpip)

#define FILE_WRITE(ip,start,buf,length,rlength)	(ip->i_ops->i_write) (ip,start,buf,length,rlength)

#define FILE_READ(ip,start,buf,length,rlength)	(ip->i_ops->i_read) (ip,start,buf,length,rlength)

#define FILE_CREATE(ip,path,oflag,mode,acc,newip)	\
	(ip->i_ops->i_create)(ip,path,oflag,mode,acc,newip)

#define FILE_UNLINK(ip,path,acc)	\
	(ip->i_ops->i_unlink)(ip,path,acc)

#define DIR_CREATE(ip, path, mode, acc, newip) \
	(ip->i_ops->i_mkdir)(ip, path, mode, acc, newip)

#define DIR_UNLINK(ip,path,acc)\
	(ip->i_ops->i_rmdir)(ip, path, acc)

#define GET_DENTS(ip, caller, offset, buf, length, rsize, fsize) \
	(ip->i_ops->i_getdents)(ip, caller, offset, buf, length, rsize, fsize)

struct fsops
{
  W		(*fs_mount)();
  W		(*fs_mountroot)();
  W		(*fs_umount)();
  W		(*fs_statfs)();
  W		(*fs_syncfs)();
  W		(*fs_read_inode)();
  W		(*fs_write_inode)();
  W		(*fs_read_super)();
  W		(*fs_write_super)();
  W		(*fs_get_inode)();		/* open */
  W		(*fs_put_inode)();		/* close */
};


struct fs
{
  struct fs	*fs_prev;
  struct fs	*fs_next;
  W		fs_magicid;
  W		fs_typeid;
  W		fs_refcount;
  W		fs_rflag;
  struct fsops	*fs_ops;
  W		fs_lock;
  UW		fs_device;
  struct inode	*fs_ilist;		/* 使用中の inode のリスト */
  W		fs_blksize;
  struct inode	*rootdir;

  W		fs_allblock;
  W		fs_freeblock;
  W		fs_usedblock;

  W	        fs_allinode;
  W		fs_freeinode;
  W		fs_usedinode;

  UW		fs_isearch;		/* この番号以下の inode は使用中 */
  UW		fs_bsearch;		/* この番号以下の block は使用中 */

  union
    {
      struct sfs_superblock	sfs_fs;
    } fs_private;
};



struct inode
{
  struct inode	*i_prev;
  struct inode	*i_next;
  struct fs	*i_fs;
  UW		i_device;
  UW		i_lock;
  struct iops	*i_ops;
  W		i_refcount;
  W		i_dirty;	/* この Inode は変更されており、ファイル上に変更が
				 * 反映されていない
				 */
  struct inode	*coverfile;	/* もし、*ここ* が マウントポイントの時には、 */
				/* この要素を実際のファイルとして処理する */

  /* in disk */
  UW		i_mode;
  UW		i_link;
  UW		i_index;
  UW		i_uid;
  UW		i_gid;
  UW		i_dev;		/* if device file */
  UW		i_size;
  UW		i_atime;
  UW		i_ctime;
  UW		i_mtime;
  UW		i_size_blk;

  /* ここに各ファイルシステムの独自の情報が入る (union で... )*/
  union
    {
      struct sfs_inode	sfs_inode;
    } i_private;
};


struct iops
{
  W		(*i_lookup)();
  W		(*i_create)();
  W		(*i_close)();
  W		(*i_read)();
  W		(*i_write)();
  W		(*i_stat)();
  W		(*i_truncate)();
  W		(*i_link)();
  W		(*i_unlink)();
  W		(*i_symlink)();		/* not used */
  W		(*i_chmod)();
  W		(*i_chown)();
  W		(*i_chgrp)();
  W		(*i_rename)();
  W		(*i_sync)();
  W		(*i_mkdir)();
  W		(*i_rmdir)();
  W		(*i_getdents)();
};


struct access_info
{
  W		uid;
  W		gid;
};

#endif /* KERNEL */


/* stat システムコール用
 */
struct stat
{
  UW		st_dev;
  UW		st_ino;
  UW		st_mode;
  UW		st_nlink;
  UW		st_uid;
  UW		st_gid;
  UW		st_rdev;
  UW		st_size;
  UW		st_blksize;
  UW		st_blocks;
  UW		st_atime;
  UW		st_mtime;
  UW		st_ctime;
};


/* statfs システムコール用
 */
struct statfs
{
  W		f_type;
  W		f_bsize;
  W		f_blocks;
  W		f_bfree;
  W		f_bavail;
  W		f_files;
  W		f_free;
};


struct special_file
{
  UW			major_minor;		/* スペシャルファイルのメジャー/マイナー番号 */
  						/* 最上位ビットが 0 の場合はキャラクタ型 */
  						/* 最上位ビットが 1 の場合はブロック型 */
  B			name[MAX_MODULE_NAME];	/* デバイスドライバの登録名 */
  ID			port;			/* デバイスドライバへのポート */
  UW			dd;			/* デバイスの機器番号およびパーティション番号 */
  ER			(*handle)();		/* 特別の処理(ネームドパイプなど) をするときのハンドラ */
};

extern struct special_file	special_file_table[];



#ifdef KERNEL

/* filesystem.c */
extern W		init_fs (void);
extern W		get_device_info (UW major_minor, ID *port, UW *dd);
extern struct inode	*alloc_inode (void);
extern W		dealloc_inode (struct inode *);
extern struct fs	*alloc_fs (void);
extern void		dealloc_fs (struct fs *);

extern W		fs_open_file (B *path, W oflag, W mode, struct access_info *acc,
				      struct inode *startip, struct inode **newip);
extern W		fs_close_file (struct inode *ip);
extern W		fs_lookup (struct inode	*startip, char *path, W	oflag,
				   W mode, struct access_info *acc, struct inode **newip);
extern W		fs_create_file (struct inode *startip, char *path, W oflag,
					W mode, struct access_info *acc, struct inode **newip);
extern W		fs_sync_file (struct inode *ip);
extern W		fs_read_file (struct inode *ip, W start, B *buf, W length, W *rlength);
extern W		fs_write_file (struct inode *ip, W start, B *buf, W length, W *rlength);
extern W		fs_remove_file (struct inode *startip, B *path,	struct access_info *acc);
extern W		fs_remove_dir (struct inode *startip, B *path, struct access_info *acc);
extern W		fs_statfs (ID device, struct statfs *result);


extern W		mount_root (ID device, W fstype, W option);
extern W		mount_fs (struct inode *device, struct inode *mountpoint, W fstype, W option);


extern struct inode	*fs_check_inode (struct fs *fsp, W index);
extern W		fs_register_inode (struct inode *ip);
extern W		init_special_file (void);
extern W		fs_convert_path (struct inode *ip, B *buf, W length);

extern W		permit(struct inode *ip, struct access_info *acc, UW bits);


extern struct fs	fs_buf[], *free_fs, *rootfs;
extern struct inode	inode_buf[], *free_inode, *rootfile;
extern struct file	file_buf[], *free_file;


#endif

#endif /* __POSIX_FS_H__ */


