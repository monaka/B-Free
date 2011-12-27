/*

B-Free Project ÇÃê∂ê¨ï®ÇÕ GNU Generic PUBLIC LICENSE Ç…è]Ç¢Ç‹Ç∑ÅB

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_fstat.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Id: sys_fstat.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "../native.h"


/* stat 
 *
 */
int
stat (int fd, struct stat *st)
{
  return (call_lowlib (PSC_STAT, fd, st));
}


