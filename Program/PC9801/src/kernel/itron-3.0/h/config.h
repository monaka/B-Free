/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
  This file is part of BTRON/386

  $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/h/config.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $

  

*/

#ifndef _ITRON_CONFIG_H_
#define _ITRON_CONFIG_H_	1

#include "../i386/i386.h"

#define GDT_ADDR	0x80001000
#define IDT_ADDR	0x80002000
#define MAX_GDT		255		/* GDT �κ���Υ���ǥå��� */
#define MAX_IDT		255		/* IDT �κ���Υ���ǥå��� */

#define TASK_DESC_MIN	TSS_BASE
#define KERNEL_ESEG	0x0008		/* �����ͥ�Υ����ɥ������� */
#define KERNEL_DSEG	0x0010		/* �����ͥ�Υǡ����������� */
#define USER_ESEG	0x0018		/* �桼���Υ����ɥ������� */
#define USER_DSEG	0x0020		/* �桼���Υǡ����������� */

#define PAGE_DIR_ADDR	0x3000		/* ���ۥڡ����ǥ��쥯�ȥ�ơ��֥� */
#define PAGE_ENTRY_ADDR	0x4000		/* ���ۥڡ����ơ��֥� */

/* �����������ط� */ 
#define MAX_TSKID	128		/* ����Υ����� ID ��	*/
#define MIN_TSKID	1		/* �Ǿ��Υ����� ID ��	*/
#define NTASK		(MAX_TSKID - MIN_TSKID + 1)	/* ��������	*/

#define MIN_PRIORITY	0		/* �Ǿ��Υץ饤����ƥ�	*/
#define MAX_PRIORITY	31		/* ����Υץ饤����ƥ�	*/

#define MIN_USERTASKID	21
#define MAX_USERTASKID	MAX_TSKID

#define MAX_SUSPEND_NEST	10	/* �����ڥ�ɤΥͥ��ȤΥ����С��ե� */


/* ���ޥե������ط� */
#define MIN_SEMAID		1
#define MAX_SEMAID		256
#define NSEMAPHORE		(MAX_SEMAID - MIN_SEMAID + 1)
#define MIN_USERSEMAID		10
#define MAX_USERSEMAID		MAX_SEMAID


/* ���٥�ȥե饰�����ط� */
#define MIN_EVENTFLAG		1
#define MAX_EVENTFLAG		256
#define NEVENTFLAG		(MAX_SEMAID - MIN_SEMAID + 1)
#define MIN_USEREVENTID		10
#define MAX_USEREVENTID		MAX_EVENTFLAG


/* ��å������Хåե������ط� */
#define MIN_MSGBUF		1
#define MAX_MSGBUF		256
#define NMSGBUF			(MAX_MSGBUF - MIN_MSGBUF + 1)
#define MAX_MSGENTRY		1000
/*
 * ��ͳ�˻Ȥ����å������Хåե��κǾ��� ID �Ⱥ���� ID
 */
#define MIN_USERMBFID		100
#define MAX_USERMBFID		MAX_MSGBUF


/* ��������ط� */
#define MAX_MEMPOOL	100
#define PAGE_SIZE	I386_PAGESIZE
#define PAGE_SHIFT	12
#define DIR_SHIFT	22
#define	MIN_KERNEL	0x80000000
#define MAX_KERNEL	0xffffffff

#define MEM_SIZE		0x100000	/* ��ĥ����� 1M �Х��� */
#define MAX_FREEMEM_ENTRY	1000	/* �ե꡼�����������뤿��Υơ��֥� */
#define MAX_MEMORY_POOL		100
#define SYSTEM_MEMORY_SIZE	(2 * 1024 * 1024)	/* �����ƥ����� 2M */
#define MEMORY_POOL_BASE	(0x80010000U)		/* �����ƥ����Ȥ��ƴ��������ΰ�Υ١��� */

#define SYSTEM_MEMORY_POOL	0	/* �����ƥ����ס���� ID */

#define MAX_REGION		5	/* �ƥ�������¸�ߤǤ��� REGION �ο� */

/* IO �����ط� */
#define MAX_DEVICE_NAME		16	/* �ǥХ���̾�κ���Ĺ (16 �Х���) */
#define MIN_DEVID		0
#define MAX_DEVID		19
#define MAX_DEVICE		(MAX_DEVID - MIN_DEVID + 1)	/* �ǥХ����κ���� */


/* ���󥿡��Х륿���� */
#define MAX_TIMER	500


/* �����ƥॿ�����Υꥹ�� */
#define NSYSTEM_TASK	20		/* �����ƥॿ�����ο� */

#define ITRON_IDLE	1
#define ITRON_RS232C	2		/* ITRON �� RS232C �ɥ饤�Х����� */
#define ITRON_KEYBOARD	3
#define ITRON_CONSOLE	4		/* ���󥽡���ǥХ����ɥ饤�� */
#define ITRON_DEBUG	5		/* �ǥХå��ѥץ���: ���ޥ�ɥ��󥿥ץ꥿�� */

/*
 * �ݡ��ȥޥ͡����㤬�׵�����Ĥ��Ѥ˻��ꤹ���å������Хåե��� ID 
 */
#define PORT_MANAGER_PORT	11


/* �����ƥॻ�ޥե��Υꥹ�� */
#define ITRON_KEYBOARD_SEM	3	/* �����ܡ��������ѤΥ��ޥե�	 */


/* �����ƥ��å������Хåե��Υꥹ�� */
#define ITRON_DEBUG_MBF		1	/* �ǥХå��ѥץ���: ���ޥ�ɥ��󥿥ץ꥿�ǻ��� */
#define ITRON_RS232C_MBF	2	/* ITRON �� RS232C �ɥ饤�Х������ǻ��� */
#define ITRON_KEYBOARD_MBF	3	/* �����ܡ��������ѤΥ�å������Хåե� */

extern W	ndevice;
extern ER	(*devices[])();


#endif /* _ITRON_CONFIG_H_ */
