/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dircache.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dircache.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: dircache.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:38  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:14:30  night
 * dircache の機能を使わない指定ができるように #ifdef DIRCACHE ... #endif
 * を追加。
 *
 * Revision 1.1  1995/03/18  14:01:11  night
 * 最初の登録
 *
 *
 */

#ifdef DIRCACHE
/*
 * このファイルは、B-Free OS/POSIX 環境で動くファイルサーバの一部です。
 * このファイルでは、ディレクトリキャッシュの管理を行います。
 *
 */

#include "FM.h"

/*
 * ディレクトリのキャッシュノード
 */
struct dir_entry
{
  char	dirname[PATH_MAX];	/* 1024 */
  ID	port;
};


/*
 * 大域変数の宣言 (実際に場所を確保する)
 */
/*
 * ディレクトリキャッシュテーブル
 */
struct dir_entry	dircache[MAXDIRCACHE];



/* 
 * ディレクトリキャッシュの初期化
 */
void
init_dircache (void)
{
  return;
}


/*
 * ディレクトリキャッシュにエントリを追加する。
 */
void
add_dircache (char *dir, ID port)
{
  return;
}


/*
 * ディレクトリキャッシュから指定したパス名をもつディレクトリをみつけ
 * る。
 * ディレクトリのエントリがキャッシュの中にあった場合には、そのディレ
 * クトリへつながるポートを返す。みつからなかった場合には、0 を返す。
 *
 */
ID
find_dircache (char *dir)
{
  return (0);
}


#endif DIRCACHE
