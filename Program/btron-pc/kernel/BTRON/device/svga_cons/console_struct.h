/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/console_struct.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: console_struct.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-04-17 17:48:42  kishida0
 * minor bug fix
 *
 *
 */

#ifndef __CONSOLE_STRUCT_H__
#define __CONSOLE_STRUCT_H__   1

struct gr_point_t
{
  W     x, y;
  W     color;
  W     mode;
};


struct gr_line_t
{
  W     x1, y1;
  W     x2, y2;
  W     color;
  W     mode;
};


struct gr_box_t
{
  W     x, y;
  W     width, height;
  W     color;
};

struct gr_fillbox_t
{
  W     x, y;
  W     width, height;
  W     color;
};

struct gr_circle_t
{
  W     x, y;
  W     r;
  W     color;
};


struct gr_setbg_t
{
  ID    caller;
  W     width, height;
  W     size;
  VP    bitmap;
  W     frcolor;
  W     bgcolor;
};


struct gr_setbitmap_t
{
  ID    caller;
  W     x, y;
  W     width, height;
  W     size;
  VP    bitmap;
  W     frcolor;
  W     bgcolor;
};


union gr_cmd_t
{
  struct gr_point_t     point;
  struct gr_line_t      line;
  struct gr_box_t       box;
  struct gr_fillbox_t   fillbox;
  struct gr_circle_t    circle;
  struct gr_setbg_t     setbg;
  struct gr_setbitmap_t setbitmap;
};


#endif __CONSOLE_STRUCT_H__
