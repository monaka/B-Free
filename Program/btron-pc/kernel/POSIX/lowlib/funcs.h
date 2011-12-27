/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/funcs.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: funcs.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-08-31 13:13:42  night
 * とりあえず、OS ファイルに入れるところまでできた。
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC 版への最初の登録
 *
 * ----------------
 *
 * Revision 1.4  1995/09/21  15:52:56  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/03/18  14:24:16  night
 * システムコール関数のプロトタイプ宣言の変更。
 * 元は、引数をシステムコールの引数のとおりに書いていた。
 * それを、引数はすべて void * に変更した。
 *
 * Revision 1.2  1995/02/20  15:20:18  night
 * RCS の Log マクロを入れる註釈部を追加。
 *
 *
 */

/*
 * システムコール処理関数のプロトタイプ宣言
 *
 */

#ifndef __FUNCS_H__
#define __FUNCS_H__	1

#ifdef notdef

#include <sys/types.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/times.h>
#include <sys/utsname.h>
#include <sys/utime.h>


#endif

/*
 * システムコール処理用関数の定義。
 */
#include "syscalls/funcs.h"

/*
 * POSIX ではインプリメント依存となっているシステムコール
 */
extern int	psys_mount ();
extern int	psys_umount ();
extern void	*psys_grow_heap ();

/*
 * lowlib.c
 */
extern ER	lowlib_exit ();
extern ER	lowlib_start (VP stack_top);
extern ER	lowlib_syscall (W syscallno, VP arg);

#endif /* __FUNCS_H__ */
