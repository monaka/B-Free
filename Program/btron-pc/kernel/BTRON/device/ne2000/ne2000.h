/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/ne2000.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: ne2000.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  1999-03-07 12:48:05  night
 * �ޥ��� DEFAULT_INTR �� NE2000_DEFAULT_INTR ���ѹ���
 * �������ֹ�� 3 ���� 35 ���ѹ���
 *
 * Revision 1.5  1999/03/02 15:16:16  night
 * �ޥ��� DEFAULT_INTR ���ɲá�
 *
 * Revision 1.4  1997/10/29 22:39:27  night
 * NE2000 �����ץ��� SA_PROM ���ɤ߼��������ɲá�
 *
 * Revision 1.3  1997/10/28 14:44:42  night
 * NE2000_PROBE �ޥ�����ɲá�
 *
 * Revision 1.2  1997/10/28 12:46:54  night
 * NE2000_RESET �Υޥ�������ɲá�
 * #include "global.h" �� #include "funcs.h" �Υ��󥯥롼��ʸ�κ����
 *
 * Revision 1.1  1997/05/06 12:43:31  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 */
#ifndef	__NE2000_H__
#define	__NE2000_H__	1


#define NE2000_DRIVER	"driver.ne2000"

#define MAX_NE2000		1

#define MACADDRLEN		6

#define NE2000_DEFAULT_INTR		35

/* Control command */
#define NE2000_RESET		0x00000001
#define NE2000_PROBE		0x00000002
#define NE2000_GETMAC		0x00000003

#endif /* __NE2000_H__ */
