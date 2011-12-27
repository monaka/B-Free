/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/hcm_pci/hcm_pcilow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/hcm_pci/hcm_pcilow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 *
 *
 */

#include "hcm_pci.h"
#include "hcm_pci_internal.h"

UW	initilized = 0;

UW get_config_data(int iBusNum, int iDevNum, int iFuncNum, int iRegAddr)
{
  UW config_addr;
  UW config_data;

  config_addr = ((0x8000 | iBusNum) << 16) |
    (iDevNum << 11) |
      (iFuncNum << 8) |
	(iRegAddr << 2);

  outl(0x0CF8, config_addr);

  config_data = inl(0x0CFC);

  return config_data;
}


/* HCM_PCI アダプタが存在しているかどうかをチェックする
 */
ER
probe (struct device *dev)
{
  int i;
  int j;
  UW config_data;

  dbg_printf("HCM_PCI: start probing.\n");
  for(i = 0; i < 0xFF; i++)
    {
      for(j = 0; j < 0x1F; j++)
	{
	  config_data =
	    get_config_data(i, j, 0, 0);
	  if ((config_data & 0xFFFF) == 0xFFFF)
	    {
	      continue;
	    }
	  
	  dbg_printf("HCM_PCI: Device found at BUS=%d DEV=%d\n", i, j);
	}
    }

  return (E_OK);
}

/* 
 * $Log: hcm_pcilow.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-03-21 01:02:20  monaka
 * Minor fix.
 *
 * Revision 1.1  1999/03/15 05:55:38  monaka
 * First version.
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */
