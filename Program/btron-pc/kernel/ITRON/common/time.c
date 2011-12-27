/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * 時間管理機能
 */
/*
 * $Log: time.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-04-03 14:34:48  naniwa
 * to call timer handler in task
 *
 * Revision 1.3  2000/02/27 15:30:51  naniwa
 * to work as multi task OS
 *
 * Revision 1.2  2000/02/06 09:10:58  naniwa
 * minor fix
 *
 * Revision 1.1  1999/11/14 14:53:38  naniwa
 * add time management function
 *
 */

#include "itron.h"
#include "errno.h"
#include "misc.h"
#include "ibmpc.h"
#include "time.h"
#include "task.h"
#include "func.h"

/* 
 * 変数宣言
 */

static T_CYCHANDLER cyctable[MAX_CYCLIC];
static T_ALMHANDLER almtable[MAX_ALARM];

void init_time(void)
{
  bzero(cyctable, sizeof(T_CYCHANDLER)*MAX_CYCLIC);
  bzero(almtable, sizeof(T_ALMHANDLER)*MAX_ALARM);
}

/*
 * タスク遅延
 */

static void
dly_func (VP p)
{
  T_TCB *taskp;

#ifdef CALL_HANDLER_IN_TASK
  dis_dsp();
#else
  dis_int();
#endif
  taskp = (T_TCB *)p;
  taskp->slp_err = E_OK;
  taskp->tskwait.time_wait = 0;
#ifdef CALL_HANDLER_IN_TASK
  ena_dsp();
#else
  ena_int();
#endif
  wup_tsk (taskp->tskid);
}

ER dly_tsk(DLYTIME dlytim)
{
  if (dlytim < 0) return(E_PAR);
  set_timer (dlytim, (void (*)(VP))dly_func, run_task);
  run_task->tskwait.time_wait = 1;
  slp_tsk ();
  unset_timer ((void (*)(VP))dly_func, run_task);
  if (run_task->slp_err)
    return (run_task->slp_err);
  return(E_OK);
}

/*
 * 周期起動ハンドラ定義
 */

ER def_cyc(HNO cycno, T_DCYC *pk_dcyc)
{
  return(E_NOSPT);
}

/*
 * 周期起動ハンドラ活性制御
 */

ER act_cyc(HNO cycno, UINT cycact)
{
  return(E_NOSPT);
}

/*
 * 周期起動ハンドラ状態参照
 */

ER ref_cyc(T_RCYC *pk_rcyc, HNO cycno)
{
  return(E_NOSPT);
}

/*
 * アラームハンドラ定義
 */

ER def_alm(HNO almno, T_DALM *pk_dalm)
{
  if (almno < 0) return(E_PAR);
  else if (almno >= MAX_ALARM) return(E_PAR);
  if (pk_dalm == NULL) return (E_PAR);
  if (pk_dalm == (T_DALM *) NADR) {
    if (almtable[almno].almhdr != NULL) {
      unset_timer((void (*)(VP))almtable[almno].almhdr, almtable[almno].exinf);
    }
    bzero(&(almtable[almno]), sizeof(T_ALMHANDLER));
  }
  else {
    if (pk_dalm->tmmode == TTM_ABS) {
      return(E_NOSPT);
    }
    if (pk_dalm->almtim.utime > 0) {
      return(E_NOSPT);
    }
    if (almtable[almno].almhdr != NULL) {
      unset_timer((void (*)(VP))almtable[almno].almhdr, almtable[almno].exinf);
    }

    almtable[almno].exinf = pk_dalm->exinf;
    almtable[almno].almhdr = pk_dalm->almhdr;
    almtable[almno].almatr = pk_dalm->almatr;
    almtable[almno].tmmode = pk_dalm->tmmode;
    bcopy(&(pk_dalm->almtim), &(almtable[almno].almtim), sizeof(ALMTIME));
    set_timer(pk_dalm->almtim.ltime, (void (*)(VP))pk_dalm->almhdr,
	      pk_dalm->exinf);
  }
  return(E_OK);
}

/*
 * アラームハンドラ状態参照
 */

ER ref_alm(T_RALM *pk_ralm, HNO almno)
{
  if (almno < 0) return(E_PAR);
  else if (almno >= MAX_ALARM) return(E_PAR);
  if (pk_ralm == NULL) return(E_PAR);
  if (almtable[almno].almhdr == NULL) return(E_NOEXS);
  pk_ralm->exinf = almtable[almno].exinf;
  pk_ralm->lfttim.utime = 0;
  pk_ralm->lfttim.ltime = left_time((void (*)(VP))almtable[almno].almhdr,
				    almtable[almno].exinf);
  return(E_OK);
}
