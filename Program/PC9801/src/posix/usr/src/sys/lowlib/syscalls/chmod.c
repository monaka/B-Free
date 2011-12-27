/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chmod.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chmod.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: chmod.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:08  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/02/28  14:09:48  night
 * 自分でファイルサーバへ送るメッセージを組み立てるように変更した。
 *
 * Revision 1.1  1995/02/26  14:55:39  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX システムコール chmod の LOWLIB 関数。
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <server/file.h>
#include "../funcs.h"
#include "../global.h"

/*
 * 指定したファイルのパーミッションを変更する。
 *
 * Syntax:
 *	int chmod (const char *FILENAME, mode_t MODE)
 *
 * 
 */
int
psys_chmod (void *argp)
{
  struct a
    {
      char	*path;	/* パーミッションを変更したいファイルのパス名 */
      mode_t	mode;	/* 設定したいパーミッション値 */
    } *args = (struct a *)argp;
  char	path[PATH_MAX];
  int	retval;			/* API 関数からの返り値 */
  int	err_val;		/* エラー番号 */
  file_message_t	req;	/* ファイルサーバへ送るメッセージ */
  file_reply_t		reply;	/* ファイルサーバからの返答メッセージ */
  
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
   * ファイルマネージャに指定したファイルのパーミッションを変更させる
   * 要求パケットを作成。
   */
  req.request = FILE_SETATTR;
  req.pid     = my_pid;
  strcpy (req.body.file_setattr_req.path, args->path);
  req.body.file_setattr_req.mode = args->mode;

  retval = fm_call (&req, &reply);
  return (retval);
}
