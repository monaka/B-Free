/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* io.c --- デバイス IO に関するコントロールタワー
 *
 * 外部関数：
 *	init_io 	Ｉ／Ｏデバイス管理機能の初期化
 *	def_dev 	デバイスの登録
 *	get_ioport 	デバイス用のメッセージポートの獲得
 *	io_request 	デバイスに対して入出力要求を送る
 *	io_response 	非同期モードのデバイス入出力のときにＩＯのレス
 *			ポンスを受けとる
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
#include "../h/message.h"
#include "../io/io.h"

#define MAX_QUEUE	10


/**********************************************************************
 * io_device 構造体
 *
 */
struct io_device
{
  ID			id;
  ID			portid;		/* IO デバイスへメッセージを送 */
					/* るためのメッセージポート ID */
  IO_TYPE		type;
  B			name[MAX_DEVICE_NAME];
};

struct io_device	device_table[MAX_DEVICE];

static ID	get_ioport (void);



/*************************************************************************
 * init_io --- Ｉ／Ｏデバイス管理機能の初期化
 *
 * 引数：
 *	なし
 *
 * 返り値：
 *	エラー番号
 *
 * 機能：
 *	デバイステーブルの初期化
 *
 */
ER
init_io (void)
{
   W	i;

  for (i = 0; i < MAX_DEVICE; i++)
    {
      device_table[i].id = UNDEF_DEVICE;
    }
  return (E_OK);
}

/************************************************************************
 * def_dev -- デバイスの登録
 *
 * 引数：
 *	name	デバイス名
 *	type	デバイスタイプ
 *	id	デバイス ID 
 *	rid	デバイス ID (返り値)
 *
 * 返り値：
 *	エラー番号
 *
 * 機能：
 *	
 *
 *
 */
ER
def_dev (B *name, IO_TYPE type, ID id, ID *rid)
{
  W	i;
  ID	port;

  if (id == ANY_DEVICE)
    {
      for (i = 0; i < MAX_DEVICE; i++)
	{
	  if (device_table[i].id == UNDEF_DEVICE)
	    {
	      id = i;
	      break;
	    }
	}
      if (id == ANY_DEVICE)
	return (E_NOMEM);
    }

  if (device_table[id].id != UNDEF_DEVICE)
    return (E_ID);

  port = get_ioport ();
  if (port <= 0)
    {
      return (E_NOMEM);
    }
  device_table[id].id   = id;
  device_table[id].type = type;
  device_table[id].portid = port;
  bcopy (name, device_table[id].name, strlen (name) + 1);
  *rid = id;

  return (E_OK);
}


/************************************************************************
 * ref_dev -- デバイスの参照
 *
 * 引数：
 *	name	デバイス名
 *	type	デバイスタイプ
 *	rid	デバイス ID (返り値)
 *
 * 返り値：
 *	エラー番号
 *	E_PAR	デバイスタイプの不正
 *	E_OBJ	指定されたデバイスは存在しない
 *
 * 機能：
 *	デバイス名とデバイスタイプからデバイス ID を返す。
 *
 *
 */
ER
ref_dev (B *name, IO_TYPE type, ID *rid)
{
  W	i;

  if ((type != CHAR) && (type != BLOCK))
    {
      return (E_PAR);
    }

  for (i = 0; i < MAX_DEVICE; i++)
    {
      if (device_table[i].id != UNDEF_DEVICE)
	{
	  if ((strncmp (name, device_table[i].name, MAX_DEVICE_NAME) == 0) &&
	      (type == device_table[i].type))
	    {
	      *rid = device_table[i].id;
	      return (E_OK);
	    }
	}
    }

  return (E_OBJ);
}


/***********************************************************************
 * get_ioport --- デバイス用のメッセージポートの獲得
 *
 * 引数：
 *	なし
 *
 * 返り値：
 *	正常時：	取得したデバイス用のメッセージポート ID
 *	異常時：	NULL (0)
 *
 * 機能：
 *	デバイス用のメッセージバッファポートをアロケートする。
 *	ITRON のメッセージバッファにはメッセージサイズの指定が必要だが
 *      メッセージサイズは、T_IO_REQUESTQ とした。
 *	なお、バッファとして メッセージ 10 個分を確保した。
 */
static ID
get_ioport (void)
{
  W		id;
  T_CMBF	pk_cmbf;
  ER		err;

  pk_cmbf.mbfatr = TA_TFIFO;
  pk_cmbf.bufsz = sizeof (T_IO_REQUEST) * MAX_QUEUE;
  pk_cmbf.maxmsz = sizeof (T_IO_REQUEST);
  for (id = MIN_USERMBFID; id <= MAX_USERMBFID; id++)
    {
      err = cre_mbf (id, &pk_cmbf);
      if (err == E_OK)		/* メッセージポートが確保できた。*/
	{
	  break;
	}
      else if (err != E_OBJ)	/*  ID 不正以外のエラーで失敗: エラー */
	{			/*  リターン			      */
	  return (NULL);
	}
    }

  if (id > MAX_USERMBFID)
    {
      return (NULL);
    }
  return (id);
}


/*************************************************************************
 * get_req --- デバイスドライバ側でのメッセージ受け付け
 *
 * 引数：
 *	device
 *	req
 *
 * 返り値：
 *	エラー番号
 *
 * 機能：
 *
 *
 */
ER
get_ioreq (ID device, T_IO_REQUEST *req)
{
  ER	err;
  INT		rsize;

  if ((device < MIN_DEVID) || (device > MAX_DEVID))
    {
      return (E_ID);
    }

  if (device_table[device].id == UNDEF_DEVICE)
    {
      return (E_ID);
    }

  err = rcv_mbf ((VP)req, &rsize, device_table[device].portid);
  if (err != E_OK)
    {
      return (err);
    }
  return (E_OK);
}


/*************************************************************************
 *
 *
 *
 *
 */
ER
put_res (ID device, T_IO_REQUEST *req, T_IO_RESPONSE *res)
{
  ER	err;

  err = snd_mbf (req->resport, sizeof (T_IO_RESPONSE), res);
  return (err);
}



/*************************************************************************
 * io_request --- デバイスに対して入出力要求を送る
 *
 * 引数：
 *	device	要求を送るデバイスのＩＤ
 *	req	要求パケット
 *	res	応答パケット (非同期モードのときには使用しない)
 *	mode	同期/非同期モードの指定
 *		IO_SYNC		同期モード
 *		IO_ASYNC	非同期モード
 *
 * 返り値：
 *	エラー番号：	このエラー番号はデバイスに要求の送受信ができた
 *			かどうかだけを知らせるもので、Ｉ／Ｏ処理自体が
 *			うまくできたかどうかは受信パケットの中を見るし
 *			かない。
 *	E_OK	正常終了
 *
 * 機能：
 *	要求パケットに指定されたデバイスに対して、入出力要求を送る。
 *	
 *
 */
ER
io_request (ID device, T_IO_REQUEST *req, T_IO_RESPONSE *res, W mode)
{
  ID	res_port;
  ER	err;
  INT	size;

  res_port = req->resport;
  printk ("io_request: command = %d\n", req->command);
  err = snd_mbf (device_table[device].portid, sizeof (T_IO_REQUEST), req);
  if (err != E_OK)		/* メッセージ送信に失敗 */
    {
      return (err);
    }

  if (mode == IO_SYNC)	/* 同期モードの時：レスポンスが返るのを待つ */
    {
      err = rcv_mbf (&req, &size, res_port);
      if (err != E_OK)
	{
	  return (err);
	}
      if (size != sizeof (T_IO_RESPONSE))
	{
	  return (E_OBJ);
	}
    }

  return (E_OK);
}


/*************************************************************************
 * io_response ---	非同期モードのデバイス入出力のときにＩＯのレス
 *			ポンスを受けとる
 *
 */
ER
io_response (ID device, T_IO_REQUEST *req, T_IO_RESPONSE *res)
{
  ER	err;
  INT	size;

  err = rcv_mbf (&req, &size, req->resport);
  if (err != E_OK)
    {
      return (err);
    }
  if (size != sizeof (T_IO_RESPONSE))
    {
      return (E_OBJ);
    }
  return (E_OK);
}
