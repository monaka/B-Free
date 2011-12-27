/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: posix_if.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/init/posix_if.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: posix_if.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.20  2000-02-16 08:20:36  naniwa
 * to reorder POSIX system calls
 *
 * Revision 1.19  2000/01/28 10:28:03  monaka
 * Some hooks for POSIX manager (especially file functions) was deleted.
 * Now Init is more close to POSIX libs.
 *
 * Revision 1.18  1999/11/10 10:23:58  naniwa
 * to support execve
 *
 * Revision 1.17  1999/07/26 08:59:09  naniwa
 * minor chage of posix_fork()
 *
 * Revision 1.16  1999/07/24 04:35:07  naniwa
 * minor change
 *
 * Revision 1.15  1999/07/09 08:25:20  naniwa
 * minor fix
 *
 * Revision 1.14  1999/04/13 04:14:45  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.13  1999/03/16 16:22:14  monaka
 * File operators was modified to use POSIX lowlib.
 *
 * Revision 1.12  1999/03/15 08:43:25  monaka
 * Minor fix.
 *
 * Revision 1.11  1999/02/17 09:43:10  monaka
 * Modified for 'device descriptor'.
 *
 * Revision 1.10  1998/05/23 15:30:54  night
 * POSIX サーバへの呼び出しコマンドを READ から READDIR に変更。
 *
 * Revision 1.9  1998/02/24 14:05:52  night
 * posix_fork() の修正。
 * デバッグ用のプリント文の追加。
 * 子プロセスのタスクのスタックサイズを 10KB から 12 KB に
 * 変更(ページ境界に合わせた)。
 *
 * Revision 1.8  1998/02/16 14:12:52  night
 * posix_fork () 関数の追加。
 * この関数は、fork システムコールの処理のテストを行うための
 * 関数。
 * コマンド pfork を実行するときに呼び出される。
 *
 * Revision 1.7  1998/01/06 16:39:18  night
 * レクエストパケットにコマンド情報が入っていなかったので、追加した。
 *
 * Revision 1.5  1997/10/11 16:20:15  night
 * ファイルの書き込み処理を lowlib を介さないように変更。
 *
 * Revision 1.4  1997/09/09 13:49:30  night
 * posix_write 関数を追加。
 *
 * Revision 1.3  1997/08/31 14:05:06  night
 * BOOT が HD から行われた時には、自動的に POSIX の root file system を
 * 設定するように処理を変更した。
 *
 * Revision 1.2  1997/07/09 15:00:42  night
 * 文字コードを EUC に変更。
 * posix_access() と posix_newprocess() の追加。
 *
 * Revision 1.1  1997/07/07 12:18:56  night
 * 最初の登録
 *
 *
 */


#include "/usr/include/linux/a.out.h"
#include "init.h"


#define LOWLIB_INFO_ADDR	0x88030000
#define POSIX_MIN_PRIORITY	10
#define X_OK			1


struct posix_process
{
  W	procid;

  ID	maintask;
  ID	signaltask;

  UW	text_start;
  UW	text_size;

  UW	data_start;
  UW	data_size;

  UW	stack_start;
  UW	stack_size;

  UW	entry;
};


struct posix_lowlib_info
{
  UW	main_entry_addr;
  UW	signal_entry_addr;
};


static ID	posix_port;
static ID	recv_port;
static ID	myself;

W	posix_errno;

static W	posix_mountroot (W);


static W	posix_make_vm (struct posix_process *procinfo);
static W	posix_load_program (W fileid, struct posix_process *procinfo);
static W	posix_create_task (struct posix_process *procinfo, struct posix_lowlib_info *lowlibinfo);


/*
 *
 */
W
posix_init (W root_device)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  W			rsize;


  error = find_port ("manager.posix", &posix_port);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      return (error);
    }

  recv_port = get_port (sizeof (res), sizeof (res));
  if (recv_port == 0)
    {
      printf ("Cannot allocate port\n");
      return (E_NOMEM);
    }

  if (get_tid (&myself) != E_OK)
    {
      printf ("Can not get own taskid.\n");
      return (E_SYS);
    }

#ifdef DEBUG
  printf ("init: pinit, send port = %d, receive port = %d\n",
	  posix_port, recv_port);
#endif

  /* ROOT ファイルシステムの設定
   */
  error = posix_mountroot (root_device);
  if (error)
    {
      return (E_SYS);
    }
  
  /* init プロセスの情報の設定
   */
  req.receive_port = recv_port;
  req.msg_length = sizeof (res);
  req.operation = PSC_MISC;
  req.procid = 0;
  req.caller = myself;
  req.param.par_misc.cmd = M_SET_PROCINFO;
  req.param.par_misc.arg.set_procinfo.proc_maintask = myself;
  req.param.par_misc.arg.set_procinfo.proc_signal_handler = NULL;
  req.param.par_misc.arg.set_procinfo.proc_uid = 0;
  req.param.par_misc.arg.set_procinfo.proc_gid = 0;
  req.param.par_misc.arg.set_procinfo.proc_euid = 0;
  req.param.par_misc.arg.set_procinfo.proc_egid = 0;
  req.param.par_misc.arg.set_procinfo.proc_umask = 022;
  req.param.par_misc.arg.set_procinfo.proc_pid = 0;
  req.param.par_misc.arg.set_procinfo.proc_ppid = 0;
  req.param.par_misc.arg.set_procinfo.proc_access = VM_READ | VM_WRITE | VM_EXEC;
  snd_mbf (posix_port, sizeof (req), &req);

  rsize = sizeof (res);
  rcv_mbf (&res, &rsize, recv_port);


  return (E_OK);
}


static W
posix_mountroot (W root_device)
{
  ID	posix_manager;
  ER	error;
  struct posix_request	req;
  struct posix_response res;
  W	rsize;

  error = find_port ("manager.posix", &posix_manager);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      return (error);
    }

  req.receive_port = recv_port;
  req.msg_length = sizeof (req);
  req.operation = PSC_MOUNTROOT;
  req.param.par_mountroot.device = root_device;
  req.param.par_mountroot.fstype = 1;
  req.param.par_mountroot.option = 0;
  snd_mbf (posix_manager, sizeof (req), &req);
  rsize = sizeof (res);
  rcv_mbf (&res, &rsize, recv_port);
  if (res.errno)
    {
      printf ("syscall error = %d\n", (int)res.errno);
    }
  else
    {
      printf ("syscall success.\n");
    }

  return (E_OK);
}




/* posix_opendir
 *
 *
 */
W
posix_opendir (B *path)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  W			rsize;


  req.param.par_open.path = path;
  req.param.par_open.pathlen = strlen (path);
  req.param.par_open.oflag = 0;		/* o_rdonly | o_wronly | o_rdwr | o_creat */
  req.param.par_open.mode = 0;		/* no use */
  req.receive_port = recv_port;
  req.msg_length = sizeof (res);
  req.operation = PSC_OPEN;
  req.procid = 0;
  req.caller = myself;
  snd_mbf (posix_port, sizeof (req), &req);

  rsize = sizeof (res);
  error = rcv_mbf (&res, &rsize, recv_port);
  if (error)
    {
      printf ("error on rcv_msg: errno = %d\n", error);
    }
  else if (res.errno)
    {
      printf ("syscall error = %d (%d), res.status = %d\n", res.errno, error, res.status);
    }
  else
    {
      printf ("opendir: fileid = %d\n", res.status);
    }

  return (res.status);
}


W
posix_closedir (W fileid)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  W			rsize;


  req.param.par_close.fileid = fileid;
  req.receive_port = recv_port;
  req.msg_length = sizeof (res);
  req.operation = PSC_CLOSE;
  req.procid = 0;
  req.caller = myself;
  snd_mbf (posix_port, sizeof (req), &req);

  rsize = sizeof (res);
  error = rcv_mbf (&res, &rsize, recv_port);
  if (error)
    {
      printf ("error on rcv_msg: errno = %d\n", error);
    }
  else if (res.errno)
    {
      printf ("syscall error = %d (%d)\n", res.errno, error);
    }
  else
    {
      printf ("closed: fileid = %d\n", res.status);
    }

  return (res.status);
}


W
posix_readdir (W fileid, B *buf, W length)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  W			rsize;


  req.param.par_read.fileid = fileid;
  req.param.par_read.buf = buf;
  req.param.par_read.length = length;
  req.receive_port = recv_port;
  req.msg_length = sizeof (res);
  req.operation = PSC_READ;
  req.procid = 0;
  req.caller = myself;
  snd_mbf (posix_port, sizeof (req), &req);

  rsize = sizeof (res);
  error = rcv_mbf (&res, &rsize, recv_port);
  if (error)
    {
      printf ("error on rcv_msg: errno = %d\n", error);
      return (-1);
    }
  else if (res.errno)
    {
      printf ("syscall error = %d (%d)\n", res.errno, error);
      return (-1);
    }

  return (res.status);
}


#if 0
W
posix_stat (W fileid, struct stat *st)
{
  W result;
  bzero (st, sizeof (struct stat));
  result = stat (fileid, st);

  printf ("st_dev = 0x%x\n", st->st_dev);
  printf ("st_ino = %d\n", st->st_ino);
  printf ("st_mode = 0x%x\n", st->st_mode);
  printf ("st_size = %d\n", st->st_size);

  return result;
}
#endif


/* posix 環境で動く、新しいプロセスを生成する。
 *
 *
 */
W
posix_newprocess (B *program)
{
  W				fileid;
  struct exec			exec_info;
  struct posix_lowlib_info	*lowlib_info;
  struct posix_process		procinfo;
  ER				err;


  if (access (program, X_OK) == -1)
    {
      printf ("cannot access.\n");
      return (-1);
    }

  /* lowlib 情報へアクセスする
   */
  lowlib_info = (struct posix_lowlib_info *)LOWLIB_INFO_ADDR;


  /* posix プロセスを実行する itron タスクの生成
   */
  if (posix_create_task (&procinfo, lowlib_info) < 0)
    {
      printf ("cannot create new tasks.\n");
      return (-1);
    }

  fileid = open (program, O_RDONLY, 0);
  if (fileid < 0)
    {
      printf ("cannot open.\n");
      return (-1);
    }


  if (read (fileid, (VP)&exec_info, sizeof (exec_info)) <= 0)
    {
      printf ("cannot read exec infomation.");
      return (-1);
    }

  /* posix プロセスのための仮想空間の情報設定
   */
  procinfo.text_start = 0;
  procinfo.text_size  = exec_info.a_text;;
  procinfo.data_start = ROUNDUP (exec_info.a_text, PAGE_SIZE);
  procinfo.data_size  = ROUNDUP (exec_info.a_data + exec_info.a_bss, PAGE_SIZE);
  procinfo.stack_start = 0x30008000;
  procinfo.stack_size = 10 * 1024;;
  procinfo.entry = exec_info.a_entry;

  /* 仮想空間の作成
   */
  if (posix_make_vm (&procinfo))
    {
      /* 生成したタスクの破棄 
       */
      return (-1);
    }

  /* 作成した仮想空間へプログラムの内容を満たす
   */
  posix_load_program (fileid, &procinfo);


  /* タスクをスタートする
   */
  err = sta_tsk (procinfo.maintask, 0);
  if (err)
    {
      printf ("cannot start main task.\n");
      ter_tsk (procinfo.signaltask);
      return (-1);
    }
  sta_tsk (procinfo.signaltask, 0);
  if (err)
    {
      printf ("cannot start main task.\n");
      ter_tsk (procinfo.maintask);
      return (-1);
    }

  return (E_OK);
}


/* posix プロセスが仕様する仮想空間を作成する。
 * 作成した仮想空間は、タスクに結びつける。
 */
static W
posix_make_vm (struct posix_process *procinfo)
{
  /* まず、main task の仮想空間を作成する
   */


  /* 作成した仮想空間を signal task の仮想空間と
   * 共有させる。
   */
}


/* POSIX 環境で動作するプログラムの実行イメージを
 * 仮想空間にロードする。
 *
 */
static W
posix_load_program (W fileid, struct posix_process *procinfo)
{
  
}


/* POSIX プロセスの実行主体である ITRON タスクを生成する
 *
 */
static W
posix_create_task (struct posix_process *procinfo, struct posix_lowlib_info *lowlibinfo)
{
  ER		err;
  T_CTSK	task_info;
  ID		taskid;

  bzero (&task_info, sizeof (task_info));
  task_info.tskatr = TA_HLNG;
  task_info.startaddr = (VP)procinfo->entry;
  task_info.itskpri = POSIX_MIN_PRIORITY;
  task_info.stksz = 1024 * 10;
  task_info.startaddr = (VP)lowlibinfo->main_entry_addr;
  for (taskid = MIN_USERTASKID; taskid <= MAX_USERTASKID; taskid++)
    {
      err = cre_tsk (taskid, &task_info);
      if (err == E_OK)
	break;
    }

  if (err)
    {
      return (err);
    }
  procinfo->maintask = taskid;

  bzero (&task_info, sizeof (task_info));
  task_info.tskatr = TA_HLNG;
  task_info.startaddr = (VP)lowlibinfo->signal_entry_addr;
  task_info.itskpri = POSIX_MIN_PRIORITY;
  task_info.stksz = 1024 * 10;
  task_info.startaddr = (VP)lowlibinfo->signal_entry_addr;
  for (taskid = MIN_USERTASKID; taskid <= MAX_USERTASKID; taskid++)
    {
      err = cre_tsk (taskid, &task_info);
      if (err == E_OK)
	break;
    }

  if (err)
    {
      ter_tsk (procinfo->maintask);
      return (err);
    }
  procinfo->signaltask = taskid;

  return (E_OK);
}



W
posix_exec (B *path)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  W			rsize;


  req.receive_port = recv_port;
  req.msg_length = sizeof (res);
  req.operation = PSC_EXEC;
  req.param.par_execve.name = path;
  req.param.par_execve.pathlen = strlen (path);
#ifdef notdef
  req.param.par_execve.start_main = 0;
  req.param.par_execve.start_signal = 0;
#endif
  req.procid = 0;
  req.caller = myself;
  snd_mbf (posix_port, sizeof (req), &req);

  rsize = sizeof (res);
  error = rcv_mbf (&res, &rsize, recv_port);
  if (error)
    {
      printf ("error on rcv_msg: errno = %d\n", error);
    }
  else if (res.errno)
    {
      printf ("syscall error = %d (%d)\n", res.errno, error);
    }

  printf ("exec: sucess\n");
  return (res.status);
}


W
posix_fork (void *main_func, void *signal_func)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;
  W			rsize;
  ID			main_task, signal_task;
  ID			posix;
  T_CTSK		task_info;
  static B		page_buf[PAGE_SIZE];


  /* main/signal タスクの生成 
   */
  bzero (&task_info, sizeof (task_info));
  task_info.tskatr = TA_HLNG;
  task_info.startaddr = (VP)main_func;
  task_info.itskpri = POSIX_MIN_PRIORITY;
  task_info.stksz = 1024 * 12;
#if 0
  for (main_task = MIN_USERTASKID; main_task <= MAX_USERTASKID; main_task++)
    {
      error = cre_tsk (main_task, &task_info);
      if (error == E_OK)
	break;
    }
  if (error)
    {
      del_tsk (main_task);
      return (error);
    }
#endif
  error = vcre_tsk(&task_info, &main_task);
  if (error != E_OK) return (error);
  
  bzero (&task_info, sizeof (task_info));
  task_info.tskatr = TA_HLNG;
  task_info.startaddr = (VP)signal_func;
  task_info.itskpri = POSIX_MIN_PRIORITY;
  task_info.stksz = 1024 * 12;
#if 0
  for (signal_task = MIN_USERTASKID; signal_task <= MAX_USERTASKID; signal_task++)
    {
      error = cre_tsk (signal_task, &task_info);
      if (error == E_OK)
	break;
    }
  if (error)
    {
      ter_tsk (main_task);
      ter_tsk (signal_task);
      return (error);
    }
#endif
  error = vcre_tsk(&task_info, &signal_task);
  if (error != E_OK) {
    del_tsk(main_task);
    return(error);
  }

  error = find_port ("manager.posix", &posix);
  if (error != E_OK)
    {
      printf ("Cannot find port.\n");
      del_tsk (main_task);
      del_tsk (signal_task);
      return (error);
    }

  /* POSIX マネージャに対して、fork メッセージを送る。
   */
  req.receive_port = recv_port;
  req.msg_length = sizeof (req);
  req.operation = PSC_FORK;
  req.param.par_fork.main_task = main_task;
  req.param.par_fork.signal_task = signal_task;
  req.procid = 0;
  req.caller = myself;
  snd_mbf (posix, sizeof (req), &req);

  rsize = sizeof (res);
  error = rcv_mbf (&res, &rsize, recv_port);
  if (error)
    {
      printf ("fork: error on rcv_msg: errno = %d\n", error);
      del_tsk (main_task);
      del_tsk (signal_task);
      return (error);
    }
  else if (res.errno)
    {
      printf ("fork: syscall error = %d (%d)\n", res.errno, error);
      del_tsk (main_task);
      del_tsk (signal_task);
      return (res.errno);
    }


#ifdef notdef
  error = vget_reg (main_task, (VP) 0x8070, PAGE_SIZE, page_buf);
  if (error)
    {
      printf ("cannot get data from child process (errno = %d)\n", error);
    }
  else
    {
      W	i, j;

      printf ("main_task = %d\n", main_task);	/* */
      for (i = 0; i < 64; )
	{
	  for (j = 0; j < 8; j++, i++)
	    {
	      if (page_buf[i] < 0xA)
		{
		  printf ("0x0%x ", page_buf[i]);
		}
	      else
		{
		  printf ("0x%x ", page_buf[i]);
		}
	    }
	  printf ("\n");
	}
    }
#endif

  printf("main task ID [%d] signal task ID [%d]\n", main_task, signal_task);
  error = sta_tsk (main_task, 0);
  if (error)
    {
      /* main task が起動できなかった */
      printf ("cannot start task.\n");
      del_tsk (main_task);
      del_tsk (signal_task);
      return (error);
    }

  printf ("child process is %d\n", res.status);
  del_tsk (signal_task);

  {
    W pid, status;
    printf("waiting...\n");
    pid = waitpid(-1, &status, 0);
    printf("child process exited pid = %d status = %d\n", pid, status);
  }

  return (E_OK);
}
