/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char	rcsid[] = "$Id: sys_misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: sys_misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-07-09 16:37:59  kishida0
 * add return value type
 *
 * Revision 1.4  1999/11/11 09:52:47  naniwa
 * minor fix
 *
 * Revision 1.3  1999/11/10 10:30:38  naniwa
 * to support execve, etc
 *
 * Revision 1.2  1999/07/30 08:19:59  naniwa
 * add vcpy_stk()
 *
 * Revision 1.1  1999/04/18 17:48:34  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.4  1999/03/16 02:43:56  monaka
 * Minor changes. Fixed wrong comment.
 *
 * Revision 1.3  1997/08/31 14:10:50  night
 * vsys_inf() の追加。
 *
 * Revision 1.2  1996/11/07  12:40:27  night
 * 関数 vsys_msc() の追加。
 *
 * Revision 1.1  1996/11/06  12:38:26  night
 * 最初の登録
 *
 *
 */



#include "../ITRON/h/types.h"
#include "../ITRON/h/itron.h"
#include "../ITRON/h/syscall.h"
#include "../ITRON/h/errno.h"


/* vget_csl
 *
 */
ER
vget_csl (W *x, W *y)
{
  return (call_syscall (SYS_VGET_CSL, x, y));
}


/* vset_cns
 *
 */
ER
vset_cns (ID driver)
{
  return (call_syscall (SYS_VSET_CNS, driver));
}


/* vsys_msc
 *
 */
ER
vsys_msc (W cmd, VP argp)
{
  return (call_syscall (SYS_VSYS_MSC, cmd, argp));
}

/* vsys_inf
 *
 */
ER
vsys_inf (W func, W sub, VP argp)
{
  return (call_syscall (SYS_VSYS_INF, func, sub, argp));
}

/* vcpy_stk */
ER
vcpy_stk (ID src, W esp, W ebp, ID dst)
{
  return (call_syscall (SYS_VCPY_STK, src, esp, ebp, dst));
}

/* vset_cxt */
ER
vset_ctx (ID tid, W eip)
{
  return (call_syscall (SYS_VSET_CTX, tid, eip));
}
