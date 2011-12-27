/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/svga_cons.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: svga_cons.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-04-20 16:19:47  kishida0
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/11 14:41:33  kishida0
 * for debug function
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/*
 *
 */
#ifndef	__SVGA_CONS_H__
#define	__SVGA_CONS_H__	1


#define SVGA_CONS_DRIVER	"driver.svga_cons"

#define MAX_SVGA_CONS	1


#include "device.h"

/*  定数の設定
 */

#define MAX_COLUMN      80
#define MAX_LINE        25

#define TEXT_MODE       0
#define GRAPHIC_MODE    1

/* Console control */
#define CONSOLE_CLEAR   0x00000001
#define CONSOLE_MOVE    0x00000002
#define CONSOLE_GRAPHIC 0x00000003
#define CONSOLE_TEXT    0x00000004

/* グラフィックモード時のコマンド 
 *
 * 描画関係 (線とか)
 */
#define GR_NULL         0x00010000
#define GR_POINT        0x00010001
#define GR_LINE         0x00010002
#define GR_BOX          0x00010003
#define GR_FILLBOX      0x00010004
#define GR_CIRCLE       0x00010005
#define GR_SETBG        0x00010006
#define GR_SETBITMAP    0x00010007



#define NORM_ATTR       0x1e            /* 通常文字                     */

#define TEXT_VRAM_ADDR  0x800B8000      /* TEXT VRAM のアドレス         */
#define TEXT_VRAM_SIZE  2000

#define GDC_ADDR        0x03d4
#define GDC_DATA        0x03d5

#define GDC_STAT        0x03d4
#define GDC_COMMAND     0x03d5


/* proto type define */




#endif /* __SVGA_CONS_H__ */
