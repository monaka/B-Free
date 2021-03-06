/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library native functions.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/chdir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/chdir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: chdir.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:22  night
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.1  1995/02/19  14:43:01  night
 * 最初の登録。
 *
 *
 */


#include <sys/types.h>
#include <native/syscall.h>

chdir (const char *filename)
{
  return (CALL_SYSCALL (PSYS_CHDIR, filename));
}
