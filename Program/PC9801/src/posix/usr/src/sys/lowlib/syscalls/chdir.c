/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chdir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chdir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: chdir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:08  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:29:44  night
 * 註釈の変更。
 *
 * Revision 1.1  1995/02/26  14:55:38  night
 * 最初の登録
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include "../funcs.h"
#include "../global.h"

/*
 * 機能：
 * プロセスがおぼえているカレントディレクトリ (CWD) を移動する。
 * ディレクトリ名の指定方法としては、次の形式のパス名が使用できる。
 *
 * 1) 絶対パスによる指定方法
 *    /a/b/c	
 *
 * 3) 相対パスによる指定方法
 *    a/b/c
 *
 * カレントディレクトリの情報は、ファイルのオープン時などのパス名を指
 * 定する処理のなかでパス名を辿るときの始点として使用する。
 * (もちろん、これはパス名を相対パス名として指定したときの話)
 *
 * 処理：
 * プロセスマネージャに対し、カレントディレクトリを移動するという要求
 * を送る。各プロセスのカレントディレクトリは、プロセスマネージャが管
 * 理している。
 * 
 */
int
psys_chdir (void *argp)
{
  int	error;
  int	retval;		/* API 関数の返り値 */
  char	path[PATH_MAX];
  struct a
    {
      const char	*path;	/* 移動先のディレクトリのパス名 */
    } *args = (struct a *)argp;

  /*
   * 指定されているパス名が絶対パスでない場合には、
   * 絶対パス名に変換する。
   */
  if (args->path[0] != '/')
    {
      if (resolv_fullpath (args->path, path) == NULL)
	{
	  /*
	   * 絶対パス名に変換できなかった。
	   * 引数がおかしい (EINVAL) のエラーを返す。
	   */
	  errno = EINVAL;
	  return (-1);
	}

      args->path = path;
    }

  /*
   * POSIX プロセスマネージャに対して、chdir を行うという要求を送る。
   * proc_chdir に対しては、絶対パス名で送る。
   */
  retval = proc_chdir (args->path, &error);
  if (error)	/* エラーとなってしまった。*/
    {
      errno = error;
    }
  return (retval);
}
