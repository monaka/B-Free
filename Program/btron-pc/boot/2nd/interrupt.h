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
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/interrupt.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: interrupt.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1998-11-20 08:02:31  monaka
 * *** empty log message ***
 *
 * Revision 1.4  1996/07/30 18:22:57  night
 * IDE HD 用の定数類の定義を追加。
 *
 * Revision 1.3  1996/07/06  13:09:40  night
 * Floppy disk 関連の割り込み番号の定義を PC 互換機用に変更
 *
 * Revision 1.2  1996/05/11  15:50:13  night
 * 割り込みコントローラの I/O アドレスを変更。
 *
 * Revision 1.1  1996/05/11  10:45:03  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * Revision 1.2  1995/09/21 15:50:39  night
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

#define MASTER_8259A_COM	0x20
#define MASTER_8259A_DATA	0x21

#define SLAVE_8259A_COM		0xa0
#define SLAVE_8259A_DATA	0xa1

#define INT_KEYBOARD		33
#if 0
#define INT_HD			41		/* SASI type */
#endif
#define	INT_FD			38		/* 1M type */
#define	INT_IDE			46		/* IDE HD */
#define	INT_IDE2		47		/* IDE HD */

extern int	wait_int (int *flag);

extern void	int33_handler (void);
extern void	int43_handler (void);
extern void	int46_handler (void);
extern void	int47_handler (void);
extern void	int38_handler (void);	/* floppy disk */
extern void	init_8259A (void);
extern void	interrupt (int intn);
extern void	trap (int intn);
extern void	fault (int intn);
extern void	intr_keyboard (void);
extern void	open_interrupt (void);
extern void	ignore_handler (void);

extern void	intr_ide ();

#endif __INTERRUPT_H__

 
