/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

static char	rcsid[] = "@(#)$Id: posix.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* posix.c - POSIX 環境マネージャ
 *
 *
 *
 * $Log: posix.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.21  2000-04-03 14:32:33  naniwa
 * minor fix
 *
 * Revision 1.20  2000/02/19 03:02:14  naniwa
 * minor change on debug write
 *
 * Revision 1.19  2000/01/26 08:24:34  naniwa
 * to prevent memory leak
 *
 * Revision 1.18  1999/03/15 08:39:24  monaka
 * Some fixes for using dbg_printf().
 *
 * Revision 1.17  1997/10/24 13:58:14  night
 * 実行開始時に init_malloc () を呼び出すようにした。
 *
 * Revision 1.16  1997/09/21 13:34:11  night
 * ライブラリの malloc マクロを使用しないようにした。
 *
 * Revision 1.15  1997/08/31 13:30:57  night
 * デバッグ用出力の一部を #ifdef DEBUG ... #endif で新しく囲んだ。
 *
 * Revision 1.14  1997/07/06 11:56:41  night
 * malloc 機能の初期化指定を追加。
 *
 * Revision 1.13  1997/07/04 15:07:39  night
 * ・スペシャルファイル - デバイスドライバポートの対応表の関連処理の追加。
 * ・ファイルの読み込み処理の改訂。
 *
 * Revision 1.12  1997/07/03 14:24:30  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.11  1997/05/08 15:11:29  night
 * プロセスの情報を設定する機能の追加。
 * (syscall misc の proc_set_info コマンド)
 *
 * Revision 1.10  1997/04/28 15:28:18  night
 * デバッグ用の文を追加。
 *
 * Revision 1.9  1997/04/24 15:40:30  night
 * mountroot システムコールの実装を行った。
 *
 * Revision 1.8  1997/03/25 15:45:23  night
 * デバッグ用に追加した無限ループ文(初期化終了時に停止するようにしていた)を
 * 削除した。
 *
 * Revision 1.7  1997/03/25 13:34:53  night
 * ELF 形式の実行ファイルへの対応
 *
 * Revision 1.6  1996/11/20  12:09:54  night
 * rcsid の追加。
 *
 * Revision 1.5  1996/11/11  13:37:06  night
 * コメント追加
 *
 * Revision 1.4  1996/11/10  11:54:27  night
 * システムコール関数にリクエスト情報を渡すときに、ポインタ渡しでなければ
 * いけないところを実体を渡していた。
 *
 * Revision 1.3  1996/11/07  21:11:49  night
 * Version 番号を出力するときに最後に改行を出力するように変更した。
 *
 * Revision 1.2  1996/11/07  12:46:38  night
 * システムコール処理部を作成した。
 *
 * Revision 1.1  1996/11/05  15:13:45  night
 * 最初の登録
 *
 */

#include "posix.h"


/*
 * POSIX 環境マネージャのメインルーチン
 *
 * 次の処理を行う
 *
 * ・初期化
 *   ポートマネージャに要求受け付け用のポートを登録する
 *   ファイルシステム/プロセス/メモリの各処理の初期化を行う
 *
 * ・要求の受け付け
 *   要求の受け付けは、要求受け付けポートによって行う。
 *
 * ・要求の処理
 *
 *   要求の受けつけから処理がおわるまでは他の要求は受け付けない。
 */
void
posix_start (void)
{
  struct posix_request	request;
#if 0
  extern B		_end;
#endif

  if (init_port () == FAIL)
    {
      dbg_printf ("Cannot allocate port.\n");
      slp_tsk ();
    }
  init_log ();
#if 0
  /* これではプログラムの最後をとることはできない */
  init_malloc ((UW)&_end);
#else
  /* 余裕を見て設定してあるが、manager が大きくなったときには調整が必要 */
  init_malloc (0x100000);
#endif

  /* 各機能単位での初期化
   */
  init_fs ();
  init_process ();
  init_memory ();

  banner ();

  for (;;)
    {
      /* 次の要求メッセージを待つ */
      if (get_request (&request) == FAIL)
	{
	  /* リクエスト取得に失敗した */
#ifdef DEBUG
	  dbg_printf ("Cannot get request.\n");
#endif
	  continue;
	}

#ifdef DEBUG
      dbg_printf ("OP = %d\n ", request.operation);
#endif

      /* 取得したリクエストを処理する */
      if ((request.operation < 0) || (request.operation > NR_POSIX_SYSCALL))
	{
	  /* リクエスト要求にあるオペレーションは、サポートしていない */
	  error_response (&request, EP_NOSUP);
	}
      else
	{
#ifdef DEBUG
	  dbg_printf ("systemcall: %s\n", syscall_table[request.operation].name);
#endif

	   (*syscall_table[request.operation].syscall)(&request);
	}
#ifdef DEBUG
      dbg_printf ("posix: systemcall end.\n");
#endif
    }
  /* ここには来ない */
}


void
banner (void)
{
  extern char	*posix_version;
  ID		taskid;
#ifdef notdef
  W	      	pid, low, hi;
  static B     	num[] = "0123456789";
  static B     	buf[3];
#endif

  dbg_printf ("POSIX Manager ver %s started.", posix_version);
  if (get_tid (&taskid) == E_OK)
    {
#ifdef notdef
      low = pid % 10;
      hi = pid / 10;
      buf[0] = num[hi];
      buf[1] = num[low];
      buf[2] = '\0';
      printk ("(TASK ID = %d  / PID = %s)", taskid, buf);
#else
      dbg_printf ("(TASK ID = %d)", taskid);
#endif
    }
  dbg_printf ("\n");
}


