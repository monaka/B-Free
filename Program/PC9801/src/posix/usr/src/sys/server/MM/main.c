/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:42  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:22:51  night
 * メッセージ構造体を posix_message_t から mm_args に変更。
 *
 * Revision 1.1  1995/03/18  14:01:50  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX 環境でのメモリマネージャ
 *
 */

#include <sys/types.h>
#include <sys/errno.h>
#include <server/memory.h>


/*
 * 大域変数の宣言 (実際に場所を確保する)
 */
ID	recvport;	/* 要求を受信するためのメッセージポート */

/*
 * このファイルだけで使用する関数
 */
static void	init_memory_manager (void);


/*
 * メモリマネージャの処理を行う。
 *
 */
void
_main (int ac, char **av)
{
  struct mm_msg	request;
  int		error;

  init_memory_manager ();	/* メモリマネージャ全体の初期化 */

  /*
   * メッセージの受信 - 処理 - 返答のループ
   */
  for (;;)
    {
      /* メッセージの受信 */
      error = get_req (recvport, &request, sizeof (request));
      if (error >= 0)
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
	  switch (error)
	    {
	    default:	/* Unknown error */
	      dbg_puts ("POSIX/MM: Unknown error\n");
	      break;

	    case EMM_SYSTEM:
	      break;
	    }
	}
    }
  /* DO NOT REACHED */
}


/*
 * メモリマネージャの初期化
 *
 *
 */
static void
init_memory_manager (void)
{
  int	error;

  init_pageinfo ();	/* 仮想メモリ情報の初期化 */

  recvport = get_port (sizeof (struct mm_msg), sizeof (struct mm_msg));
  if (recvport == 0)	/* メッセージポートの取得に失敗した */
    {
      /* error */
    }
  error = regist_port (MEMORY_MANAGER_NAME, recvport);
  if (error != E_OK)	/* メッセージポートの登録に失敗した */
    {
      /* error */
    }
}

#ifdef notdef
/*
 * メッセージの受信処理を行うための関数。
 */
get_req (ID recvport, void *req, int size)
{
  int		rsize;
  struct mm_res	res;

  rcv_mbf (req, &size, recvport);
  if (size != rsize)
    {
      /* サイズが一致しなかった、一部破損した可能性がある */
      /* エラーを返す */
      return (EMM_GARBADGE);
    }
  return (EMM_OK);
}
#endif
