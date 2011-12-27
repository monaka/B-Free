/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/local.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


static char rcsid[] = "$Id: local.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "lowlib.h"


/* プロセス固有の情報を管理する */

#ifdef notdef
struct posix_process_info	*posix_info;
#else
struct lowlib_data *posix_info;
#endif
