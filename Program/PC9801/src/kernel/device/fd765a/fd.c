/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/fd.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/fd.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: fd.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.6  1995-10-03 14:41:48  night
 * フロッピィデバイスの準備が整っていないときに、エラーで戻るようにした。
 * (以前は、割り込み待ちで無限待ちしていた)
 *
 * Revision 1.5  1995/10/03  14:07:41  night
 * FD ドライバの動作版。
 * まだ、最大 1K バイトの読み書きしかできないが、任意のオフセットを指定で
 * きるようになった。
 *
 * Revision 1.4  1995/10/01  12:52:32  night
 * FDC に関係する部分を fdc.c に移動。
 *
 * Revision 1.3  1995/09/21  15:50:57  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/06  16:12:26  night
 * 中間バージョン。
 * start() の中で起動メッセージを出すところまで作成。
 *
 * Revision 1.1  1995/03/18  14:09:01  night
 * 最初の登録
 *
 *
 */

/**************************************************************************
 Discription

 PC98 用 FD ドライバ

	インタフェースＬＳＩ：ＰＤ７６５の制御を行う。
	サポートしている PD765 の命令は次のとおり。

	Specify			FDC の初期化を行う。
	Recalibrate		０トラック目にヘッドを移動する。
	Read Data		データの読み込み。
	Write Data		データの書き込み。
	Seek			ヘッドをシーク。
	Sense Interrupt Status	割り込み原因を調べる。


関数説明

  高レベル関数

    ここにあげた関数が、他のモジュールと I/F をとる。

    	  o init_fd	--- ドライバ全体の初期化を行う。
	  o open_fd	--- 指定したドライバ番号をもつ FD をオープンする。
	  o close_fd	--- 指定したドライバ番号をもつ FD をクローズする。
	  o read_fd	--- データの読み取り。
	  o write_fd	--- データの書き込み。
	  o contorl_fd	--- ドライバコントロール。
	  o change_fd	--- アクセスモード切り替え。
	  o status_fd	--- ステータスチェック。
	  o intr_fd	--- 割り込みハンドラ。

  低レベル関数 

    次の関数は、直接 FDC の制御を行う。上位レベルの関数は、この関数を
    使って FD を制御する。これらの関数はすべて LOCAL として宣言される。

    	  o write_fdc	--- FDC にデータを送る。
	  o read_fdc	--- FDC からデータを読み取る。
	  o reset_fdc	--- FDC のリセットを行う。
	  o ready_check	--- FDC が ready 状態かどうかをチェックする。
	  o recalibrate --- 指定したドライブの recalibrate を行う。
	  o specify     --- FDC の初期化l (specify) を行う。
	  o on_motor	--- モーターを起動する。
	  o stop_motor	--- モーターを停止する(この関数は何もしない)。
	  o seek	--- FD のシークを行う。
	  o get_data	--- FD からデータを読み取る。
	  o read_result	--- リザルトステータスレジスタの内容を読み取る。
	  o sense_interrupt --- 割り込み状態を調査する。


	関数階層図

	init_fd
	  reset_fdc
	    write_fdc
	    specify
	    on_motor

	open_fd

	close_fd

	read_fd
	  setup_dma
	  seek
	    write_fdc
	  get_data
	    write_fdc
	
	read_fd
	  setup_dma
	  seek
	    write_fdc
	  get_data
	    write_fdc

	control_fd

	change_fd

	status_fd

	intr_fd
	
	
**********************************************************************/

#include "fd.h"


/*********************************************************************
 *	 大域変数群の宣言
 *
 */
struct spec	*fd_data[MAXFD];
W		intr_flag;


/*********************************************************************
 *	 局所変数群の宣言
 *
 */
static W	mydevid;	/* 自分自身のid */
static ID	recvport;	/* 要求受けつけ用ポート */
static W	initialized;

/*
 *	局所関数群の定義
 */
static void	main_loop (void);
static void	init_fd_driver (void);
static void	doit (DDEV_REQ *packet);

extern void	slp_tsk (void);

/*
 * FD デバイスドライバの main 関数
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
  init_fd_driver ();

  /*
   * 立ち上げメッセージ
   */
  dbg_printf ("floppy disk driver start\n");
  dbg_printf ("  receive port is %d\n", recvport);

  /*
   * ドライバを初期化する。
   */
  main_loop ();
}

static void
main_loop ()
{
  DDEV_REQ	req;
  extern ER	sys_errno;

  /*
   * 要求受信 - 処理のループ
   */
  for (;;)
    {
      /* 要求の受信 */
/*      dbg_printf ("fd: get_req\n");	/* */
      get_req (recvport, &req, sizeof (req));
      switch (sys_errno)
	{
	case E_OK:
	  /* 正常ケース */
/*	  dbg_printf ("fd: receive packet type = %d\n", req.header.msgtyp);	/* */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("fd: get_req() Unknown error(error = %d)\n", sys_errno);
	  dbg_printf ("FD driver is halt.\n");
	  slp_tsk ();
	  break;
	}
    }

  /* ここの行には、来ない */
}

/*
 *
 */
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
      open_fd (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* デバイスのクローズ */
      close_fd (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_fd (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_fd (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_fd (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/*
 * 初期化
 *
 * o ファイルテーブル (file_table) の初期化
 * o 要求受けつけ用のメッセージバッファ ID をポートマネージャに登録
 */
static void
init_fd_driver (void)
{
  int		i;
  ID		root_dev;
  ER		error;

  /*
   * 要求受けつけ用のポートを初期化する。
   */
  recvport = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recvport <= 0)
    {
      dbg_printf ("FD: cannot make receive porrt.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (FD_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

  fd_data[0] = get_fdspec ("2HC");
  fd_data[1] = get_fdspec ("2HC");
  init_fd ();
}

/*
 * init_fd --- FD ドライバの初期化
 *
 */
W
init_fd (void)
{
  W		status;
  T_DINT	pkt;
  ER		err;

  pkt.intatr = ATR_INTR;
  pkt.inthdr = (FP)intr_fd;
  err = def_int (INT_FD, &pkt);
  if (err != E_OK)
    {
      dbg_printf ("fd: error on def_int (errno = %d)\n", err);
      return (err);
    }
  reset_intr_mask (11);
  intr_flag = FALSE;
/*  reset_fdc (0); */
/*  reset_fdc (1); */
#ifdef FDDEBUG
  fd_test ();
#endif
}

/************************************************************************
 * open_fd --- FD のオープン
 *
 * 引数：	dd	FD ドライバ番号
 *		o_mode	オープンモード
 *		error	エラー番号
 *
 * 返値：	
 *
 * 処理：	指定した FD のステータスを調べ、もし使われていなければ、
 *		使用状態 (DRIVE_USING) にする。
 *
 */
W
open_fd (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  outb(0x439, (inb(0x439) & 0xfb)); /* DMA Accsess Control over 1MB */
  outb(0x29, (0x0c | 0));	/* Bank Mode Reg. 16M mode */
  outb(0x29, (0x0c | 1));	/* Bank Mode Reg. 16M mode */
  outb(0x29, (0x0c | 2));	/* Bank Mode Reg. 16M mode */
  outb(0x29, (0x0c | 3));	/* Bank Mode Reg. 16M mode */
  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * fd_close --- ドライバのクローズ
 *
 * 引数：	dd	FD ドライバ番号
 *		o_mode	オープンモード
 *		error	エラー番号
 *
 * 返値：	
 *
 * 処理：	指定した FD のステータスを調べ、もし使われていなければ、
 *		使用状態 (DRIVE_USING) にする。
 *
 */
W
close_fd (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_fd --- 
 *
 * 引数：	caller	呼び出し元への返答を返すためのポート
 *		packet	読み込みデータのパラメータ
 *
 * 返値：	エラー番号
 *
 * 処理：	この関数は、以下の処理を行う。
 *
 *		1) 論理ブロック番号 (1024 byte 単位) から物理ブロック番号へ変換
 *		2) 物理ブロック番号からさらに Head/Cylinder/Sector の各々の数へ変換
 *		3) 変換した数を引数にして get_data() を呼び出す
 *
 */
W
read_fd (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;
  W		blockno;	/* 物理ブロック番号 */
  W		bcount;		/* 論理ブロックが物理ブロックより大きい場合に使用する。*/
  				/* 物理ブロックを読みとるときの回数となる 	       */
  W		cylinder;
  W		head;
  W		sector;
  W		drive;
  W		i, try;
  W		done_length;	/* 本当に読み込んだバイト数 */
  static B	buff[BLOCK_SIZE * 2];
  UW		bp;
  ER		ret;
  ER		error;
  UW		bufstart;
  UW		buflength;

  drive = packet->dd & 0xff;
  if (reset_fdc (drive) == FALSE)
    {
      goto bad;
    }

  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);
/*  dbg_printf ("bufstart = %d, buflength = %d\n", bufstart, buflength);	/* */

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
/*      dbg_printf ("read_fd: bp = %d, length = %d\n", bp, BLOCK_SIZE); */
      /* バイトオフセットから物理ブロック番号への変換 */
      blockno = ((UW)(bp + bufstart) / BLOCK_SIZE) * (BLOCK_SIZE / fd_data[packet->dd & 0xff]->length);
      if (BLOCK_SIZE >= (fd_data[packet->dd & 0xff]->length))
	{
	  bcount = BLOCK_SIZE / (fd_data[packet->dd & 0xff]->length);
	}
  
#define H	(fd_data[packet->dd & 0xff]->head)
#define S	(fd_data[packet->dd & 0xff]->sector)
#define LEN	(fd_data[packet->dd & 0xff]->length)
#define CHAN	(fd_data[packet->dd & 0xff]->dmachan)

      done_length = 0;
      error = E_OK;
      ret = E_OK;

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef FDDEBUG
	  dbg_printf ("read_fd: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < (fd_data[packet->dd & 0xff]->retry); try++)
	    {
	      ret = get_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
	      if (ret == E_OK)
		break;
	    }
	  if (ret == E_OK)
	    {
	      done_length += LEN;
	      blockno++;
	    }
	  else
	    {
	      error = ret;
	      goto bad;
	    }
	}
    }

/*  dbg_printf ("bcopy(): %d, %d, %d\n", packet->start - bufstart, 0, 
	      (done_length < packet->size) ? done_length : packet->size);	/* */
  bcopy (&buff[packet->start - bufstart], 
	 res.body.rea_res.dt, 
	 (done_length < packet->size) ? done_length : packet->size);
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = done_length;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
/*  dbg_printf ("fd: snd_mbuf done.\n"); */
  return (E_OK);

bad:
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = done_length;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
  
#undef H
#undef S
#undef LEN
#undef CHAN
}


/************************************************************************
 *	write_fd
 */
W
write_fd(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  W		blockno;	/* 物理ブロック番号 */
  W		bcount;		/* 論理ブロックが物理ブロックより大きい場合に使用する。*/
  				/* 物理ブロックを読みとるときの回数となる 	       */
  W		cylinder;
  W		head;
  W		sector;
  W		drive;
  W		i, try;
  W		done_length;	/* 本当に読み込んだバイト数 */
  static B	buff[BLOCK_SIZE * 2];
  ER		ret;
  ER		error;
  UW		bp;
  UW		bufstart;
  UW		buflength;


  drive = packet->dd & 0xff;
  if (reset_fdc (drive) == FALSE)
    {
      goto bad;
    }

  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);
/*  dbg_printf ("bufstart = %d, buflength = %d\n", bufstart, buflength);	/* */

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
/*      dbg_printf ("read_fd: bp = %d, length = %d\n", bp, BLOCK_SIZE);		/* */
      /* バイトオフセットから物理ブロック番号への変換 */
      blockno = ((UW)(bp + bufstart) / BLOCK_SIZE) * (BLOCK_SIZE / fd_data[packet->dd & 0xff]->length);
      if (BLOCK_SIZE >= (fd_data[packet->dd & 0xff]->length))
	{
	  bcount = BLOCK_SIZE / (fd_data[packet->dd & 0xff]->length);
	}
  
#define H	(fd_data[packet->dd & 0xff]->head)
#define S	(fd_data[packet->dd & 0xff]->sector)
#define LEN	(fd_data[packet->dd & 0xff]->length)
#define CHAN	(fd_data[packet->dd & 0xff]->dmachan)

      done_length = 0;
      error = E_OK;
      ret = E_OK;

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef FDDEBUG
	  dbg_printf ("read_fd: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < (fd_data[packet->dd & 0xff]->retry); try++)
	    {
	      ret = get_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
	      if (ret == E_OK)
		break;
	    }
	  if (ret == E_OK)
	    {
	      done_length += LEN;
	      blockno++;
	    }
	  else
	    {
	      error = ret;
	      goto bad;
	    }
	}
    }

  done_length = 0;
  error = E_OK;
  ret = E_OK;
  bcopy (packet->dt, &buff[packet->start - bufstart], packet->size);
  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
      /* バイトオフセットから物理ブロック番号への変換 */
      blockno = ((bp + bufstart) / BLOCK_SIZE) * (BLOCK_SIZE / fd_data[packet->dd & 0xff]->length);
      if (BLOCK_SIZE >= (fd_data[packet->dd & 0xff]->length))
	{
	  bcount = BLOCK_SIZE / (fd_data[packet->dd & 0xff]->length);
	}

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef FDDEBUG
	  dbg_printf ("read_fd: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < (fd_data[packet->dd & 0xff]->retry); try++)
	    {
	      ret = put_data (drive,
			      head, 
			      cylinder, 
			      sector, 
			      (void *)(W)(buff + (bp + i * LEN)));
	      if (ret == E_OK)
		break;
	    }
	  if (ret == E_OK)
	    {
	      done_length += LEN;
	      blockno++;
	    }
	  else
	    {
	      error = ret;
	      goto bad;
	    }
	}
    }
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = done_length;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);

bad:
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = done_length;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
  
#undef H
#undef S
#undef LEN
#undef CHAN
}

/************************************************************************
 *	control_fd
 */
W
control_fd (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;
  ER		error = E_OK;
  W		drive;

  switch (packet->cmd)
    {
    case CHANGE_MODE:
      switch (packet->param[0])
	{
	case M2HD:
	  fd_data[packet->param[1]] = get_fdspec ("2HD");
	  if (reset_fdc (packet->param[1]) == FALSE)
	    error = E_DEV;
/*	  dbg_printf ("control_fd: change FD type to 2HD\n");	/* */
	  break;

	case M2HC:
	  fd_data[packet->param[1]] = get_fdspec ("2HC");
	  if (reset_fdc (packet->param[1]) == FALSE)
	    error = E_DEV;
/*	  dbg_printf ("control_fd: change FD type to 2HC\n");	/* */
	  break;

	case OTHER_FD:
	  dbg_printf ("control_fd: Unknown FD type.\n");	/* */
	  break;

	default:
	  dbg_printf ("control_fd: unknown FD type (%d)\n", packet->param[0]);
	  error = E_PAR;
	  break;
	}
      break;

    case NOWORK:
      dbg_printf ("control_fd: nowork\n");
      break;

    default:
      error = E_NOSPT;
      break;
    }
  res.body.ctl_res.dd = packet->dd;
  res.body.ctl_res.errcd = error;
  res.body.ctl_res.errinfo = error;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
}
