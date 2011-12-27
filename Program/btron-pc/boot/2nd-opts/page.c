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
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/page.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: page.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.3  1999/03/15 01:35:31  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.2  1998/11/20 08:02:38  monaka
 * *** empty log message ***
 *
 * Revision 1.1  1996/05/11 10:45:06  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * Revision 1.4  1995/09/21  15:50:42  night
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

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/page.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "types.h"
#include "config.h"
#include "location.h"
#include "errno.h"
#include "page.h"


struct page_directory_entry	*page_dir;
struct page_table_entry		*page_entry;

static void	init_dir_ent (struct page_directory_entry *dir,
			      ULONG page_addr,
			      ULONG entry);

static void	init_page_ent (struct page_table_entry *page,
			       ULONG paddr,
			       ULONG entry,
			       ULONG vaddr,
			       ULONG mode);


/**************************************************************************
 * vm_init
 *
 */
void
init_vm (void)
{
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
get_page_entry (ULONG addr)
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
	      ULONG page_addr,
	      ULONG entry)
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
  ULONG paddr,
  ULONG entry,
  ULONG vaddr,
  ULONG mode)
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
  ULONG v;
  ULONG dirent;
  ULONG pageent;
  ULONG offset;
  struct page_directory_entry	*cr3;
  struct page_table_entry	*pageptr;

  v = (ULONG) virtual;
  dirent = (v & 0xffc0) >> 22;
  pageent = (v >> 12) & 0x3ff;
  offset = (v & 0xfff);
  cr3 = (struct page_directory_entry *)get_cr3 ();

  boot_printf ("vir_to_phys: %d:%d:%d\n", dirent, pageent, offset);

  if (cr3[dirent].present == 0)
    {
      boot_printf ("vir_to_phys: 0x%x, address not mapped.\n", virtual);
      return E_OK;
    }

  pageptr = (struct page_table_entry *)((cr3[dirent].frame_addr) << 12);
  if (pageptr[pageent].present == 0)
    {
      boot_printf ("vir_to_phys: 0x%x, address not mapped.\n", virtual);
      return E_OK;
    }

  boot_printf ("vir_to_phys: virtual = 0x%x, physical = 0x%x\n", virtual, (pageptr[pageent].frame_addr << 12) + offset);
  return (pageptr[pageent].frame_addr << 12);
}
