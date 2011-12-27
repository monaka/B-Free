/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT fd routines.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/fd.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: fd.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.9  2000-01-18 14:32:48  naniwa
 * modified fd_seek
 *
 * Revision 1.8  2000/01/08 09:03:41  naniwa
 * minor tune
 *
 * Revision 1.7  1999/12/19 10:57:04  naniwa
 * made disk read check strict
 *
 * Revision 1.6  1999/03/15 01:35:22  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.5  1998/11/20 08:02:27  monaka
 * *** empty log message ***
 *
 * Revision 1.4  1996/07/08 14:51:21  night
 * FD のブロックサイズを 512 バイトから 1024 バイトに変更
 *
 * Revision 1.3  1996/07/06  13:09:14  night
 * Floppy disk 関連の定義を PC 互換機用に変更
 *
 * Revision 1.2  1996/06/11  16:41:15  night
 * FD のハードウェアに関係した定数を IBM-PC に合わせ変更した。
 *
 * Revision 1.1  1996/05/11  10:45:02  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * Revision 1.3  1995/09/21 15:50:38  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1994/07/05  17:19:23  night
 * Change FD format; 512bytes/block.
 *
 * Revision 1.1  1993/10/11  21:28:58  btron
 * btron/386
 *
 * 
 */

#ifndef __FD_H__
#define __FD_H__	1

#define FDC_CMD		0x3F0
#define FDC_STAT	0x3f4
#define FDC_DATA	0x3f5
#define FDC_EXTERNAL	0xBE	/* 外部レジスタへのアドレス */
#define FDC_WCNTL	0x3f2


#define STATUS_BUSYMASK	0x0F		/* drive busy mask */
#define FD0_BUSY	0x01
#define FD1_BUSY	0x02
#define FD2_BUSY	0x04
#define FD3_BUSY	0x08
#define FDC_BUSY	0x10
#define FDC_DIN		0x40
#define FDC_DOUT	0x00
#define FDC_DCR		0x3f7		/* Diskette Control Register (write)*/
/*#define FDC_NDM		0x20 */
#define FDC_MASTER	0x80

/* FDC COMMANDS */
#define FDC_SPECIFY	0x03
#define FDC_READ	0xE6		/* <MT><MF><SK> 0 0 1 1 0 */
#define FDC_SEEK	0x0f
#define FDC_RECALIBRATE	0x07
#define FDC_DEVSTAT	0x04
#define FDC_SENSE	0x08
#define FDC_DEVICE_STAT	0x040


#define FD0		0x00
#define FD1		0x01

#define HEAD0		0x0
#define HEAD1		0x1

#define MOTOR_SHIFT	4


#ifdef SUPPORT_144

#define HD_TYPE		0
#define HD_HEAD		2
#define HD_EXTERN	3
#define HD_SECTOR	18
#define HD_LENGTH	1024
#define HD_LENCODE	2
#ifdef notdef
#define HD_GAP		0x2A		/* HD の場合の GAP3 の値 */
#else
#define HD_GAP		0x1B		/* HD の場合の GAP3 の値 */
#endif
#define HD_DTL		0xff

#else
#ifdef SUPPORT_2HD

#define HD_TYPE		0
#define HD_HEAD		2
#define HD_EXTERN	3
#define HD_SECTOR	8
#define HD_LENGTH	1024
#define HD_LENCODE	3
#define HD_GAP		0x35		/* HD の場合の GAP3 の値 */
#define HD_DTL		0xff

#else	/* 2HC */

#define HD_TYPE		0
#define HD_HEAD		2
#define HD_EXTERN	3
#define HD_SECTOR	15
#define HD_LENGTH	512
#define HD_LENCODE	2
#define HD_GAP		0x1B		/* HC の場合の GAP3 の値 */
#define HD_DTL		0xff

#endif
#endif /* SUPPORT_144 */

#define DD_TYPE		1

#include "types.h"

extern int	init_fd (void);
extern void	intr_fd (void);
extern int	fd_ready_check (void);
extern int	on_motor (BYTE drive);
extern int	stop_motor (BYTE drive);
extern int fd_read (BYTE drive, int blockno, BYTE *buff);
extern int	fd_get_status (BYTE drive, int datan);
extern int	fd_recalibrate (BYTE drive);
extern int	fd_seek (BYTE drive, int head, int cylinder, int motor);
extern int	fd_specify (UWORD32,UWORD32,UWORD32,UWORD32);
extern int	fd_reset (void);


#endif __FD_H__

