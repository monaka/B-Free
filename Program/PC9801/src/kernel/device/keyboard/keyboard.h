/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/keyboard.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: keyboard.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1996-01-06 15:59:49  night
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
#include <pc9801/pc98.h>
#include <h/itron.h>
#include "../../kernlib/device.h"

#define KEYBOARD_DRIVER	"driver.keyboard"

#define KEYBOARD_INT		33

#define KEYBOARD_CLEAR		1
#define KEYBOARD_CHANGEMODE	2

#define KEY_COM		0x43
#define KEY_DATA	0x41
#define KEY_STAT	0x43

/* ʸ���򥳥�ȥ��륭�����Ѵ����� */
#define C(x)		(x & 0x1f)

#define	NOKEYS	0
#define ONKEYS	1		/* ���������Ϥ��Ƥ��� */

/* keydevice mode */
#define NOWAITMODE	0x0001
#define WAITMODE	0x0002
#define RAWMODE		0x0004


extern ID	recvport;
extern W	initialized;
extern ID	waitflag;	/* �����ܡ��ɤ��饭�����Ϥ��ԤĻ��� */
				/* ���Ѥ��륤�٥�ȥե饰�� ID */
extern W	driver_mode;

/* main.c */
extern W    	init_keyboard(void);	/* �����		*/
extern W    	open_keyboard();	/* �����ץ�		*/
extern W  	close_keyboard();	/* ������		*/
extern W    	read_keyboard();	/* �ɤ߹���		*/
extern W    	write_keyboard();	/* �񤭹���		*/
extern W    	control_keyboard();	/* ����ȥ���		*/


#endif __KEYBOARD_H__


