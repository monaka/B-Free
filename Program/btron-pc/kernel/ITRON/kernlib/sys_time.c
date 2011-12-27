/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
 * sys_time.c -- ���ִ����ط��Υ����ƥॳ����
 */

#include "types.h"
#include "itron.h"
#include "syscall.h"
#include "errno.h"

/*
 * �������ٱ�
 */

ER dly_tsk(DLYTIME dlytim)
{
  return call_syscall (SYS_DLY_TSK, dlytim);
}

/*
 * ������ư�ϥ�ɥ����
 */

ER def_cyc(HNO cycno, T_DCYC *pk_dcyc)
{
  return call_syscall (SYS_DEF_CYC, cycno, pk_dcyc);
}

/*
 * ������ư�ϥ�ɥ��������
 */

ER act_cyc(HNO cycno, UINT cycact)
{
  return call_syscall (SYS_ACT_CYC, cycno, cycact);
}

/*
 * ������ư�ϥ�ɥ���ֻ���
 */

ER ref_cyc(T_RCYC *pk_rcyc, HNO cycno)
{
  return call_syscall (SYS_REF_CYC, pk_rcyc, cycno);
}

/*
 * ���顼��ϥ�ɥ����
 */

ER def_alm(HNO almo, T_DALM *pk_dalm)
{
  return call_syscall (SYS_DEF_ALM, almo, pk_dalm);
}

/*
 * ���顼��ϥ�ɥ���ֻ���
 */

ER ref_alm(T_RALM *pk_ralm, HNO almno)
{
  return call_syscall (SYS_REF_ALM, pk_ralm, almno);
}
