/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscall.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: syscall.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:58  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:29:01  night
 * 註釈の変更。
 *
 * Revision 1.1  1995/02/20  15:16:43  night
 * はじめての登録
 *
 *
 */

/*
 * POSIX API (システムコール) に関する定義ファイル
 *
 */

#ifndef __SYSCALL_H__
#define __SYSCALL_H__	1

/*
 * システムコール関数を登録するための構造体
 *
 */
struct syscall_entry
{
  int	(*func)();
  char	*name;
};

/*
 * システムコール番号の数。
 */
extern int	nsyscall;

/*
 * システムコール関数を定義しているテーブル
 */
extern struct syscall_entry	syscalls[];

#endif /* __SYSCALL_H__ */
