/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* 
 * $Log: graphic.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-19 03:03:41  naniwa
 * to support window control functions, etc.
 *
 * Revision 1.2  2000/02/17 14:23:20  naniwa
 * modified scroll_vga
 *
 * Revision 1.1  2000/01/29 16:25:02  naniwa
 * first version
 *
 */
/* $Id: graphic.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/others/graphic.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "graphic_internal.h"
#include <h/types.h>
#include <h/errno.h>
#include <device.h>
#include <lowlib.h>
#include <POSIX/manager/posix_syscall.h>


int graphic_mode (int fd)
{
  int errno;
  errno = fcntl(fd, FCTL_CONSOLE_GRAPHIC, 0);
  return(errno);
}


int text_mode (int fd)
{
  int errno;
  errno = fcntl(fd, FCTL_CONSOLE_TEXT, 0);
  return(errno);
}

int box_vga (int fd, int x, int y, int width, int height, int color)
{
  struct gr_box_t	param;
  int errno;
  
  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.color = color;
  errno = fcntl(fd, FCTL_GR_BOX, &param);
  return (errno);
}


int fillbox_vga (int fd, int x, int y, int width, int height, int color)
{
  struct gr_fillbox_t	param;
  int			error;

  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.color = color;
  error = fcntl(fd, FCTL_GR_FILLBOX, &param);
  return(error);
}


int circle_vga (int fd, int x, int y, int width, int height, int color)
{
  struct gr_box_t	param;
  int			error;

  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.color = color;
  error = fcntl(fd, FCTL_GR_CIRCLE, &param);
  return(error);
}


int point_vga (int fd, int x, int y, int color, int mode)
{
  struct gr_point_t	param;
  int			error;

  param.x = x;
  param.y = y;
  param.color = color;
  param.mode = mode;
  error = fcntl(fd, FCTL_GR_POINT, &param);
  return(error);
}


int line_vga (int fd, int x1, int y1, int x2, int y2, int color, int mode)
{
  struct gr_line_t	param;
  int			error;

  param.x1 = x1;
  param.y1 = y1;
  param.x2 = x2;
  param.y2 = y2;
  param.color = color;
  param.mode = mode;
  error = fcntl(fd, FCTL_GR_LINE, &param);
  return(error);
}

int ellipse_vga (int fd, int x, int y, int width, int height, int color)
{
  struct gr_ellipse_t	param;
  int			error;

  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.color = color;
  error = fcntl(fd, FCTL_GR_ELLIPSE, &param);
  return(error);
}

int fillellipse_vga (int fd, int x, int y, int width, int height, int color)
{
  struct gr_ellipse_t	param;
  int			error;

  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.color = color;
  error = fcntl(fd, FCTL_GR_FILLELLIPSE, &param);
  return(error);
}


#define bit_width 16
#define bit_height 16
static unsigned char bit_bits[] = {
   0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
   0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
   0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00};


int set_background (int fd, int width, int height, unsigned char *bitmap)
{
  struct gr_setbg_t	param;
  int			error;

  param.caller = LOWLIB_DATA->main_task;
  if (bitmap)
    {
      param.width = width;
      param.height = height;
      param.size = (width * height) / 8;
      param.bitmap = bitmap;
    }
  else
    {
      param.width = bit_width;
      param.height = bit_height;
      param.size = sizeof (bit_bits);
      param.bitmap = bit_bits;
    }

  param.frcolor = 1;
  param.bgcolor = 7;
  error = fcntl(fd, FCTL_GR_SETBG, &param);
  return(error);
}



int set_bitmap (int fd, int x, int y, int width, int height, int size, unsigned char *buf, int color, int bgcolor)
{
  struct gr_setbitmap_t	param;
  int	error;

  param.caller = LOWLIB_DATA->main_task;
  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.size = size;
  param.bitmap = buf;
  param.frcolor = color;
  param.bgcolor = bgcolor;
  error = fcntl(fd, FCTL_GR_SETBITMAP, &param);
  return(error);
}



int spl_bitmap (int fd, int x, int y, int width, int height, int size, unsigned char *buf, int color)
{
  struct gr_splbitmap_t	param;
  int			error;

  param.caller = LOWLIB_DATA->main_task;
  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.size = size;
  param.bitmap = buf;
  param.frcolor = color;
  error = fcntl(fd, FCTL_GR_SPLBITMAP, &param);
  return(error);
}



#define mark_width 16
#define mark_height 16
static unsigned char mark_bits[] = {
   0x00, 0x00, 0xfe, 0x3f, 0x02, 0x20, 0xfa, 0x2f, 0x02, 0x20, 0xfa, 0x23,
   0x02, 0x20, 0xfa, 0x23, 0x02, 0x20, 0xfa, 0x23, 0x02, 0x20, 0x02, 0x20,
   0x02, 0x20, 0xfe, 0x3f, 0x00, 0x00, 0x00, 0x00};


#define movebox_width 16
#define movebox_height 16
static unsigned char movebox_bits[] = {
   0xff, 0xff, 0x01, 0x80, 0x7d, 0xbe, 0x7d, 0xbe, 0x7d, 0xbe, 0x7d, 0xbe,
   0x3d, 0xbc, 0x81, 0x81, 0x81, 0x81, 0x3d, 0xbc, 0x7d, 0xbe, 0x7d, 0xbe,
   0x7d, 0xbe, 0x7d, 0xbe, 0x01, 0x80, 0xff, 0xff};

int scroll_vga(int fd, int direc, int x, int y, int width, int height, int step, int color)
{
  struct gr_scroll_t    param;
  int                   error;


  param.direc = direc;
  param.x = x;
  param.y = y;
  param.width = width;
  param.height = height;
  param.step  = step;
  param.color = color;
  error = fcntl(fd, FCTL_GR_SCROLL, &param);
  return(error);
}

int winmod(int fd, int mode)
{
  struct wc_winmod_t param;
  int error;

  param.mode = mode;
  error = fcntl(fd, FCTL_WC_WINMOD, &param);
  return(error);
}
