/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/noaction.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/noaction.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: noaction.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-03-21 00:57:51  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.3  1998/02/25 12:51:43  night
 * プロセス毎の固有データ情報を、それぞれ大域変数で固定的にもつのではなく、
 * lowlib_data というポインタで管理するようにした。
 * この変更に伴い、プロセス毎に違った情報を設定/参照する処理は、ポインタ
 * lowlib_data を介して行うように変更した。
 *
 * lowlib_data の値は、マクロ LOWLIB_DATA で指定したアドレス (0x7fff0000)
 * に設定される。このアドレスは、プロセス毎に違った物理ページにマッピング
 * される。仮想アドレスは固定だが、実際の領域はプロセス毎に違った場所に存
 * 在している。LOWLIB が管理するプロセス毎の情報はこの領域に入れるように
 * する。
 *
 * Revision 1.2  1997/10/11 16:24:55  night
 * こまごました修正。
 *
 * Revision 1.1  1997/08/31 13:10:46  night
 * 最初の登録
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_noaction (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response res;

  error = _make_connection(PSC_NOACTION, &req, &res);
  if (error != E_OK)
    {
      /* What should I do? */
    }

  else if (res.errno)
    {
      ERRNO = res.errno;
      return (-1);
    }

  return (res.status);
}
