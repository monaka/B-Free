/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* 
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/mkfs/sfsck.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: sfsck.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  2000-03-28 15:16:47  naniwa
 * to support device file
 *
 * Revision 1.7  1999/05/28 15:52:12  naniwa
 * modified to support sfs ver 1.1
 *
 * Revision 1.6  1999/05/10 13:39:41  naniwa
 * corrected header
 *
 * Revision 1.5  1999/05/01 04:58:42  naniwa
 * fixed to treat indirect dindirect block correctly
 *
 * Revision 1.4  1999/04/30 15:04:14  naniwa
 * modified to find out doubly allocated blocks
 *
 * Revision 1.3  1999/04/29 04:55:47  naniwa
 * remove some debug messages
 *
 * Revision 1.2  1999/04/29 04:35:33  naniwa
 * add check methods through directory tree
 *
 * Revision 1.1  1999/04/28 12:57:55  naniwa
 * First version
 *
 */

#define MAX_MODULE_NAME	40

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "../../ITRON/h/types.h"
#include "../manager/sfs/sfs_fs.h"


static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/mkfs/sfsck.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#define BLOCKSIZE 512

#define RDONLY			1
#define RDWR			2

#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
#define MIN(x,y)		((x > y) ? y : x)

/* Prototypes */
int scan_inode(int fd, struct sfs_superblock *sb);
int scan_bitmap(int fd, struct sfs_superblock *sb);
void scan_dirtree(int fd, struct sfs_superblock *sb, UW index);
void bitmap_scan_inode(int fd, struct sfs_superblock *sb);
void scan_block(int fd, struct sfs_superblock *sb, struct sfs_inode *ip);

/* copy from statfs.c */
W read_inode (int fd, struct sfs_superblock *sb, int ino, struct sfs_inode *ip);
int get_inode_offset (struct sfs_superblock *sb, int ino);
void print_superblock (struct sfs_superblock *sb);
W read_dir (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int nentry, struct sfs_dir *dirp);
int read_file (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int start, int size, B *buf);
int read_block (int fd, int blockno, int blocksize, B *buf);
int get_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);
int get_indirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);
int get_dindirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);

/* global data for file system check */
int *inode;
char *bitmap_disk, *bitmap_used;

/* main program */
int main (int argc, char *argv[])
{
  int fd;
  struct sfs_superblock	sb;
  int i;

  if (argc < 2) {
    fprintf(stderr, "usage: %s device_name\n", argv[0]);
    return(-1);
  }
  fd = open (argv[1], O_RDONLY);
  if (fd < 0) {
    fprintf (stderr, "Cannot open file.\n");
    return (-1);
  }

  if (lseek (fd, BLOCKSIZE, 0) < 0) {
      return(-1);
  }

  if (read (fd, &sb, sizeof (struct sfs_superblock))
      != sizeof (struct sfs_superblock)) {
    perror("can't read super block");
    return (-1);
  }
  print_superblock (&sb);

  inode = (int *) calloc(sb.sfs_ninode, sizeof(int));
  bitmap_disk =
    (char *) calloc(sb.sfs_bitmapsize * sb.sfs_blocksize * 8, sizeof(char));
  bitmap_used =
    (char *) calloc(sb.sfs_bitmapsize * sb.sfs_blocksize * 8, sizeof(char));

  printf("\nCheck of inode\n");
  i = scan_inode(fd, &sb);
  printf("counted used inode\t%d\n", i);
  if (i != (sb.sfs_ninode - sb.sfs_freeinode)) {
    fprintf(stderr, "WARNING: File System Inconsistent!!\n");
    free(inode);
    free(bitmap_disk);
    free(bitmap_used);
    return(-1);
  }

  printf("\nCheck of bitmap block\n");
  i = scan_bitmap(fd, &sb);
  printf("counted used block\t%d\n", i);
  if (i != (sb.sfs_nblock - sb.sfs_freeblock)) {
    fprintf(stderr, "WARNING: File System Inconsistent!!\n");
    free(inode);
    free(bitmap_disk);
    free(bitmap_used);
    return(-1);
  }

  printf("\nCheck of directory tree\n");
#ifdef notdef
  printf("link count on i-node block\n");
  for(i = 0; i < sb.sfs_ninode; ++i) {
    printf("%2d", inode[i]);
  }
  printf("\n");
#endif
  scan_dirtree(fd, &sb, 1L);
#ifdef notdef
  printf("difference bitween i-node block and dir tree\n");
#endif
  for(i = 0; i < sb.sfs_ninode; ++i) {
#ifdef notdef
    printf("%2d", inode[i]);
#endif
    if (inode[i] != 0) {
      fprintf(stderr, "WARNING: File System Inconsistent!!\n");
      free(inode);
      free(bitmap_disk);
      free(bitmap_used);
      return(-1);
    }
  }
#ifdef notdef
  printf("\n");
#endif

  printf("\nCheck of disk blocks used in i-node\n");
  i = scan_inode(fd, &sb);
  bitmap_scan_inode(fd, &sb);
  for(i = 0;
      i < 2 + sb.sfs_bitmapsize
	+ROUNDUP(sizeof(struct sfs_inode), sb.sfs_blocksize)
	  /sb.sfs_blocksize*sb.sfs_ninode;
      ++i) {
    bitmap_used[i] = 1;
  }
#ifdef notdef
  for(i = 0; i < 800; ++i) {
    printf("%2d", bitmap_disk[i]);
  }
  printf("\nused\n");
  for(i = 0; i < 800; ++i) {
    printf("%2d", bitmap_used[i]);
  }
  printf("\n");
#endif
  for(i = 0; i < sb.sfs_nblock; ++i) {
    if (bitmap_used[i] > 1) {
      fprintf(stderr, "WARNING: File System Inconsistent!!\n");
      fprintf(stderr, "file system contains doubly allocated block\n");
      free(inode);
      free(bitmap_disk);
      free(bitmap_used);
      return(-1);
    }
    else if (bitmap_disk[i] == 1 && bitmap_used[i] == 0) {
      fprintf(stderr, "WARNING: File System Inconsistent!!\n");
      fprintf(stderr, "file system contains unfreed block\n");
      free(inode);
      free(bitmap_disk);
      free(bitmap_used);
      return(-1);
    }
    else if (bitmap_disk[i] == 0 && bitmap_used[i] == 1) {
      fprintf(stderr, "WARNING: File System Inconsistent!!\n");
      fprintf(stderr, "file system looses using block\n");
      free(inode);
      free(bitmap_disk);
      free(bitmap_used);
      return(-1);
    }
  }

  /* File System が不整合だった場合，リカバリー処理をどうするか？ */

  printf("\nFile system seems to be consistent.\n");
  free(inode);
  free(bitmap_disk);
  free(bitmap_used);
  return(0);
}

int scan_inode(int fd, struct sfs_superblock *sb)
{
  int i, errno, count;
  struct sfs_inode ip;

  count = 0;
  for(i = 0; i < sb->sfs_ninode; ++i) {
    errno = read_inode(fd, sb, i+1, &ip);
    if (errno) return(errno);
    if (ip.sfs_i_index == (i + 1)) ++count;
    inode[i] = ip.sfs_i_nlink;
  }
  return(count);
}

int scan_bitmap(int fd, struct sfs_superblock *sb)
{
  char	*buf;
  int i, j, blocksize, count, mask;
  int errno;

  blocksize = sb->sfs_blocksize * sb->sfs_bitmapsize;
  buf = alloca (blocksize);
  memset (buf, 0, blocksize);
  lseek (fd, sb->sfs_blocksize * 2, 0);
  errno = read (fd, buf, blocksize);
  if (errno < 0) {
    perror("can't read bitmap block");
    return(errno);
  }
  count = 0;
  for (i = 0; i < blocksize; ++i) {
    mask = 0x1;
    for(j = 0; j < 8; ++j) {
      if (buf[i] & mask) {
	++count;
	bitmap_disk[i*8+j] = 1;
      }
      mask <<= 1;
    }
  }
  return(count);
}

void scan_dirtree(int fd, struct sfs_superblock *sb, UW index)
{
  int i, errno;
  struct sfs_inode ip;
  int	nentry;
  struct sfs_dir	*buf;

  errno = read_inode(fd, sb, (int) index, &ip);
  if ((ip.sfs_i_perm & SFS_FMT_DIR) != 0) {
    nentry = read_dir (fd, sb, &ip, 0, NULL);
    if (nentry < 0) {
      printf("No directory entory\n");
      exit(-1);
    }
  
    buf = alloca (sizeof (struct sfs_dir) * nentry);
    if (buf == NULL) {
      perror("scan_dirtree");
      exit(-1);
    }
    if (read_dir (fd, sb, &ip, nentry, buf) != 0) {
      printf("can't read directory data\n");
      exit(-1);
    }
    for (i = 0; i < 2; ++i) {
      inode[buf[i].sfs_d_index-1] -= 1;
    }
    for (i = 2; i < nentry; ++i) {
      scan_dirtree(fd, sb, buf[i].sfs_d_index);
      inode[buf[i].sfs_d_index-1] -= 1;
    }
  }
}

void bitmap_scan_inode(int fd, struct sfs_superblock *sb)
{
  int i, errno;
  struct sfs_inode ip;

  for(i = 0; i < sb->sfs_ninode; ++i) {
    if (inode[i] > 0) {
      errno = read_inode(fd, sb, i+1 , &ip);
      if ((ip.sfs_i_perm & SFS_FMT_DEV) == 0)
	scan_block(fd, sb, &ip);
    }
  }
}

void scan_block(int fd, struct sfs_superblock *sb, struct sfs_inode *ip)
{
  int i, j, block, index;
  struct sfs_indirect	inbuf;

  block = ip->sfs_i_size/sb->sfs_blocksize + 1;
  for(i = 0; i < block; ++i) {
    index = get_block_num (fd, sb, ip, i);
    bitmap_used[index]++;
  }
  /* 1 重間接ブロックの処理 */
  for(i = 0; i < SFS_INDIRECT_BLOCK_ENTRY; ++i) {
    if (ip->sfs_i_indirect[i] > 0) {
      bitmap_used[ip->sfs_i_indirect[i]]++;
    }
  }
  /* 2 重間接ブロックの処理 */
  for(i = 0; i < SFS_DINDIRECT_BLOCK_ENTRY; ++i) {
    if (ip->sfs_i_dindirect[i] > 0) {
      read_block (fd, ip->sfs_i_dindirect[i],
		  sb->sfs_blocksize, (B*)&inbuf);
      for(j = 0; j < SFS_INDIRECT_BLOCK; ++j) {
	if (inbuf.sfs_in_block[j] > 0) {
	  bitmap_used[inbuf.sfs_in_block[j]]++;
	}
      }
    }
  }
}

/* Functions from statfs.c */

void
print_superblock (struct sfs_superblock	*sb)
{
  if (sb->sfs_magic != SFS_MAGIC)
    {
      fprintf (stderr, "Invalid Magic ID\n");
      exit (1);
    }

  fprintf (stderr, "*STATUS* \n\n");
  fprintf (stderr, "FS type\t\tSFS\n");
  fprintf (stderr, "version\t\t%d.%d\n", sb->sfs_version_hi, sb->sfs_version_lo);
  fprintf (stderr, "total size\t%ld\n", sb->sfs_nblock * sb->sfs_blocksize);
  fprintf (stderr, "size\t\t%ld\n", sb->sfs_freeblock * sb->sfs_blocksize);
  fprintf (stderr, "mount count\t%ld\n", sb->sfs_mountcount);
  fprintf (stderr, "blocksize\t%d bytes\n", sb->sfs_blocksize);
  fprintf (stderr, "block\t\t%ld block, %ld free\n", sb->sfs_nblock, sb->sfs_freeblock);
  fprintf (stderr, "bitmap\t\t%ld bytes\n", sb->sfs_bitmapsize * sb->sfs_blocksize);
  fprintf (stderr, "inode\t\t%ld inode, %ld free\n", sb->sfs_ninode, sb->sfs_freeinode);
  fprintf (stderr, "isearch\t\t%ld, bsearch\t\t%ld\n", sb->sfs_isearch, sb->sfs_bsearch);
}

int
get_inode_offset (struct sfs_superblock *sb, int ino)
{
  int	offset;
  int	nblock;
  int	blocksize;

  nblock = sb->sfs_nblock;
  blocksize = sb->sfs_blocksize;
  offset = 1 + 1 + (ROUNDUP (nblock / 8, blocksize) / blocksize);
  offset *= blocksize;
  return (offset + ((ino - 1) * sizeof (struct sfs_inode)));
}

W
read_inode (int fd, struct sfs_superblock *sb, int ino, struct sfs_inode *ip)
{
  int	offset, errno;
  offset = get_inode_offset (sb, ino);
  lseek (fd, offset, 0);
  errno = read (fd, ip, sizeof (struct sfs_inode));
  if (errno < 0) {
    perror("read error inode\n");
    return(-1);
  }
  return (0);
}

W
read_dir (int fd,
	  struct sfs_superblock *sb,
	  struct sfs_inode *ip,
	  int nentry,
	  struct sfs_dir *dirp)
{
  int	size;

  if ((nentry <= 0) || (dirp == NULL))
    {
      return (ip->sfs_i_size / sizeof (struct sfs_dir));
    }
  size = (nentry * sizeof (struct sfs_dir) <= ip->sfs_i_size) ? 
          nentry * sizeof (struct sfs_dir) :
	  ip->sfs_i_size;

  read_file (fd, sb, ip, 0, size, (B *)dirp);	/* エラーチェックが必要! */
  return (0);
}

int
read_file (int fd,
	   struct sfs_superblock *sb,
	   struct sfs_inode *ip,
	   int start,
	   int size,
	   B *buf)
{
  B	*blockbuf;
  int	copysize;
  int	offset;
  int	retsize;
  char	*bufp;

  if (start + size > ip->sfs_i_size)
    {
      size = ip->sfs_i_size - start;
    }

  retsize = size;

  bufp = buf;

/*  fprintf (stderr, "read_file: offset = %d, size = %d\n", start, size); */
  blockbuf = (B *)alloca (sb->sfs_blocksize);
  while (size > 0)
    {
      read_block (fd, 
		  get_block_num (fd, sb, ip, start / sb->sfs_blocksize),
		  sb->sfs_blocksize,
		  blockbuf);
      offset = start % sb->sfs_blocksize;
      copysize = MIN (sb->sfs_blocksize - offset, size);
      bcopy (&blockbuf[offset], buf, copysize);

      buf += copysize;
      start += copysize;
      size -= copysize;
    }
  return (0);
}

int
read_block (int fd, int blockno, int blocksize, B *buf)
{
  if (lseek (fd, blockno * blocksize, 0) < 0)
    {
      return (0);
    }

  if (read (fd, buf, blocksize) < blocksize)
    {
      return (0);
    }

  return (blocksize);
}

int
get_block_num (int fd,
	       struct sfs_superblock *sb,
	       struct sfs_inode *ip,
	       int blockno)
{
  if (blockno < SFS_DIRECT_BLOCK_ENTRY)
    {
      /* 直接ブロックの範囲内
       */
      return (ip->sfs_i_direct[blockno]);
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
  		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)))
    {
      /* 一重間接ブロックの範囲内
       */
      return (get_indirect_block_num (fd, sb, ip, blockno));
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
  		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)
		      + (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK * SFS_INDIRECT_BLOCK)))
    {
      /* 二重間接ブロックの範囲内
       */
      return (get_dindirect_block_num (fd, sb, ip, blockno));
    }

  return (-1);
}


int
get_indirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno)
{
  int	inblock;
  int	inblock_offset;
  struct sfs_indirect	inbuf;

  inblock = (blockno - SFS_DIRECT_BLOCK_ENTRY);
  inblock_offset = inblock % SFS_INDIRECT_BLOCK;
  inblock = inblock / SFS_INDIRECT_BLOCK;
  if (ip->sfs_i_indirect[inblock] <= 0)
    {
      return (0);
    }

  read_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
#ifdef notdef
  fprintf (stderr, "get_ind: inblock = %d, offset = %d, blocknum = %ld\n",
	  inblock, inblock_offset, inbuf.sfs_in_block[inblock_offset]);
#endif
  return (inbuf.sfs_in_block[inblock_offset]);
}

int
get_dindirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno)
{
  int	dinblock;
  int	dinblock_offset;
  int	inblock;
  struct sfs_indirect	inbuf;

  blockno = blockno - (SFS_DIRECT_BLOCK_ENTRY + SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);

  inblock = blockno / (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);
  dinblock = (blockno % (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)) / SFS_INDIRECT_BLOCK;
  dinblock_offset = blockno % SFS_INDIRECT_BLOCK;

#ifdef notdef
  fprintf (stderr, "GET: blockno = %d, inblock = %d, dinblock = %d, dinblock_offset = %d\n",
	   blockno, inblock, dinblock, dinblock_offset);
#endif
  if (ip->sfs_i_dindirect[inblock] <= 0)
    {
      return (0);
    }

  read_block (fd, ip->sfs_i_dindirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
  if (inbuf.sfs_in_block[dinblock] <= 0)
    {
      return (0);
    }

  read_block (fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize, (B*)&inbuf);

#ifdef notdef
  fprintf (stderr, "get_ind: inblock = %d, dinblock = %d, offset = %d, blocknum = %d\n",
	  inblock, dinblock, dinblock_offset, inbuf.sfs_in_block[dinblock_offset]);
#endif
  return (inbuf.sfs_in_block[dinblock_offset]);
}
