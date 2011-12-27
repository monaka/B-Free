/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/fs_if.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/fs_if.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: fs_if.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-06-29 15:44:31  night
 * コメントの変更
 *
 * ・漢字コードを SJIS から EUC に変更した。
 * ・RCS の Log マクロ入りのコメントを追加した(まだ入れていなかったファイルのみ)。
 *
 *
 */


#include "memory_manager.h"


/* ファイルシステムとのインタフェースを取り扱う関数群
 */


ER
open_file (B *fname, W option, W *handle)
{
  return (EMM_NOSUP);
}


ER
close_file (W handle)
{
  return (EMM_NOSUP);
}

read_file (W handle, UW offset, UW size, VP buf)
{
  return (EMM_NOSUP);
}

write_file (W handle, UW offset, UW size, VP buf)
{
  return (EMM_NOSUP);
}
