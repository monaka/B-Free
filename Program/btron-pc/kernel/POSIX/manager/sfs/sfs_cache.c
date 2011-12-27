/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_cache.c - SFS の diskblock の cache を行う
 *
 * $Log: sfs_cache.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  2000-07-02 04:13:18  naniwa
 * first version
 *
 *
 */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/sfs/sfs_cache.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../posix.h"
#include "sfs_func.h"

#define CACHE_SIZE	300
#define HASH_SIZE	30

typedef struct {
  short int dirty;
  short int lru_next;
  short int hash_next, hash_prev;
  W fd;
  W blockno;
  B buf[SFS_BLOCK_SIZE];
} SFS_BLOCK_CACHE;

static SFS_BLOCK_CACHE *cache_data;
static short int cache_head, cache_tail;
static short int hash_table[HASH_SIZE+1];

void sfs_init_cache(void)
{
  int i;
  
  cache_data = (SFS_BLOCK_CACHE *) malloc(sizeof(SFS_BLOCK_CACHE)*CACHE_SIZE);

  for(i = 0; i < CACHE_SIZE; ++i) {
    bzero((B*)&(cache_data[i]), sizeof(SFS_BLOCK_CACHE));
    cache_data[i].lru_next = i+1;
    cache_data[i].hash_next = -1;
    cache_data[i].hash_prev = -1;
    cache_data[i].blockno = -1;
  }
  cache_head = 0;
  cache_tail = CACHE_SIZE - 1;
  cache_data[cache_tail].lru_next = -1;
  for(i = 0; i <= HASH_SIZE; ++i) {
    hash_table[i] = -1;
  }
  hash_table[0] = 0;
#ifdef notdef
  sfs_init_bitmap_cache();
#endif
}

void sfs_get_cache(W fd, W blockno, W *cn, B **ptr)
{
  int i, hn;
  SFS_BLOCK_CACHE *cp;

  if (blockno < 0) {
#ifdef notdef
    printk("[SFS] alloc cache %d %d\n", blockno, cache_head);
#endif
    *cn = -1;
    *ptr = NULL;
    return;
  }
  hn = (blockno%HASH_SIZE)+1;
  for(i = hash_table[hn];
      i >= 0;
      i = cache_data[i].hash_next) {
    if (cache_data[i].fd == fd && cache_data[i].blockno == blockno) break;
  }
  if (i < 0) {
#ifdef notdef
    printk("[SFS] alloc cache %d %d\n", blockno, cache_head);
#endif
    *cn = cache_head;
    cp = &cache_data[cache_head];
    if (cp->dirty) {
      sfs_write_block(fd, blockno, SFS_BLOCK_SIZE, cp->buf);
      cp->dirty = 0;
    }
    /* remove from hash chain */
    if (cp->hash_prev == -1) {
      hash_table[(cp->blockno % HASH_SIZE)+1] = cp->hash_next;
      if (cp->hash_next != -1)
	cache_data[cp->hash_next].hash_prev = -1;
    }
    else {
      cache_data[cp->hash_prev].hash_next = cp->hash_next;
      if (cp->hash_next != -1) 
	cache_data[cp->hash_next].hash_prev = cp->hash_prev;
    }
    bzero(cp->buf, SFS_BLOCK_SIZE);
    
    sfs_read_block(fd, blockno, SFS_BLOCK_SIZE, cp->buf);
    cp->fd = fd;
    cp->blockno = blockno;
    cp->hash_next = hash_table[hn];
    cp->hash_prev = -1;
    cache_data[hash_table[hn]].hash_prev = cache_head;
    hash_table[hn] = cache_head;
    *ptr = cp->buf;
    cache_head = cp->lru_next;
    cp->lru_next = -1;
  }
  else {
#ifdef notdef
    printk("[SFS] find cache %d %d\n", blockno, i);
#endif
    *cn = i;
    *ptr = cache_data[i].buf;
  }
}

void sfs_put_cache(W cn, W dirty)
{
  int hn;
  SFS_BLOCK_CACHE *cp;

  if (cn < 0) return;
  cp = &cache_data[cn];
  hn = (cp->blockno % HASH_SIZE)+1;
  cp->dirty = dirty;
  
  /* add to lru chain */
  cache_data[cache_tail].lru_next = cn;
  cache_tail = cn;
}

W sfs_sync_cache(W fd)
{
  int i;
  SFS_BLOCK_CACHE *cp;
  W errno;

  for(i = 0; i < CACHE_SIZE; ++i) {
    cp = &cache_data[i];
    if (cp->dirty && cp->fd == fd) {
      errno = sfs_write_block(fd, cp->blockno, SFS_BLOCK_SIZE, cp->buf);
      if (errno < 0) return (errno);
      cp->dirty = 0;
    }
  }
  return(EP_OK);
}
