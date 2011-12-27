/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library native functions.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/sitsuspend.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/sitsuspend.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: sitsuspend.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.3  1995-09-21 15:52:39  night
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.2  1995/02/19  15:20:35  night
 * コンパイルできるように、インクルードするファイルの指定を変更した。
 *
 * <変更前>
 * #include <h/types.h>
 *
 * <変更後>
 * #include <sys/types.h>
 *
 * Revision 1.1  1995/02/19  14:43:42  night
 * 最初の登録。
 *
 *
 */

#include <sys/types.h>
#include <native/syscall.h>

sitsuspend ()
{
  
}
