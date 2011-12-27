/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */
static char rcsid[] = "$Id: sys_misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../native.h"

/* misc
 *
 */
int
misc (int cmd, int len, void *argp)
{
  return (call_lowlib (PSC_MISC, cmd, len, argp));
}


