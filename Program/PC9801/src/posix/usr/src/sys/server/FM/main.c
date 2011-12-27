/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:38  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:15:15  night
 * とりあえず動くようにする変更。
 *
 * Revision 1.1  1995/03/18  14:01:10  night
 * 最初の登録
 *
 *
 */


/*
 * このファイルは、B-Free OS/POSIX 環境で動くファイルサーバの一部です。
 *
 *
 */

#include <sys/types.h>
#include <sys/errno.h>
#include <server/file.h>

#include "FM.h"
#include "dosfs/dosfs.h"


/*
 * 大域変数の宣言 (実際に場所を確保する)
 */
ID		recvport;		/* 要求受けつけ用ポート */
struct subfs_t	*rootfs;

/*
 * ローカル関数の定義
 */
static void	init_file_manager (void);


/*
 *
 */
void
_main (void)
{
  fm_msg_t	request;

  init_file_manager ();		/* ファイルマネージャ全体の初期化 */

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
 * ファイルマネージャの初期化
 *
 * o ファイルテーブル (file_table) の初期化
 * o 要求受けつけ用のメッセージバッファ ID をポートマネージャに登録
 * o root ファイルシステムのマウント
 *   (root ファイルシステムがどのデバイスに対応するかは、config.c にて
 *    指定する)
 */
static void
init_file_manager (void)
{
  int		i;
  ID		root_dev;
  ER		error;

  /*
   * 要求受けつけ用のポートを初期化する。
   */
  recvport = get_port (sizeof (struct fm_msg), sizeof (struct fm_msg));
  if (recvport <= 0)
    {
      dbg_printf ("FM: cannot make receive porrt.\n");
      slp_tsk ();
      /* メッセージバッファ生成に失敗 */
    }

  error = regist_port (POSIX_FILE_SERVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

#ifdef notdef
  /*
   * (POSIX 環境にとっての) root ファイルシステムをマウント
   *
   * デバイスドライバはファイルマネージャが起動する前にすでに起動されており、
   * ドライバの要求受け付けポートは、ポートマネージャに登録されている。
   * config.h に root ファイルシステムとなるデバイスドライバのポート名
   * が指定してある。
   * デバイスドライバの要求受けつけポートをポートマネージャから取得し、
   * root ファイルシステムのファイルシステムタイプ (これも config.h で
   * 指定) に合ったファイルシステムマネージャを起動、デバイスポートを
   * 渡す。
   */

  /* 
   * デバイスドライバの要求受け付けポートをポートマネージャから取得す 
   * る。
   */
  find_port (ROOTDEV, &root_dev);

  /*
   * DOS/FS を起動する。これが root ファイルシステムとなる。
   * 変数 root_fs はファイルシステムの基点として、ファイルの traverse 
   * 操作を行うときの開始点となる。
   * --- もちろん、これはコンフィグレーションによって変更できるように
   * する必要がある。
   */
#ifdef ROOTDOS
  rootfs = dos_init (root_dev);
#endif /* ROOTDOS */
#endif /* notdef */

#ifdef DIRCACHE
  /*
   * ディレクトリキャッシュ機構の初期化
   */
  init_dircache ();
#endif /* DIRCACHE */
}

