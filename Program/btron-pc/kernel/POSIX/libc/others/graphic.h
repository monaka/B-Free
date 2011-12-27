/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
 * $Log: graphic.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-19 03:03:42  naniwa
 * to support window control functions, etc.
 *
 * Revision 1.2  2000/02/17 14:23:21  naniwa
 * modified scroll_vga
 *
 * Revision 1.1  2000/01/29 16:25:02  naniwa
 * first version
 *
 */

#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__	1

extern int graphic_mode (int fd);
extern int graphic_mode (int fd);
extern int text_mode (int fd);
extern int box_vga (int fd, int x, int y, int width, int height, int color);
extern int fillbox_vga (int fd, int x, int y,
			int width, int height, int color);
extern int circle_vga (int fd, int x, int y, int width, int height, int color);
extern int point_vga (int fd, int x, int y, int color, int mode);
extern int line_vga (int fd, int x1, int y1,
		     int x2, int y2, int color, int mode);
extern int ellipse_vga (int fd, int x, int y,
			int width, int height, int color);
extern int fillellipse_vga (int fd, int x, int y,
			    int width, int height, int color);
extern int set_background (int fd, int width, int height,
			   unsigned char *bitmap);
extern int set_bitmap (int fd, int x, int y, int width, int height, int size,
		       unsigned char *buf, int color, int bgcolor);
extern int spl_bitmap (int fd, int x, int y, int width, int height, int size,
		       unsigned char *buf, int color);
extern int scroll_vga(int fd, int direc, int x, int y,
		      int width, int height, int step, int color);
extern int winmod(int fd, int mode);
#endif
