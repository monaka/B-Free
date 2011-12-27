/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/vmtree.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


/* vmtree.c - 仮想メモリツリーの管理
 *
 * $Log: vmtree.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.16  2000-01-26 08:24:36  naniwa
 * to prevent memory leak
 *
 * Revision 1.15  2000/01/15 15:29:31  naniwa
 * minor fix
 *
 * Revision 1.14  2000/01/08 09:10:27  naniwa
 * fixed initialization of variables, etc
 *
 * Revision 1.13  1999/12/26 11:17:40  naniwa
 * add debug write
 *
 * Revision 1.12  1999/11/10 10:48:35  naniwa
 * to implement execve
 *
 * Revision 1.11  1999/07/26 08:56:26  naniwa
 * minor fix
 *
 * Revision 1.10  1999/07/23 14:39:04  naniwa
 * modified to implement exit
 *
 * Revision 1.9  1999/03/24 03:54:47  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.8  1999/03/02 15:30:58  night
 * 田畑@KMC (yusuke@kmc.kyoto-u.ac.jp) のアドバイスによる修正。
 * --------------------
 * それから別のバグですが、
 * POSIX/manager/vmtree.cの grow_vm() の最初で pageentを
 * 求めるときに
 * pageent = addr % (MAX_PAGE_ENTRY * PAGE_SIZE) ;
 * となっていますが、ここは
 * pageent = (addr / PAGE_SIZE ) % MAX_PAGE_ENTRY ;
 * とか書くべきだと思います。
 * --------------------
 *
 * Revision 1.7  1998/07/01 14:03:10  night
 * デバッグ用のデータ出力処理を #ifdef notdef 〜 #endif で囲んだ。
 *
 * Revision 1.6  1998/02/25 12:53:07  night
 * システムコール vmap_reg の引数追加(アクセス権限を指定する引数を追加)に
 * 伴った修正。
 * アクセス権としては、ACC_USER (ユーザが読み書きできる) を指定した。
 *
 * Revision 1.5  1998/02/24 14:21:42  night
 * プロセスの間の仮想ページのデータのコピーする処理を
 * 正しく動くように修正。
 *
 * Revision 1.4  1998/02/23 14:50:51  night
 * setup_vmtree 関数をきちんと動くようにした。
 *
 * Revision 1.3  1998/02/16 14:28:31  night
 * setup_vmtree() の追加。
 * この関数は、引数で指定した ITRON タスクから仮想メモリテーブル
 * vmtree を生成する。
 *
 * Revision 1.2  1997/10/24 13:56:16  night
 * ファイル中の文字コードを SJIS から EUC に変更。
 *
 * Revision 1.1  1997/10/23 14:32:33  night
 * exec システムコール関係の処理の更新
 *
 *
 */


#include "posix.h"



/* create_vm_tree - 新しくプロセスの仮想メモリツリーを作成する
 *
 */
W
create_vm_tree (struct proc *proc, UW access, UW start, UW size)
{
  struct vm_tree	*treep;
  W			i;
  ER			errno;


  treep = (struct vm_tree *)malloc (sizeof (struct vm_tree));
  if (treep == NULL)
    {
      return (EP_NOMEM);
    }

  treep->access = access;
  treep->start = start;
  treep->size = size;
  treep->vm_handler = (FP) NULL;

  for (i = 0; i < MAX_DIR_ENTRY; i++)
    {
      treep->directory_table[i] = NULL;
    }

  proc->vm_tree = treep;

#ifdef VMDEBUG
  printk ("create_vm_tree: call vcre_reg (procid = %d, start = %x, size = %x, access = %x\n",
	  proc->proc_maintask, (UW)start, (UW)size, size, access);		/* */
#endif
  errno = vcre_reg (proc->proc_maintask, (void *)start, size, size, access, NULL);
  if (errno)
    {
#ifdef VMDEBUG
      printk ("create_vm_tree: errno = %d, file= %s line = %d\n", errno, __FILE__, __LINE__);	/* */
#endif
      return (EP_INVAL);
    }

  return (EP_OK);
}



/* grow_vm - 新しく仮想ページに物理メモリを割り当てる
 *
 */
W
grow_vm (struct proc *procp, UW addr, UW access)
{
  UW			dirent;
  UW			pageent;
  struct vm_directory	*vmdir;
  struct vm_page	*vmpage;
  ER			errno;
  struct vm_tree	*treep;


  treep = procp->vm_tree;
  dirent = addr / (MAX_PAGE_ENTRY * PAGE_SIZE);		/* directory table のインデックス */
  pageent = (addr / PAGE_SIZE ) % MAX_PAGE_ENTRY;	/* page table のインデックス */

  vmdir = treep->directory_table[dirent];
  if (vmdir == NULL)
    {
      vmdir = (struct vm_directory *)alloc_vm_directory (treep, dirent * MAX_PAGE_ENTRY * PAGE_SIZE);
      if (vmdir == NULL)
	{
	  return (EP_NOMEM);
	}
      treep->directory_table[dirent] = vmdir;
    }

  vmpage = vmdir->page_table[pageent];
  if (vmpage == NULL)
    {
      vmpage = (struct vm_page *)alloc_vm_page (treep, vmdir, dirent * MAX_PAGE_ENTRY * PAGE_SIZE, access);
      if (vmpage == NULL)
	{
	  return (EP_NOMEM);
	}
      vmdir->page_table[pageent] = vmpage;
    }

  if (VM_ALLOCED_MASK (vmpage->access))
    {
      /* すでに物理メモリが割り当て済み */
      return (EP_INVAL);
    }

  /* 仮想メモリ領域に物理メモリを割り付ける
   */
  errno = vmap_reg (procp->proc_maintask, (VP)addr, PAGE_SIZE, ACC_USER);
  if (errno)
    {
      return (EP_PERM);
    }

  vmpage->access = VM_ALLOCED;
  return (EP_OK);
}


/* shorten_vm - 仮想ページを解放する
 *
 */
W
shorten_vm (struct proc *procp, UW addr)
{
  UW			dirent;
  UW			pageent;
  struct vm_directory	*vmdir;
  struct vm_page	*vmpage;
  ER			errno;
  struct vm_tree	*treep;

  treep = procp->vm_tree;
  dirent = addr / (MAX_PAGE_ENTRY * PAGE_SIZE);
  pageent = addr % (MAX_PAGE_ENTRY * PAGE_SIZE);

  vmdir = treep->directory_table[dirent];
  if (vmdir == NULL)
    {
      return (EP_INVAL);
    }
  vmpage = vmdir->page_table[pageent];
  if (vmpage == NULL)
    {
      return (EP_INVAL);
    }

  if (!VM_ALLOCED_MASK (vmpage->access))
    {
      return (EP_INVAL);
    }

  errno = vunm_reg (procp->proc_maintask, (VP)(vmpage->addr), PAGE_SIZE);
  if (errno)
    {
      return (EP_INVAL);
    }

  vmpage->access = (vmpage->access & ~VM_ALLOCED);
  return (EP_OK);
}



/* duplicate_tree - vm_tree の情報をコピーする
 *
 *
 */
W
duplicate_tree (struct proc *source_proc, struct proc *dest_proc)
{
  W			dir_index;
  W			page_index;
  struct vm_directory	*dirp, *dest_dirp;
  struct vm_page	*pagep, *dest_pagep;
  ER			errno;
  struct vm_tree	*source;
  struct vm_tree	*destination;
  static B		page_buf[PAGE_SIZE];
  W			addr;


  source = source_proc->vm_tree;
  destination = dest_proc->vm_tree;

#ifdef VMDEBUG
  printk ("duplicate_tree: %s, %d, source process ID = %d, parent = 0x%x\n",
	  __FILE__, __LINE__, source_proc->proc_pid, source_proc);	/* */
  printk ("source_proc->vmtree = 0x%x\n", source);			/* */
#endif


  /* プロセスのもつページディレクトリ情報をひとつひとつ調べていく。
   * もし、ディレクトリのエントリのうち使っているものがあれば、
   * その内容をコピーし、そのディレクトリ中のページ(複数)の内容も
   * チェックする。
   */
  for (dir_index = 0; dir_index < (MAX_DIR_ENTRY / 2); dir_index++)
    {
      dirp = source->directory_table[dir_index];
#ifdef VMDEBUG
      if ((dir_index == 0) && (dirp == 0))
	{
	  printk ("%s: %d: ?? directory 0 entry is clear.\n", __FILE__, __LINE__);	/* */
	}
#endif

      if (dirp != NULL)
	{
	  /* コピー元のページディレクトリ内に情報があった。
	   */

#ifdef notdef
	  printk ("duplicate_tree: direntry is %d\n", dir_index);	/* */
#endif

	  dest_dirp = destination->directory_table[dir_index];
	  if (dest_dirp == NULL)
	    {
	      dest_dirp = (struct vm_directory *)alloc_vm_directory (destination, dir_index * MAX_PAGE_ENTRY * PAGE_SIZE);
	      if (dest_dirp == NULL)
		{
#ifdef VMDEBUG
		  printk("duplicate_tree: empty dest_dirp %d\n", dir_index);
#endif
		  return (EP_NOMEM);
		}
	      destination->directory_table[dir_index] = dest_dirp;
#ifdef notdef
	      /* source の情報をコピーする */	/* XXX BUG XXX */
	      *dest_dirp = *dirp;
#endif
	    }
	  else
	    {
	      /* すでに受けがわページディレクトリが使用されている
	       */
#ifdef VMDEBUG
	      printk("duplicate_tree: non null dest_dirp\n");
#endif
	      return (EP_INVAL);
	    }


	  /* ディレクトリ中の各ページチェックし、もしコピー元のページが実メモリに
	   * マッピングされていた場合には、内容をコピー先にコピーする
	   */
	  for (page_index = 0; page_index < MAX_PAGE_ENTRY; page_index++)
	    {
	      pagep = dirp->page_table[page_index];

#ifdef VMDEBUG
	      printk ("duplicate_tree: page is %d, pagep = 0x%x\n", page_index, pagep);	/* */
#endif

	      if (pagep)
		{
		  dest_pagep = dest_dirp->page_table[page_index];
		  if (dest_pagep == NULL)
		    {
		      /* 新しいページ情報をアロケート */
		      dest_pagep = (struct vm_page *)alloc_vm_page (destination, 
								    dest_dirp,
								    0, 
								    pagep->access);
		      if (dest_pagep == NULL)
			{
#ifdef VMDEBUG
			  printk("duplicate_tree: empty pagep\n");
#endif
			  return (EP_NOMEM);
			}

#ifdef VMDEBUG
		      printk ("allocate new page information: 0x%x\n", dest_pagep);	/* */
#endif

		      dest_dirp->page_table[page_index] = dest_pagep;

#ifdef VMDEBUG
		      /* プロセスの仮想アドレスに実ページをマップする
		       */
		      printk ("duplicate_tree: vmap_reg (0x%x, %d)\n",
			      (VP)(dir_index * MAX_PAGE_ENTRY * PAGE_SIZE) + (page_index * PAGE_SIZE), PAGE_SIZE);	/* */
#endif

		      dest_pagep->addr = (dir_index * MAX_PAGE_ENTRY * PAGE_SIZE) + (page_index * PAGE_SIZE);
		      errno = vmap_reg (dest_proc->proc_maintask,
					(VP) dest_pagep->addr,
					PAGE_SIZE, 
					ACC_USER);
		      if (errno)
			{
#ifdef VMDEBUG
			  printk ("cannot vmap_reg: errno = %d\n", errno);
#endif
			  return (errno);
			}

		      errno = vget_phs (dest_proc->proc_maintask, 
					(VP) dest_pagep->addr,
					&addr);
		      if (errno)
			{
#ifdef VMDEBUG
			  printk ("cannot vget_phs: errno = %d\n", errno);
#endif
			  return (errno);
			}
		    }
		  else
		    {
		      /* すでに受けがわページが使用されている
		       */
#ifdef VMDEBUG
		      printk("duplicate_tree: non empty page\n");
#endif
		      return (EP_INVAL);
		    }

		  /* 送り側プロセスのメモリ中の情報を取り出す
		   */
		  errno = vget_reg (source_proc->proc_maintask,
				    (VP)dest_pagep->addr,
				    PAGE_SIZE,
				    (VP)page_buf);
		  if (errno)
		    {  
#ifdef VMDEBUG
		      printk ("vget_reg: errno = %d\n", errno);
		      printk ("          task = %d, addr = %x, buf = %x\n",
			      source_proc->proc_maintask,
			      dest_pagep->addr,
			      (VP)page_buf);
#endif
		      return (errno);
		    }

#ifdef notdef
		  if (((dir_index * MAX_PAGE_ENTRY * PAGE_SIZE) + (page_index * PAGE_SIZE)) < 0x3000)
		    {
		      W	i, j;
		      
		      printk ("vget_reg (pid = %d, addr = 0x%x, size = 0x%x, buf = 0x%x)\n",
			      source_proc->proc_maintask,
			      (VP)((dir_index * MAX_PAGE_ENTRY * PAGE_SIZE) + (page_index * PAGE_SIZE)),
			      PAGE_SIZE,
			      (VP)page_buf);

		      for (i = 0; i < 64; )
			{
			  for (j = 0; j < 8; j++, i++)
			    {
			      if (page_buf[i] < 0xA)
				{
				  printk ("0x0%x ", page_buf[i]);
				}
			      else
				{
				  printk ("0x%x ", page_buf[i]);
				}
			    }
			  printk ("\n");
			}
		    }
#endif

		  /* 受け側プロセスのメモリに取り出した情報を送る
		   */
		  errno = vput_reg (dest_proc->proc_maintask,
				    (VP)dest_pagep->addr,
				    PAGE_SIZE, 
				    (VP)page_buf);
		  if (errno)
		    {  
#ifdef VMDEBUG
		      printk ("vput_reg: errno = %d\n", errno);
#endif
		      return (errno);
		    }
		}
	    } /* ディレクトリの各ページのチェックのループの最後 */
	}
    } /* プロセスの各ページディレクトリのチェックのループの最後 */


  return (EP_OK);
}




struct vm_page *
alloc_vm_page (struct vm_tree *treep, struct vm_directory *dirp, UW addr, UW access)
{
  struct vm_page	*pagep;


  pagep = (struct vm_page *)malloc (sizeof (struct vm_page));
  if (pagep == NULL)
    {
      return (NULL);
    }

  pagep->access = access;
  pagep->addr = addr;
  pagep->directory = dirp;
  pagep->swap_file = NULL;
  pagep->swap_page = -1;
  return (pagep);
}


struct vm_directory *
alloc_vm_directory (struct vm_tree *treep, UW addr)
{
  struct vm_directory	*dirp;
  W			i;


  dirp = (struct vm_directory *)malloc (sizeof (struct vm_directory));
  if (dirp == NULL)
    {
      return (NULL);
    }

  bzero((VP) dirp, sizeof(struct vm_directory));
  dirp->access = treep->access;
  dirp->tree_top = treep;
  dirp->start = addr;
  dirp->size = MAX_PAGE_ENTRY * PAGE_SIZE;
  for (i = 0; i < MAX_PAGE_ENTRY; i++)
    {
      dirp->page_table[i] = NULL;
    }
  return (dirp);
}


/* destroy_vmtree - vm tree の情報を解放する
 *
 *
 */
W
destroy_vmtree (struct proc *procp, struct vm_tree *treep, W unmap)
{
  W			dir_index;
  W			page_index;
  struct vm_directory	*dirp;
  struct vm_page	*pagep;
  ER			errno;

  for (dir_index = 0; dir_index < MAX_DIR_ENTRY; dir_index++)
    {
      dirp = treep->directory_table[dir_index];
      if (dirp)
	{
	  for (page_index = 0; page_index < MAX_PAGE_ENTRY; page_index++)
	    {
	      pagep = dirp->page_table[page_index];
	      if (pagep)
		{
		  /* ページの情報をパージする */
		  if (pagep->swap_file)
		    {
		      /* swap 情報を解放 */
		    }
		  if (unmap) {
		    errno = vunm_reg (procp->proc_maintask,
				      (VP)(pagep->addr), PAGE_SIZE);
		    if (errno)
		      {
			return (errno);
		      }
		  }
		  /* ページを開放 */
		  free(pagep);
		}
	    }
	  /* ディレクトリの開放 */
	  free(dirp);
	}
    }

  /* リージョン情報を解放する */
  if (unmap) {
    errno = vdel_reg (procp->proc_maintask, LOW_USER_ADDR);
    if (errno)
      {
	return (errno);
      }
  }
  /*  vmtree の root の開放 */
  free(treep);

  return (EP_OK);
}


/* setup_vmtree - すでに動いている ITRON タスクの vmtree を作成する
 *
 */
W
setup_vmtree (struct proc *procp, ID taskid, UW access, FP handler, UW start, UW size)
{
  struct vm_tree	*treep;
  W			dir_index;
  W			page_index;
  struct vm_directory	*dirp;
  struct vm_page	*pagep;
  UW			paddr;
  ER			error;
  UW			addr = 0;


  printk ("posix: setup_vmtree\n");
  treep = procp->vm_tree;
  if (treep == NULL)
    {
      treep = malloc (sizeof (struct vm_tree));
      if (treep == NULL)	
	{
	  return (EP_NOMEM);
	}
      bzero((VP) treep, sizeof(struct vm_tree));
      treep->access = access;
      treep->vm_handler = handler;
      procp->vm_tree = treep;
    }

  for (dir_index = 0; dir_index < (MAX_DIR_ENTRY / 2); dir_index++)
    {
      dirp = treep->directory_table[dir_index];
      for (page_index = 0; page_index < MAX_PAGE_ENTRY; page_index++)
	{
	  /* 物理メモリアドレスを取得 */
	  error = vget_phs (taskid, (VP)addr, &paddr);
	  if (error == E_OK)
	    {
	      /* マップされている */

	      if (dirp == NULL)
		{
		  /* ディレクトリエントリを確保していないので、メモリをアロケートする */
		  dirp = malloc (sizeof (struct vm_directory));
		  if (dirp == NULL)
		    {
		      return (EP_NOMEM);
		    }
		  bzero ((VP)dirp, sizeof (struct vm_directory));
		  dirp->access = access;
		  dirp->tree_top = treep;
		  dirp->start = dir_index * (PAGE_SIZE * MAX_PAGE_ENTRY);
		  dirp->size = (PAGE_SIZE * MAX_PAGE_ENTRY);
		  treep->directory_table[dir_index] = dirp;
		}

	      pagep = dirp->page_table[page_index];
	      if (pagep == NULL)
		{

		  /* ページエントリを確保していないので、メモリをアロケートする */
		  pagep = malloc (sizeof (struct vm_page));
		  if (pagep == NULL)
		    {
		      /* メモリ不足 */
		      return (EP_NOMEM);
		    }
		  bzero ((VP)pagep, sizeof (struct vm_page));
		  dirp->page_table[page_index] = pagep;
		}
	      pagep->addr = paddr;
#ifdef VMDEBUG
	      printk ("setup_vmtree: map (vaddr = 0x%x, paddr = 0x%x)\n",
		      addr, paddr);
#endif
	    }
		 
	  addr += PAGE_SIZE;
	}
    }

  return (EP_OK);
}
