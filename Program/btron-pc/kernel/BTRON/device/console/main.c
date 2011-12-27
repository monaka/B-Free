/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";
static char revision[] = "$Revision: 1.1 $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.10  2000-07-09 16:44:47  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.9  2000/02/07 18:04:35  kishida0
 * minor fix
 *
 * Revision 1.8  2000/01/29 16:15:12  naniwa
 * to work with POSIX fcntl
 *
 * Revision 1.7  1999/03/21 12:49:23  night
 * 浪花(naniwa@mechgw.mech.yamaguchi-u.ac.jp) からの指摘による変更。
 * ------------------
 * エスケープ・シーケンスを実装している途中に気が付いたんですが，
 * console は既にバックスペースには対応されていたんですね (^^;)．frtm
 * の入力には '\b' が使えれば十分なので，エスケープシーケンスを作る必
 * 要は無くなってしまったのですが，折角なので最後まで実装しました．
 *
 * 実装したエスケープ・シーケンスの仕様です．先の mail のミスを訂正し
 * ました．
 *
 * ESC[Pl;PcH	カーソルを (Pc,Pl) へ移動．デフォルトはそれぞれ 1．
 * ESC[PnA		カーソルを Pn 行上へ移動．デフォルトは 1．
 * 		画面上端より上への移動は無視．
 * ESC[PnB		カーソルを Pn 行上へ移動．デフォルトは 1．
 * 		画面下端より下への移動は無視．
 * ESC[PnC		カーソルを Pn 桁右へ移動．デフォルトは 1．
 * 		画面右端より右への移動は無視．
 * ESC[PnD		カーソルを Pn 桁左へ移動．デフォルトは 1．
 * 		画面右端より左への移動は無視．
 * ESC[2J		画面を消去し，カーソルを (1,1) へ．
 * ESC[J		表示の終りまでを消去．
 * ESC[K		カーソル位置から行の終りまでを消去．
 *
 * カーソルの座標は左上隅が (1,1)．右下が (MAX_COLUMN, MAX_LINE)．
 *
 * kernel/BTRON/device/console の下で patch -p1 であてるようにパッチ
 * を作りました．
 * ------------------
 *
 * Revision 1.6  1998/11/30 13:25:59  night
 * malloc をこのデバイスドライバで使用するとき (マクロ USE_MALLOC を定義
 * したとき)、init_malloc () に malloc で使用する領域の先頭番地を指定する
 * 引数を追加。
 * kernlib 内で定義している malloc() は、使用に先だって init_malloc() を
 * 実行し、malloc で使用する領域の先頭番地を指定しなければならない。
 *
 * Revision 1.5  1997/09/21 13:31:24  night
 * kernlib の中の malloc ライブラリを使用しないようにした。
 * (malloc ライブラリを使ってしまうと、console ドライバ用に確保しているメ
 * モリ領域をオーバーしてしまうため)
 *
 * Revision 1.4  1997/08/31 14:00:18  night
 * VGA 処理関係の処理を追加。
 *
 * Revision 1.3  1996/11/07  15:40:46  night
 * カーネルからの出力をコンソールドライバへ向ける処理を一時的に
 * 取りやめた。
 * (ハングアップしてしまうため)
 *
 * Revision 1.2  1996/11/06  13:54:17  night
 * カーネルから現在のカーソル位置を取得する処理を追加。
 * および
 * カーネルにコンソールドライバの要求受け付けポートを
 * 登録する処理を追加。
 *
 * Revision 1.1  1996/07/24  16:03:59  night
 * 最初の登録
 *
 * Revision 1.5  1996/01/02 16:20:30  night
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

#include "console_internal.h"

/*********************************************************************
 *	 局所変数群の宣言
 *
 */

void start(void);
static void	main_loop (void);
static void	doit (DDEV_REQ *packet);
static void	writes (B *s);


/*********************************************************************
 *	 大域変数群の宣言
 *
 */
ID	recvport;
W	initialized = 0;
W	video_mode = TEXT_MODE;


/*
 * console デバイスドライバの main 関数
 *
 * この関数は、デバイスドライバ立ち上げ時に一回だけ実行する。
 *
 */
void start (void)
{
  char *p;
  /* 
   * 要求受信用のポートの作成
   */
  init_console ();

  /*
   * 立ち上げメッセージ
   */
  writes ("console driver start\n");
  p = rcsid;
  p = revision;
  main_loop ();
}

static void
main_loop (void)
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
#ifdef DEBUG
      dbg_printf ("call get_req ()\n");
#endif
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* 正常ケース */
#ifdef DEBUG
	  dbg_printf ("console: receive packet type = %d\n", req.header.msgtyp);
#endif
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
#ifdef DEBUG
	  dbg_printf ("CONSOLE: get_req() Unknown error(error = %d)\n", err);
#endif
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
  W		x, y;

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
      return E_SYS;
    }
  initialized = 1;
  dbg_printf ("console: regist port %d\n", recvport);

  if (vget_csl (&x, &y) == E_OK)
    {
      set_curpos (x, y);
    }
  else
    {
      clear_console ();
    }

#ifdef USE_MALLOC
  init_malloc (0xC0000000);	/* 適当な値 */
#endif

#ifdef notdef
  if (vset_cns (recvport) != E_OK)
    {
      /* カーネルにコンソール変更を実行させることに
       * 失敗した。
       */
      return E_SYS;
    }
#endif
  return E_OK;
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
  static int	esc_flag = 0, cnum = 0;
  W		cpos[2];
  UB		ch;

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
  ER		errno;

  if ((packet->cmd & GR_NULL) != 0)
    {
      if (video_mode != GRAPHIC_MODE)
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_PAR;
	  res.body.ctl_res.errinfo = E_PAR;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_PAR);
	}

#ifdef VGA
      errno = graphic_command (caller, packet);
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = errno;
      res.body.ctl_res.errinfo = errno;
#else
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_NOSPT;
      res.body.ctl_res.errinfo = E_NOSPT;
#endif
      snd_mbf (caller, sizeof (res), &res);
      return (errno);
    }
  else
    {
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

	case CONSOLE_GRAPHIC:
	  if (video_mode == TEXT_MODE)
	    {
#ifdef VGA
	      vga_graph ();
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_OK;
	      res.body.ctl_res.errinfo = E_OK;
	      video_mode = GRAPHIC_MODE;
#else
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_NOSPT;
	      res.body.ctl_res.errinfo = E_NOSPT;
#endif
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_OK);
	    }
	  else
	    {
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_PAR;
	      res.body.ctl_res.errinfo = E_PAR;
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_PAR);
	    }
	  
	case CONSOLE_TEXT:
	  if (video_mode == GRAPHIC_MODE)
	    {
#ifdef VGA
	      vga_text ();
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_OK;
	      res.body.ctl_res.errinfo = E_OK;
	      video_mode = TEXT_MODE;
#else
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_NOSPT;
	      res.body.ctl_res.errinfo = E_NOSPT;
#endif
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_OK);
	    }
	  else
	    {
	      res.body.ctl_res.dd = packet->dd;
	      res.body.ctl_res.errcd = E_PAR;
	      res.body.ctl_res.errinfo = E_PAR;
	      snd_mbf (caller, sizeof (res), &res);
	      return (E_PAR);
	    }
	  
	default:
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_NOSPT;
	  res.body.ctl_res.errinfo = E_NOSPT;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_NOSPT);
	}
    }
}


static void
writes (B *s)
{
  ER error;

  while (*s != '\0')
    {
      error = write_char (*s);
      if (error != E_OK)
	break;
      s++;
    }  
}
