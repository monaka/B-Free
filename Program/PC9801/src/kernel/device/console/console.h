/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/console.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: console.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.5  1995-12-05 14:16:13  night
 * device.h �� #include ʸ���ѹ�������
 *
 * Revision 1.4  1995/09/21  15:50:55  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/09/17  16:49:26  night
 * MAX_COLUMN �� MAX_LINE �������
 * ���󥽡��������ѤΥ��ޥ�� (CONSOLE_CLEAR��CONSOLE_MOVE) �������
 * �����δؿ��������
 * ��Ԥä���
 *
 * Revision 1.2  1995/09/14  04:27:47  night
 * init_console() �����ؿ����ѹ���
 * send_reply() ���ɲá�
 *
 * 
 *
 *
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__	1


#include <h/types.h>
#include <h/errno.h>
#include <pc9801/pc98.h>
#include "../../kernlib/device.h"

#define CONSOLE_DRIVER	"driver.console"

#define MAX_COLUMN	80
#define MAX_LINE	25

#define CONSOLE_CLEAR	1
#define CONSOLE_MOVE	2


/* main.c */
extern W    	init_console(void);	/* �����		*/
extern W    	open_console();		/* �����ץ�		*/
extern W  	close_console();	/* ������		*/
extern W    	read_console();		/* �ɤ߹���		*/
extern W    	write_console();	/* �񤭹���		*/
extern W    	control_console();	/* ����ȥ���		*/

extern void	goto_cursol (W x, W y);
extern void	set_curpos (W x, W y);

#endif __CONSOLE_H__


