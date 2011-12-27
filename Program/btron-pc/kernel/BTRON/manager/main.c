/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 */

/*
 *
 *
 *
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-10-29 15:48:06  night
 * 最初の登録
 *
 *
 */

#include "manager.h"


/* プログラムの中で使用する大域変数
 */
ID	recvport;


/* このファイルの中でのみ使用する関数および変数
 */
static	void	doit (MANAGER_MSG *);


/*
 * BTRON マネージャの main 関数
 *
 */
void 
main ()
{
  ER	error;
  extern ER	sys_errno;
  MANAGER_MSG	req;


  /* メッセージポートの登録
   */
  recvport = get_port (sizeof (MANAGER_MSG), sizeof (MANAGER_MSG));
  if (recvport <= 0)
    {
      dbg_printf ("manager: cannot make receive port.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (MANAGER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("manager: cannot regist port (error = %d)\n", error);
    }

  /* マネージャの初期化
   * ファイル管理部分
   * メモリ管理部分
   * プロセス管理部分
   *
   */
  /* fm_init () */
  /* pm_init () */
  /* mm_init () */


  /* メッセージ取り出し - 処理ループ
   */
  for (;;)
    {
      W	rsize;

      /* 要求の受信 */
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* 正常ケース */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("CONSOLE: get_req() Unknown error(error = %d)\n", sys_errno);
	  break;
	}
    }

  /* NOT REACHED */
}


/* doit --- 各機能別のモジュールへ処理を分岐する
 *
 */
static void
doit (MANAGER_MSG *request)
{
  switch ((request->body.request.command) & 0xff00)
    {
    case PM_MASK:		/* PM_MASK == 0x0100 */
      pm_receive (request);	/* プロセス管理へメッセージを渡す */
      break;

    case FM_MASK:		/* FM_MASK == 0x0200 */
      fm_receive (request);	/* ファイル管理へメッセージを渡す */
      break;

    case MM_MASK:		/* MM_MASK == 0x0300 */
      mm_receive (request);	/* メモリ管理へメッセージを渡す */
      break;

      /* ... デバイスドライバ等の処理が入る */

    default:		/* 理解できないメッセージを受けとった */
      unknown_error (request);
      break;
    }
}
