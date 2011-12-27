/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-10-01 12:55:24  night
 * KEYBOARD_CHANGEMODE についての処理を追加。
 *
 * Revision 1.3  1995/09/21  15:51:00  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/19  18:01:02  night
 * キーボードから入力できるところまでできた。
 *
 * Revision 1.1  1995/09/18  11:42:02  night
 * 最初の登録
 *
 *
 *
 */

/**************************************************************************
 Discription

**********************************************************************/

#include "keyboard.h"

/*********************************************************************
 *	 局所変数群の宣言
 *
 */
static void	main_loop ();
static void	doit (DDEV_REQ *packet);


/*********************************************************************
 *	 大域変数群の宣言
 *
 */
ID	recvport;
W	initialized = 0;
ID	waitflag;		/* キーボードからキー入力を待つ時に */
				/* 使用するイベントフラグの ID */
W	driver_mode;


/*
 * keyboard デバイスドライバの main 関数
 *
 * この関数は、デバイスドライバ立ち上げ時に一回だけ実行する。
 *
 */
start ()
{
  extern char	version[];

  /* 
   * 要求受信用のポートの作成
   */
  init_keyboard ();

  /*
   * 立ち上げメッセージ
   */
  dbg_printf ("keyboard driver start\n");
  dbg_printf ("  receive port is %d\n", recvport);

  main_loop ();
}

static void
main_loop ()
{
  DDEV_REQ		req;	/* 受信する要求パケット */
  ER			err;
  extern ER		sys_errno;

  /*
   * 要求受信 - 処理のループ
   */
  for (;;)
    {
      W	rsize;

      /* 要求の受信 */
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* 正常ケース */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("KEYBOARD: get_req() Unknown error(error = %d)\n", err);
	  break;
	}
    }

  /* ここの行には、来ない */
}

/*
 * 初期化
 *
 * o 要求受けつけ用のメッセージバッファ ID をポートマネージャに登録
 */
W
init_keyboard (void)
{
  int		i;
  ER		error;

  /*
   * 要求受けつけ用のポートを初期化する。
   */
  recvport = get_port (sizeof (DDEV_REQ), sizeof (DDEV_REQ));
  if (recvport <= 0)
    {
      dbg_printf ("KEYBOARD: cannot make receive port.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (KEYBOARD_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("keyboard: cannot regist port (error = %d)\n", error);
    }

  init_keyboard_interrupt ();	/* 割り込みハンドラの登録 */
  init_keybuffer ();		/* キーボードバッファの初期化 */

  /* キー入力を待つ時に使用するイベントフラグの初期化 */
  waitflag = get_flag (TA_WSGL, 0);
  dbg_printf ("keyboard: eventflag = %d\n", waitflag);	/* */

  driver_mode = WAITMODE;

  initialized = 1;
}

/************************************************************************
 *
 *
 */
static void
doit (DDEV_REQ *packet)
{
  switch (packet->header.msgtyp)
    {
    case DEV_OPN:
      /* デバイスのオープン */
      if (!initialized)
	{
	  init_keyboard ();
	}
      open_keyboard (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_keyboard (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_keyboard (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_keyboard (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_keyboard (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/************************************************************************
 * open_keyboard --- keyboard のオープン
 *
 * 引数：	caller	メッセージの送り手
 *		packet	オープンパケット
 *
 * 返値：	常に E_OK を返す。
 *
 * 処理：	E_OK をメッセージの送り手に返す。
 *
 */
W
open_keyboard (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_keyboard --- ドライバのクローズ
 *
 * 引数：	dd	keyboard ドライバ番号
 *		o_mode	オープンモード
 *		error	エラー番号
 *
 * 返値：	常に E_OK を返す。
 *
 * 処理：	キーボードはクローズの処理ではなにもしない。
 *
 */
W
close_keyboard (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_keyboard --- 
 *
 * 引数：	caller
 *		packet
 *
 * 返値：	E_NOSPT を返す。
 *
 * 処理：	メッセージの送り手に E_NOSPT を返す。
 *		コンソールドライバは表示するだけなので入力はサポートして
 *		いない。
 *
 */
W
read_keyboard (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;
  W		i;

  res.body.rea_res.dd = packet->dd;

  for (i = 0; i < packet->size; i++)
    {
      res.body.rea_res.dt[i] = read_key (driver_mode);
    }
  res.body.rea_res.a_size = i;
  res.body.rea_res.errcd = E_OK;
  res.body.rea_res.errinfo = E_OK;

  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * write_keyboard
 *
 * 引数：	caller
 *		packet
 *
 * 返値：	常に E_NOSPT を返す。
 *
 * 処理：	write は、キーボードでは行わない。
 *
 */
W
write_keyboard(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * control_keyboard
 *
 * 引数：	caller
 *		packet
 *
 * 返値：	E_NOSPT を返す。
 *
 * 処理：
 *
 */
W
control_keyboard (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;

  switch (packet->cmd)
    {
    case KEYBOARD_CLEAR:
      clear_keybuffer ();
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    case KEYBOARD_CHANGEMODE:
      driver_mode = packet->param[0];
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    default:
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_NOSPT;
      res.body.ctl_res.errinfo = E_NOSPT;
      snd_mbf (caller, sizeof (res), &res);
      return (E_NOSPT);
    }
}

