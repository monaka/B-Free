/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: lowlib.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  1995-09-21 15:52:57  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.6  1995/07/06  15:23:30  night
 * ・変数 user_stack_top を user_stack_bottom という名前に変更した。
 * ・ユーザスタック領域を BTRON 環境に合わせて 0x40000000 を最大アドレス
 *   となるように変更した。
 *
 * Revision 1.5  1995/03/18  14:27:42  night
 * errno の宣言を追加。
 * setup_trap_entry() 関数の追加。
 * posix_exit() 関数の追加。
 *
 * Revision 1.4  1995/02/21  15:20:53  night
 * ユーザのスタックに引数の文字列の終端文字を入れるときに、
 * 現在のユーザスタックポインタのトップを示している user_stack_top 変数の
 * キャストを正しく行うように変更した。
 *
 * Revision 1.3  1995/02/21  15:14:47  night
 * シグナル機構を初期化するための関数を init_signal_handler_table () から
 * init_signal() に変更した。
 * また、ユーザプログラムのスタックを初期化するための処理を追加した。
 *
 * Revision 1.2  1995/02/20  15:20:56  night
 * 余分な註釈 (古い RCS の LOG マクロが入っていた部分) を削除。
 *
 * Revision 1.1  1995/02/20  15:16:40  night
 * はじめての登録
 *
 */

/* lowlib for posix subsystem.
 *
 *
 * POSIX サブシステム用の LOWLIB.
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include "funcs.h"

/*								*/
/* 大域変数の宣言をするための領域 (実際に場所を確保する)	*/
/*								*/


/*
 * この lowlib が動いているプロセスの ID 
 * (プロセスマネージャからもらう)
 */
pid_t	my_pid;

/*
 * ユーザに返すエラー番号
 */
int	errno;

/*
 * ポートマネージャからの返答を入れるための領域。
 */
struct recv_port_message_t		recv_msg;


/*
 * 局所関数群の定義
 */

static VP	make_stack_frame (VP user_stack_bottom, int argc, 
				  char **argv, char *envp);
static void	init_user_memory (VP *user_stack_bottom, VP start_heap);
static void	setup_trap_entry (int (*posix_entry)());

/* 
 * POSIX LOWLIB の初期化を行う。
 * 初期化が終了したあとは、ユーザプログラムの startup 関数を呼びだす。
 *
 * LOWLIB は、POSIX プロセスマネージャから起動される。
 * そのときに、POSIX プロセスマネージャは、初期化スタックにプロセ
 * スへの引数を渡す(スタックに引数の情報を書き込んでおく)。
 *
 */
lowlib_start (B stack_top)
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
      pid_t	pid;		/* このプロセスの ID */
    } *args = (struct a *)&stack_top;

  VP		user_stack_bottom;	/* ユーザスタックのボトムqアドレス */
  extern int	posix_entry();


  my_pid = args->pid;	/* LOWLIB が貼りついているプロセスの */
                        /* ID を記憶する。*/

  /*
   * ユーザプロセスのためのメモリ領域のセットアップ
   */
  init_user_memory (&user_stack_bottom, args->start_heap);

  /*
   * ユーザの entry ルーチンのためのスタックフレームを作成する。 
   */
  user_stack_bottom = make_stack_frame (user_stack_bottom,
				     args->argc, 
				     args->argv, 
				     args->envp);

  /* 
   * ここで、シグナル機構を初期化する
   * シグナルハンドラの初期値は POSIX の仕様に従う。
   * シグナルを受けとるためのタスクを起動する。
   */
  init_signal ();

  /*
   * POSIX LOWLIB の割り込みエントリルーチンを設定する。
   * POSIX システムコールは、ここで設定した posix_entry () が入口となる。
   */
  setup_trap_entry (posix_entry);

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
  posix_exit ();
}


/*
 * ユーザプロセスのためのメモリ領域のセットアップ
 * o スタック領域のための region を作成
 * o ヒープに使用するための region を作成
 * (メモリマネージャへ要求する)
 * この関数は、作成したユーザスタックのトップアドレスを返す。
 */
static void
init_user_memory (VP *user_stack_bottom, VP start_heap)
{
  W	error;

  /*
   * ユーザスタック用の region を作成する。
   * ユーザスタックの領域は次のように決める。
   *  start: 0x30000000
   *  size:  256MB
   * なお、vm_create() はメモリマネージャへ領域確保の要求を行う。
   */
  error = vm_create (get_tid(), 
		     0x30000000,
		     256 * 1024 * 1024, 
		     (VM_READ | VM_WRITE),
		     VM_USER);
  if (error != PE_OK)
    {
      /* 
       * メモリ領域の確保に失敗した。
       * このタスクは終了する。
       */
      posix_exit ();
    }
  user_stack_bottom = (void *)0x3ffffff4;

  /*
   * ヒープ用のメモリを設定する。
   *  start: 引数 start_heap で指定
   *  size:  500M - start_heap
   */
  error = vm_create (get_tid (),
		     start_heap,
		     (500 * 1024 * 1024) - (int)start_heap,
		     (VM_READ | VM_WRITE),
		     VM_USER);
  if (error != PE_OK)
    {
      /* 
       * メモリ領域の確保に失敗した。
       * このタスクは終了する。
       */
      posix_exit ();
    }
}

/*
 * ここで、ユーザの entry ルーチンのためのスタックフレームを作成する。 
 * ユーザスタックのトップを示すポインタ user_stack_bottom を受けとり、
 * スタックフレームに argc, argv, envp を埋めていく。
 * (envp については、環境変数自体をスタックフレームの底に埋め込み、
 * その領域をさらにスタックに設定する)
 * この関数は、作成したスタックフレームのトップアドレスを返す。
 *
 * この関数を実行したあとは、ユーザのスタックは次のようになる。
 *
 *	+----------------------+ <-- return address.
 *	|    Argument Count    | 
 *	+----------------------+
 *	|    Argument Vector   |
 *	+----------------------+
 *	|    Environment       |
 *	+----------------------+
 *	|                      |
 *      //   引数の文字列     //
 *	|    テーブル          |
 *	+----------------------+
 *	|                      |
 *	//   環境変数          //
 *	|      の収納領域      | <- user_stack_bottom (引数)
 *	+----------------------+
 *
 */
static VP
make_stack_frame (VP user_stack_bottom, int argc, char **argv, char *envp)
{
  int	count;

  /*
   * まず、環境変数用の領域を確保する
   */
  (char *)user_stack_bottom = (char *)user_stack_bottom - (strlen (envp) + 1);

  /*
   * 確保した領域に環境変数をコピーする 
   */
  strcpy (user_stack_bottom, envp);

  /*
   * 環境変数の領域を示すポインタをスタックに push する 
   */
  envp = user_stack_bottom;
  (char **)user_stack_bottom = (char **)user_stack_bottom - 1;
  bcopy (envp, user_stack_bottom, sizeof (char *));

  /*
   * 環境変数と同様に引数の文字列用の領域を確保する。
   */
  for (count = argc - 1; count >= 0; count--)
    {
      (char *)user_stack_bottom = (char *)user_stack_bottom - (strlen (argv[count]) + 1);
      strcpy (user_stack_bottom, argv[count]);
      ((char *)user_stack_bottom)[strlen (argv[count])] = '\0';
    }

  /*
   * 最後に引数の数をスタックに push する。
   */
  (int *)user_stack_bottom += 1;
  *(int *)user_stack_bottom = argc;

  /*
   * ユーザスタックのトップを返す。
   */
  return (user_stack_bottom);
}

/*
 * POSIX LOWLIB の割り込みエントリルーチンを設定する。
 * POSIX システムコールは、ここで設定した posix_entry () が入口となる。 
 */
static void
setup_trap_entry (int (*posix_entry)())
{
  /*
   * ITRON 核の割り込みベクタ登録システムコールを使用する。
   */

}

/*
 * POSIX プログラムを終了する。
 * POSIX プロセス(タスク群)が使用していた資源をシステムに返却し、
 * すべてのタスクを終了させる。
 */
void
posix_exit (void)
{
  /*
   * ユーザプログラム部分が使用していた仮想記憶領域を解放する。
   */
  

  /*
   * そして、最後に自分自身を終了する。
   */
  exd_tsk ();
}

