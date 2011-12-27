/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/utsname.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: utsname.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:03  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:51:26  night
 * 最初の登録
 *
 *
 */

/*
 *
 *
 *
 */

#ifndef __SYS_UTSNAME_H__
#define __SYS_UTSNAME_H__	1

/*
 * システム情報構造体 (utsname システムコールで使用)
 *
 */
struct utsname
{
  char	machine[65];	/* マシンの種類 			*/
  char	nodename[65];	/* このマシンの名前 (ユーザが定義) 	*/
  char	release[65];	/* OS のバージョン 			*/
  char	sysname[65];	/* システム名 (B-Free/POSIX に固定) 	*/
  char	version[65];	/* OS のバージョン番号 			*/
};


#endif /* __SYS_UTSNAME_H__ */

