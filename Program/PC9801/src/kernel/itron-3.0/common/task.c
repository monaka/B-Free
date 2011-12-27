/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* task.c $Revision: 1.1 $
 *
 * タスク管理を行う。
 *
 *
 * 外部関数リスト
 *
 *	init_task ()		--- タスク管理データの初期化
 *	init_task1 ()		--- システム管理タスクの生成と起動
 *	cre_tsk ()		--- タスクの生成
 *	new_task ()		--- 任意のタスク ID でのタスク生成
 *
 * 大域変数
 *	run_task		--- 現在走行中のタスク
 *
 * 内部関数 (static)
 *
 *	make_task_context ()	--- タスク情報を作成する。
 *
 * 内部変数 (static)
 *
 *	task
 *	task_buffer
 *	ready_task
 *	dispatch_flag		ディスパッチするかどうかのフラグ
 */

/*
 * $Log: task.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.13  1995-10-01 12:58:37  night
 * 関数 wup_tsk() の中で wakeup するタスクの情報のアドレスのチェックを追
 * 加。
 *
 * Revision 1.12  1995/09/21  15:51:12  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.11  1995/09/17  16:57:07  night
 * task_switch() 呼び出し時の処理の変更。
 * カレントタスクより優先順位が高いタスクがいない場合タスクスイッチを行わ
 * なかったが、優先順位の低いタスクへも切りかえるように変更した。
 *
 * Revision 1.10  1995/09/14  04:32:05  night
 * タスク状態を表示する関数 (print_list()) の変更。
 * 表示内容にタスクレベルを追加。
 *
 * Revision 1.9  1995/08/26  02:15:23  night
 * RCS の Log マクロの追加。
 *
 *
 */

#include "itron.h"
#include "errno.h"
#include "task.h"
#include "misc.h"
#include "func.h"
#include "interrupt.h"

/***************************************************************************
 *	タスク管理用の変数
 *
 *
 */
T_TCB		*run_task;			/* 現在、走行中のタスク */

static T_TCB	*task;
static T_TCB	task_buffer[MAX_TSKID - MIN_TSKID + 1];
static T_TCB	*ready_task[MAX_PRIORITY - MIN_PRIORITY + 1];
static BOOL	dispatch_flag = TRUE;

static ER 	make_task_stack (T_TCB *task, W size, VP *sp);
static void	print_list (void);


void
print_task_list (void)
{
  dis_int ();
  print_list ();
  ena_int ();
}

static void
print_list (void)
{
  int	i;

  for (i = 1; i < MAX_TSKID; i++)
    {
      switch (task[i].tskstat)
	{
	case TTS_RUN:
	  printf ("%d (%d)  <RUN>           0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_RDY:
	  printf ("%d (%d)  <RDY>           0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_WAI:
	  printf ("%d (%d)  <WAIT>          0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_SUS:
	  printf ("%d (%d)  <SUSPEND>       0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_WAS:
	  printf ("%d (%d)  <WAIT-SUSPEND>  0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	}
    }
}

W
list_counter (T_TCB *list)
{
  W		i;
  T_TCB	*p, *q;

  q = NULL;
  for (i = 0, p = list;
       (q != list) && (list != NULL);
       p = q, i++)
    {	
      q = p->next;
    }
  return (i);
}


#ifdef TASK_DEBUG_PRORAM
main ()
{
  T_TCB a, b, c;
  T_TCB *list = NULL;

  a.tsklevel = 0;
  b.tsklevel = 1;
  c.tsklevel = 2;
  print_list (list);
  list = ins_tcb_list (list, &a);
  print_list (list);
  list = ins_tcb_list (list, &b);
  print_list (list);
  list = ins_tcb_list (list, &c);
  print_list (list);
}  
#endif /* TASK_DEBUG_PROGRAM */



/* init_task --- タスク管理の初期化
 *
 * TCB テーブルの内容を初期化する。そして、カレントタスクをタスク番号 -1 
 * のタスクとする。
 *
 */
void
init_task (void)
{
  W	i;

  /* TCB テーブルの作成と GDT への登録。
   */
  for (i = 0; i < NTASK; i++)
    {
      task_buffer[i].tskstat = TTS_NON;
    }
/*  bzero (kernel_tss, sizeof (TASK_STATE_SEG_T) * MAX_TASK); */
  task = &task_buffer[-1];
}


/* init_task1 --- タスク1 の情報を初期化する。
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	タスク 1 の情報を初期化する。
 *		タスク 1 の情報は、proc_table[0] に収められる。
 *		以下の情報を初期化する。
 *		  cr3		現在の cr3 の内容を入れる。
 *		  state		プロセス状態を TTS_RUN に設定(このタスクは実行中)
 *				優先度を 0 に設定
 *				親タスクのタスク id を 0 に設定。
 *		タスクレジスタの値を task[1] の context のアドレス
 *		に設定。
 *		タスクレジスタの値は、設定時には、なにも影響がないが、
 *		タスクを切り換えたとき、現在のタスクレジスタの指すTSS
 *		領域に今現在のタスク情報を退避する。
 *		そのため、事前にタスクレジスタの値を設定しておく必要がある。
 */
void
init_task1 (void)
{
  /* タスク1の情報を初期化する。*/
  task[KERNEL_TASK].tskstat = TTS_RUN;		/* タスク状態：走行状態にセット */
  task[KERNEL_TASK].tsklevel = MAX_PRIORITY;	/* タスクレベルは、31(最低位)にセット */
  task[KERNEL_TASK].tskid = KERNEL_TASK;	/* タスク ID は、KERNEL_TASK(1)にセット */

#ifdef I386
  /* タスク 1 のコンテキスト情報を初期化する			*/
  /* これらの情報は、次にタスク1がカレントタスクになった時に	*/
  /* 使用する 							*/
  task[KERNEL_TASK].context.cr3 = (UW)PAGE_DIR_ADDR;
  task[KERNEL_TASK].context.cs  = KERNEL_ESEG;
  task[KERNEL_TASK].context.ds  = KERNEL_DSEG;
  task[KERNEL_TASK].context.es  = KERNEL_ESEG;
  task[KERNEL_TASK].context.fs  = KERNEL_ESEG;
  task[KERNEL_TASK].context.gs  = KERNEL_ESEG;
  task[KERNEL_TASK].context.ss  = KERNEL_DSEG;
  task[KERNEL_TASK].context.ss0  = KERNEL_DSEG;
  task[KERNEL_TASK].context.zero = 0;
  task[KERNEL_TASK].context.ldtr = 0;
  task[KERNEL_TASK].context.backlink = 0;
  task[KERNEL_TASK].context.iobitmap = 0;
  
#endif /* I386 */

  /* 現タスクはタスク1である。*/
  run_task = &(task[KERNEL_TASK]);
  
  /* セレクタをセット */
  task[KERNEL_TASK].tss_selector = ((KERNEL_TASK + TSS_BASE) << 3) & 0xfff8;
  create_context (&task[KERNEL_TASK]);

  /* タスクレジスタの値を設定する.*/
#ifdef I386
  load_task_register ((KERNEL_TASK + TSS_BASE) << 3);
#endif I386
/*  run_task = &(task[KERNEL_TASK]); */
}


/* タスク情報を生成する:
 *
 *	引数：
 *		task		タスクのTCB領域へのポインタ
 *		stack_size	タスクのスタックサイズ
 *
 */
static ER
make_task_context (T_TCB *task, T_CTSK *pk_ctsk)
{
  VP	sp;
  ER	err;

  err = make_task_stack (task, pk_ctsk->stksz, &sp);
  if (err != E_OK)
    {
      return (err);
    }

#ifdef I386
  /* レジスタ類をすべて初期化する:
   * reset_registers()  は、以下の引数を必要とする：
   *   1) TCB 領域へのポインタ
   *   2) タスクのスタートアドレス
   *   3) カーネルスタックのアドレス
   */
/*  task->context.cr3 = (UW)PAGE_DIR_ADDR; */
  task->context.cs  = KERNEL_ESEG;
  task->context.ds  = KERNEL_DSEG;
  task->context.es  = KERNEL_ESEG;
  task->context.fs  = KERNEL_ESEG;
  task->context.gs  = KERNEL_ESEG;
  task->context.ss  = KERNEL_DSEG;
  task->context.ss0  = KERNEL_DSEG;
  task->context.esp = (UW)((sp + pk_ctsk->stksz) - sizeof (W));
  task->context.ebp = (UW)((sp + pk_ctsk->stksz) - sizeof (W));
  task->context.ldtr = 0;
  task->context.iobitmap = 0;
  task->context.eip = (UW)pk_ctsk->startaddr;
#ifdef TSKSW_DEBUG
  printf ("(UW)pk_ctsk->startaddr = 0x%x\n", (UW)pk_ctsk->startaddr);
#endif
  task->context.eflags = 0;
  task->context.eax = 0;
  task->context.ebx = 0;
  task->context.ecx = 0;
  task->context.esi = 0;
  task->context.edi = 0;
  task->context.zero = 0;
  task->context.iobitmap = 0;
  task->tss_selector = ((TSS_BASE + task->tskid) << 3) & 0xfff8;
#endif /* I386 */
  create_context (task);	/* コンテキスト領域(TSS)のアドレスをGDTにセット */
  return (E_OK); /* set_task_registers (task, pk_ctsk->startaddr, sp)); */
}

/* make_task_stack --- タスクスタックを生成する。
 *
 * カーネルモードで使用するタスク用スタックを生成する。
 *
 */
static ER 
make_task_stack (T_TCB *task, W size, VP *sp)
{
  INT	err;

/*  err = pget_blk (&sp, TMPL_SYS, ROUNDUP (size, PAGE_SIZE)); */
  /* スタックポインタは 0x80000000 の仮想アドレスでアクセスする必要がある。*/
  (*sp) = palloc (ROUNDUP (size, PAGE_SIZE) / PAGE_SIZE);
  (UW)(*sp) |= 0x80000000;
#ifdef TSKSW_DEBUG
  printf ("sp = 0x%x\n", *sp);
#endif
  if (*sp == (VP)NULL)
    {
#ifdef TSKSW_DEBUG
      printf ("make_task_stack: palloc fail.\n");
#endif
      return (E_NOMEM);
    }
  
  return (E_OK);
}

/*****************************************************************************
 * タスクリストを操作するための関数群
 *
 * 
******************************************************************************/
 
/* add_tcb_list --- 引数 list で指定されたリストの一番最後にタスクを追加する。
 *
 * 引数:
 *	list
 *	new
 *
 * 返り値:
 *	新しいリストへのポインタ
 *
 * 注意.
 *	この関数を実行しても、リストの先頭ポインタは *変更されない*。
 *	ただし、リストの要素がなかった場合には例外で、その場合には
 *	引数 new の要素だけをもつリストを返す。
 *
 */
T_TCB *
add_tcb_list (T_TCB *list, T_TCB *new)
{
  dis_int ();
  if (list == NULL)
    {
      new->before = new;
      new->next = new;
      list = new;
      ena_int();
      return list;
    }

  new->before = list;
  new->next = list->next;
  list->next = new;
  new->next->before = new;
  ena_int();
  return (list);
}

/* ins_tcb_list --- 引数 list で指定されたリストの一番最初にタスクを挿入する。
 *
 * 引数:
 *	list
 *	new
 *
 * 返り値:
 *	新しいリストへのポインタ
 *
 * 注意.
 *	この関数を実行した結果、リストの先頭ポインタが変更される。
 *	よって、この関数が返す新しいリストへのポインタをリストポインタに
 *	再代入する必要がある。
 */
T_TCB *
ins_tcb_list (T_TCB *list, T_TCB *new)
{
  dis_int ();
  if (list == NULL)
    {
      new->before = new;
      new->next = new;
      list = new;
      ena_int ();
      return (list);
    }

  new->next = list;
  new->before = list->before;
  list->before = new;
  new->before->next = new;
  list = new;
  ena_int ();
  return (list);
}


/* del_tcb_list --- 引数 list で指定されたリストから、要素 del を削除する。
 *
 * 引数:
 *	list
 *	del
 *
 * 返り値:
 *	新しいリストへのポインタ
 *
 * 例外:
 *	1) もし、要素 del がリストの先頭要素の場合、リストの先頭ポインタは、
 *	   del の次の要素になる。
 *	2) もし、要素 del がリストの唯一の要素の場合、返り値として NULL を
 *	   返す。
 *
 * 注意.
 *	この関数を実行した結果、リストの先頭ポインタが変更される。
 *	よって、この関数が返す新しいリストへのポインタをリストポインタに
 *	再代入する必要がある。
 *
 */
T_TCB *
del_tcb_list (T_TCB *list, T_TCB *del) 
{
  dis_int ();
  if (del->next == del)	/* リストに要素が一つしかなかった場合 */
    {
      ena_int ();
      return (NULL);
    }

  if (list == del)	/* リストの先頭要素だった場合 */
    {
      list = del->next;
      if (list->next == del)	/* 要素が2つしかない場合、delete する要素の次の要素は、
				   delete する要素をさしている。自分自身を指すように
				   変更する。*/
	{
	  list->next = list;
	}
    }

  del->next->before = del->before;
  del->before->next = del->next;
  ena_int ();
  return (list);
}

/* task_switch --- タスク切り換え
 *
 * 引数：	save_nowtask	TRUE のとき、現タスクを ready タスクキューに保存する
 *				FALSE のとき、現タスクを ready タスクキューに保存しない
 *
 * 返値：	エラー番号
 *
 * 処理：	ready_task の中で、一番優先順位の高いタスクをカレント
 *		タスクにする。
 *		実際のタスク切り換えは、resume () によっておこなう
 *		そのため、この関数の中での処理は、run_tsk 変数と
 *		ready_task[] の更新を行うのが主となる。
 *		
 */
ER
task_switch (BOOL save_nowtask)
{
  T_TCB	*tcb;
  ID	tskid;
  T_TCB *old;		/* */

#ifdef TSKSW_DEBUG
  printf ("task_switch(): start\n");
#endif

  if (on_interrupt)
    {
      delayed_dispatch = TRUE;
      return (E_OK);
    }
  dis_int ();
  if (dispatch_flag == FALSE)
    {
      ena_int ();
      return (E_CTX);
    }

  for (tskid = MIN_PRIORITY; tskid <= MAX_PRIORITY; tskid++)
    {
      if (ready_task[tskid] != NULL)
	break;
    }
  if (tskid > MAX_PRIORITY)
    {
#if defined (TSKSW_DEBUG)
      printf ("task_switch(): error = E_NOEXS\n");	/* */
#endif
      ena_int ();
      return (E_NOEXS);
    }

  if (save_nowtask)	/* 現タスクを ready タスクキューに保存する */
    {
      run_task->tskstat = TTS_RDY;
      ready_task[run_task->tsklevel] = add_tcb_list (ready_task[run_task->tsklevel],
						     run_task);
    }

#ifdef notdef
  /* 選択したタスクが、現タスクならば、何もしないで戻る */
  if (run_task == ready_task[tskid])
    {
      ready_task[tskid] = del_tcb_list (ready_task[tskid], run_task);
      ena_int ();
      return (E_OK);
    }
#endif /* notdef */

  /* 選択したタスクをready_task キューからはずす */
  tcb = ready_task[tskid];
  if (tcb->tskstat != TTS_RDY)
    {
      ena_int ();
      return (E_SYS);
    }
  old = run_task;	/* */
  run_task = tcb;
  run_task->tskstat = TTS_RUN;
  ready_task[run_task->tsklevel] = del_tcb_list (ready_task[run_task->tsklevel], run_task);

#ifdef TSKSW_DEBUG
  printf ("task_switch(): new task (ID = %d)\n", tcb->tskid);
#endif
  if (run_task->context.eip == 0)
    {
      printf ("ERROR!!!! context data is invalid.\n");
      printf ("OLD TASK ID = %d\n", old->tskid);
      printf ("NEW TASK ID = %d\n", run_task->tskid);
      falldown ("SYSTEM DOWN.\n");
    }
/* resume を呼び出す。resume の引数は、TSS へのセレクタ */
#ifdef TSKSW_DEBUG
  printf ("resume (0x%x)\n", ((tcb->tskid + TSS_BASE) << 3) & 0xfff8);
#endif
  delayed_dispatch = FALSE;
  resume ((UW)(tcb->tskid + TSS_BASE) << 3);
/*  print_context (((tcb->tskid + TSS_BASE) << 3) & 0xfff8); */
  ena_int ();
  return (E_OK);	/* 正常に終了した：次のタスクスイッチの時にここに戻る */
}


/* task_switch2 --- タスク切り換え
 *
 * 引数：	save_nowtask	TRUE のとき、現タスクを ready タスクキューに保存する
 *				FALSE のとき、現タスクを ready タスクキューに保存しない
 *
 * 返値：	エラー番号
 *
 * 処理：	ready_task の中で、一番優先順位の高いタスクをカレント
 *		タスクにする。
 *		実際のタスク切り換えは、resume () によっておこなう
 *		そのため、この関数の中での処理は、run_tsk 変数と
 *		ready_task[] の更新を行うのが主となる。
 *		
 */
ER
task_switch2 (BOOL save_nowtask)
{
  T_TCB	*tcb;
  ID	tskid;

#ifdef TSKSW_DEBUG
  printf ("task_switch(): start\n");
#endif

  if (on_interrupt)
    {
      delayed_dispatch = TRUE;
      return (E_OK);
    }
  dis_int ();
  if (dispatch_flag == FALSE)
    {
      ena_int ();
      return (E_CTX);
    }
  ena_int ();

  if (save_nowtask)	/* 現タスクを ready タスクキューに保存する */
    {
      run_task->tskstat = TTS_RDY;
      ready_task[run_task->tsklevel] = add_tcb_list (ready_task[run_task->tsklevel],
						     run_task);
    }

  for (tskid = MIN_PRIORITY; tskid <= MAX_PRIORITY; tskid++)
    {
      if (ready_task[tskid] != NULL)
	break;
    }
  if (tskid > MAX_PRIORITY)
    {
#if defined (TSKSW_DEBUG)
      printf ("task_switch(): error = E_NOEXS\n");	/* */
#endif
      ena_int ();
      return (E_NOEXS);
    }

  /* 選択したタスクが、現タスクならば、何もしないで戻る */
  if (run_task == ready_task[tskid])
    {
/*      printf ("task is non new.\n");	/* */
      ready_task[tskid] = del_tcb_list (ready_task[tskid], run_task);
      ena_int ();
      return (E_OK);
    }

  printf ("new task is %d\n", tskid);	/* */

  /* 選択したタスクをready_task キューからはずす */
  tcb = ready_task[tskid];
  if (tcb->tskstat != TTS_RDY)
    {
      ena_int ();
      return (E_SYS);
    }
  run_task = tcb;
  run_task->tskstat = TTS_RUN;
  ready_task[run_task->tsklevel] = del_tcb_list (ready_task[run_task->tsklevel], run_task);

/* #ifdef TSKSW_DEBUG */
  printf ("task_switch(): new task (ID = %d)\n", tcb->tskid);
/* #endif */
/* resume を呼び出す。resume の引数は、TSS へのセレクタ */
#ifdef TSKSW_DEBUG
  printf ("resume (0x%x)\n", ((tcb->tskid + TSS_BASE) << 3) & 0xfff8);
#endif
  delayed_dispatch = FALSE;
  ena_int ();
  resume ((UW)(tcb->tskid + TSS_BASE) << 3);
/*  print_context (((tcb->tskid + TSS_BASE) << 3) & 0xfff8); */
  return (E_OK);	/* 正常に終了した：次のタスクスイッチの時にここに戻る */
}


print_context (UW selector)
{
  TASK_DESC		*desc;
  T_I386_CONTEXT	*tcb;

  desc = (TASK_DESC *)get_gdt ((selector >> 3) - TSS_BASE & 0xffff);
  tcb = (T_I386_CONTEXT *)GET_TSS_ADDR (*desc);
#ifdef TSKSW_DEBUG
  printf ("task ID = %d\n", (selector >> 3) - TSS_BASE);
  printf ("TSS addr = 0x%x\n", tcb);
  printf ("EIP = 0x%x\n", tcb->eip);
#endif
}


/*****************************************************************************
 *
 *		S Y S T E M   C A L L  
 *				  for  T A S K.
 *
 *
 */

/* cre_tsk --- create task.
 *
 * タスクを1つ生成する。
 * 生成したタスクは、TTS_DMT 状態となり、sta_tsk()を実行するまでは
 * 実際に動くことはない。
 *
 *
 * 引数: tskid 		生成するタスクのID
 *	 pk_ctsk	生成するタスクの属性情報
 *			tskatr		タスク属性
 *			startaddr	タスク起動アドレス
 *			itskpri		タスク起動時優先度
 *			stksz		スタックサイズ
 *			addrmap		アドレスマップ
 *
 * 返り値：	エラー番号
 *		E_OK	正常終了
 *		E_ID	タスクの ID 番号が不正
 *		E_OBJ	同一のタスクが存在している
 *
 */
ER
cre_tsk (ID tskid, T_CTSK *pk_ctsk)
{
  T_TCB	*newtask;
  W	i;

/* タスク ID の範囲チェック */
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
/* 同一 ID のタスクが存在しているかどうかのチェック */
  if (task[tskid].tskstat != TTS_NON)
    {
      return (E_OBJ);
    }

  newtask = &task[tskid];
  bzero (newtask, sizeof (*newtask));
/* タスク生成 */
  
  newtask->tskid     = tskid;
  newtask->tskstat   = TTS_DMT;
  newtask->tsklevel  = pk_ctsk->itskpri;
  newtask->tsklevel0 = pk_ctsk->itskpri;
  if (make_task_context (newtask, pk_ctsk) != E_OK)
    {
      return (E_NOMEM);
    }

  /* 仮想メモリのマッピングテーブルを引数 pk_ctsk の指定したマップに
   * 変更する。
   * 指定がないときには、カレントプロセスと同じマップとなる。
   */
  if (pk_ctsk->addrmap != NULL)
    {
      newtask->context.cr3 = (UW)(pk_ctsk->addrmap);
    }
  else
    {
      newtask->context.cr3 = (UW)dup_vmap_table (run_task->context.cr3) & 0x7fffffff;
/*
      newtask->context.cr3 = run_task->context.cr3;
*/
    }

  /* タスクのリージョンテーブルを初期化
   */
  for (i = 0; i < MAX_REGION; i++)
    {
      newtask->regions[i].permission = 0;
    }
  newtask->regions[0].start_addr = (VP)0x80000000;
  newtask->regions[0].min_size   = 0x7fffffff;
  newtask->regions[0].max_size   = 0x7fffffff;
  newtask->regions[0].permission = VPROT_READ | VPROT_WRITE | VPROT_KERNEL;

  return (E_OK);
}

ER
del_tsk (ID tskid)
{
}

/* sta_tsk --- タスクの起動
 * 
 * 引数tskidで指定したタスクを起動する。
 * 指定したタスクは、cre_tsk で生成されている必要がある。
 *
 * 引数：
 *	tskid	起動するタスクの ID
 *	stacd	タスク起動コード
 *
 * 戻り値：
 *	E_OK	正常終了
 *
 */
ER
sta_tsk (ID tskid, INT stacd)
{
  register int	index;

#ifdef TSKSW_DEBUG
  printf ("sta_tsk: start\n");
#endif
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
  if (task[tskid].tskstat != TTS_DMT)
    {
      return (E_NOEXS);
    }

  index = task[tskid].tsklevel;
  task[tskid].tskstat = TTS_RDY;
  task[tskid].wakeup_count = 0;
  task[tskid].suspend_count = 0;
  task[tskid].total = 0;
  ready_task[index] = add_tcb_list (ready_task[index], &task[tskid]);
#ifdef TSKSW_DEBUG
  printf ("sta_tsk: task level = %d\n", index);
#endif
  return (E_OK);
}

/******************************************************************************
 * ext_tsk --- 自タスク終了
 *
 * run_task につながれているタスクを TTS_DMT 状態へ移動する。
 * メモリ資源などは返却しない。
 * 
 * 引数：	なし
 *
 * 返り値：	なし
 *
 * ！		このシステムコールを実行したあとは、もとのコンテキスト
 *		には戻らない。
 *
 * 処理内容：
 *	この関数では、ready タスクキューにつながれているタスクのうち、
 *	最も優先度の高いものを選択し、次に実行するタスクとする。
 *	(選択するタスクがないということはない。--- 必ず走行している 
 *	idle タスクがあるため)
 *	switch_task() では、今走行してるタスクをreadyタスクキューに入
 *	れる。しかし、ext_tsk() では、元のタスクは終了するため、ready
 *	タスクキューには入れず、状態をTTS_DMTにする。
 */
void
ext_tsk (void)
{
  T_TCB	*tcb;
  ID	tskid;

  /* 現在のタスクを TTS_DMT 状態にし、選択したタスクを次に走らせるよう */
  /* にする。							       */
  run_task->tskstat = TTS_DMT;
  task_switch (FALSE);
}

/******************************************************************************
 * exd_tsk --- 自タスク終了と削除
 *
 * run_task につながれているタスクを TTS_NON 状態へ移動する。
 * メモリ資源などは返却しないが、マッピングされたメモリについては、解
 * 放する。
 * 
 * 引数：	なし
 *
 * 返り値：	なし
 *
 * ！		このシステムコールを実行したあとは、もとのコンテキスト
 *		には戻らない。
 *
 * 処理内容：
 *	この関数では、ready タスクキューにつながれているタスクのうち、
 *	最も優先度の高いものを選択し、次に実行するタスクとする。
 *	(選択するタスクがないということはない。--- 必ず走行している 
 *	idle タスクがあるため)
 *	switch_task() では、今走行してるタスクをreadyタスクキューに入
 *	れる。しかし、ext_tsk() では、元のタスクは終了するため、ready
 *	タスクキューには入れず、状態をTTS_DMTにする。
 */
void
exd_tsk (void)
{
  T_TCB	*tcb;
  ID	tskid;

  /* 現在のタスクを TTS_NON 状態にし、選択したタスクを次に走らせるようにする。*/
  run_task->tskstat = TTS_NON;
  release_vmap ((ADDR_MAP)run_task->context.cr3);	/* マッピングテーブルを解放する */
  task_switch (FALSE);
}

/*************************************************************************
 * ter_tsk --- 他タスク強制終了
 *
 * 機能：
 *	引数で指定したタスクを強制的に終了させる。
 *	終了するタスクのもっている資源は解放しない。
 *	ただし、このシステムコールによって終了したタスクは、TTS_DMT 状
 *	態になっただけなので、sta_tsk システムコールによって再開する
 *	ことができる。
 */
ER
ter_tsk (ID tskid)
{
  switch (task[tskid].tskstat)
    {
    default:
    case TTS_RUN:	/* 自タスクの場合 */
      return (E_OBJ);
      /* NOT REACHED */
   
      /* ready 状態にあるタスクの場合：強制終了させる */
    case TTS_RDY:
      task[tskid].tskstat = TTS_DMT;
      break;

      /* 待ち状態にあるタスクの場合：待ち状態から解放してから強制終了させる。*/
    case TTS_WAI:
      /* relase_wait();		/* まだインプリメントしていない。*/
      task[tskid].tskstat = TTS_DMT;
      break;
    }
  return (E_OK);
}

ER
dis_dsp ()
{
  dispatch_flag = FALSE;
  return (E_OK);
}

ER
ena_dsp ()
{
  dispatch_flag = TRUE;
  return (E_OK);
}

/* chg_pri --- プライオリティの変更
 *
 */
ER
chg_pri (ID tskid, PRI tskpri)
{
  T_TCB	*p;

  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
  switch (task[tskid].tskstat)
    {
    default:
      return (E_OBJ);

    case TTS_NON:
      return (E_NOEXS);

    case TTS_RDY:
      ready_task[task[tskid].tsklevel] = del_tcb_list (ready_task[task[tskid].tsklevel], &task[tskid]);
      task[tskid].tsklevel = tskpri;
      ready_task[task[tskid].tsklevel] = add_tcb_list (ready_task[task[tskid].tsklevel], &task[tskid]);
      break;

    case TTS_RUN:
      task[tskid].tsklevel = tskpri;
      break;
    }
  return (E_OK);
}

/* rot_rdq --- 同一プライオリティでのタスクの順序を変更する
 *
 */
ER
rot_rdq (PRI tskpri)
{
  T_TCB	*first;

  if ((tskpri < MIN_PRIORITY) || (tskpri > MAX_PRIORITY))
    {
      return (E_PAR);
    }
  first = ready_task[tskpri];
  ready_task[tskpri] = del_tcb_list (ready_task[tskpri], first);
  ready_task[tskpri] = add_tcb_list (ready_task[tskpri], first);
  return (E_OK);
}

ER
rel_wai (ID tskid)
{
}

/***********************************************************************************
 * get_tid --- 自タスクのタスク ID 参照
 *
 *
 */
ER
get_tid (ID *p_tskid)
{
  *p_tskid = run_task->tskid;
  return (E_OK);
}

/***********************************************************************************
 * ref_tsk --- タスク状態の参照
 *
 * タスク状態を返す。
 *
 */
ER
ref_tsk (T_RTSK *pk_rtsk, ID tskid)
{
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
  switch (task[tskid].tskstat)
    {
    default:
      pk_rtsk->tskpri = task[tskid].tsklevel;
      pk_rtsk->tskstat = task[tskid].tskstat;
      pk_rtsk->total_time = task[tskid].total;
      break;

    case TTS_NON:
      return (E_NOEXS);
    }
  return (E_OK);
}

/*********************************************************************************
 * slp_tsk --- 自タスクを待ち状態にする
 *
 *	自分自身を待ち状態にして、他のタスクに制御を渡す。
 *	待ち要因は、この関数ではセットしない。
 *
 */
ER
slp_tsk (VOID)
{
/*  printf ("slp_tsk: start\n");	/* */
  dis_int ();
  if (run_task->wakeup_count > 0)
    {
      run_task->wakeup_count--;
/*      printf ("sleep task: wakeup count = %d\n", run_task->wakeup_count--); */
      ena_int ();
      return (E_OK);
    }
  
  run_task->slp_err = E_OK;
  run_task->tskstat = TTS_WAI;
  ena_int ();
  task_switch (FALSE);	/* run_task を ready_task キューに保存しない */
/*  task_switch2 (FALSE);	/* run_task を ready_task キューに保存しない */
  return (run_task->slp_err);
}

/*********************************************************************************
 * wup_tsk --- 指定されたタスクを起床する。
 *
 * 機能：
 * 	待ち状態フラグ(tskwai)は、この関数を呼び出す時にリセットしていなければ
 *	いけない。
 * 	もし待ち状態フラグがセットされていたならば、E_OBJ のエラーとなる。
 * 
 */
ER
wup_tsk (ID taskid)
{
  T_TCB	*p;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  p = &(task[taskid]);
  if ((p == run_task) || (p->tskstat == TTS_DMT))
    {
      ena_int ();
      return (E_OBJ);
    }

  /* すべての待ち状態が解除されていなければ、先には進まない */
  if ((p->tskwait.time_wait) || (p->tskwait.semaph_wait) || (p->tskwait.event_wait) || (p->tskwait.msg_wait))
    {
      printf ("task %d is waiting. abort wakeup.\n", p->tskid);
      printf ("(p->tskwait.time_wait) = %d\n", (p->tskwait.time_wait));
      printf ("(p->tskwait.semaph_wait) = %d\n", (p->tskwait.semaph_wait));
      printf ("(p->tskwait.event_wait) = %d\n", (p->tskwait.event_wait));
      printf ("(p->tskwait.msg_wait) = %d\n", (p->tskwait.msg_wait));
      ena_int ();
      return (E_OBJ);
    }

  if (p->tskstat == TTS_WAS)
    {
      p->tskstat = TTS_SUS;
    }
  else if (p->tskstat == TTS_WAI)
    {
      p->tskstat = TTS_RDY;
      if ((p < 0x7fffffff) || (UW)(p >= 0x81000000))
	{
	  printf ("wup_tsk: error on tasklist\n");
	  print_task_list ();
	  falldown ("kernel: task.\n");
	}
      else
	{
	  ready_task[p->tsklevel] = ins_tcb_list (ready_task[p->tsklevel], p);
	}
    }
  else if (p->tskstat == TTS_SUS)
    {
      p->wakeup_count++;
    }

  ena_int ();
  return (E_OK);
}

/*****************************************************************************
 * sus_tsk --- 指定したタスクを強制待ち状態に移行
 *
 * 引数：
 *	taskid --- suspend するタスクの ID
 *
 * 返り値：
 *
 *
 * 機能：
 *
 */
ER
sus_tsk (ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  if (&task[taskid] == run_task)
    {
      return (E_OBJ);
    }

  dis_int ();
  taskp = &task[taskid];
  taskp->suspend_count++;
  switch (taskp->tskstat)
    {
    case TTS_RDY:
      ready_task[taskp->tsklevel] = del_tcb_list (ready_task[taskp->tsklevel], taskp);
      taskp->tskstat = TTS_SUS;
      break;

    case TTS_SUS:
      if (taskp->suspend_count > MAX_SUSPEND_NEST)
	{
	  taskp->suspend_count = MAX_SUSPEND_NEST;
	  ena_int ();
	  return (E_QOVR);
	}
      break;

    case TTS_WAI:
      taskp->tskstat = TTS_WAS;
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);
      /* DO NOT REACHED */

    default:
      ena_int ();
      return (E_OBJ);
      /* DO NOT REACHED */
    }
  ena_int ();
  return (E_OK);
}

/******************************************************************************************
 * rsm_tsk --- 強制待ち状態のタスクから待ち状態を解除
 *
 * 引数：
 *	taskid --- suspend しているタスクの ID
 *
 *
 * 返り値：
 *	次のエラー番号が返る
 *	
 * E_OK     システムコールは正常に終了した
 * E_ID     タスク ID が不正
 * E_NOEXS  タスクが存在しない(TTS_NON 状態)
 * E_OBJ    タスクの状態が不正(TTS_SUS, TTS_WAS, TTS_NON 以外)
 *
 *
 * 機能：
 *	待ち状態にあるタスクを待ち状態から強制的に解除する。
 *
 *	待ち状態は多重になることがあるが、このシステムコールは、ひとつだけ待ち
 *	を解除する。
 */
ER
rsm_tsk (ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  taskp = &task[taskid];
  switch (taskp->tskstat)
    {
    case TTS_SUS:
      taskp->suspend_count--;
      if (taskp->suspend_count <= 0)
	{
	  taskp->tskstat = TTS_RDY;
	  ready_task[taskp->tsklevel] = ins_tcb_list (ready_task[taskp->tsklevel], taskp);
	}
      break;

    case TTS_WAS:
      taskp->suspend_count--;
      if (taskp->suspend_count <= 0)
	{
	  taskp->tskstat = TTS_WAI;
	}
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);

    default:
      ena_int ();
      return (E_OBJ);
    }
  ena_int ();
  return (E_OK);
}

/******************************************************************************
 * frsm_tsk --- 強制待ち状態のタスクから待ち状態を解除(多重の待ち状態用)
 *
 * 引数：
 *	taskid --- suspend するタスクの ID
 *
 * 返り値：
 *	エラー番号
 *
 * 機能：
 *
 */
ER
frsm_tsk (ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  taskp = &task[taskid];
  switch (taskp->tskstat)
    {
    case TTS_SUS:
      taskp->tskstat = TTS_RDY;
      ready_task[taskp->tsklevel] = ins_tcb_list (ready_task[taskp->tsklevel], taskp);
      break;

    case TTS_WAS:
      taskp->tskstat = TTS_WAI;
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);

    default:
      ena_int ();
      return (E_OBJ);
    }
  ena_int ();
  return (E_OK);
}

/******************************************************************************************
 * can_wup --- タスクの起床要求を無効化
 *
 * 引数：
 *	taskid --- タスクの ID
 *
 * 返り値：
 *	エラー番号
 *
 * 機能：
 *
 */
ER
can_wup (INT *p_wupcnt, ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  taskp = &task[taskid];
  switch (taskp->tskstat)
    {
    case TTS_WAS:
    case TTS_WAI:
      *p_wupcnt = taskp->wakeup_count;
      taskp->wakeup_count = 0;
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);
      /* DO NOT REACHED */

    default:
      ena_int ();
      return (E_OBJ);
      }
  ena_int ();
  return (E_OK);
}


/***********************************************************************
 * new_task --- 任意のタスク ID でのタスク生成
 *
 * 引数：
 *	pk_ctsk	生成するタスクの属性情報
 *		tskatr		タスク属性
 *		startaddr	タスク起動アドレス (run_flag == TRUE のとき)
 *		itskpri		タスク起動時優先度
 *		stksz		スタックサイズ
 *		addrmap		アドレスマップ
 *	rid	生成したタスクの ID (返り値)
 *	run_flag  生成したタスクを実行する
 *
 * 返り値：
 *	エラー番号
 *	E_OK	正常終了
 *
 * 機能：
 *	new_task は、新しいタスクを作成するという cre_tsk とほとんど
 *	同じ機能をもつ。ただし、cre_tsk がタスク ID を必要とするのに対
 *	し、new_task は、タスク ID を自動的に割りあてる。
 *
 */
ER
new_task (T_CTSK *pk_ctsk, ID *rid, BOOL run_flag)
{
  ID	i;
  ER	err;

  for (i = MIN_USERTASKID; i <= MAX_USERTASKID; i++)
    {
      err = cre_tsk (i, pk_ctsk);
      if (err == E_OK)
	{
	  *rid = i;
	  if (run_flag == TRUE)
	    sta_tsk (i, 0);
	  return (E_OK);
	}
    }
  return (E_NOMEM);
}


T_TCB *
get_tskp (ID tskid)
{
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return ((T_TCB *)NULL);
    }
  return (&task[tskid]);
}

