/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* pc98.h
 *
 * Discription
 *	PC98 に依存する部分の定義
 */

#ifndef __ITRON_PC98_H__
#define __ITRON_PC98_H__	1

#define MASTER_8259A_COM	0x00
#define MASTER_8259A_DATA	0x02

#define SLAVE_8259A_COM		0x08
#define SLAVE_8259A_DATA	0x0a

#define INT_TIMER		32
#define INT_KEYBOARD		33
#define INT_SCSI		5		/* SCSI type */
#define INT_HD			41		/* SASI type */
#define	INT_FD			38		/* 1M type */

#define RESERVE_PAGES		(1024 * 1024 / PAGE_SIZE)
#ifdef notdef
#define RESERVE_PAGES		(128 * 1024 / PAGE_SIZE)
#endif /* notdef */

#define VRAM_MEM_START		(0xA0000)
#define VRAM_MEM_END		(0xFFFFF)


#endif  __ITRON_PC98_H__


