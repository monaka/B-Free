/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/hcm_pci/hcm_pci_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: hcm_pci_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-04-21 16:43:56  kishida0
 * *** empty log message ***
 *
 * Revision 1.2  1999/03/21 01:02:19  monaka
 * Minor fix.
 *
 * Revision 1.1  1999/03/15 05:55:38  monaka
 * First version.
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */


#ifndef	__HCM_PCI_INTERNAL_H__
#define	__HCM_PCI_INTERNAL_H__	1


#include "h/itron.h"
#include "h/errno.h"
#include "../console/console.h"
#include "servers/port-manager.h"


/*** ここにデバイスの定義をかいてください ***/


/* デバイス構造体 

 * HCM_PCI デバイスは、各々この構造体で管理している
 */
struct device
{
  UW uwBusNumber;
  UW uwDeviceNumber;
  UW uwFunctionNumber;
  UH uhVendorID;
  UH uhDeviceID;
  UW uwRevisionID;
  
  /*** ここに ***/
};



#include "global.h"
#include "funcs.h"


#endif /* __HCM_PCI_INTERNAL_H__ */
