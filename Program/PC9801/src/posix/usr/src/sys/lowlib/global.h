/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/global.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: global.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:57  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/03/18  14:26:24  night
 * ��������ѿ���������ɲá�
 *
 * errno		���顼�ֹ��Ѥ��ѿ�
 * main_task	�桼���ץ����Υ����ɤ�¹Ԥ��륿����
 * signal_task	�����ʥ�μ���������Ԥ�������
 * fifo_task	�ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿����
 * alarm_task	alarm �����ƥॳ�����ѤΥ�����
 * efile		�¹ԥե������ؤ��Ƥ����å������ݡ���
 * 		���������Υڡ�������ΤȤ��˻��Ѥ���
 *
 * Revision 1.1  1995/02/21  15:10:46  night
 * �Ϥ���Ƥ���Ͽ
 *
 *
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__	1

#include <sys/types.h>

/*
 * �ץ�������ǥ桼���ץ�����¹Ԥ��Ƥ���祿�����Υ����� ID
 */
ID	main_task;

/*
 * �����ʥ��������뤿��Υ������Υ����� ID
 */
ID	signal_task;


/*
 * LOWLIB ��ư���Ƥ���ץ����� ID
 */
extern pid_t	my_pid;


/*
 * �����ƥॳ����Υ��顼�ֹ�򵭲����Ƥ���������ѿ�
 */
extern int	errno;

extern ID	main_task;	/* �桼���ץ����Υ����ɤ�¹Ԥ��륿���� */
extern ID	signal_task;	/* �����ʥ�μ���������Ԥ������� */
extern ID	fifo_task;	/* �ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿���� */
extern ID	alarm_task;	/* alarm �����ƥॳ�����ѤΥ����� */
  
extern ID	efile;		/* �¹ԥե������ؤ��Ƥ����å������ݡ��� 
				 * ���������Υڡ�������ΤȤ��˻��Ѥ��롣
				 */

#endif /* __GLOBAL_H__ */
