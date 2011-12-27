/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/stdlib.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: stdlib.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:51:53  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/03/18  14:13:58  night
 * malloc 系の関数の定義追加
 *
 * Revision 1.1  1995/03/05  04:34:11  night
 * 最初の登録
 *
 *
 */


#ifndef __STDLIB__
#define __STDLIB__	1

#include <sys/config.h>
#include <posix/limits.h>
#include <sys/types.h>


extern void	*malloc (size_t size);
extern void	*calloc (size_t nblock, size_t size);
extern void	free (void *addr);
extern void	*realloc (void *addr, size_t size);


#endif /* __STDLIB_H__ */
