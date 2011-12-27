/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/pcmcia/pcmcialow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/pcmcia/pcmcialow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 *
 *
 */

#include "pcmcia.h"
#include "pcmcia_internal.h"

UW	initilized = 0;


/* PCMCIA アダプタが存在しているかどうかをチェックする
 */
ER
probe (struct device *dev)
{
  /* */
  char *p = rcs;
  
  return (E_OK);
}


void wreg(UW ind, UB data)
{
  outb(PCICREG,ind);
  outb(PCICREG+1,data);
}
UB   rreg(UW ind, UB data)
{
  outb(PCICREG,ind);
  return inb(PCICREG+1,data);
}
ER
pmemdump(UW ad)
{
  struct a
    {
      UW ad;
    } args;

  args.ad = ad;
  vsys_msc (9998, (VP) &args);
}
ER
itron_debug(void)
{
  vsys_msc (9999, NULL);
  return E_OK;
} 

void dump_cis(void)
{
  pmemdump(0x90000);
  pmemdump(0xa0000);
}
/* 
 * $Log: pcmcialow.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-07-31 18:36:06  kishida0
 * test version
 *
 * Revision 1.4  2000/02/21 22:34:23  kishida0
 * minor fix
 *
 * Revision 1.3  1999/07/06 13:40:13  kishida0
 * for debug
 *
 * Revision 1.2  1999/05/13 16:24:01  kishida0
 * デバッグ用にコード追加、特に大きな変更ではない
 *
 * Revision 1.1  1999/04/21 17:56:28  kishida0
 * for debug
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */
