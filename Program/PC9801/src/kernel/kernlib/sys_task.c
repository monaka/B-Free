/*

B-Free Project ¤ÎÀ¸À®Êª¤Ï GNU Generic PUBLIC LICENSE ¤Ë½¾¤¤¤Þ¤¹¡£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sys_task.c -- $B%?%9%/4X78$N%7%9%F%`%3!<%k(B
 *
 * $Id: sys_task.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 */

#include "../itron-3.0/h/types.h"
#include "../itron-3.0/h/itron.h"
#include "../itron-3.0/h/syscall.h"
#include "../itron-3.0/h/errno.h"


/* cre_tsk  --- $B%?%9%/$N@8@.(B
 *
 * $B0z?t!'(B
 *
 * $BJV$jCM!'(B
 *
 */
ER
cre_tsk (ID tskid, T_CTSK *pk_ctsk)
{
  return call_syscall (SYS_CRE_TSK, tskid, pk_ctsk);
}

/* ext_tsk  --- $B<+%?%9%/=*N;(B
*/
ER
ext_tsk (void)
{
  return call_syscall (SYS_EXT_TSK);
}

/* exd_tsk  --- $B<+%?%9%/=*N;$H:o=|(B
*/
ER
exd_tsk ()
{
  return call_syscall (SYS_EXD_TSK);
}

/* can_wup  --- $B%?%9%/$N5/>2MW5a$rL58z2=(B
*/
ER
can_wup ()
{
  return call_syscall ();
}

/* chg_pri  --- $B%W%i%$%*%j%F%#$NJQ99(B
*/
ER
chg_pri ()
{
  return call_syscall ();
}

/* dis_dsp  --- $B%G%#%9%Q%C%A6X;_(B
*/
ER
dis_dsp ()
{
  return call_syscall ();
}

/* ena_dsp  --- $B%G%#%9%Q%C%A5v2D(B
*/
ER
ena_dsp ()
{
  return call_syscall ();
}

/* frsm_tsk --- $B6/@)BT$A>uBV$N%?%9%/$+$iBT$A>uBV$r2r=|(B($BB?=E$NBT$A>uBVMQ(B)
*/
ER
frsm_tsk()
{
  return call_syscall ();
}

/* get_tid  --- $B<+%?%9%/$N%?%9%/(B ID $B;2>H(B
*/
ER
get_tid (ID *rid)
{
  return call_syscall (SYS_GET_TID, rid);
}

/* ref_tsk  --- $B%?%9%/>uBV$N;2>H(B
*/
ER
ref_tsk ()
{
  return call_syscall ();
}

/* rot_rdq  --- $BF10l%W%i%$%*%j%F%#$G$N%?%9%/$N=g=x$rJQ99$9$k(B
*/
ER
rot_rdq ()
{
  return call_syscall ();
}

/* rsm_tsk  --- $B6/@)BT$A>uBV$N%?%9%/$+$iBT$A>uBV$r2r=|(B
*/
ER
rsm_tsk ()
{
  return call_syscall ();
}

/* slp_tsk  --- $B<+%?%9%/$rBT$A>uBV$K$9$k(B
*/
ER
slp_tsk ()
{
  return call_syscall (SYS_SLP_TSK);
}

/* sta_tsk  --- $B%?%9%/$N5/F0(B
*/
ER
sta_tsk ()
{
  return call_syscall ();
}

/* sus_tsk  --- $B;XDj$7$?%?%9%/$r6/@)BT$A>uBV$K0\9T(B
*/
ER
sus_tsk ()
{
  return call_syscall ();
}

/* ter_tsk  --- $BB>%?%9%/6/@)=*N;(B
*/
ER
ter_tsk ()
{
  return call_syscall ();
}

/* wup_tsk  --- $B;XDj$5$l$?%?%9%/$r5/>2$9$k!#(B
*/
ER
wup_tsk ()
{
  return call_syscall ();
}

