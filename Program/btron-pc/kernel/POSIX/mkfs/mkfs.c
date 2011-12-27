/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/*
 * $Log: mkfs.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.12  2000-05-06 03:50:59  naniwa
 * fixed file parmission
 *
 * Revision 1.11  1999/05/28 15:52:11  naniwa
 * modified to support sfs ver 1.1
 *
 * Revision 1.10  1999/05/17 12:46:34  naniwa
 * changed 4th argument to 'KB per inode'
 *
 * Revision 1.9  1999/05/06 23:17:21  naniwa
 * fixed to keep file system consistent
 *
 * Revision 1.8  1999/04/18 17:13:18  monaka
 * Fixed memory allocation in writing bitmap. (Thanx for Mr. Naniwa.)
 *
 * Revision 1.7  1999/04/13 04:15:33  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.6  1997/09/09 13:51:20  night
 * MAX_MODULE_NAME マクロの定義を追加。
 *
 * Revision 1.5  1996/11/14 13:19:07  night
 * root および lost+found ディレクトリの分のブロック数をフリーブロック数
 * から減らしてファイルシステムを作成するように変更。
 *
 * Revision 1.4  1996/11/13  12:54:19  night
 * 作成するディレクトリのパーミッションの変更。
 *
 * Revision 1.3  1996/11/11  13:38:18  night
 * ルードディレクトリの内容を一部変更。
 * ('lost+found' ディレクトリを追加)
 *
 * Revision 1.2  1996/11/10  11:56:48  night
 * mkfs するときにブートブロックの分を考慮するようにした。
 *
 * Revision 1.1  1996/11/08  11:07:09  night
 * 最初の登録
 *
 *
 */


#define MAX_MODULE_NAME	40

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mkfs.h"


static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/mkfs/mkfs.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


int	nblock;
int	blocksize;
int	inodecount;
int	boot_block;
int	super_block;
int	bitmap_block;
int	inode_block;

void write_superblock (int formatfd);
void write_bitmap (int formatfd);
void write_inode (int formatfd);
void write_rootdir (int formatfd);
void set_bit (char buf[], int index);

struct sfs_dir	rootentry[] =
{
  { 1, "." },
  { 1, ".."}, 
  { 2, "lost+found"}
};


struct sfs_inode	rootdir =
{
  1,				/* sfs_i_index */
  3,				/* sfs_i_nlink */
  sizeof (rootentry),		/* sfs_i_size */
  1,				/* sfs_i_size_blk */
  (SFS_FMT_DIR | 0777),		/* sfs_i_perm */
  0,				/* sfs_i_uid */
  0,				/* sfs_i_gid */
  0,				/* sfs_i_dev (not used) */
  0,				/* sfs_i_atime (now time) */
  0,				/* sfs_i_ctime (now time) */
  0,				/* sfs_i_mtime (now time) */
};


struct sfs_dir	lostfound_entry[] = 
{
  { 2, "." },
  { 1, ".." }
};

struct sfs_inode	lostfound =
{
  2,				/* sfs_i_index */
  2,				/* sfs_i_nlink */
  sizeof (lostfound_entry),	/* sfs_i_size */
  1,				/* sfs_i_size_blk */
  (SFS_FMT_DIR | 0777),		/* sfs_i_perm */
  0,				/* sfs_i_uid */
  0,				/* sfs_i_gid */
  0,				/* sfs_i_dev (not used) */
  0,				/* sfs_i_atime (now time) */
  0,				/* sfs_i_ctime (now time) */
  0,				/* sfs_i_mtime (now time) */
};

void
usage (void)
{
  printf ("usage: mkfs file nblock blocksize KB-per-inode\n");
}

/* main -
 *
 */
int
main (int ac, char **av)
{
  int formatfd;
  int kbpinode;

  if (ac < 5)
    {
      usage ();
      return (0);
    }

  nblock = atoi (av[2]);
  blocksize = atoi (av[3]);
  kbpinode = atoi (av[4]);

  formatfd = open (av[1], O_RDWR | O_CREAT, 0666);
  if (formatfd < 0)
    {
      printf ("Cannot open file or device (%s)\n", av[1]);
      return (1);
    }

  if (kbpinode > 99)
    {
      printf("WARNING: The 4th argument has been changed to 'KB per inode' from 'number of inode'.\n");
    }

  boot_block = 1;
  super_block = 1;
  bitmap_block = ROUNDUP (nblock / 8, blocksize) / blocksize;
  inodecount = (nblock - boot_block - super_block - bitmap_block)/
    (kbpinode*2 + ROUNDUP(sizeof(struct sfs_inode), blocksize)/blocksize);
  inode_block = ROUNDUP (inodecount * sizeof (struct sfs_inode), blocksize) / blocksize;

  printf ("superblock = %d, bitmap block = %d, inode block = %d\n",
	  super_block, bitmap_block, inode_block);

  write_superblock (formatfd);
  write_bitmap (formatfd);
  write_inode (formatfd);
  write_rootdir (formatfd);

  close (formatfd);
  exit (0);
}

void
write_superblock (int formatfd)
{
  int error;

  struct sfs_superblock	superblock;

  superblock.sfs_magic = SFS_MAGIC;
  superblock.sfs_version_hi = SFS_VERSION_HI;
  superblock.sfs_version_lo = SFS_VERSION_LO;
  superblock.sfs_mountcount = 0;
  superblock.sfs_blocksize = blocksize;
  superblock.sfs_nblock = nblock;
  superblock.sfs_freeblock = nblock - (boot_block + super_block + bitmap_block + inode_block + 2);
  superblock.sfs_bitmapsize = bitmap_block;
  superblock.sfs_ninode = inodecount;
  superblock.sfs_freeinode = inodecount - 2;	/* root ディレクトリの分 */
  superblock.sfs_datablock = (boot_block + super_block + bitmap_block + inode_block);
  superblock.sfs_isearch = 2;
  superblock.sfs_bsearch = (boot_block + super_block + bitmap_block
			    + inode_block + 2)-1;

  lseek (formatfd, blocksize, 0);
  error = write (formatfd, &superblock, sizeof (superblock));
  if (error < 0)
    {
      perror("Write error in write_superblock().\n");
    }
}


void
write_bitmap (int formatfd)
{
  char	*buf;
  int	i;
  int error;

  buf = alloca (blocksize * bitmap_block);
  memset (buf, 0, blocksize * bitmap_block);
  lseek (formatfd, blocksize * (super_block + boot_block), 0);
  for (i = 0; i < bitmap_block; i++)
    {
      write (formatfd, buf, blocksize);
    }
  lseek (formatfd, blocksize * (super_block + boot_block), 0);
  for (i = 0; i < (boot_block + super_block + bitmap_block + inode_block + 2); i++)
    {
      set_bit (buf, i);
    }

  error = write (formatfd, buf, blocksize * bitmap_block);
  if (error < 0)
    {
      perror("Write error in write_bitmap().\n");
    }
}



void
set_bit (char buf[], int index)
{
  int	byte_offset;
  int	bit_offset;
  int	value;

  byte_offset = index / 8;
  bit_offset = index % 8;

  value = 1 << bit_offset;

  buf[byte_offset] |= value;
}



void
write_inode (int formatfd)
{
  char	*buf;
  int	i;
  int error;

  buf = alloca (blocksize);
  memset (buf, 0, blocksize);
  for (i = 0; i < inode_block; i++)
    {
      error = write (formatfd, buf, blocksize);
      if (error < 0)
	{
	  perror("Write error in write_inode().\n");
	}
    }

  lostfound.sfs_i_atime = rootdir.sfs_i_atime = time (NULL);
  lostfound.sfs_i_mtime = rootdir.sfs_i_mtime = time (NULL);
  lostfound.sfs_i_ctime = rootdir.sfs_i_ctime = time (NULL);
  rootdir.sfs_i_direct[0] = (boot_block + super_block + bitmap_block + inode_block);
  lostfound.sfs_i_direct[0] = (boot_block + super_block + bitmap_block + inode_block) + 1;

  lseek (formatfd, blocksize * (boot_block + super_block + bitmap_block), 0);
  write (formatfd, &rootdir, sizeof (rootdir));
  write (formatfd, &lostfound, sizeof (lostfound));
}



void
write_rootdir (int formatfd)
{
  lseek (formatfd, blocksize * (boot_block + super_block + bitmap_block + inode_block), 0);
  write (formatfd, rootentry, sizeof (rootentry));

  lseek (formatfd, blocksize * (boot_block + super_block + bitmap_block + inode_block + 1), 0);
  write (formatfd, lostfound_entry, sizeof (lostfound_entry));
}
