/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_message.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_message.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";


/*
 *	��å�������Ϣ�����ƥॳ�����Ƥ֤���Υ饤�֥��
 *
 */


#include "../itron-3.0/h/types.h"
#include "../itron-3.0/h/itron.h"
#include "../itron-3.0/h/syscall.h"
#include "../itron-3.0/h/errno.h"

/*
 * ��å������Хåե��κ���
 */
ER
cre_mbf (ID id, T_CMBF *pk_cmbf)
{
  return (call_syscall (SYS_CRE_MBF, id, pk_cmbf));
}


/* 
 *  ��å������Хåե��κ��
 */
ER
del_mbf (ID id)
{
  return (call_syscall (SYS_DEL_MBF, id));
}


/*
 *  ��å�����������
 */
ER
snd_mbf (ID id, INT size, VP msg)
{
  return (call_syscall (SYS_SND_MBF, id, size, msg));
}


/* 
 *  ��å����������� (�ݡ���󥰵�ǽ�Ĥ�)
 */
ER
psnd_mbf (ID id, INT size, VP msg)
{
  return (call_syscall (SYS_PSND_MBF, id, size, msg));
}


/*
 *  ��å����������� (�����ॢ���ȵ�ǽ�Ĥ�)
 */
ER
tsnd_mbf (ID id, INT size, VP msg, TMO tmout)
{
  return (call_syscall (SYS_TSND_MBF, id, size, msg, tmout));
}


/*
 *  ��å������μ���
 */
ER
rcv_mbf (VP msg, INT *size, ID id)
{
  return (call_syscall (SYS_RCV_MBF, msg, size, id));
}


/*
 *  ��å������μ��� (�ݡ���󥰵�ǽ�Ĥ�)
 */

ER
prcv_mbf (VP msg, INT *size, ID id)
{
  return (call_syscall (SYS_PRCV_MBF, msg, size, id));
}


/*
 * ��å��������� (�����ॢ���ȵ�ǽ�Ĥ�)
 */
ER
trcv_mbf (VP msg, INT *size, ID id, TMO tmout)
{
  return (call_syscall (SYS_TRCV_MBF, msg, size, id, tmout));
}


