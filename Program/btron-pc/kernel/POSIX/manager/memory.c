/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* memory.c - POSIX 環境マネージャのメモリ処理部分
 *
 *
 *
 * $Log: memory.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-07-23 14:38:58  naniwa
 * modified to implement exit
 *
 * Revision 1.3  1997/10/24 13:57:42  night
 * duplicate_memory() と destroy_proc_memory () を追加。
 *
 * Revision 1.2  1997/10/23 14:32:32  night
 * exec システムコール関係の処理の更新
 *
 * Revision 1.1  1996/11/05 15:13:46  night
 * 最初の登録
 *
 */

#include "posix.h"



/* init_filesystem
 *
 */
ER
init_memory (void)
{
  return (E_OK);
}



/* setup_vm_proc - プロセスの仮想メモリ情報の初期化 
 *
 *
 */
W
setup_vm_proc (struct proc *procp)
{
  W	errno;

  errno = create_vm_tree (procp, (VM_READ | VM_WRITE | VM_USER | VM_SHARE), LOW_USER_ADDR, USER_MEM_SIZE);
  if (errno != EP_OK)
    {
      return (errno);
    }

  return (EP_OK);
}



/* alloc_memory - プロセスに対して任意の仮想メモリを割り当てる。
 *
 * 引数：
 * 	procp	メモリを割り当てるプロセス(プロセス管理構造体へのポインタ)
 *	start	割り当てるメモリのアドレス (4KB alignment)
 *	size	メモリサイズ (4KB alignment)
 *	access	メモリのアクセスモード
 *
 */
W
alloc_memory (W procid, UW start, UW size, UW access)
{
  W		i;
  W		errno;
  struct proc	*procp;


  errno = proc_get_procp (procid, &procp);
  for (i = 0; i < size / PAGE_SIZE; i++)
    {
      errno = grow_vm (procp, start + (i * PAGE_SIZE), access);
      if (errno)
	{
	  return (errno);
	}
    }

  return (EP_OK);
}


/* free_memory - 仮想メモリを解放する
 *
 */
W
free_memory (W procid, UW start, UW size)
{
  W	i;
  W	errno;
  struct proc	*procp;


  errno = proc_get_procp (procid, &procp);
  for (i = 0; i < size / PAGE_SIZE; i++)
    {
      errno = shorten_vm (procp, start + (i * PAGE_SIZE));
      if (errno)
	{
	  return (errno);
	}
    }

  return (EP_OK);
}


/* duplicate_memory - 仮想空間のデータをすべてコピーする
 *
 *
 */
W
duplicate_memory (struct proc *source,
		  struct proc *destination)
{
  W	errno;

  errno = duplicate_tree (source, destination);
  return (errno);
}



/* destroy_proc_memory - プロセスのもつすべてのメモリを解放する
 *
 */
W
destroy_proc_memory (struct proc *procp, W unmap)
{
  W	errno;

  errno = destroy_vmtree (procp, procp->vm_tree, unmap);
  if (errno)
    {
      return (errno);
    }

  procp->vm_tree = NULL;
  return (EP_OK);
}



W
vm_page_fault_handler ()
{
  return (EP_NOSUP);
}
