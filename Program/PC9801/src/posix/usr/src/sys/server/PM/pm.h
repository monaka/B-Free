/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/pm.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: pm.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:44  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:28:32  night
 * <sys/errno.h> �Υ��󥯥롼��ʸ���ɲá�
 *
 * Revision 1.1  1995/03/18  14:02:37  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX �Ķ�
 * �ץ����ޥ͡�����ǻ��Ѥ��빽¤�Τ���������
 *
 */


#ifndef __PM_H__
#define __PM_H__	1

#include <sys/types.h>
#include <sys/errno.h>
#include <server/proc.h>

/*
 * �ץ�����¤��
 * �ץ����ޥ͡����㤬��������ץ����ҤȤĤҤȤĤˤĤ���
 * �����������
 *
 */
struct process
{
  struct process	*prev;
  struct process	*next;

  enum proc_status	status;	/* �ץ����ξ��֤򼨤� */
  pid_t			pid;	/* �ץ��� ID */

  uid_t	uid;		/* �ץ�����°�����ͭ�� */
  gid_t	gid;		/* �ץ�����°���륰�롼�� */

  ID	main_task;	/* �桼���ץ����Υ����ɤ�¹Ԥ��륿���� */
  ID	signal_task;	/* �����ʥ�μ���������Ԥ������� */
  ID	fifo_task;	/* �ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿���� */
  ID	alarm_task;	/* alarm �����ƥॳ�����ѤΥ����� */
  
  ID	efile;		/* �¹ԥե������ؤ��Ƥ����å������ݡ��� 
			 * ���������Υڡ�������ΤȤ��˻��Ѥ��롣
			 */

  
};

typedef struct process	process_t;


#endif /* __PM_H__ */
