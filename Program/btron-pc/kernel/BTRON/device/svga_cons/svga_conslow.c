/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/svga_conslow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/svga_conslow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 *
 *
 */

#include "svga_cons.h"
#include "svga_cons_internal.h"

UW	initilized = 0;


/* SVGA_CONS アダプタが存在しているかどうかをチェックする
 */
ER
probe (struct device *dev)
{
  /*** ここに ***/

  return (E_OK);
}


/* 
 * $Log: svga_conslow.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-11 14:41:33  kishida0
 * for debug function
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */
