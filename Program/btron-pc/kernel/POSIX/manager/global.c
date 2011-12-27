/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/global.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* global.c - POSIX �Ķ��ޥ͡�����Τ��������ѿ�
 *
 *
 * $Log: global.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1997-10-24 13:56:32  night
 * rcsid[] �κ����
 *
 * Revision 1.2  1996/12/12 13:43:26  night
 * ʸ�������ɤ� SJIS ���� EUC ���ѹ�
 *
 * Revision 1.1  1996/12/12 13:42:30  night
 * �ǽ����Ͽ
 *
 *
 */

#include "posix.h"
#include "utsname.h"


struct utsname		system_name = 
{
  SYS_SYSNAME,
  SYS_NODENAME,
  SYS_RELEASE,
  SYS_VERSION,
  SYS_MACHINE
};


