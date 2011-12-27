/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/servers/port-manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/servers/port-manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: port-manager.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-18 17:48:36  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.3  1999/04/13 04:14:58  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.2  1999/02/17 14:56:35  night
 * メッセージバッファ属性の初期化処理を追加
 *
 * +  msg_pk.mbfatr = TA_TFIFO;
 *
 * Revision 1.1  1996/07/23 00:03:04  night
 * IBM PC 用の最初の登録
 *
 * Revision 1.3  1995/09/21  15:51:48  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/06/26  15:19:15  night
 * いくつかの printf を DEBUG マクロで囲んだ。
 *
 * Revision 1.1  1995/03/18  14:12:45  night
 * 最初の登録
 *
 *
 */

/*
 * ポートマネージャ
 *
 * <このサーバは、プログラム環境 (BTRON or POSIX) に関係なく動作する>
 *
 *
 * 周辺核がアプリケーションと通信するために使用するメッセージバッファ
 * の ID を管理する。
 * ITRON (中心核) はメッセージバッファによる通信ができる。ただし、
 * そのままでは、周辺核がどのメッセージバッファを使っているかわからな
 * い。
 * ポートマネージャは、グローバルな名前とメッセージバッファの ID の組
 * を管理する。タスクは、ポートマネージャに対して登録することによって、
 * 他のタスクからメッセージバッファをアクセスできるようにする。
 *
 * 何もしなくてもポートマネージャに対してアクセスできるように、ポート
 * マネージャ自身は、特定のメッセージバッファ ID (11) を使う。
 * 
 * ポートマネージャは、(周辺核と同様に) サーバとして動作する (ユーザモー
 * ド(CPU 特権レベル 3)で動作する)
 *
 * ポートマネージャのメッセージは次の形式が用意されている。
 *
 * regist_port_t	{ PORT_NAME name; ID port; ID task; }
 * unregist_port_t	{ PORT_NAME name; ID task; }
 * find_port_t		{ PORT_NAME name; }
 *
 * さらに、次に示すヘッダが加わったメッセージがポートマネージャに送ら
 * れることになる。
 *
 * msg_header { W msg_type; W size };
 *
 *
 * ポートマネージャに対する要求を簡略化するために、ライブラリ 
 * (libport.a) を用意している。このライブラリは、ポートマネージャに対
 * するメッセージの送受信を行う関数の他にメッセージを扱うための便利な
 * 関数が入っている。
 * で、このライブラリについては、ポートマネージャに関する関数としては、
 * 次のものがある。
 *
 * regist_port (PORT_NAME name, ID port);   メッセージバッファ ID の登録
 * unregist_port (PORT_NAME name);	    メッセージバッファ ID の抹消
 * find_port (PORT_NAME name, ID &port);    メッセージバッファ ID の検索
 * 
 */


#include <itron.h>
#include <errno.h>
#include <types.h>
#include "port-manager.h"


/*
 * グローバル変数の宣言
 */
ID	request_port;


/*
 * ポートマネージャで使う関数のプロトタイプ宣言。
 *
 */
extern void	_main (void);
extern void	regist_port (struct port_manager_msg_t *msgp);
extern void	unregist_port (struct port_manager_msg_t *msgp);
extern void	find_port (struct port_manager_msg_t *msgp);


/*
 * このファイルの中でしか使用しないスタティック関数の定義
 */
static void	recv_port_manager (ID rport, PORT_MANAGER_ERROR errno, ID port);


/*
 *	ポートマネージャの main 処理
 *	メッセージを受信する準備を行い、その後メッセージを受信 - 処理する
 *	ループに入る。
 */
void
_main (void)
{
  T_CMBF				msg_pk;
  ER					error;
  struct port_manager_msg_t		msg_buf;
  INT					size;


  /*
   * プログラムの初期化
   * メッセージバッファを作る。
   * メッセージバッファ ID は、PORT_MANAGER_PORT マクロで指定した
   * ものを使用する。
   *
   *
   * <将来の拡張へのアイデア>
   *
   * 将来は、メッセージバッファ作成時にアクセス権を指定できるようにする。
   * アクセス権を指定することによって、メッセージバッファを生成したタス
   * ク以外は、メッセージを読み取れないなどの設定を可能とする。
   *
   */
  msg_pk.bufsz = sizeof (struct port_manager_msg_t);
  msg_pk.maxmsz = sizeof (struct port_manager_msg_t) * MAX_MSG_ENTRY;
  msg_pk.mbfatr = TA_TFIFO;
  error = cre_mbf (PORT_MANAGER_PORT, &msg_pk);
  if (error != E_OK)
    {
      exd_tsk ();
      /* NOT REACHED */
    }

  /* 
   * 登録テーブルの初期化
   */
  init_regist_table ();

  dbg_puts ("port manager start.\n");
  /*
   *	メッセージ受信と処理
   *	ポートマネージャは、シングルタスクで動作する。
   *	そのため、メッセージの受信から処理の終了までは、他の要求は受け
   *	つけない。
   */
  for (;;)
    {
      /* 
       * メッセージを受信する。
       * 受信するときに使用するメッセージバッファは、PORT_MANAGER_PORT
       * マクロで指定したものを使用する。
       */
      error = rcv_mbf (&msg_buf, &size, PORT_MANAGER_PORT);
      if (error == E_OK)
	{
	  /*
	   * メッセージを受信したことを表示する。
	   * これは、デバッグのために入れた。
	   */
#ifdef DEBUG
	  dbg_puts ("port-manager: message read.\n");
#endif /* DEBUG */

	  /*
	   * メッセージの処理
	   * 受信したメッセージのヘッダの種類から適切な処理を行う。
	   */
	  switch (msg_buf.hdr.type)
	    {
	    case REGIST_PORT:	
	      /*
	       * メッセージバッファ ID の登録
	       */
	      dbg_printf ("port-manager: (regist) <%s>\n", msg_buf.body.regist.name);
	      regist_port (&msg_buf);
	      break;

	    case UNREGIST_PORT:
	      /*
	       * メッセージバッファ ID の抹消
	       */
	      unregist_port (&msg_buf);
	      break;

	    case FIND_PORT:
	      /*
	       * メッセージバッファ ID の検索
	       */
	      find_port (&msg_buf);
	      break;
	    }
	}
      /* NOT REACHED */
    }
}

/*
 * メッセージバッファ ID の登録処理を行う。
 *
 */
void
regist_port (struct port_manager_msg_t *msgp)
{
  PORT_MANAGER_ERROR errno;

  /*
   * データベースに登録する
   */
  errno = regist_database (msgp->body.regist.name,
			   msgp->body.regist.port,
			   msgp->body.regist.task);

#ifdef DEBUG  
  dbg_printf ("port-server: regist_port: <%s>\n", msgp->body.regist.name);
#endif /* DEBUG */

  /*
   * 要求の送り元に対して返答メッセージを送る。
   * 処理が正常に終了した場合とで送るメッセージが違ってくるので、
   * 返答処理も2通り存在する。
   */
  if (errno == E_PORT_OK)
    {
      /*
       * 正常に処理が終了した場合の返答メッセージ送信
       */
      recv_port_manager (msgp->hdr.rport,
			 errno, 
			 msgp->body.regist.port);
    }
  else
    {
      /*
       * 正常に処理が終わらなかった場合の返答メッセージ送信。
       * 返答メッセージのうちメッセージバッファ ID については
       * 取得できなかったので、0 を返す。
       */
      recv_port_manager (msgp->hdr.rport, errno, 0);
    }
}



/*
 * メッセージバッファ ID の登録抹消処理を行う。
 *
 */
void
unregist_port (struct port_manager_msg_t *msgp)
{
  PORT_MANAGER_ERROR	errno;
  ID		     	port;

  /*
   * データベースから情報を削除する。
   */
  errno = unregist_database (msgp->body.unregist.name,
			     &port,
			     msgp->body.unregist.task);

  /*
   * 登録抹消を要求したタスクへ返答メッセージを返す。
   */
  if (errno != E_PORT_OK)
    {
      /*
       * 正常に処理が終了した場合の返答メッセージ送信
       */
      recv_port_manager (msgp->hdr.rport,
			 errno, 
			 port);
    }
  else
    {
      /*
       * データベースからエントリを削除できなかった。
       * エラーメッセージを返答する。
       * エラー番号以外の内容はすべて 0 を埋めて返す。
       */
      recv_port_manager (msgp->hdr.rport, errno, 0);
    }
}

/*
 * メッセージバッファ ID の検索
 * 
 *
 */
void
find_port (struct port_manager_msg_t *msgp)
{
  PORT_MANAGER_ERROR	errno;
  ID		     	port;

  /*
   * データベースから情報を検索する。
   */
  errno = find_database (msgp->body.find.name, &port);

  /*
   * デバッグ文：データベースからの検索結果
   */
#ifdef DEBUG
  dbg_printf ("port-manager: find_port: errno = %d, port = %d\n", errno, port);
#endif /* DEBUG */

  /*
   * メッセージバッファ ID を要求したタスクへ返答メッセージを返す。
   */
  if (errno == E_PORT_OK)
    {
      /*
       * 正常に処理が終了した場合の返答メッセージ送信。
       * ポート番号として、find_database() で発見したメッセージバッファ 
       * ID を返す。
       */
      recv_port_manager (msgp->hdr.rport,
			 errno, 
			 port);
    }
  else
    {
      /*
       * データベースからエントリを削除できなかった。
       * エラーメッセージを返答する。
       * エラー番号以外の内容はすべて 0 を埋めて返す。
       */
      recv_port_manager (msgp->hdr.rport, errno, 0);
    }
}


/*
 * ポートマネージャに対して要求を送ったタスクに対して返答メッセージを返す。
 */
static void
recv_port_manager (ID rport, PORT_MANAGER_ERROR errno, ID port)
{
  /*
   * 返答メッセージの内容が入る領域。
   * 構造体 port_recv_port_message_t によって構造を規定する。
   */
  struct recv_port_message_t	recv_msg;

  /*
   * 返答メッセージの組み立て
   */
  recv_msg.error = errno;	/* エラー番号の設定 */
  recv_msg.port  = port;	/* メッセージバッファ ID の設定 */

  /*
   * 返答メッセージを要求元に送る。
   * 今のところ、snd_mbf() システムコールのエラーは無視している。
   */
  snd_mbf (rport, sizeof (recv_msg), &recv_msg);
}

