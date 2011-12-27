/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /usr/local/src/master/B-Free/Program/btron-pc/kernel/BTRON/devic
e/fd765a/fd.c,v 1.5 1999/04/13 04:14:40 monaka Exp $ */
static char rcsid[] = "@(#)$Header: /usr/local/src/master/B-Free/Program/btron-p
c/kernel/BTRON/device/fd765a/fd.c,v 1.5 1999/04/13 04:14:40 monaka Exp $";

/*
 * $Log: aha152xlow.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-12-18 15:24:03  kishida0
 * linuxのソースを追加した
 *
 * Revision 1.3  1999/12/09 17:13:12  kishida0
 * *** empty log message ***
 *
 * Revision 1.2  1999/12/02 16:02:34  kishida0
 * Here is start.
 *
 * Revision 1.1  1999/07/06 13:53:31  kishida0
 * add debug aha152x src
 *
 *
 *
 */
#define __AHA152XLOW_C__

#include "isa_scsi_internal.h"
#include "aha152x.h"
#include "aha152xlow.h"


//void outp(UW ind, UB data){
//  outb(ind,data);
//}




/*
 * 普通はここが aha152xlow.c
 *
 */

UW get_config_data(int iBusNum, int iDevNum, int iFuncNum, int iRegAddr)
{
int config_data = 0;

  return config_data;
}


/* aha152x アダプタが存在しているかどうかをチェックする
 */
ER
probe (struct device *dev)
{
  dbg_printf("ISA_SCSI:probe\n");
  dbg_printf("%s",rcsid);
  return (E_OK);
}

/*
 * 普通はここ以下が main.c
 *
 */

/*
 *	 局所変数群の宣言
 *
 */
static W	mydevid;	/* 自分自身のid */
static ID	recvport;	/* 要求受けつけ用ポート */
static W	initialized;

struct device	dev_table[MAX_ISA_SCSI];


static void	main_loop (void);
static void	init_driver (void);


/* start --- sa_scsi ドライバのメイン関数
 */
void
start ()
{
  probe (&dev_table[0]); /* デバイスが存在しているかをチェックする */
  init_driver ();

  printf ("registed isa_scsi driver.\n");
  main_loop ();
}

void
panic(char *s)
{
      dbg_printf (s);
      slp_tsk ();
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
      panic ("isa_scsi: cannot make receive port.\n");
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (ISA_SCSI_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
      panic("isa_scsi: cannot regist_port");
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
	  panic ( " Unknown error ");/**/
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
      open_isa_scsi (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_isa_scsi (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_isa_scsi (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_isa_scsi (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_isa_scsi (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * デバイスのオープン
 */
ER
open_isa_scsi (ID caller, DDEV_OPN_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

/*
 * デバイスのクローズ
 */
ER
close_isa_scsi (ID caller, DDEV_CLS_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
read_isa_scsi (ID caller, DDEV_REA_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
write_isa_scsi (ID caller, DDEV_WRI_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
control_isa_scsi (ID caller, DDEV_CTL_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}
