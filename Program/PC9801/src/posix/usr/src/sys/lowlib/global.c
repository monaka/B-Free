/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: global.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:52:56  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/03/18  14:25:49  night
 * ��������ѿ���������ɲá�
 *
 * main_task	�桼���ץ����Υ����ɤ�¹Ԥ��륿����
 * signal_task	�����ʥ�μ���������Ԥ�������
 * fifo_task	�ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿����
 * alarm_task	alarm �����ƥॳ�����ѤΥ�����
 * efile		�¹ԥե������ؤ��Ƥ����å������ݡ���
 * 		���������Υڡ�������ΤȤ��˻��Ѥ���
 *
 * Revision 1.2  1995/02/20  15:22:12  night
 * #include <sys/types.h> �Υ��󥯥롼��ʸ���ɲá�
 * #include <posix.h> �Υ��󥯥롼��ʸ����������
 *
 * Revision 1.1  1995/02/20  15:16:42  night
 * �Ϥ���Ƥ���Ͽ
 *
 *
 */

#include <sys/types.h>
#include "global.h"


ID	main_task;	/* �桼���ץ����Υ����ɤ�¹Ԥ��륿���� */
ID	signal_task;	/* �����ʥ�μ���������Ԥ������� */
ID	fifo_task;	/* �ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿���� */
ID	alarm_task;	/* alarm �����ƥॳ�����ѤΥ����� */
  
ID	efile;		/* �¹ԥե������ؤ��Ƥ����å������ݡ��� 
			 * ���������Υڡ�������ΤȤ��˻��Ѥ��롣
			 */

