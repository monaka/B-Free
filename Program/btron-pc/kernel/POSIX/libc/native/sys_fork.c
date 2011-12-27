/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Id: sys_fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "../native.h"


/* fork 
 *
 */
int
_fork (int esp, int ebp)
{
  return (call_lowlib (PSC_FORK, esp, ebp));
}
