/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/fd.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 *
 * 
 *
 *
 */

#ifndef __FD_H__
#define __FD_H__	1


#include "h/itron.h"
#include <h/types.h>
#include <h/errno.h>
#include <pc9801/pc98.h>
#include "dma.h"
#include "device.h"

#define ROUNDDOWN(x,b)	((x / b) * b)
#define ROUNDUP(x,b)	((x + (b -1)) / b * b)

#define MAXFD		2

#define FD_DRIVER	"driver.fd"

#define BLOCK_SIZE	1024

#define FD_INT_VECTOR	43
#define FD_MASK_LEVEL	0

#define MAX_FD		2


/* �ƣķ���:
 *	BTRON_FD	BTRON ������ FD(�֥�å��ֹ椬 + 1 �����)��
 *	OTHER_FD	BTRON �����ΰʳ��� FD
 */
#define BTRON_FD	       	0
#define M2HD			0
#define M2HC			1
#define OTHER_FD		2

/* device control
 */
#define NOWORK			0
#define CHANGE_MODE		1	/* FD �������ڤ��ؤ� */

/*********************************************************************
 *
 *	FDC ��������������
 *
 */
/*
 * �ݡ��ȥ��ɥ쥹
 */
#define FDC_CMD		0x92	/* ���ޥ�ɥ쥸�������ݡ���	*/
#define FDC_STAT	0x90	/* ���ơ������쥸�������ݡ���	*/
#define FDC_DATA	0x92	/* �ǡ����ɤ߼��ݡ���		*/
#define FDC_EXTERNAL	0xBE	/* �����쥸�����ؤΥ��ɥ쥹	*/
#define FDC_WCNTL	0x94


/*	status register 
 *
 *	+---+---+---+---+---+---+---+---+
 *	|RQM|DIN|NDM| CB|D3B|D2B|D1B|D0B|
 *	+---+---+---+---+---+---+---+---+
 *
 *	RQM...	Request of Master 
 *		DIO �� 0 �ΤȤ� 
 *		  1 ... ���ޥ�ɽ񤭹����׵�
 *		DIO �� 1 �ΤȤ� 
 *		  1 ... �ǡ����ɤ߼���׵�
 *	DIO...	Data In/Out.	
 *		0 ... �ǡ����������
 *		1 ... FDC ���ɤ߼����٤��ǡ���������
 *	NDM...	Non DMA Mode.
 *		0 ... DMA ��Ĥ��äƥǡ���ž���򤷤Ƥ��롣
 *		1 ... DMA ��Ĥ���ʤ��ǥǡ���ž���򤷤Ƥ��롣
 *	CB....	FDC Busy.
 *		0 ... FDC �ϡ����ޥ�ɤ�����դ��롣
 *		1 ... FDC �ϡ����ޥ�ɼ¹��椢�뤤�ϡ��ꥶ��ȥ⡼�ɤˤ��롣
 *	D3B...	FD3 Busy.
 *		1 ... �ƣĥ�˥å� 3 �ϡ�seek �椢�뤤�� seek ��������α��.
 *		0 ... �ƣĥ�˥å� 3 �ϡ�seek �椢�뤤�� seek ������
 *		      ��α��Ǥʤ���
 *	D2B...	FD2 Busy.
 *		1 ... �ƣĥ�˥å� 2 �ϡ�seek �椢�뤤�� seek ��������α��.
 *		0 ... �ƣĥ�˥å� 2 �ϡ�seek �椢�뤤�� seek ������
 *		      ��α��Ǥʤ���
 *	D1B...	FD1 Busy.
 *		1 ... �ƣĥ�˥å� 1 �ϡ�seek �椢�뤤�� seek ��������α��.
 *		0 ... �ƣĥ�˥å� 1 �ϡ�seek �椢�뤤�� seek ������
 *		      ��α��Ǥʤ���
 *	D0B...	FD0 Busy.
 *		1 ... �ƣĥ�˥å� 0 �ϡ�seek �椢�뤤�� seek ��������α��.
 *		0 ... �ƣĥ�˥å� 0 �ϡ�seek �椢�뤤�� seek ������
 *		      ��α��Ǥʤ���
 *
 */
#define FD0_BUSY	0x01	/* FD0 �ϡ�seek ��			*/
#define FD1_BUSY	0x02	/* FD1 �ϡ�seek ��			*/
#define FD2_BUSY	0x04	/* FD2 �ϡ�seek ��			*/
#define FD3_BUSY	0x08	/* FD3 �ϡ�seek ��			*/
#define FDC_BUSY	0x10	/* FDC �ϡ����ޥ�ɼ¹���		*/
#define FDC_NDM		0x20	/* non-DMA �⡼�ɤǥǡ���ž����		*/
#define FDC_DIN		0x40	/* FDC ���ɤ߼��٤��ǡ���������	*/
#define FDC_DOUT	0x00	/* FDC �˥ǡ������񤭹��߲�ǽ		*/
#define FDC_MASTER	0x80	/* �ǡ������ɤ߼��/�񤭹����׵�	*/

/* FDC COMMANDS */
#define FDC_SPECIFY	0x03	/* Specify ���ޥ��			*/
#define FDC_READ	0xE6	/* <MT><MF><SK> 0 0 1 1 0		*/
#define FDC_WRITE	0xC5	/* <MT><MF>  0  0 0 1 0 1		*/
#define FDC_SEEK	0x0f	/* Seek ���ޥ��			*/
#define FDC_RECALIBRATE	0x07	/* Recalibrarete ���ޥ��		*/
#define FDC_DEVSTAT	0x04
#define FDC_SENSE	0x08

/* Write control registers bit. IO port address 0x94 */
#define FDC_RESET	0x80
#define FDC_FRY		0x40
#define FDC_DMAE	0x10
#define FDC_MTON	0x08

/* �ꥶ��ȥ��ơ������쥸�������ͤ���� (�ޥ�����)
 *
 * ST0	+---+---+---+---+---+---+---+---+
 *	|  IC   |SE |EC |NR |HD |US1|US0|
 *	+---+---+---+---+---+---+---+---+
 * 	IC	Interrupt Code
 *	SE	Seek End
 *	EC	Equipment Check
 *	NR	Not Ready
 *	HD	Head Address
 *	US1	Unit Select1
 *	US0	Unit Select0
 *
 *
 * ST1	+---+---+---+---+---+---+---+---+
 *	|EN | 0 |DE |OR | 0 |ND |NW |MA |
 *	+---+---+---+---+---+---+---+---+
 *	EN	End of Cylinder
 *	DE	Data Error
 *	OR	OverRun
 *	ND	No Data
 *	NW	Not Writable
 *	MA	Missing Address Mark
 *
 *
 * ST2	+---+---+---+---+---+---+---+---+
 *	| 0 |CM |DD |NC |SH |SN |BC |MD |
 *	+---+---+---+---+---+---+---+---+
 *	CM	Control Mark
 *	DD	Data error in Data Field
 *	NC	No Cylinder
 *	SH	Scan Equal Hit
 *	SN	Scan Not Satisfied
 *	BC	Bad Cylinder
 *	MD	Missing Address Mark in Data Field
 *
 *
 * ST3	+---+---+---+---+---+---+---+---+
 *	|FT |WP |RY |T0 |TS |HD |US1|US0|
 *	+---+---+---+---+---+---+---+---+
 *	FT	Fault
 *	WP	Write Protect
 *	RY	Ready
 *	T0	Track 0
 *	TS	Two Side
 *	HD	Head Address
 *	US1	Unit Select 1
 *	US0	Unit Select 0
 *
 *
 */
#define ST0_IC			0xC0
#define 	ST0_IC_NT	0x00
#define 	ST0_IC_AT	0x40
#define		ST0_IC_IC	0x80
#define		ST0_IC_AI	0xC0
#define ST0_SE			0x20
#define ST0_EC			0x10
#define ST0_NR			0x08
#define ST0_HD			0x04
#define ST0_US			0x03
#define ST0_US1			0x02
#define ST0_US0			0x01

#define ST1_EN			0x80
#define ST1_DE			0x20
#define ST1_OR			0x10
#define ST1_ND			0x04
#define ST1_NW			0x02
#define ST1_MA			0x01

#define ST2_CM			0x40
#define ST2_DD			0x20
#define ST2_NC			0x10
#define ST2_SH			0x08
#define ST2_SN			0x04
#define ST2_BC			0x02
#define ST2_MD			0x01

#define ST3_FT			0x80
#define ST3_WP			0x40
#define ST3_RY			0x20
#define ST3_T0			0x10
#define ST3_TS			0x08
#define ST3_HD			0x04
#define ST3_US			0x03
#define ST3_US1			0x02
#define ST3_US0			0x01

#define FD0		0x00
#define FD1		0x01

#define HEAD0		0x0
#define HEAD1		0x1

#define HD_HEAD		2
#define HD_EXTERN	3
#define HD_SECTOR	8
#define HD_LENGTH	1024
#define HD_LENCODE	3
#define HD_GAP		0x35		/* HD �ξ��� GAP3 ���� */
#define HD_DTL		0xff

#define HC_HEAD		2
#define HC_EXTERN	3
#define HC_SECTOR	15
#define HC_LENGTH	512
#define HC_LENCODE	3
#define HC_GAP		0x35		/* HC �ξ��� GAP3 ���� */
#define HC_DTL		0xff


#define DD_TYPE		1

/*
 * �ɥ饤�� ���ơ��������
 *	����������ϡ�fd_data[] �� status ���Ф����롣
 *
 */
#define DRIVE_FREE		0x00
#define DRIVE_USING		0x80
#define DRIVE_BUSY		0x01
#define DRIVE_WAIT_INT		0x02
#define DRIVE_DMA		0x04
#define DRIVE_NOINIT		0x08
#define ON_MOTOR		1
#define OFF_MOTOR		0

#define MAX_FD_RETRY		10


struct spec
{
  B	*typename;	/* ������̾; */
  UW	head;		/* �إåɤο� */
/*  UW	cylinder;	/* �������� */
  UW	sector;		/* �������� / �ȥ�å� */
  UW	length;		/* ������Ĺ*/
  UW	dtl;		/* ������Ĺ (lencode = 0 �λ��˻���)�����Ѥ��ʤ����ˤ� 0xFF */
  UW	lencode;	/* ������Ĺ�Υ����� */
  UW	gap3;		/* �������� GAP */
  UW	gapskip;	/* �������� GAP ���ɤ����Ф��Х��ȿ� */
  UW	srt;		/* */
  UW	hut;		/* */
  UW	hlt;		/* */
  UW	nd;		/* */
  UW	dmachan;	/* DMA ����ͥ� */
  UW	retry;		/* �����ȥ饤�� */	
  UW	motor;		/* */
};


/* fd.c */
extern W    	init_fd(void);	/* �����		*/
extern W    	open_fd();	/* �����ץ�		*/
extern W  	close_fd();	/* ������		*/
extern W    	read_fd();	/* �ɤ߹���		*/
extern W    	write_fd();	/* �񤭹���		*/
extern W    	control_fd();	/* ����ȥ���		*/
extern W    	change_fd();	/* ���������⡼���ڤ��ؤ� */
extern W    	status_fd();	/* ���ơ�����		*/
extern W	intr_flag;
extern struct spec	*fd_data[];

/* fdc.c */
extern void    	intr_fd();	/* �����ߥϥ�ɥ�	*/
extern void	reset_intr_mask ();
extern W	wait_int (W *flag);
extern W	write_fdc (UW value);
extern W	read_fdc (void);
extern W	ready_check (void);
extern W	reset_fdc (W drive);
extern W	sense_interrupt (W drive, UW *result);
extern W	recalibrate (W drive);
extern W	sense_fdc (W drive);
extern W	specify (W drive,UW srt,UW hut,UW hlt,UW nd);
extern W	read_result (W drive, UW *tab, W n);
extern void	on_motor (W drive);
extern void	stop_motor (W drive);
extern W	seek (W drive, W cylinder);
extern W	get_data (W drive, W head, W cylinder, W sector, B *buff);
extern W	put_data (W drive, W head, W cylinder, W sector, B *buff);

/* spec.c */
extern struct spec *get_fdspec (B *type);
extern struct spec	fd_spec[];


#endif __IO_FD_H__


