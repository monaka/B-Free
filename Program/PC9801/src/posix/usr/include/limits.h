/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/limits.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: limits.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:52  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/03/18  14:14:37  night
 * 最初の登録
 *
 *
 */


#ifndef __LIMITS_H__
#define __LIMITS_H__	1

#include <posix/limits.h>

#define NFILES		1024		/* POSIX 環境全体でのファイル */
					/* の最大オープン数 */

#define ARG_MAX		_POSIX_ARG_MAX
#define CHILD_MAX	_POSIX_CHILD_MAX
#define LINK_MAX	_POSIX_LINK_MAX
#define MAX_CANON	_POSIX_MAX_CANON
#define MAX_INPUT	_POSIX_MAX_INPUT
#define NAME_MAX	_POSIX_NAME_MAX
#define NGROUPS_MAX	_POSIX_NGROUPS_MAX
#define OPEN_MAX	_POSIX_OPEN_MAX
#define PATH_MAX	_POSIX_PATH_MAX
#define PIPE_BUF	_POSIX_PIPE_BUF
#define SSIZE_MAX	_POSIX_SSIZE_MAX
#define STREAM_MAX	_POSIX_STREAM_MAX
#define TZNAME_MAX	_POSIX_TZNAME_MAX

#define MAX_SIGNAL	19


#endif /* __LIMITS_H__ */
