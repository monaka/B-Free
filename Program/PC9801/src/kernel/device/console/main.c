/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.5  1996-01-02 16:20:30  night
 * 立ち上げ時に画面をクリアするように変更した。
 *
 * Revision 1.4  1995/09/21  15:50:55  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/09/17  16:50:38  night
 * 送受信パケットの定義を src/kernel/kernlib/device.h に合わせた。
 * open/close/read/write/control 関数を実装した。
 *
 * Revision 1.2  1995/09/14  04:28:43  night
 * init_driver() を init_console() に変更。
 * メッセージ受信処理の追加。
 *
 * Revision 1.1  1995/09/12  18:18:11  night
 * 最初の登録
 *
 *
 *
 */

/**************************************************************************
 Discription

**********************************************************************/

#include "console.h"

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


/*
 * console デバイスドライバの main 関数
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
  init_console ();

  /*
   * 立ち上げメッセージ
   */
  dbg_printf ("console driver start\n");
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
/*      dbg_printf ("call get_req ()\n"); */
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* 正常ケース */
/*	  dbg_printf ("console: receive packet type = %d\n", req.header.msgtyp);	/* */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("CONSOLE: get_req() Unknown error(error = %d)\n", err);
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
init_console (void)
{
  int		i;
  ER		error;

  /*
   * 要求受けつけ用のポートを初期化する。
   */
  recvport = get_port (sizeof (DDEV_REQ), sizeof (DDEV_REQ));
  if (recvport <= 0)
    {
      dbg_printf ("CONSOLE: cannot make receive port.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (CONSOLE_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("console: cannot regist port (error = %d)\n", error);
    }
  initialized = 1;
  clear_console ();
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
	  init_console ();
	}
      open_console (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_console (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_console (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_console (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_console (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/************************************************************************
 * open_console --- console のオープン
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
open_console (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_console --- ドライバのクローズ
 *
 * 引数：	dd	console ドライバ番号
 *		o_mode	オープンモード
 *		error	エラー番号
 *
 * 返値：	常に E_OK を返す。
 *
 * 処理：	コンソールはクローズの処理ではなにもしない。
 *
 */
W
close_console (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_console --- 
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
read_console (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * write_console
 *
 * 引数：	caller
 *		packet
 *
 * 返値：	
 *
 * 処理：	カレントポインタの位置に文字列を表示する。
 *		このとき、エスケープシーケンスによる処理も行う。		
 *
 */
W
write_console(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  int		i;
  ER		error;

  error = E_OK;
  for (i = 0; i < (packet->size); i++)
    {
      error = write_char (packet->dt[i]);
      if (error != E_OK)
	break;
    }  
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = error;
  res.body.wri_res.a_size = i;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
}

/************************************************************************
 * control_console
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
control_console (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;

  switch (packet->cmd)
    {
    case CONSOLE_CLEAR:
      clear_console ();
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    case CONSOLE_MOVE:
      if (packet->len != 2)
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_PAR;
	  res.body.ctl_res.errinfo = E_PAR;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_PAR);
	}
      set_curpos (packet->param[0], packet->param[1]);
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

