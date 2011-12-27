/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* posix.h - POSIX 環境マネージャ用のヘッダファイル
 *
 * $Log: utsname.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1996-12-12 13:44:29  night
 * RCS のログマクロを追加。
 *
 *
 */

#ifndef __UTSNAME_H__
#define __UTSNAME_H__	1


#define SYS_SYSNAME		"B-Free/POSIX"
#define SYS_NODENAME		""
#define SYS_RELEASE		"A"
#define SYS_VERSION		"1.0"
#define SYS_MACHINE		"PC"

#define SYS_SYSNAME_LENGTH	32
#define SYS_NODENAME_LENGTH	64
#define SYS_RELEASE_LENGTH	4
#define SYS_VERSION_LENGTH	4
#define SYS_MACHINE_LENGTH	32


struct utsname
{
  char		sysname[SYS_SYSNAME_LENGTH];
  char		nodename[SYS_NODENAME_LENGTH];
  char		release[SYS_RELEASE_LENGTH];
  char		version[SYS_VERSION_LENGTH];
  char		machine[SYS_MACHINE_LENGTH];
};

#endif /* __UTSNAME_H__ */
