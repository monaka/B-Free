/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/*
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/sfs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: sfs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.4  1999/03/15 01:35:33  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.3  1997/06/29 13:13:46  night
 * HD の SFS フォーマットされたパーティションからの BTRON OS をロードできる
 * 機能を追加。
 *
 * Revision 1.2  1996/11/20  12:08:09  night
 * 間接ブロックに対応した。
 *
 * Revision 1.1  1996/11/13  16:30:15  night
 * 最初の登録
 *
 *
 *
 */

#ifndef __SFS_H__
#define __SFS_H__	1



/*
 * POSIX で定義している error 番号
 */
#define E2BIG		1	/* Arg list too long */
#define EACCESS		2	/* Permission denied */
#define EAGAIN		3	/* Resouce temporarily unavailable */
#define EBADF		4	/* Bad file descriptor */
#define EBUSY		5	/* Resouce busy */
#define ECHILD		6	/* No child process */
#define EDEADLK		7	/* Resouce deadlock avoided */
#define EDOM		8	/* Domain error */
#define FEXIST		9	/* File exists */
#define EFAULT		10	/* Bad address */
#define EFBIG		11	/* File too large */
#define EINTR		12	/* Interrupted function call */
#define EINVAL		13	/* Invalid argument */
#define EIO		14	/* Input/output error */
#define EISDIR		15	/* Is a directory */
#define EMFILE		16	/* Too many open files */
#define EMLINK		17	/* Too many links */
#define ENAMETOOLONG	18	/* Filename too long */
#define ENFILE		19	/* Too many open files in system */
#define ENODEV		20	/* No such device */
#define ENOENT		21	/* No such file or directory */
#define ENOEXEC		22	/* Exec format error */
#define ENOLCK		23	/* No locks available */
#define ENOMEM		24	/* Not enough space */
#define ENOSPC		25	/* No space left on device */
#define ENOSYS		26	/* Function not implemented */
#define ENOTDIR		27	/* Not a directory */
#define ENOTEMPTY	28	/* Directory not empty */
#define ENOTTY		29	/* Inappropriate I/O control operation */
#define ENXIO		30	/* No such device or address */
#define EPERM		31	/* Operation not permitted */
#define EPIPE		32	/* Broken pipe */
#define ERANGE		33	/* Result too large */
#define EROFS		34	/* Read-only file system */
#define ESPIPE		35	/* Invalid seek */
#define ESRCH		36	/* No such process */
#define EXDEV		37	/* Improper link */


#define SFS_BLOCK_SIZE	512

#ifndef O_RDONLY
#define O_RDONLY	0
#define O_WRONLY	1
#define	O_RDWR		4
#endif

#define FS_FMT_REG	(0010000)
#define FS_FMT_DIR	(0020000)
#define FS_FMT_DEV	(0040000)

#define SFS_MAGIC			0x12345678
#define SFS_MAXNAMELEN			14
#define SFS_VERSION_HI			1
#define SFS_VERSION_LO			0
#define SFS_DIRECT_BLOCK_ENTRY		245

#define SFS_INDIRECT_BLOCK_ENTRY	128
#define SFS_DINDIRECT_BLOCK_ENTRY 	128
#define SFS_INDIRECT_BLOCK		128


#define SFS_FMT_DIR	(FS_FMT_DIR)
#define SFS_FMT_REG	(FS_FMT_REG)
#define SFS_FMT_DEV	(FS_FMT_DEV)


struct sfs_superblock
{
  unsigned long	sfs_magic;
  short		sfs_version_hi;
  short		sfs_version_lo;
  unsigned long	sfs_mountcount;
  short		sfs_blocksize;		/* ブロックサイズ */

  unsigned long	sfs_nblock;
  unsigned long	sfs_freeblock;

  unsigned long	sfs_bitmapsize;

  unsigned long	sfs_ninode;
  unsigned long	sfs_freeinode;

  unsigned long	sfs_datablock;		/* データ領域の開始位置。
				 * inode は、スーパーブ
				 * ロックの直後になるので、
				 * 特に記録しない。*/
};



struct sfs_inode
{
  unsigned long	sfs_i_index;	/* SFS の場合、inode は 1 からはじまる */
  unsigned long	sfs_i_nlink;
  unsigned long	sfs_i_size;
  unsigned long	sfs_i_size_blk;
  
  unsigned long	sfs_i_perm;
  unsigned long	sfs_i_uid;
  unsigned long	sfs_i_gid;
  unsigned long	sfs_i_dev;	/* not used */

  unsigned long	sfs_i_atime;
  unsigned long	sfs_i_ctime;
  unsigned long	sfs_i_mtime;

  unsigned long	sfs_i_direct[SFS_DIRECT_BLOCK_ENTRY];
  unsigned long	sfs_i_indirect[SFS_INDIRECT_BLOCK_ENTRY];
  unsigned long	sfs_i_dindirect[SFS_DINDIRECT_BLOCK_ENTRY];
};


struct sfs_indirect
{
  unsigned long	sfs_in_block[SFS_INDIRECT_BLOCK];
};


struct sfs_dir
{
  unsigned long	sfs_d_index;		/* inode 番号 */
  unsigned char	sfs_d_name[SFS_MAXNAMELEN];
  unsigned char	pad[2];			/* padding */
};


struct ic_sfs_superblock
{
  int				devid;
  int				partition;

  int				position;
  struct sfs_superblock		sb;
  struct in_sfs_inode		*rootp;
};


extern int sfs_read_file (int fd,
	   struct sfs_superblock *sb,
	   struct sfs_inode *ip,
	   int start,
	   int size,
	   char *buf);

extern struct ic_sfs_superblock		fs_table[1];
extern struct sfs_inode			rootdir_buf;
extern struct sfs_inode			*rootdirp;



#endif /* __SFS_H__ */
