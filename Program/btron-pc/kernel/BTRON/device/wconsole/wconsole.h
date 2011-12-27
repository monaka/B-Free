/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/wconsole/wconsole.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: wconsole.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-27 15:29:26  naniwa
 * to work as multi task OS
 *
 * Revision 1.2  2000/02/19 03:00:13  naniwa
 * to support multi window environment
 *
 * Revision 1.1  1999/05/15 09:56:04  naniwa
 * first version
 *
 */

#ifndef __WCONSOLE_H__
#define __WCONSOLE_H__	1

#include "../console/console.h"
#include "../keyboard/keyboard.h"
#include "../../../ITRON/kernlib/device.h"

/*  定数の設定
 */

#define WCONSOLE_DRIVER	"driver.wconsole"

#define WC_NULL		0x00004000
#define WC_WINMOD	0x00004001
#define WC_CHGFCS	0x00004002

struct wc_winmod_t
{
  W	mode;
};

union wc_cmd_t
{
  struct wc_winmod_t winmod;
};

#endif __WCONSOLE_H__
