/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* pc98.h
 *
 * Discription
 *	IBMPC �˰�¸������ʬ�����
 */

#ifndef __ITRON_IBMPC_H__
#define __ITRON_IBMPC_H__	1

#define MASTER_8259A_COM	0x20
#define MASTER_8259A_DATA	0x21

#define SLAVE_8259A_COM		0xa0
#define SLAVE_8259A_DATA	0xa1

#define INT_TIMER		32
#define INT_KEYBOARD		33
#if 0
#define INT_SCSI		5		/* SCSI type */
#define INT_HD			41		/* SASI type */
#endif
#define	INT_FD			38		/* 1M type */

#define RESERVE_PAGES		(1024 * 1024 / PAGE_SIZE)
#ifdef notdef
#define RESERVE_PAGES		(128 * 1024 / PAGE_SIZE)
#endif /* notdef */

#define VRAM_MEM_START		(0xB8000)
#define VRAM_MEM_END		(0xFFFFF)


#endif  __ITRON_IBMPC_H__


