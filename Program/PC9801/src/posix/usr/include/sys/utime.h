/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/utime.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: utime.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:02  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:51:11  night
 * 最初の登録
 *
 *
 */

/*
 *
 *
 *
 */

#ifndef __SYS_UTIME_H__
#define __SYS_UTIME_H__	1

/*
 * utime システムコールで使用する構造体
 *
 */
struct utimbuf
{
  time_t	actime;		/* アクセス時刻 	*/
  time_t	modtime;	/* 変更時刻		*/
};


#endif /* __SYS_UTIME_H__ */

