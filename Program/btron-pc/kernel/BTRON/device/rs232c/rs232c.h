/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/rs232c/rs232c.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: rs232c.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-04-18 18:43:35  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1998/12/07 14:21:52  night
 * ボーレート指定用マクロ (Bxxxx) の追加。
 *
 * Revision 1.1  1997/05/17 10:47:03  night
 * 最初の登録
 *
 *
 */

/*
 *
 */
#ifndef	__RS232C_H__
#define	__RS232C_H__	1


#define RS232C_DRIVER	"driver.rs232c"


#include "h/itron.h"
#include "h/errno.h"
#include "../console/console.h"
#include "servers/port-manager.h"


#define B50	0
#define B75	1
#define B150	2
#define B300	3
#define B600	4
#define B1200	5
#define B2400	6
#define B4800	7
#define B9600	8
#define B19200	9
#define B38400	10
#define B57600	11
#define B115200	12



/* misc.c */
extern void	init_log (void);
extern void	print_digit (UW d, UW base);
extern W	printf (B *fmt,...);
extern W	put_string (B *line, ID port);
extern W 	putc (int ch, ID port);



#endif /* __NE2000_H__ */
