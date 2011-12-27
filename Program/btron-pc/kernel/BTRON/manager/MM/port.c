/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: port.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-06-29 15:44:34  night
 * コメントの変更
 *
 * ・漢字コードを SJIS から EUC に変更した。
 * ・RCS の Log マクロ入りのコメントを追加した(まだ入れていなかったファイルのみ)。
 *
 *
 */

#include "memory_manager.h"


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
  if ((port = get_port (sizeof (struct mm_request_t), sizeof (struct mm_request_t))) <= 0)
    {
      /* ポートが作成できなかった */
      return (FAIL);
    }

  request_port = port;

  /* ポートマネージャに登録する */
  if (regist_port (BTRON_MM_MANAGER, port) != E_PORT_OK)
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
get_request (struct mm_request_t *req)
{
  extern ER	sys_errno;

  ASSERT (req != NULL);

  sys_errno = rcv_mbf (req, sizeof (struct mm_request_t), request_port);
  if (sys_errno != E_OK)
    return (0);
  return (SUCCESS);
}


/* put_response -
 *
 */
W
put_response (struct mm_request_t *req, 
	      W errno,
	      W status,
	      W ret1,
	      W ret2)
{
  static struct mm_response_t	res;
  ER				syserr;

  if (req == NULL)
    {
      return (EMM_INVAL);
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
  return (EMM_OK);
}


/* エラーになったことをリクエストの送り元に返す
 *
 */
W
error_response (struct mm_request_t *req, W errno)
{
  /* 要求元に送信する */
  return (put_response (req, errno, 0, 0, 0));
}
