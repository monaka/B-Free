/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/pcmcia/pcmcia_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: pcmcia_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-05-13 16:24:00  kishida0
 * デバッグ用にコード追加、特に大きな変更ではない
 *
 * Revision 1.1  1999/04/21 17:56:28  kishida0
 * for debug
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */


#ifndef	__PCMCIA_INTERNAL_H__
#define	__PCMCIA_INTERNAL_H__	1


#include "h/itron.h"
#include "h/errno.h"
#include "../console/console.h"
#include "servers/port-manager.h"


/*** ここにデバイスの定義をかいてください ***/


/* デバイス構造体 
 * PCMCIA デバイスは、各々この構造体で管理している
 */
struct device
{
  UW uwSlotNumbber;
  UW uwDeviceName;
  UH uhVendorID;
  UH uhDeviceID;
  UW uwRevisionID;
  B  reg[15];
};



#include "global.h"
#include "funcs.h"


#endif /* __PCMCIA_INTERNAL_H__ */
