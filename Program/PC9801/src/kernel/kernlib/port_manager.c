/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/port_manager.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/port_manager.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: port_manager.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-12-05 15:10:22  night
 * unregist_port () の追加。
 *
 * Revision 1.3  1995/09/21  15:51:42  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/06/28  14:14:25  night
 * print 文を #ifdef DEBUG 〜 #endif で囲った。
 *
 * Revision 1.1  1995/02/26  14:18:12  night
 * 最初の登録
 *
 *
 */
/*
 * ポートマネージャとの通信を簡単に行うためのライブラリ関数。
 *
 * regist_port (PORT_NAME name, ID port);   メッセージバッファ ID の登録
 * unregist_port (PORT_NAME name);	    メッセージバッファ ID の抹消
 * find_port (PORT_NAME name, ID &port);    メッセージバッファ ID の検索
 * alloc_port (void);                       ポートの生成
 */

#include <itron.h>
#include <errno.h>
#include <types.h>
#include "../servers/port-manager.h"
#include "libkernel.h"

/*
 * ポートマネージャにメッセージバッファ情報を登録する。
 * この関数の中で一時的に受信用のメッセージポートを作成する。
 *
 */
PORT_MANAGER_ERROR
regist_port (PORT_NAME *name, ID port)
{
  struct recv_port_message_t	recv_msg;
  struct port_manager_msg_t  	send_msg;
  ID			     	recv_port;
  W			     	rsize;


  /*
   * ポートマネージャからの返答受けつけ用のメッセージバッファ作成。
   */
  recv_port = get_port (sizeof (recv_msg), sizeof (recv_msg));
  if (recv_port == 0)
    {
#ifdef DEBUG
      dbg_printf ("regist_port: cannot get port.\n");
#endif /* DEBUG */
      return (E_PORT_SYSTEM);
    }

  /*
   * ポートマネージャへ送る要求メッセージを作成。
   */
  send_msg.hdr.type  = REGIST_PORT;
  send_msg.hdr.size  = sizeof (send_msg);
  send_msg.hdr.rport = recv_port;
  strcpy (&(send_msg.body.regist.name), name);
  send_msg.body.regist.port = port;

#ifdef DEBUG
  dbg_printf ("regist_port: name = <%s>\n", &(send_msg.body.regist.name));
#endif /* DEBUG */

  /*
   * ポートマネージャに対して登録要求メッセージを送信する。
   */
  if (snd_mbf (PORT_MANAGER_PORT, sizeof (send_msg), &send_msg) != E_OK)
    {
      del_mbf (recv_port);
      return (E_PORT_SYSTEM);
    }

#ifdef DEBUG
  dbg_printf ("port_manager:snd_msg: ok.\n");
#endif /* DEBUG */

  /*
   * ポートマネージャからの返答メッセージの受けつけ
   */
  if (rcv_mbf (&recv_msg, &rsize, recv_port) != E_OK)
    {
      del_mbf (recv_port);
      return (E_PORT_SYSTEM);
    }

#ifdef DEBUG
  dbg_printf ("port_manager:rcv_msg: ok.\n");
#endif /* DEBUG */


  /*
   * 使いおわった返答受信用のメッセージバッファを削除する。
   */
  del_mbf (recv_port);

#ifdef DEBUG
  dbg_printf ("port_manager: errno = %d\n", recv_msg.error);
#endif /* DEBUG */
  return (recv_msg.error);
}

PORT_MANAGER_ERROR
unregist_port (PORT_NAME *name)
{
  struct recv_port_message_t	recv_msg;
  struct port_manager_msg_t  	send_msg;
  ID			     	recv_port;
  W			     	rsize;


  /*
   * ポートマネージャからの返答受けつけ用のメッセージバッファ作成。
   */
  recv_port = get_port (sizeof (recv_msg), sizeof (recv_msg));
  if (recv_port == 0)
    {
#ifdef DEBUG
      dbg_printf ("regist_port: cannot get port.\n");
#endif /* DEBUG */
      return (E_PORT_SYSTEM);
    }

  /*
   * ポートマネージャへ送る要求メッセージを作成。
   */
  send_msg.hdr.type  = UNREGIST_PORT;
  send_msg.hdr.size  = sizeof (send_msg);
  send_msg.hdr.rport = recv_port;
  strcpy (&(send_msg.body.regist.name), name);

#ifdef DEBUG
  dbg_printf ("regist_port: name = <%s>\n", &(send_msg.body.regist.name));
#endif /* DEBUG */

  /*
   * ポートマネージャに対して登録要求メッセージを送信する。
   */
  if (snd_mbf (PORT_MANAGER_PORT, sizeof (send_msg), &send_msg) != E_OK)
    {
      del_mbf (recv_port);
      return (E_PORT_SYSTEM);
    }

#ifdef DEBUG
  dbg_printf ("port_manager:snd_msg: ok.\n");
#endif /* DEBUG */

  /*
   * ポートマネージャからの返答メッセージの受けつけ
   */
  if (rcv_mbf (&recv_msg, &rsize, recv_port) != E_OK)
    {
      del_mbf (recv_port);
      return (E_PORT_SYSTEM);
    }

#ifdef DEBUG
  dbg_printf ("port_manager:rcv_msg: ok.\n");
#endif /* DEBUG */


  /*
   * 使いおわった返答受信用のメッセージバッファを削除する。
   */
  del_mbf (recv_port);

#ifdef DEBUG
  dbg_printf ("port_manager: errno = %d\n", recv_msg.error);
#endif /* DEBUG */
  return (recv_msg.error);
}


/*
 * ポートマネージャからメッセージバッファ情報を検索する。
 * この関数の中で一時的に受信用のメッセージポートを作成する。
 *
 */
PORT_MANAGER_ERROR
find_port (PORT_NAME *name, ID *rport)
{
  struct recv_port_message_t	recv_msg;
  struct port_manager_msg_t  	send_msg;
  ID			     	recv_port;
  W			     	rsize;


  /*
   * ポートマネージャからの返答受けつけ用のメッセージバッファ作成。
   */
  recv_port = get_port (sizeof (recv_msg), sizeof (recv_msg));
  if (recv_port == 0)
    {
#ifdef DEBUG
      dbg_printf ("regist_port: cannot get port.\n");
#endif /* DEBUG */
      return (E_PORT_SYSTEM);
    }

  /*
   * ポートマネージャへ送る要求メッセージを作成。
   */
  send_msg.hdr.type  = FIND_PORT;
  send_msg.hdr.size  = sizeof (send_msg);
  send_msg.hdr.rport = recv_port;
  strcpy (&(send_msg.body.find.name), name);

#ifdef DEBUG
  dbg_printf ("find_port: name = <%s>\n", &(send_msg.body.find.name));
#endif /* DEBUG */
  /*
   * ポートマネージャに対して検索要求メッセージを送信する。
   */
  if (snd_mbf (PORT_MANAGER_PORT, sizeof (send_msg), &send_msg) != E_OK)
    {
      del_mbf (recv_port);
      return (E_PORT_SYSTEM);
    }

#ifdef DEBUG
  dbg_printf ("port_manager:snd_msg: ok.\n");
#endif /* DEBUG */

  /*
   * ポートマネージャからの返答メッセージの受けつけ
   */
  if (rcv_mbf (&recv_msg, &rsize, recv_port) != E_OK)
    {
      del_mbf (recv_port);
      return (E_PORT_SYSTEM);
    }

#ifdef DEBUG
  dbg_printf ("port_manager:rcv_msg: ok.\n");
#endif /* DEBUG */

  /*
   * ポートマネージャが検索したポート番号を返す。
   */
  *rport = recv_msg.port;

  /*
   * 使いおわった返答受信用のメッセージバッファを削除する。
   */
  del_mbf (recv_port);

#ifdef DEBUG
  dbg_printf ("port_manager: errno = %d\n", recv_msg.error);
#endif /* DEBUG */
  return (recv_msg.error);
}

/*
 * メッセージバッファを生成する。
 * メッセージバッファ ID は、自動的に空いているものを使用する。
 *
 */
ID
alloc_port (W size, W max_entry)
{
  ID		msg_port;
  T_CMBF	create_argument;

  /*
   * 要求受けつけのためのメッセージバッファを作成する。
   * メッセージバッファの ID は特に決まっていない。空いているメッセー
   * ジバッファを適当に選ぶ。
   */
  create_argument.bufsz  = size;
  create_argument.maxmsz = size * max_entry;
  create_argument.mbfatr = TA_TFIFO;
  for (msg_port = MIN_USERMBFID;
       msg_port <= MAX_USERMBFID;
       msg_port++)
    {
      if (cre_mbf (msg_port, &create_argument) == E_OK)
	{
	  /*
	   * メッセージバッファの取得に成功した。
	   */
	  return (msg_port);
	}
    }

  /*
   * メッセージバッファが取得できなかった。
   */
#ifdef DEBUG
  dbg_printf ("posix.process server: cannot allocate messege buffer\n");
#endif /* DEBUG */
  return (0);
}
