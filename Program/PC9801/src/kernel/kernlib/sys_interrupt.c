/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_interrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_interrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: sys_interrupt.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:43  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/09/19  18:07:38  night
 * �ǽ����Ͽ
 *
 *
 */

#include "../itron-3.0/h/types.h"
#include "../itron-3.0/h/itron.h"
#include "../itron-3.0/h/syscall.h"
#include "../itron-3.0/h/errno.h"


/*
 * �����ߥϥ�ɥ����Ͽ
 */
ER
def_int (W intno, T_DINT *pk_dint)
{
  return (call_syscall (SYS_DEF_INT, intno, pk_dint));
}
