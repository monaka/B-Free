/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_errno.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/sys_errno.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/* $Log: sys_errno.c,v $
/* Revision 1.1  2011/12/27 17:13:36  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:51:42  night
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.1  1995/09/17  17:07:03  night
 * 最初の登録
 *
 *
 */

#include "types.h"

/* エラー番号を記憶するためのモジュール。
 * エラー番号を返さない関数が使用する。
 */

ER	sys_errno;
