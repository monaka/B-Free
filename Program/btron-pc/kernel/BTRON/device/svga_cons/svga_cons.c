/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/svga_cons.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $*/
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/svga_cons.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: svga_cons.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-05-04 14:27:37  kishida0
 * デバッグ用のコンソールとして作成予定
 * なんちゃってDPとは関連しないので注意してください
 *
 * Revision 1.3  1999/04/26 18:28:49  kishida0
 * console から API を持って来た
 *
 * Revision 1.2  1999/04/20 16:19:45  kishida0
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/11 14:41:33  kishida0
 * for debug function
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */
#ifndef __SVGA_CONS_C__
#define __SVGA_CONS_C__ 1

/*
 *
 *
 */

#include "svga_cons.h"
#include "svga_cons_internal.h"


/*
 *	 局所変数群の宣言
 *
 */
static W	mydevid;	/* 自分自身のid */
static ID	recvport;	/* 要求受けつけ用ポート */
static W	initialized;

struct device	dev_table[MAX_SVGA_CONS];


static void	main_loop (void);
static void	init_driver (void);


/* start --- svga_cons ドライバのメイン関数
 */
void
start (void)
{
  probe (&dev_table[0]); /* デバイスが存在しているかをチェックする */
  init_driver ();

  printf ("registed svga_cons driver.\n");
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
      dbg_printf ("svga_cons: cannot make receive porrt.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (SVGA_CONS_DRIVER, recvport);
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
      open_svga_cons (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_svga_cons (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_svga_cons (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_svga_cons (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_svga_cons (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * デバイスのオープン
 *
 * 引数：       caller  メッセージの送り手
 *              packet  オープンパケット
 *
 * 返値：       常に E_OK を返す。
 *
 * 処理：       E_OK をメッセージの送り手に返す。
 *
 */
ER
open_svga_cons (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES      res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*
 * デバイスのクローズ
 */
ER
close_svga_cons (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES      res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*
 *
 */
ER
read_svga_cons (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES      res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/*
 *
 */
ER
write_svga_cons (ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES      res;
  int           i;
  ER            error;
  static int    esc_flag = 0, cnum = 0;
  W             cpos[2];
  UB            ch;

  error = E_OK;
  for (i = 0; i < (packet->size); i++)
    {
      ch = packet->dt[i];
      if (esc_flag == 1) {
        if (ch == '[') {
          esc_flag = 2;
        }
        else {
          error = write_char (ch);
          esc_flag = 0;
        }
      }
      else if (esc_flag == 2) {
        if (ISDIGIT(ch)) {
          cpos[cnum] *= 10;
          cpos[cnum] += ch-'0';
        }
        else {
          if (cpos[cnum] == 0) cpos[cnum] = 1;
          switch(ch) {
          case ';':
            cnum++;
            if (cnum == 2) esc_flag = 0;
            break;
          case 'H':
            if (cpos[1] == 0) cpos[1] = 1;
            set_curpos(cpos[1]-1, cpos[0]-1);
            esc_flag = 0;
            break;
          case 'A':
            move_curpos(0, -cpos[0]);
            esc_flag = 0;
            break;
          case 'B':
            move_curpos(0, cpos[0]);
            esc_flag = 0;
            break;
          case 'C':
            move_curpos(cpos[0], 0);
            esc_flag = 0;
            break;
          case 'D':
            move_curpos(-cpos[0], 0);
            esc_flag = 0;
            break;
          case 'J':
            if (cpos[0] == 2) {
              set_curpos(0, 0);
              clear_console();
            }
            else if (cpos[0] == 1) {
              clear_rest_screen();
            }
            esc_flag = 0;
            break;
          case 'K':
            clear_rest_line();
            esc_flag = 0;
            break;
          default:
            esc_flag = 0;
            error = write_char(ch);
          }
        }
      }
      else {
        if (ch == 0x1B) {
          esc_flag = 1;
          cpos[0] = 0; cpos[1] = 0;
          cnum = 0;
        }
        else {
          error = write_char (ch);
        }
      }
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

/*
 *
 */
ER
control_svga_cons (ID caller, DDEV_CTL_REQ *packet)
{
  /*** ここにコードをついかしてください ***/

  return (E_NOSPT);
}

#endif //__SVGA_CONS_C__

