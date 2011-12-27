/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* event.c --- イベントフラグ機能
 *
 *
 */


#include "itron.h"
#include "config.h"
#include "memory.h"
#include "errno.h"
#include "func.h"
#include "pc98.h"
#include "eventflag.h"

static T_EVENTFLAG	flag_table[NEVENTFLAG];

static void	tflg_func (VP p);

/*******************************************************************************
 * init_eventflag
 *
 * 引数：
 *
 * 返り値：
 *
 * 機能：
 *
 */
ER
init_eventflag (void)
{
  W	i;

  for (i = MIN_EVENTFLAG; i <= MAX_EVENTFLAG; i++)
    {
      flag_table[i].flgatr = TA_FREE;
      flag_table[i].id = i;
    }
  return (E_OK);
}

/*******************************************************************************
 * cre_flg --- イベントフラグの生成
 *
 * 引数：
 *
 * 返り値：
 *
 * 機能：
 *
 */
ER
cre_flg (ID flgid, T_CFLG *pk_flg)
{
  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr != TA_FREE)
    {
      return (E_OBJ);
    }

  if ((pk_flg->flgatr != TA_WSGL) && (pk_flg->flgatr != TA_WMUL))
    {
      return (E_PAR);
    }

  flag_table[flgid].flgatr = pk_flg->flgatr;
  flag_table[flgid].iflgptn = pk_flg->iflgptn;
  flag_table[flgid].wait_task = NULL;
  flag_table[flgid].exinf = pk_flg->exinf;
  return (E_OK);
}


/*******************************************************************************
 * del_flg --- イベントフラグの削除
 *
 * 引数：
 *
 * 返り値：
 *	エラー番号を返す。
 *	E_OK
 *	E_ID
 *	E_OBJ
 *
 * 機能：
 *
 */
ER
del_flg (ID flgid)
{
  T_TCB	*p;

  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_OBJ);
    }

  for (p = flag_table[flgid].wait_task; p != NULL; p = p->next)
    {
      p->tskwait.event_wait = 0;
      p->slp_err = E_TMOUT;
      wup_tsk (p->tskid);
    }

  flag_table[flgid].flgatr = TA_FREE;
  return (E_OK);
}

/*******************************************************************************
 * set_flg --- イベントフラグに値をセットする
 *
 * 引数：
 *
 * 返り値：
 *	エラー番号を返す。
 *	E_OK
 *	E_ID
 *	E_OBJ
 *
 * 機能：
 *
 */
ER
set_flg (ID flgid, UINT setptn)
{
  T_TCB	*p, *q;
  BOOL	result;
  BOOL	clear = FALSE;
  BOOL	tsw_flag = FALSE;
  W	wakeup_count = 0;

  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_NOEXS);
    }

/*  printf ("set_flag: count = %d\n", list_counter (flag_table[flgid].wait_task)); */
  dis_int ();
  flag_table[flgid].iflgptn |= setptn;

  /* set_flg() では、変化したフラグを待っていたタスクをすぐにフラグテーブル-リストから
     除去してしまう。
     そのため、除去するタスクがフラグテーブル-リストの先頭にあった場合、ループを再度や
     り直さないとうまくいかない(除去した時点でflag_table[flgid].wait_taskが正しくなくな
     ってしまうため)。*/
reloop:
  q = NULL;
  for (p = flag_table[flgid].wait_task;
       (q != flag_table[flgid].wait_task) && (flag_table[flgid].wait_task != NULL);
       p = q)
    {
      result = FALSE;
      switch ((p->wfmode) & (TWF_ANDW | TWF_ORW))
	{
	case TWF_ANDW:
	  result = ((flag_table[flgid].iflgptn & p->flag_pattern) == p->flag_pattern);
	  break;
	case TWF_ORW:
	  result = (flag_table[flgid].iflgptn & p->flag_pattern) ? TRUE : FALSE;
	  break;
	}

      wakeup_count++;	/* debug */

      q = p->next;
      if (result == TRUE)
	{
/*	  printf ("set_flag: count = %d - ", list_counter (flag_table[flgid].wait_task)); */
	  flag_table[flgid].wait_task = del_tcb_list (flag_table[flgid].wait_task, p);
/*	  printf ("%d(deleted)\n", list_counter (flag_table[flgid].wait_task)); */
	  p->rflgptn = flag_table[flgid].iflgptn;
	  p->tskwait.event_wait = 0;
	  wup_tsk (p->tskid);
	  if (p->wfmode & TWF_CLR)
	    {
	      clear = TRUE;
	    }
	  goto reloop;
	}
    }	

  ena_int ();
  if (clear)
    {
      flag_table[flgid].iflgptn = 0;
    }

/*  printf ("wakeup_count = %d, list is %s\n", 
	  wakeup_count, flag_table[flgid].wait_task ? "no NULL" : "null");
 */

  if (tsw_flag)
    {
      task_switch (TRUE);
    }

  return (E_OK);
}

/*******************************************************************************
 * clr_flg --- イベントフラグの値をクリアする
 *
 * 引数：
 *
 * 返り値：
 *	エラー番号を返す。
 *	E_OK
 *	E_ID
 *	E_OBJ
 *
 * 機能：
 *
 */
ER
clr_flg (ID flgid, UINT clrptn)
{
  T_TCB	*p, *q;
  BOOL	result;
  BOOL	clear = FALSE;
  BOOL	tsw_flag = FALSE;

  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_NOEXS);
    }

  flag_table[flgid].iflgptn &= clrptn;
reloop:
  q = NULL;
  for (p = flag_table[flgid].wait_task;
       (q != flag_table[flgid].wait_task) && (flag_table[flgid].wait_task != NULL);
       p = q)
    {
      result = FALSE;
      switch ((p->wfmode) & (TWF_ANDW | TWF_ORW))
	{
	case TWF_ANDW:
	  result = ((flag_table[flgid].iflgptn & p->flag_pattern) == p->flag_pattern);
	  break;
	case TWF_ORW:
	  result = (flag_table[flgid].iflgptn & p->flag_pattern);
	  break;
	}

      q = p->next;
      if (result == TRUE)
	{
/*	  printf ("clr_flag: %d - ", list_counter (flag_table[flgid].wait_task));	/* debug */
	  flag_table[flgid].wait_task = del_tcb_list (flag_table[flgid].wait_task, p);
/*	  printf ("%d\n", list_counter (flag_table[flgid].wait_task));	/* debug */
	  p->rflgptn = flag_table[flgid].iflgptn;
	  p->tskwait.event_wait = 0;
	  tsw_flag = TRUE;
	  wup_tsk (p->tskid);
	  if (p->wfmode & TWF_CLR)
	    {
	      clear = TRUE;
	    }
	  goto reloop;
	}
    }	
      
  if (clear)
    {
      flag_table[flgid].iflgptn = 0;
    }
  if (tsw_flag)
    {
      task_switch (TRUE);
    }
      
  return (E_OK);
}


/*******************************************************************************
 * wai_flg --- イベントフラグ待ち
 *
 * 引数：
 *
 * 返り値：
 *
 * 機能：
 *
 */
ER
wai_flg (UINT *flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
  T_TCB	*p;

  *flgptn = NULL;
  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_NOEXS);
    }

  if ((flag_table[flgid].flgatr == TA_WSGL) && (flag_table[flgid].wait_task))
    {
      return (E_OBJ);
    }

/* for debug */
/*  printf ("wai_flg: before = %d  ", list_counter (flag_table[flgid].wait_task));  /* debug */

  dis_int ();
  run_task->flag_pattern = waiptn;
  run_task->wfmode = wfmode;
  run_task->tskwait.event_wait = 1;
  run_task->event_id = flgid;
  run_task->next = NULL;
  flag_table[flgid].wait_task = ins_tcb_list (flag_table[flgid].wait_task, run_task);
  ena_int ();

/* for debug */
/*  printf ("wai_flg: after = %d\n", list_counter (flag_table[flgid].wait_task)); /* debug */

/*  task_switch (TRUE); */
  slp_tsk ();
  *flgptn = run_task->rflgptn;
  return (E_OK);
}


/*******************************************************************************
 * pol_flg --- イベントフラグ待ち(ポーリング付き)
 *
 * 引数：
 *
 * 返り値：
 *
 * 機能：
 *
 */
ER
pol_flg (UINT *flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
  T_TCB	*p;

  *flgptn = NULL;
  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_NOEXS);
    }

  if ((flag_table[flgid].flgatr == TA_WSGL) && (flag_table[flgid].wait_task))
    {
      return (E_OBJ);
    }

  switch ((wfmode) & (TWF_ANDW | TWF_ORW))
    {
    case TWF_ANDW:
      if (!((flag_table[flgid].iflgptn & waiptn) == waiptn))
	{
	  return (E_TMOUT);
	}
      /* DO NOT REACHED */

    case TWF_ORW:
      if (!(flag_table[flgid].iflgptn & waiptn))
	{
	  return (E_TMOUT);
	}
      /* DO NOT REACHED */

    default:
      return (E_TMOUT);
    }

  *flgptn = flag_table[flgid].iflgptn;
  if (wfmode & TWF_CLR)
    {
      flag_table[flgid].iflgptn = 0;
    }

  return (E_OK);
}


/*******************************************************************************
 * twai_flg  --- イベントフラグ待ち(タイムアウト付き)
 *
 * 引数：
 *
 * 返り値：
 *
 * 機能：
 *
 */
ER
twai_flg (UINT *flgptn, ID flgid, UINT waiptn, UINT wfmode, TMO tmout)
{
  T_TCB	*p;

  *flgptn = NULL;
  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_NOEXS);
    }

  if ((flag_table[flgid].flgatr == TA_WSGL) && (flag_table[flgid].wait_task))
    {
      return (E_OBJ);
    }

  run_task->slp_err = E_OK;
  run_task->tskwait.event_wait = 1;
  run_task->event_id = flgid;
  run_task->next = NULL;
  flag_table[flgid].wait_task = ins_tcb_list (flag_table[flgid].wait_task, run_task);
  set_timer (tmout, tflg_func, run_task);
  slp_tsk ();
  *flgptn = run_task->rflgptn;
  unset_timer (tflg_func, run_task);
  return (run_task->slp_err);
}

/*******************************************************************************
 * ref_flg --- イベントフラグ状態参照
 *
 * 引数：
 *
 * 返り値：
 *
 * 機能：
 *
 */
ER
ref_flg (T_RFLG *pk_rflg, ID flgid)
{
  if (flgid < MIN_EVENTFLAG || flgid > MAX_EVENTFLAG)
    {
      return (E_ID);
    }

  if (flag_table[flgid].flgatr == TA_FREE)
    {
      return (E_NOEXS);
    }

  pk_rflg->exinf = flag_table[flgid].exinf;
  pk_rflg->wtsk = flag_table[flgid].wait_task ? TRUE : FALSE;
  pk_rflg->flgptn = flag_table[flgid].iflgptn;
  return (E_OK);
}

/*********************************************************************************
 * tflg_func --- タイムアウト時間が過ぎた場合に実行する関数
 *
 * 引数：
 *	タイムアウトしたタスクのＴＣＢ領域へのポインタ
 *
 * 返り値：
 *	なし
 *
 * 機能：
 *	本関数は、twai_flg によってイベントフラグ待ちしていたタスクが
 *	タイムアウトした場合に呼びだされる。
 */
static void
tflg_func (VP p)
{
  T_TCB *taskp;
  T_EVENTFLAG	*eventp;

  taskp = (T_TCB *)p;
  taskp->slp_err = E_TMOUT;
  taskp->tskwait.event_wait = 0;

  eventp = &flag_table[taskp->event_id];
  eventp->wait_task = del_tcb_list (eventp->wait_task, taskp);
  taskp->rflgptn = NULL;
  wup_tsk (taskp->tskid);
}
