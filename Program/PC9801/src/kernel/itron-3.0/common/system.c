/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *
 *
 */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/common/system.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "itron.h"
#include "errno.h"
#include "syscall.h"
#include "func.h"


ER
sys_vsys_inf (W func, W sub, VP buf)
{
  SYS_MODULE_INFO *info;

  switch (func)
    {
    case MODULE_INFO:
      info = (SYS_MODULE_INFO *)buf;
      return (E_OK);
      break;
      
    default:
      return (E_PAR);
    }

}
