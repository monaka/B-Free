/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/servers/port-manager.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: port-manager.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:49  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/03/18  14:12:46  night
 * 最初の登録
 *
 *
 */

/* 
 *  ポートマネージャに関する構造体や定数の定義。
 *
 *  ポートマネージャに対して要求を送るタスクは、このヘッダファイルをイ
 *  ンクルードすべきである。
 *  ただし、メッセージに関するライブラリ libport.a を使う場合には、特
 *  にこのヘッダファイルを参照しなくてもよい。
 *  libport.a ライブラリが、適当な処理を行ってくれる。
 *
 */

#ifndef __PORT_MANAGER_H__
#define __PORT_MANAGER_H__


/*
 * メッセージタイプの定義
 * ポートマネージャに対してメッセージバッファを登録/抹消/参照するとき
 * には、このメッセージタイプを使って動作の種類を指定する。
 */
#define NULL_PORT	0	/* 何もしない */
#define REGIST_PORT	1	/* メッセージバッファの登録 */
#define UNREGIST_PORT	2	/* メッセージバッファの抹消 */
#define FIND_PORT	3	/* メッセージバッファの参照 */



/*
 * 登録するときに使用する名前の最大長
 */
#define PORT_NAME_LEN	40


/*
 * 登録するエントリの最大数
 */
#define MAX_PORT_ENTRY	100


/*
 * ITRON (中心核) 内に、あらかじめ確保しておくメッセージを受信するため
 * に使用するメッセージバッファ用の領域。
 * 要求受けつけ用のメッセージバッファを cre_mbf システムコールを使って
 * 作成するときにこの値を使用する。
 */
#define MAX_MSG_ENTRY	1


/*
 * メッセージのヘッダ部分の定義
 * ポートマネージャに対して送信するメッセージは、この構造体で規定して
 * いるデータを最初につける。
 */
struct port_message_header_t
{
  W	type;	/* REGIST_PORT/UNREGIST_PORT/FIND_PORT のいずれかの値 */
		/* が入る(もしくは、NULL_PORT が入ることもあるが、ポー */
		/* トマネージャは、NULL_PORT のメッセージは処理せず、 */
		/* 単に無視する。                                     */

  W	size;	/* メッセージのサイズが入る。サイズにはこのヘッダ部分 */
		/* も入る。					      */

  ID	rport;	/* 返答するためのメッセージバッファ。ポートマネージャ  */
		/* に対してメッセージを送ったときに使ったメッセージバッ*/
		/* ファは返答には使用しない (他のタスクも同じメッセー  */
		/* ジバッファを使用するため)。その代わり、rport で指定 */
		/* したメッセージバッファを使用する。                  */
};


/*
 * メッセージバッファを識別するためにつける名前の型
 */
typedef B	PORT_NAME[PORT_NAME_LEN + 1];


/*
 * メッセージ登録処理を行うときに使用する構造体
 */
struct regist_port_t
{
  PORT_NAME	name;	/* ポートの名前； この名前はシステムグローバル */
			/* でなければいけない。もし、すでに同じ名前が  */
			/* 登録されていたときにはエラーとなる。        */

  ID		port;	/* 登録するメッセージバッファの ID が入る。メッ */
			/* セージバッファの ID は、すでに登録されてい   */
			/* ても構わない。ポートマネージャは特にメッセー */
			/* ジバッファが重複して登録されているかのチェッ */
			/* クは行わない。                               */
  			/* 残念ながら、ポートマネージャは、メッセージ   */
			/* ポートを適当に割り当てることはしない。       */

  ID		task;	/* ポートマネージャに対してリクエストを送った */
			/* タスクの ID 。この値は、削除するときに識別 */
			/* するために使用する。これは、登録したタスク */
			/* とは別のタスクが不正にメッセージバッファ情 */
			/* 報を削除することを防ぐために使用するための */
			/* 値である。                                 */
};


/*
 * メッセージバッファの抹消処理を行うときに使用する構造体
 */
struct unregist_port_t
{
  PORT_NAME name;	/* 登録テーブルから抹消するメッセージバッファ */
			/* の名前。登録するときに使用した名前を指定す */
			/* る。*/

  ID task;		/* 抹消処理を要求したタスクの ID 。この値は、 */
			/* メッセージバッファを登録したタスクと同じタ */
			/* スクかどうかを判別するために使用する。     */
};


/*
 * メッセージバッファの ID を検索するときに使用する構造体
 */
struct find_port_t
{
  PORT_NAME	name;	/* 検索するポートの名前; メッセージバッファを */
			/* 登録するときに使用した名前を指定する。     */
};



/*
 * メッセージ本体部分の定義
 */
union port_message_body_t
{
  struct regist_port_t		regist;		/* 登録するときに使用 */
  				                /* する情報。         */

  struct unregist_port_t	unregist;	/* 登録を抹消するとき */
						/* に使用する情報。   */

  struct find_port_t		find;		/* 登録したメッセージ */
						/* バッファを検索する */
						/* ときに使用する情報。*/
};


/*
 * ポートマネージャに送るメッセージ
 */
struct port_manager_msg_t
{
  struct port_message_header_t	hdr;	/* メッセージのヘッダ部分。こ */
					/* の部分によってメッセージの */
					/* 後ろの情報が決まってくる。 */

  union port_message_body_t	body;	/* メッセージ本体。union で定 */
					/* 義されていることから分かる */
					/* ように、メッセージ本体は、 */
					/* メッセージの種類ごとに異なっ */
					/* た形式となっている。*/
};


/*
 * ポートマネージャからの返答；エラー番号
 * ポートマネージャへ要求を送ったときには、この中に指定した値のどれか
 * が必ず返る。
 */
typedef enum port_manager_error_t
{
  E_PORT_OK,			/* 正常 */

  E_PORT_DUPLICATION,		/* 登録しようとしたメッセージバッファ */
				/* の名前はすでに登録されている。     */

  E_PORT_NOTFOUND,		/* 登録テーブルから検索しようとしたメッ */
				/* セージバッファは存在しなかった。     */

  E_PORT_FULL,			/* 登録するための空きエントリがなかっ */
				/* た。                               */

  E_PORT_INVALID,		/* 登録したタスクと抹消要求を出したタ */
				/* スクが異なっていた (権限のチェック) */

  E_PORT_SYSTEM			/* システムコールでエラーが発生した。 */
} PORT_MANAGER_ERROR;


/*
 * ポートマネージャからの返答メッセージ
 * ポートマネージャに対しては、いくつかメッセージの種類があった。
 * 返答は、一種類の構造体ですます。
 *
 */
struct recv_port_message_t
{
  PORT_MANAGER_ERROR   	error;	/* ポートマネージャの処理が正常に終っ */
				/* たかを示す。エラー番号が入る。もし、 */
				/* エラーとなったときには、次の要素  */
				/* port は、意味がなくなる。         */

  ID	port;			/* メッセージバッファの ID 。         */
                                /* 登録処理ではこの値は特に必要としな */
				/* いが、いちおう登録したときのメッセー */
				/* ジバッファの ID が入る。           */
				/* ポートマネージャから登録を抹消する */
				/* ときにも、同様に登録したときのメッ */
				/* セージバッファの ID が入る。 抹消し */
				/* た後ではメッセージバッファは使えな */
				/* くなることが考えられるので、登録処 */
				/* 理よりも意味がない値である。       */
  			        /* 一番この値を使用するのは、メッセー */
				/* ジバッファ ID を検索する処理のとき */
				/* だろう。                           */
};


/*
 * データベース操作関数。
 * <登録> <削除> <参照 (検索)> の処理を行う。
 */
extern PORT_MANAGER_ERROR regist_database (PORT_NAME name, ID port, ID task);
extern PORT_MANAGER_ERROR unregist_database (PORT_NAME name, ID *port, ID task);
extern PORT_MANAGER_ERROR find_database (PORT_NAME name, ID *port);


#endif /* __PORT_MANAGER_H__ */
