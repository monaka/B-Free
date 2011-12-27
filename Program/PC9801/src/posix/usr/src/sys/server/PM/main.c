/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1996-01-02 13:59:01  night
 * 立ち上げ時のメッセージを表示する処理を追加。
 * 追加関数：banner()
 *
 * Revision 1.4  1996/01/02 13:54:18  night
 * コンソールのオープン処理を追加。
 * 実際にオープン処理を行うのは、open_console という libkernel.a の中の関
 * 数である。
 *
 * Revision 1.3  1995/09/21  15:53:44  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:25:38  night
 * メッセージを proc_message_t を pm_msg_t に変更。
 *
 * Revision 1.1  1995/03/18  14:02:36  night
 * 最初の登録
 *
 *
 */


/*
 * このファイルは、B-Free OS/POSIX 環境で動くプロセスサーバの一部です。
 *
 *
 */

#include "pm.h"

ID	recvport;	/* 要求を受信するためのメッセージポート */

/*
 * このファイルだけで使用する関数
 */
static void	init_process_manager (void);
static void	banner (void);

/*
 * このファイルの中だけで使用する変数
 */
static B	banner_message[] = "POSIX environment (Process manager). Version 1.0\n";


/*
 * プロセスマネージャの main 関数。
 *
 */
int
_main (void)
{
  pm_msg_t	request;

  init_process_manager ();

  banner ();

  /*
   * メッセージの受信 - 処理 - 返答のループ
   */
  for (;;)
    {
      /* メッセージの受信 (get_req は libkernel.a にある関数) */
      if (get_req (recvport, &request, sizeof (request)) > 0)
	{
	  /*
	   * リクエストの処理：返答も doit() で行う。
	   */
	  doit (&request);
	}
      else
	{
	  /* 
	   * 受信でエラーとなった。
	   * エラーとなった原因を reject して、次の要求を受けつける。
	   */
	}
    }
  /* DO NOT REACHED */
}

/*
 * プロセスマネージャの初期化
 *
 */
static void
init_process_manager ()
{
  /*
   *
   */
  int	error;

  open_console ();	/* コンソールのオープン */
  init_processinfo ();	/* プロセス情報の初期化 */

  recvport = get_port (sizeof (struct pm_msg), sizeof (struct pm_msg));
  if (recvport == 0)	/* メッセージポートの取得に失敗した */
    {
      /* error */
    }
  error = regist_port (PROCESS_MANAGER_NAME, recvport);
  if (error != E_OK)	/* メッセージポートの登録に失敗した */
    {
      /* error */
    }

}

static void
banner (void)
{
  write_console (banner_message, sizeof (banner_message));
}
