/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
 * $Log: graphic_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-19 03:03:43  naniwa
 * to support window control functions, etc.
 *
 * Revision 1.2  2000/02/17 14:24:44  naniwa
 * minor fix on log comment
 *
 * Revision 1.1  2000/01/29 16:25:02  naniwa
 * first version
 *
 */

#ifndef __GRAPHIC_INTERNAL_H__
#define __GRAPHIC_INTERNAL_H__	1

#include <../BTRON/device/wconsole/wconsole.h>
#include "graphic.h"

/* definition for fcntl */
#define FCTL_CONSOLE_GRAPHIC	(CONSOLE_GRAPHIC << 16)
#define FCTL_CONSOLE_TEXT	(CONSOLE_TEXT << 16)

#define FCTL_GR_BOX	(GR_BOX << 16 | (sizeof(struct gr_box_t) & 0x0FFFF))
#define FCTL_GR_FILLBOX	(GR_FILLBOX << 16 | (sizeof(struct gr_fillbox_t) & 0x0FFFF))
#define FCTL_GR_CIRCLE	(GR_CIRCLE << 16 | sizeof(struct gr_box_t) & 0xFFFF)
#define FCTL_GR_POINT	(GR_POINT << 16 | sizeof(struct gr_point_t) & 0xFFFF)
#define FCTL_GR_LINE	(GR_LINE << 16 | sizeof(struct gr_line_t) & 0xFFFF)
#define FCTL_GR_ELLIPSE (GR_ELLIPSE << 16 | sizeof(struct gr_ellipse_t) & 0xFFFF)
#define FCTL_GR_FILLELLIPSE (GR_FILLELLIPSE << 16 | sizeof(struct gr_ellipse_t) & 0xFFFF)
#define FCTL_GR_SETBG	(GR_SETBG << 16 | sizeof(struct gr_setbg_t) & 0xFFFF)
#define FCTL_GR_SETBITMAP (GR_SETBITMAP << 16 | sizeof(struct gr_setbitmap_t) & 0xFFFF)
#define FCTL_GR_SPLBITMAP (GR_SPLBITMAP << 16 | sizeof(struct gr_splbitmap_t) & 0xFFFF)
#define FCTL_GR_SCROLL (GR_SCROLL << 16 | sizeof(struct gr_scroll_t) & 0xFFFF)
#define FCTL_WC_WINMOD (WC_WINMOD << 16 | sizeof(struct wc_winmod_t) & 0xFFFF)

#endif
