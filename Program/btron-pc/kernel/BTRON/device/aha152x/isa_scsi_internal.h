/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/aha152x/isa_scsi_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: isa_scsi_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-12-02 16:02:39  kishida0
 * Here is start.
 *
 * Revision 1.1  1999/07/06 13:53:31  kishida0
 * add debug aha152x src
 *
 *
 *
 */


#ifndef	__ISA_SCSI_INTERNAL_H__
#define	__ISA_SCSI_INTERNAL_H__	1


#include "h/itron.h"
#include "h/errno.h"
#include "../console/console.h"
#include "servers/port-manager.h"


/*** ここにデバイスの定義をかいてください ***/


/* デバイス構造体 */ 

struct device
{
  UW test;
  /*** ここに ***/
};


#include "aha152xlow.h"
#include "global.h"
#include "funcs.h"


#endif /* __ISA_SCSI_INTERNAL_H__ */
