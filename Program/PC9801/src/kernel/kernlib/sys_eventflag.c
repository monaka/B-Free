/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_eventflag.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_eventflag.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: sys_eventflag.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:43  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/09/19  18:07:38  night
 * 最初の登録
 *
 *
 */

/*
 * イベントフラグ関係のシステムコールインタフェース関数
 */

#include "../itron-3.0/h/types.h"
#include "../itron-3.0/h/itron.h"
#include "../itron-3.0/h/syscall.h"
#include "../itron-3.0/h/errno.h"


ER
cre_flg (ID flgid, T_CFLG *pk_flg)
{
  return (call_syscall (SYS_CRE_FLG, flgid, pk_flg));
}

ER
del_flg (ID flgid)
{
  return (call_syscall (SYS_DEL_FLG, flgid));
}

ER
set_flg (ID flgid, UINT setptn)
{
  return (call_syscall (SYS_SET_FLG, flgid, setptn));
}

ER
clr_flg (ID flgid, UINT clrptn)
{
  return (call_syscall (SYS_SET_FLG, flgid, clrptn));
}

ER
wai_flg (UINT *flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
  return (call_syscall (SYS_WAI_FLG, flgptn, flgid, waiptn, wfmode));
}

ER
pol_flg (UINT *flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
  return (call_syscall (SYS_POL_FLG, flgptn, flgid, waiptn, wfmode));
}

ER
twai_flg (UINT *flgptn, ID flgid, UINT waiptn, UINT wfmode, TMO tmout)
{
  return (call_syscall (SYS_TWAI_FLG, flgptn, flgid, waiptn, wfmode, tmout));
}

ER
ref_flg (T_RFLG *pk_rflg, ID flgid)
{
  return (call_syscall (SYS_REF_FLG, pk_rflg, flgid));
}
