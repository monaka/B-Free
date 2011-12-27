/*

B-Free Project ÇÃê∂ê¨ï®ÇÕ GNU Generic PUBLIC LICENSE Ç…è]Ç¢Ç‹Ç∑ÅB

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_sigprocmask.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Id: sys_sigprocmask.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "../native.h"


/* sigprocmask 
 *
 */
int
sigprocmask (int how, sigset_t *new, sigset_t *old)
{
  return (call_lowlib (PSC_SIGPROCMASK, how, new, old));
}


