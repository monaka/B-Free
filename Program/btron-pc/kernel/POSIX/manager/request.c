/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/request.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


/* request.c - POSIX 関係のメッセージ処理を行う
 *
 *
 *
 * $Log: request.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  2000-02-19 03:02:15  naniwa
 * minor change on debug write
 *
 * Revision 1.6  1999/03/24 08:00:20  monaka
 * Modified include pathes (POSIX manager should be independ from ITRON.)
 *
 * Revision 1.5  1997/10/24 14:00:10  night
 * 変数の引数合わせを厳密にした。
 *
 * Revision 1.4  1997/04/28 15:29:10  night
 * デバイスドライバからのメッセージを受信する領域をスタック上に
 * とっていたところを、static 領域に設定した。
 *
 * Revision 1.3  1996/11/20  12:12:47  night
 * エラーチェックの追加。
 *
 * Revision 1.2  1996/11/07  12:48:52  night
 * get_request() と put_response() の追加。
 *
 * Revision 1.1  1996/11/05  15:13:46  night
 * 最初の登録
 *
 */

#include "posix.h"
#include <port-manager.h>


static ID	request_port;	/* 要求受け付け用のポート	*/
				/* ITRON のメッセージポート	*/




/* init_port - 要求受け付け用のポートを初期化する
 *
 * 処理
 *	もし、すでにメッセージポートが初期化されていた場合には
 * 	何もしない。その場合には、SUCCESS が返る。
 *
 * 返り値
 *	SUCCESS	処理が成功
 *	FAIL	処理が失敗
 *
 */
W
init_port (void)
{
  ID	port;

  /* すでにポートが初期化されていた
   */
  if (request_port != 0)
    {
      return (SUCCESS);
    }

  /* ポートを作成する */
  if ((port = get_port (sizeof (struct posix_request), sizeof (struct posix_request))) <= 0)
    {
      /* ポートが作成できなかった */
      return (FAIL);
    }

  request_port = port;

  /* ポートマネージャに登録する */
  if (regist_port ((PORT_NAME *)POSIX_MANAGER, port) != E_PORT_OK)
    {
      /* ポートを削除する処理がここに入る */
      return (FAIL);
    }

  return (SUCCESS);	/* ポートが作成できた */
}


/* get_request - リクエストを受信する
 *
 */
W
get_request (struct posix_request *req)
{
  extern ER	sys_errno;
  INT		size;

  ASSERT (req != NULL);

  size = sizeof (struct posix_request);
  sys_errno = rcv_mbf (req, &size, request_port);
  if (sys_errno != E_OK) {
    dbg_printf("[PM] get_request: rcv_mbf error %d\n", sys_errno);
    return (FAIL);
  }
  return (SUCCESS);
}


/* put_response -
 *
 */
W
put_response (struct posix_request *req, 
	      W errno,
	      W status,
	      W ret1,
	      W ret2)
{
  static struct posix_response res;
  ER	syserr;

  if (req == NULL)
    {
      return (EP_INVAL);
    }

  res.receive_port = request_port;
  res.msg_length = sizeof (res);
  res.operation = req->operation;
  res.errno = errno;
  res.status = status;
  res.ret1 = ret1;
  res.ret2 = ret2;

  /* 要求元に送信する */
  syserr = snd_mbf (req->receive_port, sizeof (res), &res);
  return (EP_OK);
}


/* エラーになったことをリクエストの送り元に返す
 *
 */
W
error_response (struct posix_request *req, W errno)
{
  /* 要求元に送信する */
  return (put_response (req, errno, 0, 0, 0));
}
