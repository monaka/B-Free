/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/beep/beep.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $*/
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/beep/beep.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: beep.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-03-15 05:58:22  monaka
 * modified some debug messages.
 *
 * Revision 1.1  1999/02/17 04:23:56  monaka
 * First version.
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/*
 *
 *
 */

#include "beep.h"
#include "beep_internal.h"


/*
 *	 局所変数群の宣言
 *
 */
static W	mydevid;	/* 自分自身のid */
static ID	recvport;	/* 要求受けつけ用ポート */
static W	initialized;

/*struct device	dev_table[MAX_BEEP];*/


static void	main_loop (void);
static void	init_driver (void);


/* start --- beep ドライバのメイン関数
 */
void
start ()
{
/*  probe (&dev_table[0]); /* デバイスが存在しているかをチェックする */
  init_driver ();

  printf ("beep driver started.\n");
  main_loop ();
}


/*
 * 初期化
 *
 * o ファイルテーブル (file_table) の初期化
 * o 要求受けつけ用のメッセージバッファ ID をポートマネージャに登録
 */
static void
init_driver (void)
{
  int		i;
  ER		error;

  init_log ();

  /*
   * 要求受けつけ用のポートを初期化する。
   */
  recvport = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recvport <= 0)
    {
      dbg_printf ("beep: cannot make receive porrt.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (BEEP_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

}



/*
 *
 */
static void
main_loop (void)
{
  DDEV_REQ	req;
  extern ER	sys_errno;
  UW		rsize;

  /*
   * 要求受信 - 処理のループ
   */
  for (;;)
    {
      /* 要求の受信 */
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* 正常ケース */
	  process_request (&req);
	  break;

	default:
	  /* Unknown error */
	  slp_tsk ();
	  break;
	}
    }

  /* ここの行には、来ない */
}


/*
 *
 */
ER
process_request (DDEV_REQ *req)
{
  switch (req->header.msgtyp)
    {
    case DEV_OPN:
      /* デバイスのオープン */
      open_beep (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_beep (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_beep (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_beep (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_beep (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * デバイスのオープン
 */
ER
open_beep (ID caller, DDEV_OPN_REQ *packet)
{
  /*** ここにコードをついかしてください ***/
  B bPortData;
  bPortData = inb(BEEP_PORT);
  bPortData |= 3;
  outb(BEEP_PORT, bPortData);

  return (E_OK);
}

/*
 * デバイスのクローズ
 */
ER
close_beep (ID caller, DDEV_CLS_REQ *packet)
{
  /*** ここにコードをついかしてください ***/
  B bPortData;
  bPortData = inb(BEEP_PORT);
  bPortData &= 0xFC;
  outb(BEEP_PORT, bPortData);

  return (E_OK);
}

/*
 *
 */
ER
read_beep (ID caller, DDEV_REA_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
write_beep (ID caller, DDEV_WRI_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
control_beep (ID caller, DDEV_CTL_REQ *packet)
{
  /*** ここにコードをついかしてください ***/
  
  return (E_NOSPT);
}
