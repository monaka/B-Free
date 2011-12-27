/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT interrupt definition
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/interrupt.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: interrupt.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:39  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:29:14  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:22  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:23  btron
 * BTRON SYSTEM 1.0
 * 
 */

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__		1

#define MASTER_8259A_COM	0x00
#define MASTER_8259A_DATA	0x02

#define SLAVE_8259A_COM		0x08
#define SLAVE_8259A_DATA	0x0a

#define INT_KEYBOARD		33
#define INT_HD			41		/* SASI type */
#define	INT_FD			43		/* 1M type */

extern int	wait_int (int *flag);

extern void	int33_handler (void);
extern void	int43_handler (void);
extern void	init_8259A (void);
extern void	interrupt (int intn);
extern void	trap (int intn);
extern void	fault (int intn);
extern void	intr_keyboard (void);
extern void	open_interrupt (void);
extern void	ignore_handler (void);

#endif __INTERRUPT_H__

 
