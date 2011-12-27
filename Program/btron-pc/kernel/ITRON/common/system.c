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

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/common/system.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "config_boot.h"
#include "itron.h"
#include "errno.h"
#include "syscall.h"
#include "func.h"

ER
sys_vsys_inf (VP argp)
{
  struct 
    {
      W func;
      W sub;
      VP buf;
    } *args = argp;
  W		  *rootfs;
  struct boot_header	*info;
  

  switch (args->func)
    {
      
    case ROOTFS_INFO:
      rootfs = (W *)args->buf;
      info = (struct boot_header *)MODULE_TABLE;
      (UW)info |= 0x80000000;
      *rootfs = info->machine.rootfs;
      printk ("vsys_inf: rootfs_info: rootfs = 0x%x\n", info->machine.rootfs);	/* */
      return (E_OK);

    default:
      return (E_PAR);
    }

}
