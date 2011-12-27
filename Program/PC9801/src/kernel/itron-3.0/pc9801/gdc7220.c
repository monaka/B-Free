/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* gdc7220.c --- グラフィックディスプレイコントローラ PD7220 のドライバ
 *
 *
 * 
 */


#include "itron.h"
#include "errno.h"
#include "task.h"
#include "misc.h"
#include "func.h"
#include "interrupt.h"
#include "../h/graphics.h"
#include "../io/io.h"

#define PLANE0		0x800A8000
#define PLANE1		0x800B0000
#define PLANE2		0x800B8000
#define PLANE3		0x800E0000



/* このファイルで使用するマクロ */
#define CHK_ERR		if (err) return (err);



/* GDC に関係しているIOポートの定義 */
#define GDC_COMMAND	0xa2		/* ライトコマンド	*/
#define GDC_OUT		0xa0		/* ライトパラメータ	*/
#define GDC_IN		0xa2		/* リードデータ		*/
#define GDC_STATUS	0xa0		/* リードステータス	*/
#define GDC_DISLAY_SEL	0xa4		/* 表示画面選択		*/
#define GDC_DRAW_SEL	0xa6		/* 描画画面選択		*/
#define GDC_PAL_D	0xa8		/* ライトパレットレジスタ D */
#define GDC_PAL_C	0xaa		/* ライトパレットレジスタ C */
#define GDC_PAL_B	0xac		/* ライトパレットレジスタ B */
#define GDC_PAL_A	0xae		/* ライトパレットレジスタ A */

/* GDC コマンド */
#define GDC_RESET	0x0000		/* リセットコマンド	    */

/* sync コマンド */
#define MODE_DISP	0x0f		/* 表示開始		*/
#define MODE_UNDISP	0x0e		/* 表示停止		*/


/* ローカル関数 */
static ER	gdc_reset(void);			/* GDCのリセットを行う			*/
static ER	gdc_display(void);			/* 表示開始				*/
static ER	gdc_undisplay(void);			/* 表示停止				*/
static ER	gdc_line (W x1, W y1, W x2, W y2);	/* 画面上に直線を引く			*/
static ER	gdc_circle (W x, W y, W r);		/* 画面上に円を描く			*/
static ER	gdc_dot (W x, W y);			/* 画面上に点を描く			*/
static ER	gdc_write (W num, ...);			/* GDCにコマンドを送出する 		*/
static ER	point2addr (W x, W y);			/* POINT2ADDR: x,y 座標から VRAM	*/ 
							/* 上のアドレスを計算する 		*/

/* SYNC コマンド；画面の各種設定 */
static ER	gdc_sync (B mode, UH column, UH vs, UH hs, UH hfp, UH hbp, UH vfp, UH line, UH vbp);

static void	gdc_server (void);

static ID	deviceid;	/* デバイス番号(デバイステーブルのエントリ番号) */
				/* 主番号として使用する。*/
static ID	taskid;		/* ＰＤＣ７２２０ドライバのタスクＩＤ */



/*****************************************************************************
 * init_gdc7220 --- PD7220 の初期化
 *
 * 引数：
 *	なし
 *
 * 返り値：
 *	エラー番号
 *	E_OK	正常終了
 *
 * 機能：
 *	グラフィック GDC の初期化を行う。
 *	画面設定する：
 *	  サイズ：	640x400
 *	  カラー数：	16 Color
 *
 *
 * 処理概要：
 *	1. RESETコマンドの発行
 *	2. 動作モードの選択 
 *	3. 表示の開始
 *
 *	4. ドライバ登録の実行
 *
 */
ER
init_pd7220 (void)
{
  ER	err;
  T_CTSK	pktsk;

/*  err = gdc_reset ();
  CHK_ERR;
  err = gdc_sync (MODE_DISP, 80, 0x08, 0x07, 0, 0, 0, 400, 0);
  CHK_ERR;
*/
  gdc_display ();

  err = def_dev (L"ＤＳＰ", CHAR, ANY_DEVICE, &deviceid);
  if (err != E_OK)
    {
      printf ("cannot initialize for PD7220 device. err = %d\n", err);
      return (err);
    }
  pktsk.tskatr = TA_HLNG;
  pktsk.startaddr = gdc_server;
  pktsk.itskpri = 1;
  pktsk.stksz = PAGE_SIZE * 2;
  pktsk.addrmap = NULL;
  err = new_task (&pktsk, &taskid, TRUE);
  if (err != E_OK)
    {
      printf ("cannot create task for PD7220. err = %d\n", err);
      return (err);
    }
  printf ("PD7220: TASK ID = %d\n", taskid);
  return (E_OK);
}


static void
gdc_server (void)
{
  T_IO_REQUEST	rcv_packet;
  T_IO_RESPONSE	res_packet;
  ER		err;
  
  printf ("PD7220(GDC): server start.\n");
  for (;;)
    {
      err = get_ioreq (deviceid, &rcv_packet);
      if (err == E_OK)
	{
	  printf ("GDC: Receive request %d\n", rcv_packet.command);
	  /* コマンド解釈部を実行する */
	  switch (rcv_packet.command)
	    {
	      /* IO_NULL, IO_OPEN, IO_CLOSE については何もしない */
	    case IO_NULL:
	    case IO_OPEN:
	    case IO_CLOSE:
	      res_packet.stat = E_OK;
	      break;
	      
	    case IO_READ:
	      res_packet.stat = E_NOSPT;
	      break;

	    case IO_WRITE:
	      res_packet.stat = E_NOSPT;
	      break;

	    case IO_STAT:
	      res_packet.stat = E_NOSPT;
	      break;

	    case IO_CONTROL:
	      res_packet.stat = gdc_control (&(rcv_packet.s));
	      break;

	    default:
	      res_packet.stat = E_PAR;
	      break;
	    }
	}
      
      put_res (deviceid, &rcv_packet, &res_packet);
    }
}

ER
gdc_control (struct io_control_packet *pack)
{
  ER	err;
  struct graphic_packet *arg;

  arg = pack->argp;
  switch (arg->command)
    {
    case Draw_Line:
      err = gdc_line (arg->b.line.x1, arg->b.line.x1, arg->b.line.x1, arg->b.line.x1);
      break;

    default:
      printf ("GDC: unknown request %d\n", arg->command);
      err = E_NOSPT;
      break;
    }
  return (err);
}

/* ------------------------------------------------------------------------------ */


/* GDCのリセットを行う	*/
static ER
gdc_reset(void)
{
  gdc_write (1, GDC_RESET);
}


/* 表示開始		*/
static ER
gdc_display(void)
{

}


/* 表示停止		*/
static ER	
gdc_undisplay(void)
{

}


/* 画面上に直線を引く	*/
static ER
gdc_line (W x1, W y1, W x2, W y2)	
{
  printf ("gdc: line (%d, %d) - (%d, %d)\n", x1, y1, x2, y2);
}


/* 画面上に円を描く	*/
static ER
gdc_circle (W x, W y, W r)
{

}


/* 画面上に点を描く	*/
static ER
gdc_dot (W x, W y)
{
  VB	*addr;
  B	buf; 

/* プレーン 0 */
  addr = (VB *)PLANE0;	/* VRAM の先頭アドレス */
  addr += (y * (640 / 8)) + (x / 8);
  buf = *addr;
  buf |= 1 << (x % 8);
  *addr = buf;

/* プレーン 1 */
  addr = (VB *)PLANE1;	/* VRAM の先頭アドレス */
  addr += (y * (640 / 8)) + (x / 8);
  buf = *addr;
  buf |= 1 << (x % 8);
  *addr = buf;

/* プレーン 2 */
  addr = (VB *)PLANE2;	/* VRAM の先頭アドレス */
  addr += (y * (640 / 8)) + (x / 8);
  buf = *addr;
  buf |= 1 << (x % 8);
  *addr = buf;

/* プレーン 3 */
  addr = (VB *)PLANE3;	/* VRAM の先頭アドレス */
  addr = (y * (640 / 8)) + (x / 8);
  buf = *addr;
  buf |= 1 << (x % 8);
  *addr = buf;
}


/* SYNC コマンド；画面の各種設定 */
static ER
gdc_sync (B mode, UH column, UH vs, UH hs, UH hfp, UH hbp, UH vfp, UH line, UH vbp)
{
  printf ("gdc: SYNC\n");
  printf ("     Not work.\n");
}



/* GDCにコマンドを送出する */
static ER
gdc_write (W num, ...)
{

}


