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

#define DMA_INIT		0x0A
#define DMA_WRITE_COMMAND	0x08
#define DMA_WRITE_MODE		0x0B
#define DMA_WRITE_REQUEST	0x09
#define DMA_WRITE_SINGLE_MASK	0x0A
#define DMA_WRITE_ALL_MASK	0x0F
#define DMA_READ_STATUS		0x08

#define DMA_CHANNEL0_ADDR	0x00
#define DMA_CHANNEL0_COUNT	0x01
#define DMA_CHANNEL1_ADDR	0x02
#define DMA_CHANNEL1_COUNT	0x04
#define DMA_CHANNEL2_ADDR	0x04	/* for FD */
#define DMA_CHANNEL2_COUNT	0x05	/* for FD */
#define DMA_CHANNEL3_ADDR	0x07
#define DMA_CHANNEL3_COUNT	0x08

#define DMA_CHANNEL0_BANK	0x87
#define DMA_CHANNEL1_BANK	0x83
#define DMA_CHANNEL2_BANK	0x81
#define DMA_CHANNEL3_BANK	0x82

#define DMA_CLEAR_MASK		0x0E
#define DMA_READ_TMP		0x0D
#define DMA_CLEAR_BYTE		0x0C
#define DMA_MASTER_CLEAR	0x0D

#define DMA_BANKADDR_MODE	0x29


#define DMA_READ		0x44		/* I/O -> memory */
#define DMA_WRITE		0x48		/* memory -> I/O */
#define DMA_MASK		0x02		/* ?? */

extern int	setup_dma (void *addr, int mode, int length, int mask);

#endif __DMA_H__

