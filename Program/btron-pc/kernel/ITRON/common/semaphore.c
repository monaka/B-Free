/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* semaphore.c --- セマフォ関連
 *
 */


#include "itron.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "ibmpc.h"
#include "semaphore.h"

static T_SEMAPHORE	semaphore_table[MAX_SEMAID + 1];

static void		twaisem_timer (VP arg);
static void del_sem_wait(ID sid, ID tid);

/********************************************************************************
 * init_semaphore --- セマフォの初期化
 *
 */
ER
init_semaphore (void)
{
  /* 現在のところ何もしない */
  return (E_OK);
}

/********************************************************************************
 * cre_sem --- セマフォ生成
 *
 * 引数：
 *	semid
 *	pkcsem
 *
 * 返り値：
 *	ER
 *
 * 機能：
 *
 */
ER
cre_sem (ID semid, T_CSEM *pkcsem)
{
  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem > 0)
    {
      return (E_OBJ);
    }
  
  if ((pkcsem->maxsem <= 0) || (pkcsem->isemcnt < 0))
    {
      return (E_PAR);
    }

  if (pkcsem->sematr != TA_TFIFO)
    {
      return (E_PAR);
    }

  semaphore_table[semid].sematr = pkcsem->sematr;
  semaphore_table[semid].isemcnt = pkcsem->isemcnt;
  semaphore_table[semid].maxsem = pkcsem->maxsem;
  semaphore_table[semid].exinf = pkcsem->exinf;
  return (E_OK);
}

/********************************************************************************
 * del_sem --- セマフォ削除
 *
 * 引数：
 *	semid
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	引数で指定したセマフォを削除する。
 */
ER
del_sem (ID semid)
{
  T_TCB	*p;
#if 0
  T_TCB *next;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }
  
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  p = semaphore_table[semid].waitlist;
  while (p != NULL)
    {
      /* セマフォを待っているタスクを解放する */
      /* エラー番号 E_DLT が返る。	      */
      p->slp_err = E_DLT;
      /* タスクを待ち状態から、ready 状態へ移動する  */
      /* ただし、タスクディスパッチはおこらない。    */
      p->tskwait.semaph_wait = 0;
      wup_tsk (p->tskid);
      p = p->sem_next;
    }
  semaphore_table[semid].waitlist = NULL;
  semaphore_table[semid].waittail = NULL;
  semaphore_table[semid].isemcnt = 0;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  return (E_OK);
}

/********************************************************************************
 * sig_sem --- セマフォ資源返却
 *
 * 引数：
 *	semid
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	この関数は、いわゆるセマフォの V 動作を行う。
 */
ER
sig_sem (ID semid)
{
  T_TCB	*p;
#if 0
  T_TCB *next;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  p = semaphore_table[semid].waitlist;
  if (p != NULL)
    {
      printk ("sig_sem: wakeup task = %d\n", p->tskid);	/* */
#ifdef notdef
      semaphore_table[semid].waitlist = del_tcb_list (semaphore_table[semid].waitlist, p);
#else
      semaphore_table[semid].waitlist = p->sem_next;
      if (p->sem_next == NULL) {
	semaphore_table[semid].waittail = NULL;
      }
      p->sem_next = NULL;
#endif
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      p->tskwait.semaph_wait = 0;
      wup_tsk (p->tskid);
    }
  else
    {
      /* セマフォを待っているタスクがない */
      semaphore_table[semid].isemcnt++;
    }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  return (E_OK);
}

/********************************************************************************
 * wai_sem --- セマフォ資源獲得
 *
 * 引数：
 *	semid	獲得するセマフォの ID 番号
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	この関数は、いわゆるセマフォの P 動作を行う。
 */
ER
wai_sem (ID semid)
{
  INT wcnt;
#if 0
  T_TCB	*p;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* 生成されていないセマフォだった */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  /* すでにセマフォを獲得しているタスクがある。 */
  /* 待ち行列に自分自身をつなぎ、CPU を解放する */
  if (semaphore_table[semid].isemcnt == 0)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
#ifdef notdef
      printk ("wai_sem: sleep task = %d\n", run_task->tskid);	/* */
#endif
      run_task->sem_id = semid;
#ifdef notdef
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
#else
      run_task->sem_next = NULL;
      if (semaphore_table[semid].waittail != NULL) {
	semaphore_table[semid].waittail->sem_next = run_task;
      }
      semaphore_table[semid].waittail = run_task;
      if (semaphore_table[semid].waitlist == NULL) {
	semaphore_table[semid].waitlist = run_task;
      }
#endif
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      run_task->tskwait.semaph_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();	/* 待ち状態へ移動 */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      semaphore_table[semid].isemcnt--;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
    }
  run_task->sem_id = 0;
  return (E_OK);
}

/********************************************************************************
 * preq_sem --- セマフォ資源獲得 (ポーリング)
 *
 * 引数：
 *	semid	獲得するセマフォの ID 番号
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	この関数は、いわゆるセマフォの P 動作を行う。
 */
ER
preq_sem (ID semid)
{
#if 0
  T_TCB	*p;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* 生成されていないセマフォだった */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  /* すでにセマフォを獲得しているタスクがある。 */
  if (semaphore_table[semid].isemcnt >= 1)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      semaphore_table[semid].isemcnt--;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
    }
  else if (semaphore_table[semid].isemcnt == 0)
    {
      return (E_TMOUT);	
    }
  return (E_OK);
}

/********************************************************************************
 * twai_sem --- セマフォ資源獲得 (タイムアウト有)
 *
 * 引数：
 *	semid	獲得するセマフォの ID 番号
 *	tmout	タイムアウト時間
 *
 * 返り値：
 *	ER	エラー番号
 *
 * 機能：
 *	この関数は、いわゆるセマフォの P 動作を行う。
 */
ER
twai_sem (ID semid, TMO tmout)
{
  INT wcnt;
#if 0
  T_TCB	*p;
#endif

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* 生成されていないセマフォだった */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  /* すでにセマフォを獲得しているタスクがある。 */
  /* 待ち行列に自分自身をつなぎ、CPU を解放する */
  if (semaphore_table[semid].isemcnt == 0)
    {	
      set_timer (tmout, (void (*)(VP))twaisem_timer, run_task);	/* タイマーをセット */
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
#ifdef notdef
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
#else
      run_task->sem_next = NULL;
      if (semaphore_table[semid].waittail != NULL) {
	semaphore_table[semid].waittail->sem_next = run_task;
      }
      semaphore_table[semid].waittail = run_task;
      if (semaphore_table[semid].waitlist == NULL) {
	semaphore_table[semid].waitlist = run_task;
      }
#endif
      run_task->sem_id = semid;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
      run_task->tskwait.semaph_wait = 1;
      can_wup(&wcnt, run_task->tskid);
      slp_tsk ();	/* 待ち状態へ移動 */
      unset_timer ((void (*)(VP))twaisem_timer, (VP)(run_task->tskid));	/* タイマーをアンセット */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
#ifdef CALL_HANDLER_IN_TASK
      dis_dsp();
#else
      dis_int();
#endif
      semaphore_table[semid].isemcnt--;
#ifdef CALL_HANDLER_IN_TASK
      ena_dsp();
#else
      ena_int();
#endif
    }
  run_task->sem_id = 0;
  if (run_task->slp_err) {
    if (run_task->slp_err == E_TMOUT) {
      del_sem_wait(semid, run_task->tskid);
    }
    return (run_task->slp_err);
  }
  return (E_OK);
}

/******************************************************************************
 * twaisem_timer -- twai_sem 関数の時間切れのときに呼びだされる関数。
 *
 * この関数は、割り込みの延長上で動く。
 *
 */
static void
twaisem_timer (VP arg)
{
  T_TCB	*p;
  ID semid;

  dis_int();
  p = (T_TCB *)arg;
  p->slp_err = E_TMOUT;
#ifdef notdef
  semaphore_table[p->sem_id].waitlist = del_tcb_list (semaphore_table[p->sem_id].waitlist,
						      p);
#endif
#if 0
  /* この部分の処理は怪しい。*/
  semid = p->sem_id;
  p = semaphore_table[semid].waitlist;
  semaphore_table[semid].waitlist = p->sem_next;
  if (p->sem_next == NULL) {
    semaphore_table[semid].waittail = NULL;
  }
  p->sem_next = NULL;
#endif
  ena_int();
  p->tskwait.msg_wait = 0;
  wup_tsk (p->tskid);
}

/********************************************************************************
 * ref_sem --- セマフォ状態参照
 *
 */
ER
ref_sem (T_RSEM *pk_rsem, ID semid)
{
  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  /* 生成されていないセマフォだった */
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  pk_rsem->exinf = semaphore_table[semid].exinf;
  pk_rsem->wtsk  = semaphore_table[semid].waitlist ? TRUE : FALSE;
  pk_rsem->semcnt  = semaphore_table[semid].isemcnt;
  return (E_OK);
}



/* del_task_sem -- セマフォからタスクを削除
 *
 *
 */

static void del_sem_wait(ID sid, ID tid)
{
  T_TCB *p, *q;

  if (semaphore_table[sid].maxsem <= 0) return;
#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  q = NULL;
  for (p = semaphore_table[sid].waitlist; p != NULL; p = p->sem_next) {
    if (p->tskid == tid) {
      if (p->sem_next == NULL) {
	semaphore_table[sid].waittail = q;
      }
      if (q == NULL) {
	semaphore_table[sid].waitlist = p->sem_next;
      }
      else {
	q->sem_next = p->sem_next;
      }
    }
    else {
      q = p;
    }
  }
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
}

void del_task_sem(ID tid)
{
  ID sid;

  for(sid = MIN_SEMAID; sid <= MAX_SEMAID; ++sid) {
    del_sem_wait(sid, tid);
  }
}
