/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library native functions.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/opendir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/opendir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: opendir.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.4  1996-11-24 14:38:48  night
/* 関数の追加。
/*
 * Revision 1.3  1995/09/21  15:52:32  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/02/19  15:20:20  night
 * コンパイルできるように、インクルードするファイルの指定を変更した。
 *
 * <変更前>
 * #include <h/types.h>
 *
 * <変更後>
 * #include <sys/types.h>
 *
 * Revision 1.1  1995/02/19  14:43:26  night
 * 最初の登録。
 *
 *
 */

#include <sys/types.h>
#include <native/syscall.h>

/* opendir --- 指定したディレクトリをオープンする
 *
 * オープンしたディレクトリは、以下のシステムコールによって
 * 内容を読みとることができる。
 * このとき、読み取る単位は、ディレクトリに登録している
 * ファイル/ディレクトリ単位となる。
 *
 */
DIR
opendir (const char *dirname)
{
  return (CALL_SYSCALL (PSYS_OPENDIR, dirname));
}

