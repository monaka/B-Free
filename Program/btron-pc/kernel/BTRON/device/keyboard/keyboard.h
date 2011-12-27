/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/keyboard.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: keyboard.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  2000-04-03 14:30:04  naniwa
 * to call timer handler in task
 *
 * Revision 1.7  2000/02/27 15:28:42  naniwa
 * to work as multi task OS
 *
 * Revision 1.6  2000/01/30 19:02:51  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.5  2000/01/24 23:28:53  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.4  2000/01/23 15:50:51  kishida0
 * 106JP��101US �����ܡ������ؤ��ѤΥ��ޥ�ɤ��ɲä�����
 * �ؿ��ˤ��Ƥ���Τ����ޤ����������Ȥꤢ�����ϻ��Ѳ�ǽ
 * note-pc�ѤˤϤ⤦���������Υơ��֥���ǧ����ɬ�פ�����
 * �ץ�ݡ���
 *
 * Revision 1.3  1999/04/18 18:43:31  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1997/07/02 13:09:44  night
 * PC9801 ��¸�������󥯥롼�ɥե������ IBMPC �ѤΤ�Τ��ѹ�������
 *
 * Revision 1.1  1996/07/24 16:28:44  night
 * IBM PC �� B-Free OS �ؤκǽ����Ͽ
 *
 * Revision 1.4  1996/01/06 15:59:49  night
 * �إå��Υ��󥯥롼�ɥѥ��ΰ����ѹ�
 *
 * Revision 1.3  1995/09/21  15:51:00  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/19  18:01:01  night
 * �����ܡ��ɤ������ϤǤ���Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1995/09/18  11:42:03  night
 * �ǽ����Ͽ
 *
 * 
 *
 *
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__	1


#include <h/types.h>
#include <h/errno.h>
#ifdef PC98
#include <pc9801/pc98.h>
#else
#include <ibmpc/ibmpc.h>
#endif
#include <h/itron.h>
#include "../../../ITRON/kernlib/device.h"
#include "../../../POSIX/manager/posix.h"

#define KEYBOARD_DRIVER	"driver.keyboard"

#define KEYBOARD_INT		33

#define KEYBOARD_CLEAR		1
#define KEYBOARD_CHANGEMODE	2
#define KEYBOARD_CHANGE_106JP   3
#define KEYBOARD_CHANGE_101US   4

#define KEY_COM		0x64
#define KEY_DATA	0x60
#define KEY_STAT	0x64

/* ʸ���򥳥�ȥ��륭�����Ѵ����� */
#define C(x)		(x & 0x1f)

#define	NOKEYS	0
#define ONKEYS	1		/* ���������Ϥ��Ƥ��� */

/* keydevice mode */
#define NOWAITMODE	0x0001
#define WAITMODE	0x0002
#define RAWMODE		0x0004


extern W keyboard_select;

extern ID	recvport;
extern W	initialized;
extern ID	waitflag;	/* �����ܡ��ɤ��饭�����Ϥ��ԤĻ��� */
				/* ���Ѥ��륤�٥�ȥե饰�� ID */
extern W	driver_mode;
extern W	send_switch;

extern ID	my_tskid;

/* main.c */
extern W    	init_keyboard(void);	/* �����		*/
extern W    	open_keyboard();	/* �����ץ�		*/
extern W  	close_keyboard();	/* ������		*/
extern W    	read_keyboard();	/* �ɤ߹���		*/
extern W    	write_keyboard();	/* �񤭹���		*/
extern W    	control_keyboard();	/* ����ȥ���		*/
extern W	posix_read_keyboard();
extern W	send_switch_msg();

#endif __KEYBOARD_H__


