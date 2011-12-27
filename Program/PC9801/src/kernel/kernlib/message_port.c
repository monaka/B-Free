/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/message_port.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/message_port.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: message_port.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:51:41  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/09/17  17:06:23  night
 * get_req() 関数の中で rcv_mbf システムコールを呼び出す。このときのエラー
 * を kernlib の大域変数 sys_errno に記憶するように変更した。
 * 以前は、エラーはすてていた。
 *
 * Revision 1.2  1995/08/05  12:09:48  night
 * get_req() 関数の追加。
 *
 * Revision 1.1  1995/02/26  14:17:51  night
 * 最初の登録
 *
 *
 */
/*
 * メッセージバッファを簡単に扱うための関数群。
 *
 */
#include <h/types.h>
#include <h/itron.h>
#include <h/config.h>
#include <h/errno.h>
#include "../servers/port-manager.h"


/*
 * 適当な ID をもつメッセージバッファを作成する。
 */
ID
get_port (W minsize, W maxsize)
{
  ID		msg_port;
  T_CMBF	create_argument;
  W		errno;

  /*
   * 要求受けつけのためのメッセージバッファを作成する。
   * メッセージバッファの ID は特に決まっていない。空いているメッセー
   * ジバッファを適当に選ぶ。
   */
  create_argument.bufsz  = minsize;
  create_argument.maxmsz = maxsize;
  create_argument.mbfatr = TA_TFIFO;
  for (msg_port = MIN_USERMBFID;
       msg_port <= MAX_USERMBFID;
       msg_port++)
    {
      errno = cre_mbf (msg_port, &create_argument);
      if (errno == E_OK)
	{
	  return (msg_port);
	}
    }
  return (0);
}

/*
 * 引数で指定したポートからメッセージを受信する。
 * マネージャで使用するための関数。
 */
W
get_req (ID port, VP request, W *size)
{
  extern ER	sys_errno;

  sys_errno = rcv_mbf (request, size, port);
  if (sys_errno != E_OK)
    return (0);
  return (*size);
}

