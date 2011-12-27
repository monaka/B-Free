/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscall.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: syscall.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:58  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/03/18  14:29:01  night
 * �����ѹ���
 *
 * Revision 1.1  1995/02/20  15:16:43  night
 * �Ϥ���Ƥ���Ͽ
 *
 *
 */

/*
 * POSIX API (�����ƥॳ����) �˴ؤ�������ե�����
 *
 */

#ifndef __SYSCALL_H__
#define __SYSCALL_H__	1

/*
 * �����ƥॳ����ؿ�����Ͽ���뤿��ι�¤��
 *
 */
struct syscall_entry
{
  int	(*func)();
  char	*name;
};

/*
 * �����ƥॳ�����ֹ�ο���
 */
extern int	nsyscall;

/*
 * �����ƥॳ����ؿ���������Ƥ���ơ��֥�
 */
extern struct syscall_entry	syscalls[];

#endif /* __SYSCALL_H__ */
