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
#include "pc98.h"
#include "semaphore.h"

static T_SEMAPHORE	semaphore_table[NSEMAPHORE];

static void		twaisem_timer (VP arg);

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
  T_TCB	*p, *next;

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }
  
  dis_dsp ();
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
  semaphore_table[semid].isemcnt = 0;
  ena_dsp ();
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
  T_TCB	*p, *next;

  if ((semid < MIN_SEMAID) || (semid > MAX_SEMAID))
    {
      return (E_ID);
    }
  
  if (semaphore_table[semid].maxsem <= 0)
    {
      return (E_OBJ);
    }

  dis_int ();
  p = semaphore_table[semid].waitlist;
  if (p != NULL)
    {
      printf ("sig_sem: wakeup task = %d\n", p->tskid);	/* */
      semaphore_table[semid].waitlist = del_tcb_list (semaphore_table[semid].waitlist, p);
      ena_int ();
      p->tskwait.semaph_wait = 0;
      wup_tsk (p->tskid);
    }
  else
    {
      /* セマフォを待っているタスクがない */
      semaphore_table[semid].isemcnt++;
    }
  ena_int ();
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
  T_TCB	*p;

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
      dis_int ();
      printf ("wai_sem: sleep task = %d\n", run_task->tskid);	/* */
      run_task->sem_id = semid;
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
      ena_int ();
      run_task->tskwait.semaph_wait = 1;
      slp_tsk ();	/* 待ち状態へ移動 */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
      dis_int ();
      semaphore_table[semid].isemcnt--;
      ena_int ();
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
  T_TCB	*p;

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
      dis_int ();
      semaphore_table[semid].isemcnt--;
      ena_int ();
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
  T_TCB	*p;

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
      set_timer (tmout, twaisem_timer, run_task);	/* タイマーをセット */
      dis_int ();
      semaphore_table[semid].waitlist = add_tcb_list (semaphore_table[semid].waitlist, run_task);
      run_task->sem_id = semid;
      ena_int ();
      run_task->tskwait.semaph_wait = 1;
      slp_tsk ();	/* 待ち状態へ移動 */
      unset_timer (twaisem_timer, (VP)(run_task->tskid));	/* タイマーをアンセット */
    }
  else if (semaphore_table[semid].isemcnt >= 1)
    {
      dis_int ();
      semaphore_table[semid].isemcnt--;
      ena_int ();
    }
  run_task->sem_id = 0;
  if (run_task->slp_err)
    return (run_task->slp_err);
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

  dis_int ();
  p = (T_TCB *)arg;
  p->slp_err = E_TMOUT;
  semaphore_table[p->sem_id].waitlist = del_tcb_list (semaphore_table[p->sem_id].waitlist,
						      p);
  ena_int ();
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



