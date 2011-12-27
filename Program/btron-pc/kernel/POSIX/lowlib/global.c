/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: global.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1998-02-25 12:47:52  night
 * �ץ�����θ�ͭ�ǡ�������򡢤��줾������ѿ��Ǹ���Ū�ˤ�ĤΤǤϤʤ���
 * lowlib_data �Ȥ����ݥ��󥿤Ǵ�������褦�ˤ�����
 * lowlib_data ���ͤϡ��ޥ��� LOWLIB_DATA �ǻ��ꤷ�����ɥ쥹 (0x7fff0000)
 * �����ꤵ��롣���Υ��ɥ쥹�ϡ��ץ�����˰�ä�ʪ���ڡ����˥ޥåԥ�
 * ����롣���ۥ��ɥ쥹�ϸ���������ºݤ��ΰ�ϥץ�����˰�ä�����¸
 * �ߤ��Ƥ��롣LOWLIB ����������ץ�����ξ���Ϥ����ΰ�������褦��
 * ���롣
 *
 * Revision 1.2  1997/08/31 13:13:42  night
 * �Ȥꤢ������OS �ե�����������Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 * ----------------
 *
 * Revision 1.4  1995/09/21  15:52:56  night
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

#include "lowlib.h"


struct lowlib_data	*lowlib_data = (struct lowlib_data *)LOWLIB_DATA;

ID	posix_manager = 0;

int	errno;
