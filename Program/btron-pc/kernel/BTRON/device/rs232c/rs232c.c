/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/rs232c/rs232c.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: rs232c.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1998-12-07 14:19:14  night
 * probe_rs232c ()、setup_rs232c ()、write_byte_rs232c ()、
 * read_byte_rs232c () の関数の追加。
 * 中身はまだ作っていない。
 *
 * Revision 1.2  1998/02/23 14:40:19  night
 * get_req の引数が間違っていたため修正。
 * 今までのコードだと、仮想空間の先頭部分を破壊していた。
 *
 * Revision 1.1  1997/05/17 10:47:03  night
 * 最初の登録
 *
 *
 */

/*
 *
 */

#include "rs232c.h"



/*
 *	 局所変数群の宣言
 *
 */
static W	mydevid;	/* 自分自身のid */
static ID	recvport;	/* 要求受けつけ用ポート */
static W	initialized;

static void	main_loop (void);
static void	init_driver (void);


/* 
 *
 */
void
start (void)
{
  /* probe rs232c hardware */

  init_driver ();
  printf ("registed rs232c driver\n");

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
      dbg_printf ("rs232c: cannot make receive porrt.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (RS232C_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

  /* hardware_setup */
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
      open_rs232c (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_rs232c (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_rs232c (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_rs232c (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_rs232c (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 *
 */
ER
open_rs232c (ID caller, DDEV_OPN_REQ *packet)
{
}

/*
 *
 */
ER
close_rs232c (ID caller, DDEV_CLS_REQ *packet)
{
}

/*
 *
 */
ER
read_rs232c (ID caller, DDEV_REA_REQ *packet)
{
}

/*
 *
 */
ER
write_rs232c (ID caller, DDEV_WRI_REQ *packet)
{
}

/*
 *
 */
ER
control_rs232c (ID caller, DDEV_CTL_REQ *packet)
{
}


/*******************************************************************************
 *
 *	以下の関数群では、 RS232C のハードウェアの操作を行う
 *
 *	probe_rs232c
 *	reset_rs232c
 *	setup_rs232c
 *	write_byte_rs232c
 *	read_byte_rs232c
 *
 */


W
probe_rs232c (W number)
{
  return (TRUE);
}


ER
setup_rs232c (W port, 
	      W speed,
	      W parity,
	      W start_stop_bit,
	      W data_length)
{
  return (E_NOSPT);
}


ER
write_byte_rs232c (W port, UB data)
{
  return (E_NOSPT);
}


ER
read_byte_rs232c (W port, UB *data)
{
  return (E_NOSPT);
}


