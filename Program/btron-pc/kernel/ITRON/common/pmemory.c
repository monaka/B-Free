/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* memory.c
 *
 *	物理メモリ管理モジュール
 *
 *	このファイルではメモリをページ単位で管理している。
 *
 *
 */

#include "itron.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "ibmpc.h"
#include "misc.h"
#include "config_boot.h"

/* memory_map: 0x80000000 から最大 12 M bytes 分のメモリ領域を管理する
 *  ただし最初の 1M と周辺核を読み込んだ部分については管理しない。
 */
static B	memory_map[MEMORY_MAP_SIZE];

unsigned int	physmem_max;	/* 物理メモリの最大量 (base_mem + ext_mem) */
unsigned int	base_mem;	/* 下位にあるメモリのサイズ (PC9801 の場合 640K) */
unsigned int	ext_mem;	/* 拡張メモリのサイズ */
static UW       free_mem = 0;

#ifdef notdef
main ()
{
  W n;

  init_memory ();
  printk ("alloc mem = %d\n", alloc_memory (10));
  printk ("alloc mem = %d\n", alloc_memory (10));
  printk ("alloc mem = %d\n", n = alloc_memory (5));
  printk ("free mem  = %d\n", free_memory (n, 5));
  printk ("alloc mem = %d\n", alloc_memory (20));
}
#endif


static void
release_memory (W paddr, W length)
{
  W	i, j;
  W	page;

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  page = ROUNDUP (length, PAGE_SIZE) / PAGE_SIZE;
  for (i = 0; i < page; i++)
    {
      j = ROUNDUP (paddr, PAGE_SIZE) / PAGE_SIZE + i;
      if (j < MEMORY_MAP_SIZE) memory_map[j] = MEM_USE;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
}

/*
 *
 */
void
pmem_init (void)
{
  W	i;
  struct boot_header	*boot_info;

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  for (i = 0; i < MEMORY_MAP_SIZE; i++)
    {
      memory_map[i] = MEM_FREE;
    }

  /* 最初の RESERVE_PAGES 分 (1M) は、管理しない */
  for (i = 0; i < RESERVE_PAGES; i++)
    {
      memory_map[i] = MEM_USE;
    }

#ifdef notdef
  /* そして VRAM の領域も管理しない */
  for (i = (VRAM_MEM_START / PAGE_SIZE); i <= (VRAM_MEM_END / PAGE_SIZE); i++)
    {
      memory_map[i] = MEM_USE;
    }
#endif /* notdef */


  /* boot が読み込んだITRON と 周辺核の分の領域をフリーリストから解除する */
  boot_info = (struct boot_header *)MODULE_TABLE;
  if (boot_info->count == 0)
    {
      extern unsigned char	end;

/* #ifdef DEBUG */
      printk ("release_memory: 0x%x, 0x%x\n", 
	      0x00020000, 
	      ((unsigned int)(&end) - 0x80020000) & 0x7fffffff);
/* #endif */
      release_memory (0x0020000, ((unsigned int)(&end) - 0x80020000) & 0x7fffffff);
    }
  else
    {
      for (i = 0; i < boot_info->count; i++)
	{
#ifdef DEBUG
	  printk ("pmem_init(): release memory %d(0x%x) (%d length)\n",
		  boot_info->modules[i].paddr, 
		  boot_info->modules[i].paddr,
		  boot_info->modules[i].mem_length);
#endif /* DEBUG */
	  release_memory (boot_info->modules[i].paddr, boot_info->modules[i].mem_length);
	}
    }

  free_mem = 0;
  for(i = 0; i < MEMORY_MAP_SIZE; ++i) {
    if (memory_map[i] == MEM_FREE) ++free_mem;
  }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif

  physmem_max = boot_info->machine.real_mem;
  base_mem = boot_info->machine.base_mem;
  ext_mem = boot_info->machine.ext_mem;
  printk ("physmem = %d, base_mem = %d, ext_mem = %d\n", physmem_max, base_mem, ext_mem);
}

/* palloc --- 物理メモリをアロケートする。
 *
 * 引数はアロケートするサイズを指定する。単位は、ページ数である。
 */
VP
palloc (W size)
{
  W i, j, k;

  if (free_mem < size) return (NULL);
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  for (i = 0; i < (MEMORY_MAP_SIZE - size + 1); i++)
    {
      if (memory_map[i] == MEM_FREE)
	{
	  for (j = i + 1; ((j - i) < size) && (j < MEMORY_MAP_SIZE); j++)
	    {
	      if (memory_map[j] != MEM_FREE)
		{
		  break;
		}
	    }
	  if ((j - i) >= size)
	    break;
	}
    }
  if (i + size > MEMORY_MAP_SIZE)
    {
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      return (NULL);
    }

  for (k = 0; k < size; k++)
    {
      memory_map[i + k] = MEM_USE;
      --free_mem;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
/*  return ((VP)((UW)(i * PAGE_SIZE) + (UW)0x80100000)); */
  return ((VP)((UW)(i * PAGE_SIZE) | (UW)0x80000000));
}

/*
 *
 */
ER
pfree (VP p, W size)
{
  W	i;
  W	index;

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  index = ((UW)p) / PAGE_SIZE;
  if ((index + size) > MEMORY_MAP_SIZE)
    {
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      return (E_PAR);
    }

  for (i = 0; i < size; i++)
    {
      memory_map[i + index] = MEM_FREE;
      ++free_mem;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  return (E_OK);
}

void pmemstat(void)
{
#ifdef notdef
  W i, count = 0;
  
  for (i = 0; i < MEMORY_MAP_SIZE; i++) {
    if (memory_map[i] == MEM_FREE) ++count;
  }
#endif
  printk("SIZE %d FREE %d\n", MEMORY_MAP_SIZE, free_mem);
}

UW pmemfree(void)
{
  return(free_mem);
}
void pmemdump(VP *argp)
{
  struct a
    {
      UW ad;
    } *args = (struct a*)argp;

  int i;
  UW ev;
  char *p;
    p = (char *)args->ad;
    ev = args->ad;
    printk("pointer\n");
    if(ev==0){
        printk("args->ad 0x%x:\n",args->ad);
        printk("pmemdump error!\n"); 
        return; 
     //}else(ev==){
    }
    printk("0x%x:",p);
    for(i=0;i<16;i++){
      printk("%x ",p[i]);
    }
    printk("\n");
}

