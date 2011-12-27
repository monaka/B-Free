/*

B-Free Project �̐������� GNU Generic PUBLIC LICENSE �ɏ]���܂��B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/sys_open.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Id: sys_open.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "../native.h"


/* open 
 *
 */
int
open (char *path, int oflag, int mode)
{
  int pathLen;

  pathLen = strlen (path);
  return (call_lowlib (PSC_OPEN, pathLen, path, oflag, mode));
}


