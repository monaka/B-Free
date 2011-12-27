/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/fm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/fm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: fm.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:56  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/02  09:33:30  night
 * コメント追加
 *
 * Revision 1.1  1995/03/04  14:23:11  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX ファイル管理マネージャ (FM) と通信を行うための関数群
 *
 * このファイルの中で定義する関数は以下のとおり：
 *
 * fm_call
 * fm_send
 * fm_receive
 */

#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include <server/proc.h>
#include <server/file.h>
#include "../../../../../kernel/servers/port-manager.h"
#include "funcs.h"


/***********************************************************************/
/*			                                               */
/*			    static 変数の宣言			       */
/*		    (このファイルの中でのみ使用する)                   */
/*			                                               */
/***********************************************************************/


/*
 * ファイルサーバへ要求パケットを送信するためのメッセージバッファの ID 
 * を記憶しておくための変数。
 *
 * ファイルサーバへ要求を送るとき、この変数をチェックする。
 * もし、この変数の値が 0 の場合には、ファイルサーバの要求受けつけ用の
 * メッセージバッファをみつけるために、ポートマネージャに問い合わせる。
 * (ポートマネージャに登録するときの名前は、マクロ POSIX_FILE_SERVER で
 * 定義している)
 * 
 */
static ID	file_server_port;



/***********************************************************************/
/*			                                               */
/*			    static 関数の定義			       */
/*		    (このファイルの中でのみ使用する)                   */
/*			                                               */
/***********************************************************************/


/*
 * ファイルマネージャと通信に使うポートを得るために使用する関数。
 * この関数は、static 変数 file_server_port を使用する。
 *
 */
static ID	get_request_port (void);


 
/*
 * ファイルマネージャへ要求パケットを送信し、返答を受けとる。
 *
 */
int
fm_call (file_message_t *req, file_reply_t *reply)
     /*
      * req	ファイルマネージャへ送信するパケット
      * reply	ファイルマネージャからの返答がはいる領域
      */
{
  ID receive_port;	/* 返答を受けとるためのメッセージポート */

  receive_port = get_port (sizeof (file_reply_t), sizeof (file_reply_t));
  if (receive_port == 0)
    {
      /* ポートが取得できなかった */
      reply->error = FILE_ENOMEM;
      return (-1);
    }
  req->caller = receive_port;	/* 返答用ポートを送信メッセージに	*/
				/* 組み入れる 				*/

  /*
   * ファイルマネージャにメッセージを送信
   */
  if (fm_send (req) == 0)
    {
      /* 送信できなかった */
      /* ポートを破棄する */
      
      reply->error = FILE_ENOMEM;
      return (-1);
    }

  /*
   * ファイルマネージャからの返答を受けとる (まだ途中)
   */


  return (0);
}


/*
 * POSIX ファイルマネージャのもつ要求メッセージ受けつけ用メッセージポー
 * トを取得するときに使用する関数。
 *
 * 要求受けつけ用のメッセージバッファ ID は、file_server_port に登録さ
 * れている。 この関数は、file_server_port が 0 (つまり、メッセージポー
 * トは登録されていない) 場合にポートマネージャと通信し、登録してある
 * 要求受けつけ用のメッセージバッファを得る。
 *
 */
static ID
get_request_port (void)
{
  PORT_MANAGER_ERROR	error;

  if (file_server_port == 0)
    {
      /* static 変数 file_server_port が 0 の場合には、ファイルマネー
       * ジャのもつ要求メッセージポートの ID をポートマネージャに問い
       * 合わせる。                                                  
       */
      error = find_port (POSIX_FILE_SERVER, &file_server_port);
      if (error != E_PORT_OK)
	{
	  /* ポートが取得できなかった。
	   * 0 を返す。
	   */
	  return (0);
	}
    }

  return (file_server_port);
}
