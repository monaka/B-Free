/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dosfs/dosfs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: dosfs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:53:40  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  11:03:02  night
 * 最初の登録
 *
 *
 */


/*
 * このファイルは、B-Free OS/POSIX 環境で動く MS-DOS ファイルシステム
 * サーバの一部です。
 *
 */

#ifndef __DOSFS_H__
#define __DOSFS_H__	1

extern struct subfs_t	*dos_init (ID device);


#endif /* __DOSFS_H__ */
