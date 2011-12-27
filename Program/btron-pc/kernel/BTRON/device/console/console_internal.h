/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/console_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: console_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-07-09 16:44:46  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.4  2000/02/27 15:25:17  naniwa
 * minor fix
 *
 * Revision 1.3  1999/04/18 18:43:20  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1998/11/30 13:16:56  night
 * �ޥ��� MAX_PATTERN_SIZE �������
 * ���Υޥ���ϡ����󥽡���ǥХ����ɥ饤�Ф˥Хå����饦��ɥѥ��������
 * ��� region ������ʤɤ�Ԥ��Ȥ��ΰ���Хåե� (copy_buffer) �Υ�����
 * ����ꤹ�뤿��˻��Ѥ��롣
 * �Ȥꤢ������10KB (1024 * 10) ���ꤷ�Ƥ��롣
 *
 * Revision 1.1  1997/08/31 13:59:23  night
 * �ǽ����Ͽ
 *
 *
 */


#ifndef __CONSOLE_INTERNAL_H__
#define __CONSOLE_INTERNAL_H__	1

#include "h/types.h"
#include "h/errno.h"
#include "h/misc.h"
#include "servers/port-manager.h"
#include "i386/i386.h"
#include "h/itron_struct.h"
#include "h/itron_syscall_func.h"
#include "h/itron_misc_func.h"
#include "ibmpc/ibmpc.h"
#include "kernlib/device.h"

#include "console.h"
#include "misc.h"
#include "vga.h"

#define MAX_PATTERN_SIZE	(1024 * 10)


/* main.c */
extern W    	init_console(void);	/* �����		*/
extern W    	open_console(ID caller, DDEV_OPN_REQ *packet);		/* �����ץ�		*/
extern W  	close_console(ID caller, DDEV_CLS_REQ *packet);	/* ������		*/
extern W    	read_console(ID caller, DDEV_REA_REQ *packet);		/* �ɤ߹���		*/
extern W    	write_console(ID caller, DDEV_WRI_REQ *packet);	/* �񤭹���		*/
extern W    	control_console(ID caller, DDEV_CTL_REQ *packet);	/* ����ȥ���		*/

extern ER	write_char(UB c);
extern void	goto_cursol (W x, W y);
extern void	set_curpos (W x, W y);
extern void	move_curpos (W x, W y);
#endif /* __CONSOLE_INTERNAL_H__ */
