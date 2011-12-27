/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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


/* ＦＤ形式:
 *	BTRON_FD	BTRON 形式の FD(ブロック番号が + 1 される)。
 *	OTHER_FD	BTRON 形式の以外の FD
 */
#define BTRON_FD	       	0
#define M2HD			0
#define M2HC			1
#define OTHER_FD		2

/* device control
 */
#define NOWORK			0
#define CHANGE_MODE		1	/* FD 形式の切り替え */

/*********************************************************************
 *
 *	FDC 制御用定数の定義
 *
 */
/*
 * ポートアドレス
 */
#define FDC_CMD		0x92	/* コマンドレジスタ・ポート	*/
#define FDC_STAT	0x90	/* ステータスレジスタ・ポート	*/
#define FDC_DATA	0x92	/* データ読み取りポート		*/
#define FDC_EXTERNAL	0xBE	/* 外部レジスタへのアドレス	*/
#define FDC_WCNTL	0x94


/*	status register 
 *
 *	+---+---+---+---+---+---+---+---+
 *	|RQM|DIN|NDM| CB|D3B|D2B|D1B|D0B|
 *	+---+---+---+---+---+---+---+---+
 *
 *	RQM...	Request of Master 
 *		DIO が 0 のとき 
 *		  1 ... コマンド書き込み要求
 *		DIO が 1 のとき 
 *		  1 ... データ読み取り要求
 *	DIO...	Data In/Out.	
 *		0 ... データを送れる
 *		1 ... FDC に読み取られるべきデータがある
 *	NDM...	Non DMA Mode.
 *		0 ... DMA をつかってデータ転送をしている。
 *		1 ... DMA をつかわないでデータ転送をしている。
 *	CB....	FDC Busy.
 *		0 ... FDC は、コマンドを受け付ける。
 *		1 ... FDC は、コマンド実行中あるいは、リザルトモードにある。
 *	D3B...	FD3 Busy.
 *		1 ... ＦＤユニット 3 は、seek 中あるいは seek 割り込み保留中.
 *		0 ... ＦＤユニット 3 は、seek 中あるいは seek 割り込み
 *		      保留中でない。
 *	D2B...	FD2 Busy.
 *		1 ... ＦＤユニット 2 は、seek 中あるいは seek 割り込み保留中.
 *		0 ... ＦＤユニット 2 は、seek 中あるいは seek 割り込み
 *		      保留中でない。
 *	D1B...	FD1 Busy.
 *		1 ... ＦＤユニット 1 は、seek 中あるいは seek 割り込み保留中.
 *		0 ... ＦＤユニット 1 は、seek 中あるいは seek 割り込み
 *		      保留中でない。
 *	D0B...	FD0 Busy.
 *		1 ... ＦＤユニット 0 は、seek 中あるいは seek 割り込み保留中.
 *		0 ... ＦＤユニット 0 は、seek 中あるいは seek 割り込み
 *		      保留中でない。
 *
 */
#define FD0_BUSY	0x01	/* FD0 は、seek 中			*/
#define FD1_BUSY	0x02	/* FD1 は、seek 中			*/
#define FD2_BUSY	0x04	/* FD2 は、seek 中			*/
#define FD3_BUSY	0x08	/* FD3 は、seek 中			*/
#define FDC_BUSY	0x10	/* FDC は、コマンド実行中		*/
#define FDC_NDM		0x20	/* non-DMA モードでデータ転送中		*/
#define FDC_DIN		0x40	/* FDC に読み取るべきデータがある	*/
#define FDC_DOUT	0x00	/* FDC にデータが書き込み可能		*/
#define FDC_MASTER	0x80	/* データの読み取り/書き込み要求	*/

/* FDC COMMANDS */
#define FDC_SPECIFY	0x03	/* Specify コマンド			*/
#define FDC_READ	0xE6	/* <MT><MF><SK> 0 0 1 1 0		*/
#define FDC_WRITE	0xC5	/* <MT><MF>  0  0 0 1 0 1		*/
#define FDC_SEEK	0x0f	/* Seek コマンド			*/
#define FDC_RECALIBRATE	0x07	/* Recalibrarete コマンド		*/
#define FDC_DEVSTAT	0x04
#define FDC_SENSE	0x08

/* Write control registers bit. IO port address 0x94 */
#define FDC_RESET	0x80
#define FDC_FRY		0x40
#define FDC_DMAE	0x10
#define FDC_MTON	0x08

/* リザルトステータスレジスタの値の定義 (マスク値)
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
#define HD_GAP		0x35		/* HD の場合の GAP3 の値 */
#define HD_DTL		0xff

#define HC_HEAD		2
#define HC_EXTERN	3
#define HC_SECTOR	15
#define HC_LENGTH	512
#define HC_LENCODE	3
#define HC_GAP		0x35		/* HC の場合の GAP3 の値 */
#define HC_DTL		0xff


#define DD_TYPE		1

/*
 * ドライバ ステータス定数
 *	これらの定数は、fd_data[] の status メンバに入る。
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
  B	*typename;	/* タイプ名; */
  UW	head;		/* ヘッドの数 */
/*  UW	cylinder;	/* シリンダ数 */
  UW	sector;		/* セクタ数 / トラック */
  UW	length;		/* セクタ長*/
  UW	dtl;		/* セクタ長 (lencode = 0 の時に使用)。使用しない場合には 0xFF */
  UW	lencode;	/* セクタ長のコード */
  UW	gap3;		/* セクタ間 GAP */
  UW	gapskip;	/* セクタ間 GAP の読み飛ばしバイト数 */
  UW	srt;		/* */
  UW	hut;		/* */
  UW	hlt;		/* */
  UW	nd;		/* */
  UW	dmachan;	/* DMA チャネル */
  UW	retry;		/* 最大リトライ数 */	
  UW	motor;		/* */
};


/* fd.c */
extern W    	init_fd(void);	/* 初期化		*/
extern W    	open_fd();	/* オープン		*/
extern W  	close_fd();	/* クローズ		*/
extern W    	read_fd();	/* 読み込み		*/
extern W    	write_fd();	/* 書き込み		*/
extern W    	control_fd();	/* コントロール		*/
extern W    	change_fd();	/* アクセスモード切り替え */
extern W    	status_fd();	/* ステータス		*/
extern W	intr_flag;
extern struct spec	*fd_data[];

/* fdc.c */
extern void    	intr_fd();	/* 割り込みハンドラ	*/
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


