/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_mountroot.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Id: sys_mountroot.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "../native.h"


/* mountroot
 *
 */
int
mountroot (int devnum)
{
  int result;
  printf("mountroot: begin.");
  result = call_lowlib (PSC_MOUNTROOT, devnum); 
  printf("mountroot: end.");
  return (result);
}


