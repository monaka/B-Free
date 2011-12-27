/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/rs232c/rs232c_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: rs232c_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1998-12-07 14:29:00  night
 * 各シリアルポートの ベース I/O アドレスを指定する COMx_BASE マクロを
 * 追加。
 * 分周レジスタを指定するときに使用する UART_FRE マクロを追加。
 * マクロ UART_IER のコメント変更(割り込みイネーブルレジスタ -> 割り込み
 * 許可レジスタ)。
 *
 * Revision 1.1  1998/12/04 17:05:07  night
 * first version.
 *
 *
 *
 */

/* RS232C ドライバで使用する定数等の定義。
 *
 */

#ifndef	__RS232C_INTERNAL_H__
#define	__RS232C_INTERNAL_H__	1

#define COM0_BASE	0x03f8
#define COM1_BASE	0x02f8
#define COM2_BASE	0x03e8
#define COM3_BASE	0x02e8

#define RS232C_IRQ	


/* RS232C (8250) のレジスタ
 */
#define UART_RX		0	/* In:  受信バッファ */
#define UART_TX		0	/* Out: 送信バッファ */
#define UART_FRE	0	/* 分周レジスタ */
#define UART_DLL	0	/* Out: Divisor Latch Low (DLAB=1) */
#define UART_DLM	1	/* Out: Divisor Latch High (DLAB=1) */
#define UART_IER	1	/* Out: 割り込み許可レジスタ */
#define UART_IIR	2	/* In:  割り込み認識レジスタ */
#define UART_FCR	2	/* Out: FIFO 制御レジスタ */
#define UART_EFR	2	/* I/O: Extended Features Register */
				/* (DLAB=1, 16C660 only) */
#define UART_LCR	3	/* Out: 回線制御レジスタ */
#define UART_MCR	4	/* Out: モデム制御レジスタ */
#define UART_LSR	5	/* In:  回路ステータスレジスタ */
#define UART_MSR	6	/* In:  モデムステータスレジスタ */
#define UART_SCR	7	/* I/O: Scratch Register */



#endif /* __RS232C_INTERNAL_H__ */
