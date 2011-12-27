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
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/fd.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: fd.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:50:38  night
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

#define FDC_CMD		0x92
#define FDC_STAT	0x90
#define FDC_DATA	0x92
#define FDC_EXTERNAL	0xBE	/* 外部レジスタへのアドレス */
#define FDC_WCNTL	0x94


#define FD0_BUSY	0x01
#define FD1_BUSY	0x02
#define FD2_BUSY	0x04
#define FD3_BUSY	0x08
#define FDC_BUSY	0x10
#define FDC_NDM		0x20
#define FDC_DIN		0x40
#define FDC_DOUT	0x00
#define FDC_MASTER	0x80

/* FDC COMMANDS */
#define FDC_SPECIFY	0x03
#define FDC_READ	0xE6		/* <MT><MF><SK> 0 0 1 1 0 */
#define FDC_SEEK	0x0f
#define FDC_RECALIBRATE	0x07
#define FDC_DEVSTAT	0x04
#define FDC_SENSE	0x08

#define FD0		0x00
#define FD1		0x01

#define HEAD0		0x0
#define HEAD1		0x1

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

#define DD_TYPE		1

extern int	init_fd (void);
extern void	intr_fd (void);
extern int	fd_ready_check (void);
extern int	on_motor (void);
extern int	off_motor (int drive);
extern int	fd_get_status (int drive, int datan);
extern int	fd_recalibrate (int drive);
extern int	fd_seek (int drive, int cylinder);
extern int	fd_specify (unsigned int,
			    unsigned int,
			    unsigned int,
			    unsigned int);
extern int	fd_reset (void);

#endif __FD_H__

