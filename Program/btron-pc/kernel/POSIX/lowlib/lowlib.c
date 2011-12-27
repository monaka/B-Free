/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: lowlib.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.9  2000-02-16 08:16:48  naniwa
 * not to depend on sys/types.h
 *
 * Revision 1.8  1999/07/18 10:50:30  naniwa
 * fixed setup_port()
 *
 * Revision 1.7  1999/04/18 18:43:53  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.6  1999/03/30 13:19:05  monaka
 * Minor changes.
 *
 * Revision 1.5  1998/02/25 12:50:06  night
 * プロセス毎の固有データ情報を、それぞれ大域変数で固定的にもつのではなく、
 * lowlib_data というポインタで管理するようにした。
 * この変更に伴い、プロセス毎に違った情報を設定/参照する処理は、ポインタ
 * lowlib_data を介して行うように変更した。
 *
 * lowlib_data の値は、マクロ LOWLIB_DATA で指定したアドレス (0x7fff0000)
 * に設定される。このアドレスは、プロセス毎に違った物理ページにマッピング
 * される。仮想アドレスは固定だが、実際の領域はプロセス毎に違った場所に存
 * 在している。LOWLIB が管理するプロセス毎の情報はこの領域に入れるように
 * する。
 *
 * Revision 1.4  1997/10/11 16:24:05  night
 * デバッグ用 print 文を DEBUG マクロが定義されているときのみ有効になるよ
 * う修正。
 *
 * Revision 1.3  1997/09/23 13:52:34  night
 * デバッグ文の追加。
 *
 * Revision 1.2  1997/08/31 13:13:43  night
 * とりあえず、OS ファイルに入れるところまでできた。
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC 版への最初の登録
 *
 * ----------------
 *
 *
 */

/* lowlib for posix subsystem.
 *
 *
 * POSIX サブシステム用の LOWLIB.
 *
 *
 */

#include "lowlib.h"
#include "servers/port-manager.h"




/* 
 * POSIX LOWLIB の初期化を行う。
 */
ER
lowlib_init (struct lowlib_info *lowlib)
{
  ER	error;


  dbg_printf ("POSIX lowlib: now loading...\n");

  /* POSIX マネージャの port の取得 */

  lowlib->intr = SYSCALL_INTR;
  lowlib->start = lowlib_start;
  lowlib->quit = lowlib_exit;
  lowlib->intr_func = lowlib_syscall;


  return (E_OK);
}



/* 
 * ユーザプログラムの startup 関数を呼びだす。
 *
 * LOWLIB は、POSIX プロセスマネージャから起動される。
 * そのときに、POSIX プロセスマネージャは、初期化スタックにプロセ
 * スへの引数を渡す(スタックに引数の情報を書き込んでおく)。
 *
 */
ER
lowlib_start (VP stack_top)
{
  /* 
   * lowlib が受けとる引数群の定義 
   */
  struct a
    {
      int	(*ap_start)();	/* ユーザプログラムのエントリアドレス */
      int	argc;		/* ユーザプログラムに渡す argc */
      char	**argv;		/* ユーザプログラムに渡す argv */
      char	*envp;		/* ユーザプログラムの環境変数一覧 */
      VP	start_heap;	/* ユーザプログラムのヒープ用領域の先 */
				/* 頭アドレス */
      int	pid;		/* このプロセスの ID */
    } *args = (struct a *)stack_top;

  VP		user_stack_bottom;	/* ユーザスタックのボトムqアドレス */


  lowlib_data->my_pid = args->pid;	/* LOWLIB が貼りついているプロセスの 
					 * ID を記憶する。
					 */


  /* プロセス固有情報のための領域を確保する
   */


  /* 
   * ここで、シグナル機構を初期化する
   * シグナルハンドラの初期値は POSIX の仕様に従う。
   * シグナルを受けとるためのタスクを起動する。
   */
  init_signal ();

  /*
   * ここで、ユーザプログラムのスタートアップ関数をコールする。
   * lowlib はカーネルモードで動いているので、ユーザモードに遷移しなけ
   * ればいけない。
   * そのために、アセンブラで作成した関数 jmp_user_entry () を呼んでい
   * る。
   */
  jmp_user_entry ();

  /*
   * ここには、戻ってこない。もし、もどってきた場合には、ユーザプロセ
   * スの破棄を行う。
   */
  lowlib_exit ();
}



B	*posix_name;

ER
setup_port (void)
{
  ER	error;

#ifdef DEBUG
  dbg_printf ("setup_port\n");
#endif
  /* port の初期化 */
  posix_name = "manager.posix";
  if (posix_manager == 0) {
    error = find_port (posix_name, &posix_manager);
    if (error != E_OK)
      {
#ifdef DEBUG
	dbg_printf ("Cannot find port.\n");
#endif
	return (error);
      }
  }
#ifdef DEBUG
  dbg_printf("Port found.(%d)\n", lowlib_data);
#endif
  if (lowlib_data->recv_port == 0) {
    lowlib_data->recv_port = get_port (sizeof (struct posix_response), sizeof (struct posix_response));
#ifdef DEBUG
    dbg_printf("Port got.\n");
#endif
    if (lowlib_data->recv_port == 0)
      {
#ifdef DEBUG
	dbg_printf ("Cannot allocate port\n");
#endif
	return (E_NOMEM);
      }
  }
  return (E_OK);
} 


/*
 *	POSIX システムコールの処理関数 (入口)
 */
ER
lowlib_syscall (W syscallno, VP arg)
{
  int	(*func)();
  ER errno;

  if ((syscallno < 0) || (syscallno >= nsyscall))
    {
      return (EP_INVAL);
    }

  func = syscalls[syscallno].func;
  if (func == NULL)
    {
      return (EP_NOSYS);
    }

  if (lowlib_data->recv_port == 0)
    {
      errno = setup_port ();
      if (errno != E_OK) return(errno);
    }

  return ((*func)(arg));
}


/*
 * POSIX プログラムを終了する。
 * POSIX プロセス(タスク群)が使用していた資源をシステムに返却し、
 * すべてのタスクを終了させる。
 */
ER
lowlib_exit ()
{
  /*
   * ユーザプログラム部分が使用していた仮想記憶領域を解放する。
   */
  

  /*
   * そして、最後に自分自身を終了する。
   */
  exd_tsk ();
}

