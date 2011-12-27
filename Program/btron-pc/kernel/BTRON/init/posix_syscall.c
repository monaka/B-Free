/*

B-Free Project ÇÃê∂ê¨ï®ÇÕ GNU Generic PUBLIC LICENSE Ç…è]Ç¢Ç‹Ç∑ÅB

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/init/posix_syscall.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "$Id: posix_syscall.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "init.h"

W
sys_noaction (void)
{
  return call_posix (0, NULL);
}
