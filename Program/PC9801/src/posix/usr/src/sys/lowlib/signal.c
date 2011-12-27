/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/signal.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/signal.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: signal.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  1995-09-21 15:52:58  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.6  1995/09/02  09:34:54  night
 * コメント追加
 *
 * Revision 1.5  1995/03/18  14:28:41  night
 * sig_stop() 関数の内容を記述。
 *
 * Revision 1.4  1995/02/21  15:23:17  night
 * シグナルハンドラテーブルの変数宣言の部分を修正した。
 * また、sig_user() でユーザのシグナルハンドラを局所変数に記録するときに、
 * キャストが正しく行われていなかったので修正した。
 *
 * Revision 1.3  1995/02/21  15:13:39  night
 * シグナル機構の初期化関数 (init_signal()) の記述の追加とシグナルハンド
 * ラ用のテーブル (signal_handler[]) の宣言を行なった。
 * また、シグナルハンドラとして以下の関数のスケルトンを書いた。
 * (中身がなく、単に関数を宣言しただけ)
 *
 * 	sig_null	単にシグナルを無視 (IGNORE)する。
 * 	sig_user	ユーザが定義したシグナルハンドラを実行する。
 * 			ユーザのシグナルハンドラへのポインタは、
 * 			引数として sig_user() へ渡す。
 * 	sig_core	コアダンプしたのち、プロセスを終了する。
 * 			もし、プロセスが何らかの端末デバイスと結びつい
 * 			ている場合には、端末に対して コアダンプしたと
 * 			いうことを表示する。
 * 	sig_kill	単にプロセスを終了する(コアダンプもしない)。
 * 	sig_stop	プロセスを一時的に停止させる。
 * 	sig_cont	sig_stop によって一時的に停止したプロセスの処
 * 			理を再開する。
 *
 * Revision 1.2  1995/02/20  15:26:34  night
 * シグナルハンドラを実行可能状態にする関数、init_signal_handler_table()
 * を宣言した。ただし、今のところこの関数は何もしていない。
 *
 * Revision 1.1  1995/02/20  15:16:44  night
 * はじめての登録
 *
 *
 */

/*
 * POSIX 仕様で定義するシグナルを実現するための関数群が入っているファ
 * イル 
 *
 * このファイルの中で宣言している大域変数 signal_handler_table は、
 * シグナル (最大 32 個まで定義できる) のおのおののハンドラ情報が入っ
 * ている。
 *
 * ハンドラ関数には、次の種類がある：
 *
 *	sig_null	単にシグナルを無視 (IGNORE)する。
 *	sig_user	ユーザが定義したシグナルハンドラを実行する。
 *			ユーザのシグナルハンドラへのポインタは、
 *			引数として sig_user() へ渡す。
 *	sig_core	コアダンプしたのち、プロセスを終了する。
 *			もし、プロセスが何らかの端末デバイスと結びつい
 *			ている場合には、端末に対して コアダンプしたと
 *			いうことを表示する。
 *	sig_kill	単にプロセスを終了する(コアダンプもしない)。
 *	sig_stop	プロセスを一時的に停止させる。
 *	sig_cont	sig_stop によって一時的に停止したプロセスの処
 *			理を再開する。
 * 
 * これらのハンドラは、ユーザプログラムを実行している主タスクとは別個
 * のシグナル処理用タスク上で実行することに注意すること。
 *
 * POSIX プロセスマネージャは、ユーザプロセスにシグナルを送る場合には、
 * シグナル処理用タスクに対してシグナルメッセージを送る。
 * そのために使用するメッセージポートは、init_signal() 関数が、確保/初
 * 期化する。
 * 
 */

#include <sys/types.h>
#include <limits.h>
#include <sys/perrno.h>
#include <signal.h>
#include "global.h"


/*
 * ユーザがシグナルハンドラを定義できるかどうかを示すための値
 */
#define USER_OK		1
#define USER_NOK	0


/*
 * このファイルの中でのみ使用する構造体
 */
struct signal_handler
{
  int	(*handler)();	/* シグナルを受信したとき呼び出すハンドラ関数 */
  void	*argp;		/* ハンドラ関数に渡す引数 (デフォルトは NULL) */
  int	user_flag;	/* ユーザがシグナルハンドラを定義できるかのフ */
			/* ラグ */
};


/*
 * このファイルの中でのみ参照可能な関数 (static function)
 */
static int	sig_null (void *arg);	/* 何もしない */
static int	sig_user (void *arg);	/* ユーザハンドラを実行 */
static int	sig_core (void *arg);	/* コアダンプする */
static int	sig_kill (void *arg);	/* 終了する */
static int	sig_stop (void *arg);	/* プロセスを一時停止する */
static int	sig_cont (void *arg);	/* プロセスの実行を再開する */

/*
 * 大域変数の宣言 (実際に場所を確保する)
 */

struct signal_handler signal_handler_table[MAX_SIGNAL] =
{
  { sig_core, NULL }, /* SIGHUP */
  { sig_core, NULL }, /* SIGINT */
  { sig_core, NULL }, /* SIGQUIT */
  { sig_core, NULL }, /* SIGILL */
  { sig_core, NULL }, /* SIGABRT */
  { sig_core, NULL }, /* SIGFPE */
  { sig_stop, NULL }, /* SIGTTIN */
  { sig_stop, NULL }, /* SIGTTOU */
  { sig_kill, NULL }, /* SIGKILL */
  { sig_core, NULL }, /* SIGSEGV */
  { sig_core, NULL }, /* SIGALRM */
  { sig_stop, NULL }, /* SIGSTOP */
  { sig_null, NULL }, /* SIGUSR1 */
  { sig_null, NULL }, /* SIGUSR2 */
  { sig_core, NULL }, /* SIGTERM */
  { sig_null, NULL }, /* SIGCHLD */
  { sig_stop, NULL }, /* SIGTSTP */
  { sig_cont, NULL }, /* SIGCONT */
  { sig_core, NULL }, /* SIGPIPE */
};



/*
 * ここでシグナル機構を初期化する
 * 
 * 処理：
 *	(1) シグナル受信用のメッセージポートの初期化。
 *	    ITRON 核のシステムコールでメッセージバッファを作成する。
 *	    作成したメッセージバッファは、プロセスマネージャに (プロセ
 *	    ス情報として) 登録する。
 *	(2) シグナル処理用のタスクを起動する。
 */
int
init_signal (void)
{
  ID	sigport;

  /*
   * シグナル受信用のメッセージポートを初期化する。
   */
  sigport = alloc_port ();		/* メッセージバッファの取得 */

  /*
   * シグナルを受信するポートをプロセスマネージャに登録する。
   * この proc_set_signal_port () は、POSIX プロセスマネージャと通信を
   * するための関数。
   */
  proc_set_signal_port (my_pid, sigport);

  /*
   * シグナルを受信するためのタスクを起動する。
   * もちろん、シグナルは↑の関数で生成/登録したポートを使用する。
   */
  
}

/*
 * シグナルを受け取っても、何もしない関数
 */
static int
sig_null (void *arg)
{
  return (PE_OK);  /* 一応正常終了したことだけは返す。*/
}

/*
 * ユーザハンドラを (主タスクのコンテキストで) 実行する。
 *
 * ユーザハンドラは、いくつかの理由からユーザプログラムを実行している
 * 主タスクが実行する必要がある。
 * そのため sig_user() は、
 *
 *   (1) 主タスクを一時停止させ (SUS_TSK)、
 *   (2) 今実行しているアドレスへ戻るようスタックを書き換え、
 *   (3) 最後にシグナルハンドラを実行するようコンテキスト情報を
 *       変更する。
 *
 */
static int
sig_user (void *arg)
{
  int	(*user_handler)();

  user_handler = (int (*)())arg;

  sus_tsk (main_task);	/* 主タスクを強制停止 */

  /* スタックの書き換え */

  /* コンテキスト情報の書き換え */

  return (PE_OK);
}


/*
 * コアダンプする。
 */
static int
sig_core (void *arg)
{
  /* ユーザのデータ領域およびスタック領域をcoreファイルに吐き出す */

}


/*
 * プロセスを終了する。
 */
static int
sig_kill (void *arg)
{


}


/*
 * プロセスを一時的に停止する。
 *
 * メインタスクをシグナルが入ってくるまで停止させる。
 *
 */
static int
sig_stop (void *arg)
{
  /*
   * メインタスクに対して slp_tsk () を実行する。
   */
  slp_tsk (main_task);

  /*
   * プロセスマネージャに対してプロセスが stop 状態になったことを知ら
   * せる。
   */
  
  return (PE_OK);
}


/*
 * 一時停止したプロセスを再開する。
 */
static int
sig_cont (void *arg)
{

}


