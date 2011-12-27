/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/ne2000_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: ne2000_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  1999-04-20 16:25:53  kishida0
 * *** empty log message ***
 *
 * Revision 1.6  1999/03/07 12:49:28  night
 * 構造体 ne2000_device の要素を変更。
 *
 * +  UW		tx_error;
 * +  UW		collision;
 * -  UW		error_count;
 * +  UW		rx_error;
 *
 * Revision 1.5  1999/03/04 15:51:59  night
 * マクロ追加。
 * MAX_MTU_LENGTH		NE2000 アダプタに渡すことができるデータの最大長
 * MIN_ETHER_LENGTH	ETHERNET におけるデータの最小長
 *
 * Revision 1.4  1999/03/02 15:19:18  night
 * 構造体 struct device の名前を struct ne2000_device に変更。中身も
 * 変更した。
 *
 * 次のマクロを新たに定義した。
 * #define LOC_MEM_START		0x4000
 * #define LOC_MEM_END		0x8000
 * #define LOC_MEM_SIZE		0x4000
 * #define LOC_RX_MEM_START	0x4c00
 * #define TX_PAGE_START		64
 * #define RX_PAGE_START		76
 * #define RX_PAGE_END		128
 * #define NE2000_PAGE_SIZE	256
 * #define TX_BUF_SIZE		6
 * #define MAX_TX_BUF		2
 *
 * Revision 1.3  1999/02/28 22:14:27  night
 * コメントの変更。
 *
 * Revision 1.2  1997/10/29 22:39:27  night
 * NE2000 アダプタの SA_PROM の読み取り処理の追加。
 *
 * Revision 1.1  1997/10/28 12:47:23  night
 * 最初の登録
 *
 *
 *
 */


#ifndef	__NE2000_INTERNAL_H__
#define	__NE2000_INTERNAL_H__	1


#include "h/itron.h"
#include "h/errno.h"
#include "../console/console.h"
#include "servers/port-manager.h"



/* I/O ポートの定義 */
#define IO_BASE(dev)		(dev->io_base)
#define IO_CMD(dev)		(dev->io_base + 0x00)
#define IO_DATA(dev)		(dev->io_base + 0x10)
#define IO_RESET(dev)		(dev->io_base + 0x1f)
#define IO_EXTENT(dev)		(dev->io_base + 0x20)
#define IO_ISR(dev)		(dev->io_base + 0x07)

/* データバッファの定義 */
#define IOD_START_PAGE		0x20
#define IOD_STOP_PAGE		0x40



/* 8390 のコマンドリスト */
#define E8390_STOP		0x01		/* 8390 のストップ */
#define E8390_START		0x02		/* チップの実行開始 */
#define E8390_TRANS		0x04		/* データの転送 */
#define E8390_RREAD		0x08
#define E8390_RWRITE		0x10
#define E8390_NODMA		0x20
#define E8390_PAGE0		0x00
#define E8390_PAGE1		0x40
#define E8390_PAGE2		0x80
#define E8390_CMD		0x00		/* コマンドレジスタ */

#define E8390_TX_IRQ_MASK	0xa	/* For register EP0_ISR */
#define E8390_RX_IRQ_MASK  	0x5
#define E8390_RXCONFIG		0x4	/* EN0_RXCR: broadcasts, no multicast,errors */
#define E8390_RXOFF		0x20	/* EN0_RXCR: Accept no packets */
#define E8390_TXCONFIG		0x00	/* EN0_TXCR: Normal transmit mode */
#define E8390_TXOFF		0x02	/* EN0_TXCR: Transmitter off */



/* 8390 のレジスタリスト */
#define EP0_CLDALO	0x01	/* Low byte of current local dma addr  RD */
#define EP0_STARTPG	0x01	/* Starting page of ring bfr WR */
#define EP0_CLDAHI	0x02	/* High byte of current local dma addr  RD */
#define EP0_STOPPG	0x02	/* Ending page +1 of ring bfr WR */
#define EP0_BOUNDARY	0x03	/* Boundary page of ring bfr RD WR */
#define EP0_TSR		0x04	/* Transmit status reg RD */
#define EP0_TPSR	0x04	/* Transmit starting page WR */
#define EP0_NCR		0x05	/* Number of collision reg RD */
#define EP0_TCNTLO	0x05	/* Low  byte of tx byte count WR */
#define EP0_FIFO	0x06	/* FIFO RD */
#define EP0_TCNTHI	0x06	/* High byte of tx byte count WR */
#define EP0_ISR		0x07	/* Interrupt status reg RD WR */
#define EP0_CRDALO	0x08	/* low byte of current remote dma address RD */
#define EP0_RSARLO	0x08	/* Remote start address reg 0 */
#define EP0_CRDAHI	0x09	/* high byte, current remote dma address RD */
#define EP0_RSARHI	0x09	/* Remote start address reg 1 */
#define EP0_RCNTLO	0x0a	/* Remote byte count reg WR */
#define EP0_RCNTHI	0x0b	/* Remote byte count reg WR */
#define EP0_RSR		0x0c	/* rx status reg RD */
#define EP0_RXCR	0x0c	/* RX configuration reg WR */
#define EP0_TXCR	0x0d	/* TX configuration reg WR */
#define EP0_COUNTER0	0x0d	/* Rcv alignment error counter RD */
#define EP0_DCFG	0x0e	/* Data configuration reg WR */
#define EP0_COUNTER1	0x0e	/* Rcv CRC error counter RD */
#define EP0_IMR		0x0f	/* Interrupt mask reg WR */
#define EP0_COUNTER2	0x0f	/* Rcv missed frame error counter RD */

/* Bits in EP0_ISR - Interrupt status register */
#define EPISR_RX	0x01	/* Receiver, no error */
#define EPISR_TX	0x02	/* Transmitter, no error */
#define EPISR_RX_ERR	0x04	/* Receiver, with error */
#define EPISR_TX_ERR	0x08	/* Transmitter, with error */
#define EPISR_OVER	0x10	/* Receiver overwrote the ring */
#define EPISR_COUNTERS	0x20	/* Counters need emptying */
#define EPISR_RDC	0x40	/* remote dma complete */
#define EPISR_RESET	0x80	/* Reset completed */
#define EPISR_ALL	0x3f	/* Interrupts we will enable */

/* Bits in EP0_DCFG - Data config register */
#define EPDCFG_WTS	0x01	/* word transfer mode selection */

/* Page 1 register offsets. */
#define EP1_PHYS   0x01	/* This board's physical enet addr RD WR */
#define EP1_CURPAG 0x07	/* Current memory page RD WR */
#define EP1_MULT   0x08	/* Multicast filter mask array (8 bytes) RD WR */

/* Bits in received packet status byte and EP0_RSR*/
#define EPRSR_RXOK	0x01	/* Received a good packet */
#define EPRSR_CRC	0x02	/* CRC error */
#define EPRSR_FAE	0x04	/* frame alignment error */
#define EPRSR_FO	0x08	/* FIFO overrun */
#define EPRSR_MPA	0x10	/* missed pkt */
#define EPRSR_PHY	0x20	/* physical/multicase address */
#define EPRSR_DIS	0x40	/* receiver disable. set in monitor mode */
#define EPRSR_DEF	0x80	/* deferring */

/* Transmitted packet status, EP0_TSR. */
#define EPTSR_PTX 0x01	/* Packet transmitted without error */
#define EPTSR_ND  0x02	/* The transmit wasn't deferred. */
#define EPTSR_COL 0x04	/* The transmit collided at least once. */
#define EPTSR_ABT 0x08  /* The transmit collided 16 times, and was deferred. */
#define EPTSR_CRS 0x10	/* The carrier seese was lost. */
#define EPTSR_FU  0x20  /* A "FIFO underrun" occurred during transmit. */
#define EPTSR_CDH 0x40	/* The collision detect "heartbeat" signal was lost. */
#define EPTSR_OWC 0x80  /* There was an out-of-window collision. */


#define LOC_MEM_START		0x4000
#define LOC_MEM_END		0x8000
#define LOC_MEM_SIZE		0x4000
#define LOC_RX_MEM_START	0x4c00

#define TX_PAGE_START		64
#define RX_PAGE_START		76
#define RX_PAGE_END		128

#define NE2000_PAGE_SIZE	256
#define TX_BUF_SIZE		6
#define MAX_TX_BUF		2

#define MAX_MTU_LENGTH   	1514
#define MIN_ETHER_LENGTH	64

#define MAC_ADDR_LENGTH		6


/* デバイス構造体 
 * NE2000 デバイスは、各々この構造体で管理している
 */
struct ne2000_device
{
  /* ボートの定義情報 */
  UW		io_base;

  /* ボートの情報 */
  UB		sa_prom[32];	/* SAPROM の内容のコピー(実は、最初の 16 バイトしか必要でない) */

  /* 送信バッファ情報 */
  B		tx_busy;
  B		tx_use;
  B		tx_new;
  B		tx_next;
  B		tx_len[MAX_TX_BUF];
  
  /* 受信バッファ情報 */
  B		rx_next;

  B		word_access;	/* 16bit/8bit の識別(8bit は使えないということにしている) */
  
  /* statistics */
  UW		tx_count;
  UW		tx_error;
  UW		collision;

  UW		rx_count;
  UW		rx_error;


  /* 割り込み番号 */
  UB		intr_number;

  /* 送受信割り込みハンドラ */
  FP		intr_handler;
};



#include "global.h"
#include "funcs.h"


#endif /* __NE2000_INTERNAL_H__ */
