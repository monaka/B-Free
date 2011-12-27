/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/access.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/access.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: access.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1995-09-21 15:53:08  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.4  1995/03/18  14:29:28  night
 * 註釈の変更。
 *
 * Revision 1.3  1995/02/28  14:09:12  night
 * コンパイルできるように、構造体の名前などを定義されたものに直した。
 *
 * Revision 1.2  1995/02/27  14:18:25  night
 * メッセージの組み立てを自分でやるように変更。
 *
 * Revision 1.1  1995/02/26  14:55:15  night
 * 最初の登録
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <server/file.h>
#include "../funcs.h"
#include "../global.h"



/*
 * ファイルのアクセス権のチェック (access システムコール)
 *
 * 機能：
 * 引数のパス名で指定したファイル/ディレクトリのアクセス権をチェックす
 * る。
 *
 * 処理：
 * アクセス権をチェックするために、ファイルマネージャに FILE_ACCESS 要求
 * を送信する。
 *
 */
int
psys_access (void *argp)
{
  struct a
    {
      const char	*path;	/* アクセス権をチェックするファイル */
      int		mode;	/* チェックするアクセス権 */
    } *args = (struct a *)argp;
  char			path[PATH_MAX];
  int			retval;	/* API 関数からの返り値 */
  int			err_val;
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
   * ファイルマネージャに送信するメッセージを組み立てる 
   */
  req.request = FILE_ACCESS;
  strcpy (req.body.file_access_req.path, args->path);
  req.body.file_access_req.mode = args->mode;

  /*
   * ファイルマネージャへメッセージを送信する。
   * fm_call は、ファイルマネージャにメッセージを送信し、
   * 返答を受けとる関数。
   */
  retval = fm_call (&req, &reply);
  errno = reply.error;

  return (retval);
}
