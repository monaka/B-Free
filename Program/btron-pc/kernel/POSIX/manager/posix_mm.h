/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* posix_mm.h - POSIX メモリサブシステム
 *
 *
 * $Log: posix_mm.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  2000-01-08 09:09:16  naniwa
 * add some prototypes
 *
 * Revision 1.5  1999/07/23 14:39:00  naniwa
 * modified to implement exit
 *
 * Revision 1.4  1998/02/16 14:22:49  night
 * vm_tree 構造体の要素 vm_handler の型を変更。
 * W (*)() から FP に変更した。
 *
 * Revision 1.3  1997/10/24 13:59:11  night
 * VM_EXEC、VM_EXEC_MASK マクロの追加。
 * 関数定義の追加。
 *
 * Revision 1.2  1997/10/23 14:32:32  night
 * exec システムコール関係の処理の更新
 *
 * Revision 1.1  1996/11/13 12:53:35  night
 * 最初の登録
 *
 *
 */

#ifndef __POSIX_MM_H__
#define __POSIX_MM_H__	1


/* virtual memory permission. */

#define VM_READ		0x00000001
#define VM_WRITE	0x00000002
#define VM_EXEC		0x00000004
#define VM_USER		0x00010000
#define VM_SHARE	0x00000100

#define VM_ALLOCED	0x01000000


#define VM_READ_MASK(x)		((x) & VM_READ)
#define VM_WRITE_MASK(x)	((x) & VM_WRITE)
#define VM_EXEC_MASK(x)		((x) & VM_EXEC)
#define VM_USER_MASK(x)		((x) & VM_USER)
#define VM_SHARE_MASK(x)	((x) & VM_SHARE)
#define VM_ALLOCED_MASK(x)	((x) & VM_ALLOCED)


#define MAX_DIR_ENTRY		1024
#define MAX_PAGE_ENTRY		1024

#define LOW_USER_ADDR		0
#define USER_MEM_SIZE		0x40000000



/* 仮想メモリ - 実メモリの対を管理するツリー
 *
 * 仮想メモリのツリーは、次のような形となる:
 *
 *	vm_tree	 - vm_directory[0] - vm_page[0]
 *		   		     vm_page[1]
 *		   		     vm_page[2]
 *		   		     vm_page[3]
 *					.
 *					.
 *
 *		 + vm_directory[1] - vm_page[0]
 *					.
 *					.
 *					.
 *				
 */
struct vm_tree
{
  UW				access;			/* アクセス権の情報 */
  UW				start;
  UW				size;
  FP				vm_handler;	/* ページフォールト時のハンドラ */
  struct vm_directory		*directory_table[MAX_DIR_ENTRY];
};


struct vm_directory
{
  UW				access;			/* アクセス権の情報 */
  struct vm_tree		*tree_top;
  UW				start;
  UW				size;
  struct vm_page		*page_table[MAX_PAGE_ENTRY];
};


struct vm_page
{
  UW				access;			/* アクセス権の情報 */
  struct vm_directory		*directory;
  UW				addr;

  struct inode			*swap_file;
  W				swap_page;
};


#ifdef KERNEL

/* memory.c */
extern ER			init_memory (void);
extern W			alloc_memory (W procid, UW start, UW size, UW access);
extern W			setup_vm_proc (struct proc *procp);
extern W			duplicate_memory (struct proc *source, struct proc *destination);
extern W			free_memory (W procid, UW start, UW size);
extern W			destroy_proc_memory (struct proc *procp, W unmap);

/* vmtree.c */
extern W			create_vm_tree (struct proc *proc, UW access, UW start, UW size);
extern W			grow_vm (struct proc *procp, UW addr, UW access);
extern W			shorten_vm (struct proc *procp, UW addr);
extern W			duplicate_tree (struct proc *source_proc, struct proc *dest_proc);
extern struct vm_page		*alloc_vm_page (struct vm_tree *treep, struct vm_directory *dirp, UW addr, UW access);
extern struct vm_directory	*alloc_vm_directory (struct vm_tree *treep, UW addr);
extern W			destroy_vmtree (struct proc *procp, struct vm_tree *treep, W unmap);
extern W			setup_vmtree (struct proc *procp, ID taskid, UW access, FP handler, UW start, UW size);

#endif /* KERNEL */


#endif /* __POSIX_MM_H__ */


