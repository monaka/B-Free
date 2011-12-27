/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/**************************************************************************
 *
 */

#ifndef __DMA_H__
#define __DMA_H__	1

#define DMA_BUFF	0x30000

#define DMA_WRITE_COMMAND	0x11
#define DMA_WRITE_MODE		0x17
#define DMA_WRITE_REQUEST	0x13
#define DMA_WRITE_SINGLE_MASK	0x15
#define DMA_WRITE_ALL_MASK	0x1f
#define DMA_READ_STATUS		0x11
#define DMA_CHANNEL0_ADDR	0x01
#define DMA_CHANNEL0_COUNT	0x03
#define DMA_CHANNEL1_ADDR	0x05
#define DMA_CHANNEL1_COUNT	0x07
#define DMA_CHANNEL2_ADDR	0x09
#define DMA_CHANNEL2_COUNT	0x0b
#define DMA_CHANNEL3_ADDR	0x0d
#define DMA_CHANNEL3_COUNT	0x0f
#define DMA_CHANNEL0_BANK	0x27
#define DMA_CHANNEL1_BANK	0x21
#define DMA_CHANNEL2_BANK	0x23
#define DMA_CHANNEL3_BANK	0x25
#define DMA_CLEAR_MASK		0x1d
#define DMA_READ_TMP		0x1b
#define DMA_CLEAR_BYTE		0x19
#define DMA_MASTER_CLEAR	0x1b
#define DMA_BANKADDR_MODE	0x29


#define DMA_READ		0x46		/* I/O -> memory */
#define DMA_WRITE		0x4a		/* memory -> I/O */
#define DMA_MASK		0x03

extern int	setup_dma (UW chan, void *addr, int mode, int length, int mask);

#endif __DMA_H__
