/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/_exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/_exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: _exit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:07  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:52:16  night
 * 註釈の変更
 *
 * Revision 1.1  1995/02/27  14:23:29  night
 * 最初の登録
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include "../funcs.h"
#include "../global.h"

/*
 * プロセスを終了する。
 *
 * 機能：
 * このシステムコールでは、標準入出力ライブラリ (stdio) で管理している
 * バッファについては関知していない。単にプロセスがもっている資源をす
 * べて放棄して関係タスクを終了させるだけである。
 *
 * 処理内容：
 *   (1) プロセスマネージャにプロセスの終了要求を送る。
 *   (2) プロセスマネージャは、このプロセスに関する情報を削除する。
 *   (3) lowlib の posix_exit () を実行。
 *
 * Syntax:
 * void	_exit (int exit_code);
 *
 */
void
psys__exit (void *argp)
{
  struct a
    {
      int	exit_code;
    } *args = (struct a *)argp;

  /*
   * まだインプリメントしていない。ENOSYS を返す。
   */
  errno = ENOSYS;
}
