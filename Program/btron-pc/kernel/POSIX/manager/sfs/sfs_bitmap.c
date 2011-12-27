/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_bitmap.c - SFS の bitmap の cache を行う
 *
 * $Log: sfs_bitmap.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-07-02 04:14:21  naniwa
 * to implement disk block cache
 *
 * Revision 1.1  1999/07/09 08:11:03  naniwa
 * first version
 *
 */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/sfs/sfs_bitmap.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../posix.h"
#include "sfs_func.h"

#define BLOCK_SIZE      512
#define CACHE_SIZE	10

/* 変数 */
static struct bitmap_cache {
  int dirty;
  W fd;
  W blockno;
  B buf[BLOCK_SIZE];
} cache_data[CACHE_SIZE];

static int start, count;

void sfs_init_bitmap_cache(void)
{
  int i;

  for(i = 0; i < CACHE_SIZE; ++i) {
    bzero((B*)&cache_data[i], sizeof(struct bitmap_cache));
  }
  start = 0;
  count = 0;
}

W sfs_alloc_bitmap(W fd, W blockno, B **ptr, int **dirty)
{
  int i, j;
  W errno;
  W rsize;
  W startoffset;

  for(i = 0, j = start ; i < count; ++i, j = (j+1)%CACHE_SIZE) {
    if (cache_data[j].fd == fd && cache_data[j].blockno == blockno) break;
  }
  if (i < count) {
    *ptr = cache_data[j].buf;
    *dirty = &cache_data[j].dirty;
  }
  else {
    if (count < CACHE_SIZE) {
      i = count;
      ++count;
    }
    else {
      i = start;
      start = (start+1)%CACHE_SIZE;
    }
    if (cache_data[i].dirty == 1) {
      /* disk に書き戻し */
      startoffset = (1 + 1 + cache_data[i].blockno) * BLOCK_SIZE;
      errno = sfs_write_device (fd, cache_data[i].buf, startoffset,
			       BLOCK_SIZE, &rsize);
      if (errno) {
	return(errno);
      }
      cache_data[i].dirty = 0;
    }
    cache_data[i].fd = fd;
    cache_data[i].blockno = blockno;
    /* disk から読み込み */
    startoffset = (1 + 1 + blockno) * BLOCK_SIZE;
    errno = sfs_read_device (fd, cache_data[i].buf, startoffset,
			     BLOCK_SIZE, &rsize);
    if (errno) {
      return(errno);
    }
    *ptr = cache_data[i].buf;
    *dirty = &cache_data[i].dirty;
  }
  return(EP_OK);
}

W sfs_sync_bitmap(W fd)
{
  int i;
  W errno;
  W rsize;
  W startoffset;

  for(i = 0; i < count; ++i) {
    if (cache_data[i].fd == fd && cache_data[i].dirty == 1) {
      /* disk に書き出し */
      startoffset = (1 + 1 + cache_data[i].blockno) * BLOCK_SIZE;
      errno = sfs_write_device (fd, cache_data[i].buf, startoffset,
			       BLOCK_SIZE, &rsize);
      if (errno) {
	return(errno);
      }
      cache_data[i].dirty = 0;
    }
  }
  return(EP_OK);
}
