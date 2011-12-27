/*

B-Free Project ÇÃê∂ê¨ï®ÇÕ GNU Generic PUBLIC LICENSE Ç…è]Ç¢Ç‹Ç∑ÅB

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_sigaddset.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Id: sys_sigaddset.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "../native.h"


/* sigaddset 
 *
 */
int
sigaddset (sigset_t *set, int signo)
{
  return (call_lowlib (PSC_SIGADDSET, set, signo));
}


