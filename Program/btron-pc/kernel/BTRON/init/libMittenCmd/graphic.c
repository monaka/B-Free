/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: graphic.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/init/libMittenCmd/graphic.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../init.h"
#include "kernlib/device.h"

ER
graphic_mode (void)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.cmd = CONSOLE_GRAPHIC;
  req.body.ctl_req.len = 0;

  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.wri_res.errcd);
    }      

  return (E_OK);
}


ER
text_mode (void)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.cmd = CONSOLE_TEXT;
  req.body.ctl_req.len = 0;

  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.wri_res.errcd);
    }      

  return (E_OK);
}

ER
box_vga (int x, int y, int width, int height, int color)
{
  struct gr_box_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.cmd = CONSOLE_GRAPHIC;
  req.body.ctl_req.len = sizeof (struct gr_box_t);
  
  req.body.ctl_req.cmd = GR_BOX;
  param = (struct gr_box_t *)&(req.body.ctl_req.param);
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->color = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}


ER
fillbox_vga (int x, int y, int width, int height, int color)
{
  struct gr_fillbox_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_fillbox_t);
  
  req.body.ctl_req.cmd = GR_FILLBOX;
  param = (struct gr_fillbox_t *)&(req.body.ctl_req.param);
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->color = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}


ER
circle_vga (int x, int y, int width, int height, int color)
{
  struct gr_box_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_box_t);
  
  req.body.ctl_req.cmd = GR_CIRCLE;
  param = (struct gr_box_t *)&(req.body.ctl_req.param);
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->color = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}


ER
point_vga (int x, int y, int color, int mode)
{
  struct gr_point_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_point_t);
  
  req.body.ctl_req.cmd = GR_POINT;
  param = (struct gr_point_t *)&(req.body.ctl_req.param);
  param->x = x;
  param->y = y;
  param->color = color;
  param->mode = mode;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.ctl_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}


ER
line_vga (int x1, int y1, int x2, int y2, int color, int mode)
{
  struct gr_line_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_line_t);
  
  req.body.ctl_req.cmd = GR_LINE;
  param = (struct gr_line_t *)&(req.body.ctl_req.param);
  param->x1 = x1;
  param->y1 = y1;
  param->x2 = x2;
  param->y2 = y2;
  param->color = color;
  param->mode = mode;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.ctl_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}

ER
ellipse_vga (int x, int y, int width, int height, int color)
{
  struct gr_ellipse_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_ellipse_t);
  
  req.body.ctl_req.cmd = GR_ELLIPSE;
  param = (struct gr_ellipse_t *)&(req.body.ctl_req.param);
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->color = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}

ER
fillellipse_vga (int x, int y, int width, int height, int color)
{
  struct gr_ellipse_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_ellipse_t);
  
  req.body.ctl_req.cmd = GR_FILLELLIPSE;
  param = (struct gr_ellipse_t *)&(req.body.ctl_req.param);
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->color = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}


#define bit_width 16
#define bit_height 16
static unsigned char bit_bits[] = {
   0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
   0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
   0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00};


ER
set_background (int width, int height, unsigned char *bitmap)
{
  struct gr_setbg_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif
  ID			tskid;

  error = get_tid (&tskid);
  if (error)
    {
      return (error);
    }

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_setbg_t);
  req.body.ctl_req.cmd = GR_SETBG;
  param = (struct gr_setbg_t *)&(req.body.ctl_req.param);
  param->caller = tskid;
  if (bitmap)
    {
      param->width = width;
      param->height = height;
      param->size = (width * height) / 8;
      param->bitmap = bitmap;
    }
  else
    {
      param->width = bit_width;
      param->height = bit_height;
      param->size = sizeof (bit_bits);
      param->bitmap = bit_bits;
    }

  param->frcolor = 1;
  param->bgcolor = 7;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}



ER
set_bitmap (int x, int y, int width, int height, int size, unsigned char *buf, int color, int bgcolor)
{
  struct gr_setbitmap_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif
  ID			tskid;


  error = get_tid (&tskid);
  if (error)
    {
      return (error);
    }

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_setbitmap_t);
  req.body.ctl_req.cmd = GR_SETBITMAP;
  param = (struct gr_setbitmap_t *)&(req.body.ctl_req.param);
  param->caller = tskid;
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->size = size;
  param->bitmap = buf;
  param->frcolor = color;
  param->bgcolor = bgcolor;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
}



ER
spl_bitmap (int x, int y, int width, int height, int size, unsigned char *buf, int color)
{
  struct gr_splbitmap_t	*param;
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  ID			recv;
#ifdef notdef
  ID			console;
#endif
  ID			tskid;


  error = get_tid (&tskid);
  if (error)
    {
      return (error);
    }

  recv = dev_recv;
#ifdef notdef
  console = stdout->device;
#endif

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_splbitmap_t);
  req.body.ctl_req.cmd = GR_SPLBITMAP;
  param = (struct gr_splbitmap_t *)&(req.body.ctl_req.param);
  param->caller = tskid;
  param->x = x;
  param->y = y;
  param->width = width;
  param->height = height;
  param->size = size;
  param->bitmap = buf;
  param->frcolor = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }      

  return (E_OK);
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

ER
scroll_vga(int direc, int x, int y, int width, int height, int step, int color)
{
  struct gr_scroll_t      *param;
  DDEV_REQ              req;            /* 要求パケット */
  DDEV_RES              res;            /* 返答パケット */
  W                     rsize;
  ER                    error;
  ID                    recv;
  ID                    console;

  recv = dev_recv;
  console = stdout->device;

  req.header.mbfid = recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.len = sizeof (struct gr_scroll_t);

  req.body.ctl_req.cmd = GR_SCROLL;
  param = (struct gr_scroll_t *)&(req.body.ctl_req.param);
  param->direc = direc;
  param->x     = x;
  param->y     = y;
  param->width = width;
  param->height= height;
  param->step  = step;
  param->color = color;
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }

  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.ctl_res.errcd);
    }

  return (E_OK);
}

