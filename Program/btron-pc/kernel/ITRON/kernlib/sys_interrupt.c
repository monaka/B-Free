/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: sys_interrupt.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-18 17:48:34  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1996/07/23 17:17:08  night
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

#include "../ITRON/h/types.h"
#include "../ITRON/h/itron.h"
#include "../ITRON/h/syscall.h"
#include "../ITRON/h/errno.h"


/*
 * �����ߥϥ�ɥ����Ͽ
 */
ER
def_int (W intno, T_DINT *pk_dint)
{
  return (call_syscall (SYS_DEF_INT, intno, pk_dint));
}
