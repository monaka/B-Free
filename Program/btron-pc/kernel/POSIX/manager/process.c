/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* process.c - POSIX 環境マネージャのプロセス管理処理部分
 *
 *
 *
 * $Log: process.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.30  2000-06-23 09:18:13  naniwa
 * to support O_APPEND
 *
 * Revision 1.29  2000/05/06 03:52:27  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.28  2000/02/28 09:16:32  naniwa
 * minor fix
 *
 * Revision 1.27  2000/02/27 15:33:54  naniwa
 * to work as multi task OS
 *
 * Revision 1.26  2000/02/20 09:35:38  naniwa
 * minor fix
 *
 * Revision 1.25  2000/01/26 08:24:35  naniwa
 * to prevent memory leak
 *
 * Revision 1.24  2000/01/21 14:25:19  naniwa
 * to check fileid
 *
 * Revision 1.23  2000/01/18 14:41:08  naniwa
 * to close opend file at exit
 *
 * Revision 1.22  2000/01/15 15:29:30  naniwa
 * minor fix
 *
 * Revision 1.21  1999/07/23 14:39:03  naniwa
 * modified to implement exit
 *
 * Revision 1.20  1999/07/21 15:10:08  naniwa
 * modified to implement waitpid
 *
 * Revision 1.19  1999/03/24 04:52:10  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.18  1999/03/24 03:54:46  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.17  1999/03/21 14:47:12  monaka
 * Function proc_get_ppid added.
 *
 * Revision 1.16  1998/02/24 14:20:07  night
 * プロセステーブルを初期化するときに、プロセス 0 だけは、
 * 使用中として確保するように変更した。
 * (プロセス 0 は、init プログラムにて使用する)
 *
 * Revision 1.15  1998/02/23 14:49:20  night
 * proc_vm_dump 関数の追加。
 *
 * Revision 1.14  1998/02/16 14:26:49  night
 * proc_set_info() を実行するときにプロセスの状態をチェックしていた
 * 処理を削除した。
 * proc_set_info() で対象となるプロセスは、生成していない状態の場合も
 * あるので、チェックする必要はない。
 *
 * Revision 1.13  1997/10/24 13:59:50  night
 * 変数 free_proc、run_proc の追加。
 *
 * Revision 1.12  1997/10/23 14:32:33  night
 * exec システムコール関係の処理の更新
 *
 * Revision 1.11  1997/10/22 14:56:07  night
 * proc_set_gid () を作成した。
 *
 * Revision 1.10  1997/08/31 13:34:33  night
 * プロセス情報の設定時、work directory を強制的に rootfile の値に設定するように
 * した(以前は、NULL に設定していた)。
 * 以下の関数追加。
 *
 *  proc_set_euid (W procid, W uid)
 *  proc_set_egid (W procid, W gid)
 *
 * Revision 1.9  1997/07/07 12:17:41  night
 * proc_get_euid と proc_get_egid を追加。
 *
 * Revision 1.8  1997/05/12 14:31:51  night
 * misc システムコールに M_PROC_DUMP コマンドを追加。
 *
 * Revision 1.7  1997/05/08 15:11:30  night
 * プロセスの情報を設定する機能の追加。
 * (syscall misc の proc_set_info コマンド)
 *
 * Revision 1.6  1997/05/06 12:47:50  night
 * set_procinfo システムコールの追加。
 *
 * Revision 1.5  1996/11/20  12:12:28  night
 * proc_set_file() において、ファイル記述子で指定されたファイルインデック
 * スがすでに使われているかどうかのチェックを追加。
 *
 * Revision 1.4  1996/11/18  13:44:04  night
 * 関数 proc_get_file() を追加。
 *
 * Revision 1.3  1996/11/17  16:48:30  night
 * init_process() の中身を作成した。
 * proc_get_umask(), proc_set_umask() を作成した。
 *
 * Revision 1.2  1996/11/14  13:17:38  night
 * プロセス構造体の情報を取得する関数を追加。
 *
 * Revision 1.1  1996/11/05  15:13:46  night
 * 最初の登録
 *
 */

#include "posix.h"


struct proc		proc_table[MAX_PROCESS];
struct proc		*free_proc, *tail_proc;
struct proc		*run_proc = NULL;



/* init_process
 *
 */
W
init_process (void)
{
  W	i;

  for (i = 0; i < MAX_PROCESS - 1; i++)
    {
      bzero((B *) &proc_table[i], sizeof(struct proc));
      proc_table[i].proc_status = PS_DORMANT;
      proc_table[i].proc_next = &proc_table[i + 1];
      proc_table[i].proc_pid = i;
    }
  bzero((B *) &proc_table[i], sizeof(struct proc));
  proc_table[i].proc_status = PS_DORMANT;
  proc_table[i].proc_next = NULL; 
  proc_table[i].proc_pid = i;

  free_proc = &proc_table[1];
  tail_proc = &proc_table[i];

  proc_table[0].proc_status = PS_SLEEP;		/* プロセス 0 については、最初に確保しておく */
  proc_table[0].proc_next = NULL;
  return (E_OK);
}



/* プロセスの情報を強制的に設定する
 *
 */
W
proc_set_info (struct posix_request *req)
{
  struct procinfo *procinfo;
  UW		procid;
  struct proc	*procp;
  W		error;


  procid = req->procid;
#ifdef DEBUG
  printk ("Posix:proc: proc_set_info. procid = %d\n", procid);
#endif

  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }

  /* プロセスの情報を設定する
   * 今のところ設定する情報の内容はチェックしていないが、
   * 本来は、チェックすべき
   */
  error = proc_get_procp (procid, &procp);
  if (error)
    {
      return (error);
    }

  if (procp->proc_status == PS_DORMANT)
    {
      return (EP_INVAL);
    }

  procinfo = &(req->param.par_misc.arg.set_procinfo);
  procp->proc_maintask = procinfo->proc_maintask;
  procp->proc_signal_handler = procinfo->proc_signal_handler;
  procp->proc_uid = procinfo->proc_uid;
  procp->proc_gid = procinfo->proc_gid;
  procp->proc_euid = procinfo->proc_euid;
  procp->proc_egid = procinfo->proc_egid;
  procp->proc_umask = procinfo->proc_umask;
  procp->proc_status = PS_RUN;

  /* 作業ディレクトリ (カレントディレクトリ)の設定は今のところ
   * 行っていない。
   * 本来は、'/' ディレクトリに設定する。
   * (本システムコール実行後に chdir システムコールで任意のディレクトリに移動できる)
   */
  procp->proc_workdir = rootfile;				/* XXX */
  rootfile->i_refcount++;

  procp->proc_pid = procinfo->proc_pid;
  procp->proc_ppid = procinfo->proc_ppid;

  /* 仮想テーブルの生成 */
  setup_vmtree (&(proc_table[procid]), procinfo->proc_maintask, procinfo->proc_access, 0, 0, 0x7fffffff);

  /* file discriptor 0, 1, 2 の設定 */
  error = open_special_dev(procp);
  if (error != EP_OK) {
    printk("[PM] can't open special files\n");
    return(error);
  }

#ifdef notdef
  printk ("process information:\n");
  printk ("    process ID: %d\n", procid);
  printk ("    main task:  %d\n", procinfo->proc_maintask);
  printk ("    uid:        %d\n", procinfo->proc_uid);
  printk ("    gid:        %d\n", procinfo->proc_gid);
  printk ("    euid:       %d\n", procinfo->proc_euid);
  printk ("    egid:       %d\n", procinfo->proc_egid);
  printk ("    umask:      %d\n", procinfo->proc_umask);
#endif
  
  return (EP_OK);
}


/* proc_destroy_memory - プロセスのもつすべてのメモリ資源を解放する
 *
 */
W
proc_destroy_memory (W procid)
{
  ER errno;
  
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  errno = destroy_proc_memory(&proc_table[procid], 1);
  return (errno);
}


/* プロセスを終了する
 */
W
proc_exit (W procid)
{
  int i;
  struct proc *procp;

  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }

  /* プロセス情報の初期化 */
  /* proc_pid == 0 でプロセスは未使用とはならない
  proc_table[procid].proc_pid = 0;
  */
  proc_table[procid].proc_status = PS_DORMANT;

  /* プロセスの情報の解放
   */
  procp = &proc_table[procid];
  /* working directory の開放 */
  if (procp->proc_workdir != NULL) {
    fs_close_file(procp->proc_workdir);
    procp->proc_workdir = NULL;
  }
  
  /* open されているファイルの close */
  for (i = 0; i < MAX_OPEN; ++i) {
    if (procp->proc_open_file[i].f_inode != NULL) {
      fs_close_file(procp->proc_open_file[i].f_inode);
      procp->proc_open_file[i].f_inode = NULL;
    }
  }

  /* フリー・プロセス・リストの最後に登録 */
  tail_proc->proc_next = &proc_table[procid];
  tail_proc = &proc_table[procid];

  return (EP_OK);
}


W
proc_dump (struct posix_request *req)
{
  UW		procid;
  struct proc	*procp;
  W		i, j;
  struct vm_directory	*vm_dir;
  struct vm_page	*vm_page;
 
  procid = req->param.par_misc.arg.set_procinfo.proc_pid;
  procp = &proc_table[procid];

  printk ("posix: proc %d dump\n", (int)procid);
  printk ("uid: %d\n", (int)procp->proc_uid);
  printk ("gid: %d\n", (int)procp->proc_gid);
  printk ("ppid: %d\n", (int)procp->proc_ppid);
  printk ("vmtree: 0x%x\n", (int)procp->vm_tree);
  for (i = 0; i < MAX_DIR_ENTRY; i++)
    {
      vm_dir = procp->vm_tree->directory_table[i];
      if (vm_dir)
	{
	  for (j = 0; j < 8; j++)
	    {	
	      vm_page = vm_dir->page_table[j];
	      printk ("%d.%d: paddr = 0x%x   ", i, j, vm_page ? vm_page->addr : 0);
	    }
	  printk ("\n");
	}
    }

  return (E_OK);
}



W
proc_get_procp (W procid, struct proc **procp)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *procp = &proc_table[procid];
  return (EP_OK);
}



W
proc_get_pid (W procid, W *pid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *pid = proc_table[procid].proc_pid;
  return (EP_OK);
}



W
proc_get_ppid (W procid, W *ppid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *ppid = proc_table[procid].proc_ppid;
  return (EP_OK);
}



W
proc_get_uid (W procid, W *uid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *uid = proc_table[procid].proc_uid;
  return (EP_OK);
}


W
proc_get_gid (W procid, W *gid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *gid = proc_table[procid].proc_gid;
  return (EP_OK);
}


W
proc_set_gid (W procid, W gid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  proc_table[procid].proc_egid = gid;
  return (EP_OK);
}


W
proc_get_euid (W procid, W *uid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *uid = proc_table[procid].proc_euid;
  return (EP_OK);
}



W
proc_set_euid (W procid, W uid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  proc_table[procid].proc_euid = uid;
  return (EP_OK);
}



W
proc_get_egid (W procid, W *gid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *gid = proc_table[procid].proc_egid;
  return (EP_OK);
}



W
proc_set_egid (W procid, W gid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  proc_table[procid].proc_egid = gid;
  return (EP_OK);
}


W
proc_alloc_fileid (W procid, W *retval)
{
  W	i;

  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  for (i = 0; i < MAX_OPEN; i++)
    {
      if (proc_table[procid].proc_open_file[i].f_inode == NULL)
	{
	  *retval = i;
	  bzero ((B*) &(proc_table[procid].proc_open_file[i]), sizeof (struct file));
	  return (EP_OK);
	}
    }
  return (EP_NOMEM);
}


W
proc_set_file (W procid, W fileid, W flag, struct inode *ip)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  if ((fileid < 0) || (fileid >= MAX_OPEN))
    {
      return (EP_INVAL);
    }

  if (proc_table[procid].proc_open_file[fileid].f_inode != NULL)
    {
      return (EP_BADF);
    }

  proc_table[procid].proc_open_file[fileid].f_inode = ip;
  if ((flag & O_APPEND) != 0) {
    proc_table[procid].proc_open_file[fileid].f_offset = ip->i_size;
  }
  else {
    proc_table[procid].proc_open_file[fileid].f_offset = 0;
  }
  proc_table[procid].proc_open_file[fileid].f_omode = flag & 0x03;
  return (EP_OK);
}


W
proc_get_file (W procid, W fileid, struct file **fp)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  if ((fileid < 0) || (fileid >= MAX_OPEN))
    {
      return (EP_INVAL);
    }

  *fp = &(proc_table[procid].proc_open_file[fileid]);
  return (EP_OK);
}


W
proc_rewind_file (W procid, W fileid)
{
  struct file *fp;

  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  if ((fileid < 0) || (fileid >= MAX_OPEN))
    {
      return (EP_INVAL);
    }

  fp = &(proc_table[procid].proc_open_file[fileid]);
  if (fp == (struct file *)0)
    {
      return (EP_INVAL);
    }

  fp->f_offset = 0;
  return (EP_OK);
}


W
proc_get_cwd (W procid, struct inode **cwd)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  *cwd = proc_table[procid].proc_workdir;
  if (*cwd == NULL)
    {
      return (EP_SRCH);
    }

  return (EP_OK);
}

W
proc_set_cwd (W procid, struct inode *cwd)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  if (cwd == (struct inode *)0)
    {
      return (EP_INVAL);
    }

  proc_table[procid].proc_workdir = cwd;
  return (EP_OK);
}


W
proc_set_umask (W procid, W umask)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  
  proc_table[procid].proc_umask = umask;
  return (EP_OK);
}


W
proc_get_umask (W procid, W *umask)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }

  *umask = proc_table[procid].proc_umask;
  return (EP_OK);
}


struct vm_tree *
proc_get_vmtree (W procid)
{
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (NULL);
    }

  return (proc_table[procid].vm_tree);
}




/* proc_new_proc - 新しいプロセスを生成する
 *
 *
 */
W
proc_alloc_proc (struct proc **procp)
{
  if (free_proc == NULL)
    {
      return (EP_NOMEM);
    }

  *procp = free_proc;
  free_proc = (*procp)->proc_next;
  (*procp)->proc_next = NULL;
/*  bzero ((void *)*procp, sizeof (struct proc)); */
  return (EP_OK);
}


/* proc_renew_task - プロセスに所属するタスクをひとつ生成する。
 *
 * 機能：	既存のプロセスに所属しているタスクを終了し、
 *		新しく生成したタスクに切り換える。
 *
 *
 */
W
proc_renew_task (W procid, FP main_funcp, FP signal_funcp, ID *new_main_task, ID *new_signal_task)
{
  ER		errno;
  struct proc	*procp;
  T_CTSK	pk_ctsk;
  ID		rid;

  
  if ((procid < 0) || (procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }

  procp = &proc_table[procid];
  errno = ter_tsk (procp->proc_maintask);
  if (errno)
    {
      return (EP_INVAL);
    }
  errno = ter_tsk (procp->proc_signal_handler);
  if (errno)
    {
      return (EP_INVAL);
    }

  pk_ctsk.exinf = 0;
  pk_ctsk.startaddr = main_funcp;
  pk_ctsk.itskpri = POSIX_TASK_LEVEL;
  pk_ctsk.stksz = PAGE_SIZE * 2;
  pk_ctsk.addrmap = NULL;
  errno = vcre_tsk (&pk_ctsk, &rid);
  if (errno)
    {
      return (EP_NOMEM);
    }
  *new_main_task = rid;
  procp->proc_maintask = rid;
  
  pk_ctsk.exinf = 0;
  pk_ctsk.startaddr = signal_funcp;
  pk_ctsk.itskpri = POSIX_TASK_LEVEL;
  pk_ctsk.stksz = PAGE_SIZE * 2;
  pk_ctsk.addrmap = NULL;
  errno = vcre_tsk (&pk_ctsk, &rid);
  if (errno)
    {
      return (EP_NOMEM);
    }
  *new_signal_task = rid;
  procp->proc_signal_handler = rid;
  
  return (EP_OK);  
}


W
proc_vm_dump (struct posix_request *req)
{
  struct proc		*procp;
  struct vm_tree	*treep;
  W			dir_index;
  W			page_index;
  struct vm_directory	*dirp;
  struct vm_page	*pagep;
  UW			paddr;
  ER			error;
  UW			addr = 0;
  ID			taskid;
  

  if ((req->procid < 0) || (req->procid >= MAX_PROCESS))
    {
      return (EP_INVAL);
    }
  procp = &proc_table[req->param.par_misc.arg.procid];
  taskid = procp->proc_maintask;

#ifdef DEBUG
  printk ("procid: %d\n", req->procid);
#endif
  if (procp->proc_status == PS_DORMANT)
    {
#ifdef DEBUG
      printk ("proc is dormant.\n");
#endif
      return (EP_NOENT);
    }

#ifdef DEBUG
  printk ("proc_vm_dump(): procp = 0x%x\n", procp);
#endif

  treep = procp->vm_tree;
  for (dir_index = 0; dir_index < (MAX_DIR_ENTRY / 2); dir_index++)
    {
      dirp = treep->directory_table[dir_index];

      if (dirp)
	{
#ifdef DEBUG
	  printk ("dir[%d], vmtree = 0x%x:\n", dir_index, treep);	/* */
#endif
	  for (page_index = 0; page_index < MAX_PAGE_ENTRY; page_index++)
	    {
	      /* 物理メモリアドレスを取得 */
	      error = vget_phs (taskid, (VP)addr, &paddr);
	      
	      pagep = dirp->page_table[page_index];
	      if (pagep)
		{
#ifdef DEBUG
		  printk ("vm_dump: map (vaddr = 0x%x, paddr = 0x%x)\n", addr, pagep->addr);
#endif
		}
	      else if (error == E_OK)
		{
#ifdef DEBUG
		  printk ("vm_dump: ?? vm_tree information invalid. map (vaddr = 0x%x, paddr = 0x%x)\n", addr, paddr);
#endif
		}
	      addr += PAGE_SIZE;
	    }
	}
    }

  return (EP_OK);
}
