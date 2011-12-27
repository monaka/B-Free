/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT page routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/page.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: page.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:50:42  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/06/26  15:07:26  night
 * vir_to_phys() 関数の追加。
 * この関数は、仮想アドレスから物理アドレスを仮想ページテーブルから求める。
 *
 * Revision 1.2  1994/07/05  17:19:42  night
 * Change FD format; 512bytes/block.
 *
 * Revision 1.1  1993/10/11  21:29:44  btron
 * btron/386
 *
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/page.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "config.h"
#include "location.h"
#include "errno.h"
#include "page.h"


struct page_directory_entry	*page_dir;
struct page_table_entry		*page_entry;

static void	init_dir_ent (struct page_directory_entry *dir,
			      unsigned int page_addr,
			      unsigned int entry);

static void	init_page_ent (struct page_table_entry *page,
			       unsigned int paddr,
			       unsigned int entry,
			       unsigned int vaddr,
			       unsigned int mode);


/**************************************************************************
 * vm_init
 *
 */
void
init_vm (void)
{
  int	i;
/*
  struct page_directory_entry	tmp1 = { 0, 1, 0, 0, 0, 0, 0, 0, 0 };
  struct page_table_entry	tmp2 = { 0, 1, 0, 0, 0, 0, 0, 0, 0 };
*/
  
  page_dir = (struct page_directory_entry *)PAGE_DIR_ADDR;
  page_entry = (struct page_table_entry *)PAGE_ENTRY_ADDR;
  bzero (page_dir, MAX_DIRENT * sizeof (struct page_directory_entry));
  bzero (page_entry, MAX_PAGEENT * sizeof (struct page_table_entry));
  
/*
  for (i = 0; i < MAX_DIRENT; i++)
    {
      page_dir[i] = tmp1;
    }
  for (i = 0; i < MAX_PAGEENT; i++)
    {
      page_entry[i] = tmp2;
    }
*/
  init_dir_ent (page_dir, PAGE_ENTRY_ADDR, 1);
  init_dir_ent (&page_dir[512], PAGE_ENTRY_ADDR + PAGE_SIZE * 2, 3);
  init_page_ent (page_entry, 0x0, 1024 * 2, 0, 0);
  init_page_ent (&page_entry[2048], 0x0, 1024 * 2, 0x80000000, 0);
/*
  init_page_ent (&page_entry[2048], 0x0, 1024 * 2, 0x80000000, 0);
  init_page_ent (&page_entry[2048], 0x0, 20, 0x80000000, 1);
*/
}

/***************************************************************************
 * get_page_entry ---
 */
struct page_table_entry *
get_page_entry (unsigned long addr)
{
  int dir_entry;
  int page_entry;
  struct page_table_entry *pent;

  dir_entry = (addr >> 22) & 0x3ff;
  page_entry = (addr >> 12) & 0x3ff;
  pent = (struct page_table_entry *)(page_dir[dir_entry].frame_addr << 12);
  return ((struct page_table_entry *)&pent[page_entry]);
}

/***************************************************************************
 *
 */
static void
init_dir_ent (struct page_directory_entry *dir,
	      unsigned int page_addr,
	      unsigned int entry)
{
  int	i;

  for (i = 0; i < entry; i++)
    {
      dir[i].frame_addr = (page_addr + (i * 4096)) / 4096;
      dir[i].user = 0;
      dir[i].zero1 = 0;
      dir[i].dirty = 0;
      dir[i].access = 0;
      dir[i].zero2 = 0;
      dir[i].u_and_s = 0;
      dir[i].read_write = 1;
      dir[i].present = 1;
    }
}

/***************************************************************************
 *
 */
static void
init_page_ent (struct page_table_entry *page,
	       unsigned int paddr,
	       unsigned int entry,
	       unsigned int vaddr,
	       unsigned int mode)
{
  int	i;

  for (i = 0; i < entry; i++)
    {
      page[i].frame_addr = (paddr + (i * 4096)) / 4096;
      page[i].user = 0;
      page[i].zero1 = 0;
      page[i].dirty = 0;
      page[i].access = 0;
      page[i].zero2 = 0;
      page[i].u_and_s = mode;
      page[i].read_write = 1;
      page[i].present = 1;
    }
}

int
vir_to_phys (void *virtual)
{
  unsigned long	v;
  unsigned long	dirent;
  unsigned long pageent;
  unsigned long	offset;
  struct page_directory_entry	*cr3;
  struct page_table_entry	*pageptr;

  v = (unsigned long) virtual;
  dirent = (v & 0xffc0) >> 22;
  pageent = (v >> 12) & 0x3ff;
  offset = (v & 0xfff);
  cr3 = (struct page_directory_entry *)get_cr3 ();

  printf ("vir_to_phys: %d:%d:%d\n", dirent, pageent, offset);

  if (cr3[dirent].present == 0)
    {
      printf ("vir_to_phys: 0x%x, address not mapped.\n", virtual);
      return 0;
    }

  pageptr = (struct page_table_entry *)((cr3[dirent].frame_addr) << 12);
  if (pageptr[pageent].present == 0)
    {
      printf ("vir_to_phys: 0x%x, address not mapped.\n", virtual);
      return 0;
    }

  printf ("vir_to_phys: virtual = 0x%x, physical = 0x%x\n", virtual, (pageptr[pageent].frame_addr << 12) + offset);
  return (pageptr[pageent].frame_addr << 12);
}
