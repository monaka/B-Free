/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
 * �ƥ��ꥢ��ݡ��Ȥ� �١��� I/O ���ɥ쥹����ꤹ�� COMx_BASE �ޥ����
 * �ɲá�
 * ʬ���쥸��������ꤹ��Ȥ��˻��Ѥ��� UART_FRE �ޥ�����ɲá�
 * �ޥ��� UART_IER �Υ������ѹ�(�����ߥ��͡��֥�쥸���� -> ������
 * ���ĥ쥸����)��
 *
 * Revision 1.1  1998/12/04 17:05:07  night
 * first version.
 *
 *
 *
 */

/* RS232C �ɥ饤�Фǻ��Ѥ���������������
 *
 */

#ifndef	__RS232C_INTERNAL_H__
#define	__RS232C_INTERNAL_H__	1

#define COM0_BASE	0x03f8
#define COM1_BASE	0x02f8
#define COM2_BASE	0x03e8
#define COM3_BASE	0x02e8

#define RS232C_IRQ	


/* RS232C (8250) �Υ쥸����
 */
#define UART_RX		0	/* In:  �����Хåե� */
#define UART_TX		0	/* Out: �����Хåե� */
#define UART_FRE	0	/* ʬ���쥸���� */
#define UART_DLL	0	/* Out: Divisor Latch Low (DLAB=1) */
#define UART_DLM	1	/* Out: Divisor Latch High (DLAB=1) */
#define UART_IER	1	/* Out: �����ߵ��ĥ쥸���� */
#define UART_IIR	2	/* In:  ������ǧ���쥸���� */
#define UART_FCR	2	/* Out: FIFO ����쥸���� */
#define UART_EFR	2	/* I/O: Extended Features Register */
				/* (DLAB=1, 16C660 only) */
#define UART_LCR	3	/* Out: ��������쥸���� */
#define UART_MCR	4	/* Out: ��ǥ�����쥸���� */
#define UART_LSR	5	/* In:  ��ϩ���ơ������쥸���� */
#define UART_MSR	6	/* In:  ��ǥॹ�ơ������쥸���� */
#define UART_SCR	7	/* I/O: Scratch Register */



#endif /* __RS232C_INTERNAL_H__ */
