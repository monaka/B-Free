/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* rs232c.c --- RS232C ドライバ (PC9801 用)
 *
 * $Revision: 1.1 $
 *
 *
 * 外部インタフェース
 *
 * init_rs232c		RS232C ドライバの初期化
 *
 *
 * static 関数
 *
 * rs232c_server	RS232C ドライバへのリクエストの受け付け場所
 * rs232c_intr		RS232C ハードウェアからの割りこみハンドラ
 * rs232c_read		RS232C ハードウェアからのデータ読み込み
 * rs232c_write		RS232C ハードウェアへのデータ書き込み
 * rs232c_stat		RS232C ドライバの状態
 * rs232c_control	読み書き以外のRS232Cドライバの制御
 *
 */

#include "itron.h"
#include "errno.h"
#include "task.h"
#include "misc.h"
#include "func.h"
#include "interrupt.h"
#include "../io/io.h"
#include "../io/rs232c.h"


struct data_link
{
  struct data_link	*next;
  B			ch;
};


struct rs232c
{
  W	port;
  W	speed;
  W	bitlen;
  W	stoplen;
};

static ID	deviceid;	/* デバイス番号(デバイステーブルのエントリ番号) */
				/* 主番号として使用する。*/
static ID	taskid;		/* ＲＳ２３２ＣドライバのタスクＩＤ */

/* このファイルで使用するstatic関数
 */
static void	rs232c_server (void);
static void	rs232c_intr (void);
static ER	rs232c_read (struct io_read_packet *);
static ER	rs232c_write (struct io_write_packet *);
static ER	rs232c_stat (struct io_stat_packet *);
static ER	rs232c_control (struct io_control_packet *);

/* デバッグ変数：値が TRUE の時にはデバッグ状態となる
 */
static BOOL	rs232c_debug = TRUE;

/* デバッグ用の printf 
 */
#define DPRINTF(x)	if (rs232c_debug) { printf x; }


/*****************************************************************************
 * init_rs232c --- ＲＳ２３２Ｃドライバ
 *
 * 引数：
 *	なし
 *
 * 返り値：
 *	エラー番号
 *	E_OK	正常終了
 *
 * 機能：
 *	RS232C の初期化を行う。
 *
 */
ER
init_rs232c (void)
{
  ER		err;
  T_CTSK	pktsk;

  err = def_dev (L"ＲＳ２３２Ｃ", CHAR, ANY_DEVICE, &deviceid);
  if (err != E_OK)
    {
      printf ("cannot initialize for RS232C device. err = %d\n", err);
      return (err);
    }

  DPRINTF(("init_rs232c: setup\n"))

  pktsk.tskatr = TA_HLNG;
  pktsk.startaddr = rs232c_server;
  pktsk.itskpri = 1;
  pktsk.stksz = PAGE_SIZE * 2;
  pktsk.addrmap = NULL;
  err = new_task (&pktsk, &taskid);
  if (err != E_OK)
    {
      printf ("cannot create task for RS232c. err = %d\n", err);
      return (err);
    }
  printf ("RS232C: TASK ID = %d\n", taskid);
  return (E_OK);
}


/*****************************************************************************
 * rs232c_server --- ドライバへの要求メッセージの受信関数
 *
 * 引数:
 *	なし
 *
 * 返り値：
 *	エラー番号
 *
 */
static void
rs232c_server (void)
{
  T_IO_REQUEST	rcv_packet;
  T_IO_RESPONSE	res_packet;
  ER		err;
  
  printf ("RS232C: server start.\n");
  for (;;)
    {
      err = get_ioreq (deviceid, &rcv_packet);
      if (err == E_OK)
	{
	  printf ("RS232C: Receive request %d\n", rcv_packet.command);
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
	      res_packet.stat = rs232c_read (&(rcv_packet.s.read_pack));
	      break;

	    case IO_WRITE:
	      res_packet.stat = rs232c_write (&(rcv_packet.s.write_pack));
	      break;

	    case IO_STAT:
	      res_packet.stat = rs232c_stat (&(rcv_packet.s.stat_pack));
	      break;

	    case IO_CONTROL:
	      res_packet.stat = rs232c_control (&(rcv_packet.s.control_pack));
	      break;

	    default:
	      res_packet.stat = E_PAR;
	      break;
	    }
	}
      
      put_res (deviceid, &rcv_packet, &res_packet);
    }
}


/********************************************************************************
 * rs232c_intr --- RS232C の割り込みハンドラ
 *
 * 引数：
 *	なし
 *
 * 返り値：
 *	なし
 *
 * 機能：
 *
 */
static void
rs232c_intr (void)
{
  DPRINTF(("rs232c_intr\n"))
}


/**********************************************************************************
 * 
 *
 * 引数：
 *
 * 返り値：
 *
 *
 * 機能：
 *
 */
static ER
rs232c_read (struct io_read_packet *packet)
{
  DPRINTF(("rs232c_read\n"))
}

/**********************************************************************************
 * 
 *
 * 引数：
 *
 * 返り値：
 *
 *
 * 機能：
 *
 */
static ER	
rs232c_write (struct io_write_packet *packet)
{
  DPRINTF (("rs232c_write\n"));
}

/**********************************************************************************
 * 
 *
 * 引数：
 *
 * 返り値：
 *
 *
 * 機能：
 *
 */
static ER
rs232c_stat (struct io_stat_packet *packet)
{
  DPRINTF (("rs232c_stat\n"));
}

/**********************************************************************************
 * 
 *
 * 引数：
 *
 * 返り値：
 *
 *
 * 機能：
 *
 */
static ER
rs232c_control (struct io_control_packet *packet)
{
  DPRINTF (("rs232c_control\n"));
}
