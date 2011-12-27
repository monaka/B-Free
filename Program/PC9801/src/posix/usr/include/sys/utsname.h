/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/utsname.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: utsname.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:03  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:51:26  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 *
 *
 */

#ifndef __SYS_UTSNAME_H__
#define __SYS_UTSNAME_H__	1

/*
 * �����ƥ����¤�� (utsname �����ƥॳ����ǻ���)
 *
 */
struct utsname
{
  char	machine[65];	/* �ޥ���μ��� 			*/
  char	nodename[65];	/* ���Υޥ����̾�� (�桼�������) 	*/
  char	release[65];	/* OS �ΥС������ 			*/
  char	sysname[65];	/* �����ƥ�̾ (B-Free/POSIX �˸���) 	*/
  char	version[65];	/* OS �ΥС�������ֹ� 			*/
};


#endif /* __SYS_UTSNAME_H__ */

