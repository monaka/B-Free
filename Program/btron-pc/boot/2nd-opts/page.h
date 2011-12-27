/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *
 */

#ifndef __PAGE_H__
#define __PAGE_H__	1

#define PAGE_SIZE	4096

#define PAGE_PRESENT	1
#define PAGE_NONPRESENT	0

#define DIR_SIZE	(4096 * 4096)

struct page_directory_entry
{
  unsigned int	present:1;
  unsigned int	read_write:1;
  unsigned int	u_and_s:1;
  unsigned int	zero2:2;
  unsigned int	access:1;
  unsigned int	dirty:1;
  unsigned int	zero1:2;
  unsigned int	user:3;
  unsigned int	frame_addr:20;
};

struct page_table_entry
{
  unsigned int	present:1;
  unsigned int	read_write:1;
  unsigned int	u_and_s:1;
  unsigned int	zero2:2;
  unsigned int	access:1;
  unsigned int	dirty:1;
  unsigned int	zero1:2;
  unsigned int	user:3;
  unsigned int	frame_addr:20;
};


void	init_vm (void);
int	map_vm (ULONG raddr, ULONG vaddr, ULONG size);
struct page_table_entry *get_page_entry (unsigned long addr);


#endif __PAGE_H__
