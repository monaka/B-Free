/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/dc390/dc390_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: dc390_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-05-13 16:22:04  kishida0
 * for debug
 *
 * Revision 1.2  1999/04/26 18:51:29  kishida0
 * �ְ�äƤ������������(���Ƥ��ä�̵��)
 *
 * Revision 1.1  1999/04/26 18:33:32  kishida0
 * add for debug
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */


#ifndef	__DC390_INTERNAL_H__
#define	__DC390_INTERNAL_H__	1


#include "h/itron.h"
#include "h/errno.h"
#include "../console/console.h"
#include "servers/port-manager.h"


/*** �����˥ǥХ���������򤫤��Ƥ������� ***/


/* �ǥХ�����¤�� 
 * DC390 �ǥХ����ϡ��ơ����ι�¤�ΤǴ������Ƥ���
 *
 */
struct device
{
  UW uwDeviceNumber;
  UW uwFunctionNumber;
};



#include "global.h"
#include "funcs.h"


#endif /* __DC390_INTERNAL_H__ */
