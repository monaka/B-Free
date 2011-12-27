/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sys_task.c -- タスク関係のシステムコール
 *
 * $Id: sys_task.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */

#include "../../ITRON/h/types.h"
#include "../../ITRON/h/itron.h"
#include "../../ITRON/h/syscall.h"
#include "../../ITRON/h/errno.h"


/* cre_tsk  --- タスクの生成
 *
 * 引数：
 *
 * 返り値：
 *
 */
ER
cre_tsk (ID tskid, T_CTSK *pk_ctsk)
{
  return call_syscall (SYS_CRE_TSK, tskid, pk_ctsk);
}


ER
vcre_tsk (T_CTSK *pk_ctsk, ID *rid)
{
  W	i;
  ER	err;

  for (i = MIN_USERTASKID; i <= MAX_USERTASKID; i++)
    {
      err = cre_tsk (i, pk_ctsk);
      if (err == E_OK)
	{
	  *rid = i;
	  return (E_OK);
	}
    }
  return (E_NOMEM);
}


/* ext_tsk  --- 自タスク終了
*/
void
ext_tsk (void)
{
  call_syscall (SYS_EXT_TSK);
}

/* exd_tsk  --- 自タスク終了と削除
*/
void
exd_tsk (void)
{
  call_syscall (SYS_EXD_TSK);
}

/* can_wup  --- タスクの起床要求を無効化
*/
ER
can_wup (INT *p_wupcnt, ID taskid)
{
  return call_syscall (SYS_CAN_WUP, p_wupcnt, taskid);
}

/* chg_pri  --- プライオリティの変更
*/
ER
chg_pri (ID tskid, PRI tskpri)
{
  return call_syscall (SYS_CHG_PRI, tskid, tskpri);
}

/* dis_dsp  --- ディスパッチ禁止
*/
ER
dis_dsp (void)
{
  return call_syscall (SYS_DIS_DSP);
}

/* ena_dsp  --- ディスパッチ許可
*/
ER
ena_dsp (void)
{
  return call_syscall (SYS_ENA_DSP);
}

/* frsm_tsk --- 強制待ち状態のタスクから待ち状態を解除(多重の待ち状態用)
*/
ER
frsm_tsk (ID taskid)
{
  return call_syscall (SYS_FRSM_TSK, taskid);
}

/* rel_wai --- 待ち状態の解除
 */
rel_wai (ID taskid)
{
  return call_syscall (SYS_REL_WAI, taskid);
}

/* get_tid  --- 自タスクのタスク ID 参照
*/
ER
get_tid (ID *rid)
{
  return call_syscall (SYS_GET_TID, rid);
}

/* ref_tsk  --- タスク状態の参照
*/
ER
ref_tsk (T_RTSK *stat, ID taskid)
{
  return call_syscall (SYS_REF_TSK, stat, taskid);
}

/* rot_rdq  --- 同一プライオリティでのタスクの順序を変更する
*/
ER
rot_rdq (PRI tskpri)
{
  return call_syscall (SYS_ROT_RDQ, tskpri);
}

/* rsm_tsk  --- 強制待ち状態のタスクから待ち状態を解除
*/
ER
rsm_tsk (ID taskid)
{
  return call_syscall (SYS_RSM_TSK, taskid);
}

/* slp_tsk  --- 自タスクを待ち状態にする
*/
ER
slp_tsk (void)
{
  return call_syscall (SYS_SLP_TSK);
}

/* sta_tsk  --- タスクの起動
*/
ER
sta_tsk (ID taskid, INT stacd)
{
  return call_syscall (SYS_STA_TSK, taskid, stacd);
}

/* sus_tsk  --- 指定したタスクを強制待ち状態に移行
*/
ER
sus_tsk (ID taskid)
{
  return call_syscall (SYS_SUS_TSK, taskid);
}

/* ter_tsk  --- 他タスク強制終了
*/
ER
ter_tsk (ID tskid)
{
  return call_syscall (SYS_TER_TSK, tskid);
}

/* wup_tsk  --- 指定されたタスクを起床する。
*/
ER
wup_tsk (ID taskid)
{
  return call_syscall (SYS_WUP_TSK, taskid);
}

/* del_tsk --- 他タスク削除
 */
ER
del_tsk (ID tskid)
{
  return call_syscall (SYS_DEL_TSK, tskid);
}
