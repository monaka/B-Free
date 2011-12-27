/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/types.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: types.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:02  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:47:58  night
 * 最初の登録
 *
 *
 */

/* 
 * POSIX 環境で使用する基本型の定義
 *
 */

#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__	1

#include "../../../../kernel/itron-3.0/h/types.h"


/*
 * デバイス番号を示す型 
 */
typedef unsigned short		dev_t;

/*
 * グループ番号を示す数
 */
typedef	unsigned short		gid_t;

/*
 * ファイルの番号
 */
typedef unsigned long		ino_t;


typedef unsigned long		mode_t;
typedef unsigned short		nlink_t;
typedef unsigned long long	off_t;
typedef unsigned short		pid_t;
typedef unsigned int		size_t;
typedef unsigned int		ssize_t;
typedef unsigned short		uid_t;

#endif /* __SYS_TYPES_H__ */
