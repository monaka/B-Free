/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* memory.cj
 *
 *	ʪ����������⥸�塼��
 *
 *	���Υե�����Ǥϥ����ڡ���ñ�̤Ǵ������Ƥ��롣
 *
 *
 */

#include "itron.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "pc98.h"
#include "misc.h"
#include "../../../boot/2nd/boot.h"
#include "../../../boot/2nd/config.h"

/* memory_map: 0x80000000 ������� 12 M bytes ʬ�Υ����ΰ���������
 *  �������ǽ�� 1M �ȼ��ճˤ��ɤ߹������ʬ�ˤĤ��Ƥϴ������ʤ���
 */
static B	memory_map[MEMORY_MAP_SIZE];

unsigned int	physmem_max;	/* ʪ������κ����� (base_mem + ext_mem) */
unsigned int	base_mem;	/* ���̤ˤ������Υ����� (PC9801 �ξ�� 640K) */
unsigned int	ext_mem;	/* ��ĥ����Υ����� */

#ifdef notdef
main ()
{
  W n;

  init_memory ();
  printf ("alloc mem = %d\n", alloc_memory (10));
  printf ("alloc mem = %d\n", alloc_memory (10));
  printf ("alloc mem = %d\n", n = alloc_memory (5));
  printf ("free mem  = %d\n", free_memory (n, 5));
  printf ("alloc mem = %d\n", alloc_memory (20));
}
#endif


static void
release_memory (W paddr, W length)
{
  W	i;
  W	page;

  dis_int ();
  page = ROUNDUP (length, PAGE_SIZE) / PAGE_SIZE;
  for (i = 0; i < page; i++)
    {
      memory_map[ROUNDUP (paddr, PAGE_SIZE) / PAGE_SIZE + i] = MEM_USE;
    }
  ena_int ();
}

/*
 *
 */
void
pmem_init (void)
{
  W	i;
  struct boot_header	*boot_info;

  dis_int ();
  for (i = 0; i < MEMORY_MAP_SIZE; i++)
    {
      memory_map[i] = MEM_FREE;
    }

  /* �ǽ�� RESERVE_PAGES ʬ (1M) �ϡ��������ʤ� */
  for (i = 0; i < RESERVE_PAGES; i++)
    {
      memory_map[i] = MEM_USE;
    }

  /* ������ VRAM ���ΰ��������ʤ� */
  for (i = (VRAM_MEM_START / PAGE_SIZE); i <= (VRAM_MEM_END / PAGE_SIZE); i++)
    {
      memory_map[i] = MEM_USE;
    }


  /* boot ���ɤ߹����ITRON �� ���ճˤ�ʬ���ΰ��ե꡼�ꥹ�Ȥ��������� */
  boot_info = (struct boot_header *)MODULE_TABLE;
  for (i = 0; i < boot_info->count; i++)
    {
#ifdef DEBUG
      printf ("pmem_init(): release memory %d(0x%x) (%d length)\n",
	      boot_info->modules[i].paddr, 
	      boot_info->modules[i].paddr,
	      boot_info->modules[i].mem_length);
#endif /* DEBUG */
      release_memory (boot_info->modules[i].paddr, boot_info->modules[i].mem_length);
    }
  ena_int ();

  physmem_max = boot_info->machine.real_mem;
  base_mem = boot_info->machine.base_mem;
  ext_mem = boot_info->machine.ext_mem;
  printf ("physmem = %d, base_mem = %d, ext_mem = %d\n", physmem_max, base_mem, ext_mem);
}

/* palloc --- ʪ������򥢥����Ȥ��롣
 *
 * �����ϥ������Ȥ��륵��������ꤹ�롣ñ�̤ϡ��ڡ������Ǥ��롣
 */
VP
palloc (W size)
{
  W i, j, k;

  dis_int ();
  for (i = 0; i < MEMORY_MAP_SIZE - size; i++)
    {
      if (memory_map[i] == MEM_FREE)
	{
	  for (j = i + 1; (j - i) < size; j++)
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
  if (i + size >= MEMORY_MAP_SIZE)
    {
      ena_int ();
      return (NULL);
    }

  for (k = 0; k < size; k++)
    {
      memory_map[i + k] = MEM_USE;
    }
  ena_int ();
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

  dis_int ();
  index = ((UW)p) / PAGE_SIZE;
  if ((index + size) >= MEMORY_MAP_SIZE)
    {
      ena_int ();
      return (E_PAR);
    }

  for (i = 0; i < size; i++)
    {
      memory_map[i + index] = MEM_FREE;
    }
  ena_int ();
  return (E_OK);
}
