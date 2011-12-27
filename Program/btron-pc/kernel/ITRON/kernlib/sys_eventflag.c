/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_eventflag.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_eventflag.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: sys_eventflag.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-01-28 08:55:29  monaka
 * Some define of function-call number was changed.
 *
 * Revision 1.1  1999/04/18 17:48:34  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1996/07/23 17:17:07  night
 * IBM PC �Ѥ� make �Ķ��� merge
 *
 * Revision 1.1  1996/07/22  23:52:06  night
 * �ǽ����Ͽ
 *
 * Revision 1.2  1995/09/21  15:51:43  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/09/19  18:07:38  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * ���٥�ȥե饰�ط��Υ����ƥॳ���륤�󥿥ե������ؿ�
 */

#include "../ITRON/h/types.h"
#include "../ITRON/h/itron.h"
#include "../ITRON/h/syscall.h"
#include "../ITRON/h/errno.h"


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
  return (call_syscall (SYS_CLR_FLG, flgid, clrptn));
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
