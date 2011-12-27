/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_errno.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/sys_errno.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: sys_errno.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.1  1999-04-18 17:48:34  monaka
/* Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
/*
/* Revision 1.1  1996/07/22 23:52:06  night
/* 最初の登録
/*
 * Revision 1.2  1995/09/21  15:51:42  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
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
