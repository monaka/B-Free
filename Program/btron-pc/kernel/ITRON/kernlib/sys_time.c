/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
 * sys_time.c -- 時間管理関係のシステムコール
 */

#include "types.h"
#include "itron.h"
#include "syscall.h"
#include "errno.h"

/*
 * タスク遅延
 */

ER dly_tsk(DLYTIME dlytim)
{
  return call_syscall (SYS_DLY_TSK, dlytim);
}

/*
 * 周期起動ハンドラ定義
 */

ER def_cyc(HNO cycno, T_DCYC *pk_dcyc)
{
  return call_syscall (SYS_DEF_CYC, cycno, pk_dcyc);
}

/*
 * 周期起動ハンドラ活性制御
 */

ER act_cyc(HNO cycno, UINT cycact)
{
  return call_syscall (SYS_ACT_CYC, cycno, cycact);
}

/*
 * 周期起動ハンドラ状態参照
 */

ER ref_cyc(T_RCYC *pk_rcyc, HNO cycno)
{
  return call_syscall (SYS_REF_CYC, pk_rcyc, cycno);
}

/*
 * アラームハンドラ定義
 */

ER def_alm(HNO almo, T_DALM *pk_dalm)
{
  return call_syscall (SYS_DEF_ALM, almo, pk_dalm);
}

/*
 * アラームハンドラ状態参照
 */

ER ref_alm(T_RALM *pk_ralm, HNO almno)
{
  return call_syscall (SYS_REF_ALM, pk_ralm, almno);
}
