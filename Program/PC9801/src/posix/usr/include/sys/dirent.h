/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/dirent.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: dirent.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:01  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:51:26  night
 * 最初の登録
 *
 *
 */

/*
 *
 */

#ifndef __SYS_DIRENT_H__
#define __SYS_DIRENT_H__	1

#include <sys/types.h>
#include <limits.h>

/*
 * ディレクトリエントリ構造体
 * 
 * ユーザからは中身は見えない、単にディレクトリ関係のシステムコールの
 * ために使用する。
 *
 */
struct dirent
{
  unsigned int	d_ino;
  mode_t	d_mode;
  unsigned char	d_name[NAME_MAX];
};

typedef struct dirent	DIR;


#endif /* __SYS_DIRENT_H__ */
