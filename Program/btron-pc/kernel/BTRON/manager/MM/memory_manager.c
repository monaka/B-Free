/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/memory_manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/memory_manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: memory_manager.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1997-07-06 11:54:25  night
 * banner() の処理を変更。
 *
 * Revision 1.3  1997/07/02 13:10:32  night
 * malloc の修正。
 *
 * Revision 1.2  1997/06/29 15:44:32  night
 * コメントの変更
 *
 * ・漢字コードを SJIS から EUC に変更した。
 * ・RCS の Log マクロ入りのコメントを追加した(まだ入れていなかったファイルのみ)。
 *
 *
 */


#include "memory_manager.h"


void
start ()
{
  struct mm_request_t	request;

  init ();
  banner ();

  for (;;)
    {
      /* 要求の受け付けと処理 */
      if (get_request (&request) == FAIL)
	{
	  /* リクエスト取得に失敗した */
#ifdef DEBUG
	  printf ("Cannot get request.\n");
#endif
	  continue;
	}

#ifdef DEBUG
      printf ("OP = %d\n ", request.operation);
#endif /* DEBUG */

#ifdef notdef
      /* 取得したリクエストを処理する */
      if ((request.operation < 0) || (request.operation > NR_POSIX_SYSCALL))
	{
	  /* リクエスト要求にあるオペレーションは、サポートしていない */
	  error_response (&request, EP_NOSUP);
	}
      else
	{
	  printf ("systemcall: %s\n", syscall_table[request.operation].name);
	  (*syscall_table[request.operation].syscall)(&request);
	}
#endif
    }

}


banner ()
{
  printf ("BTRON MEMORY MANAGER Version %d.%d. start\n", VERSION_ID_HIGH, VERSION_ID_LOW);
}


void
init (void)
{
  /* port の初期化 / 登録 */
  init_port ();

  /* メッセージ出力機構の初期化 */
  init_log ();

  /* 内部メモリ管理(malloc)の初期化 */
  if (init_malloc () != E_OK)
    {
      printf ("MM: cannot init memory allocate system(malloc).\n");
      stop ();
    }
}


void
stop (void)
{
  for (;;)
    {
      slp_tsk ();
    }
}
