/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/global.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: global.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-02-16 08:16:47  naniwa
 * not to depend on sys/types.h
 *
 * Revision 1.3  1998/02/25 12:48:46  night
 * �ץ�����θ�ͭ�ǡ�������򡢤��줾������ѿ��Ǹ���Ū�ˤ�ĤΤǤϤʤ���
 * lowlib_data �Ȥ����ݥ��󥿤Ǵ�������褦�ˤ�����
 * �����ȼ��������ޤǤ��ä��ץ�����ͭ�Υǡ����򼨤�����ѿ�����������
 *
 * lowlib_data ���ͤϡ��ޥ��� LOWLIB_DATA �ǻ��ꤷ�����ɥ쥹 (0x7fff0000)
 * �����ꤵ��롣���Υ��ɥ쥹�ϡ��ץ�����˰�ä�ʪ���ڡ����˥ޥåԥ�
 * ����롣���ۥ��ɥ쥹�ϸ���������ºݤ��ΰ�ϥץ�����˰�ä�����¸
 * �ߤ��Ƥ��롣LOWLIB ����������ץ�����ξ���Ϥ����ΰ�������褦��
 * ���롣
 *
 * Revision 1.2  1997/08/31 13:13:43  night
 * �Ȥꤢ������OS �ե�����������Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 * ----------------
 *
 * Revision 1.3  1995/09/21  15:52:57  night
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
#if 0
#include <sys/types.h>
#endif

extern struct lowlib_data	*lowlib_data;
extern ID			posix_manager;

extern struct syscall_entry	syscalls[];
extern int			nsyscall;


#endif /* __GLOBAL_H__ */
