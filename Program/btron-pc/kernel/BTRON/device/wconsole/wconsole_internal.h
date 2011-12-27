/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/wconsole/wconsole_internal.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: wconsole_internal.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-04-03 14:30:43  naniwa
 * to call timer handler in task
 *
 * Revision 1.3  2000/02/27 15:29:27  naniwa
 * to work as multi task OS
 *
 * Revision 1.2  2000/02/19 03:00:14  naniwa
 * to support multi window environment
 *
 * Revision 1.1  1999/05/15 09:56:04  naniwa
 * first version
 *
 */


#ifndef __WCONSOLE_INTERNAL_H__
#define __WCONSOLE_INTERNAL_H__	1

#include "h/types.h"
#include "h/errno.h"
#include "h/misc.h"
#include "ibmpc/ibmpc.h"
#include "kernlib/device.h"

#include "wconsole.h"

struct position
{
  W	x;
  W	y;
};

#define MAX_TITLE 20
#define MAX_REQ_LIST	 20

typedef struct REQ_LIST {
  struct REQ_LIST *prev, *next;
  DDEV_REQ	msg;
} REQ_LIST;

struct window {
  int column, line;
  int font_w, font_h;
  int x, y;
  int width, height;
  char title[MAX_TITLE];
  struct position curpos;
  int map;
  struct REQ_LIST *list;
};

/* main.c */
extern W    	init_wconsole(void);	/* 初期化		*/
extern W    	open_wconsole(ID caller, DDEV_OPN_REQ *packet);		/* オープン		*/
extern W  	close_wconsole(ID caller, DDEV_CLS_REQ *packet);	/* クローズ		*/
extern W    	read_wconsole(ID caller, DDEV_REA_REQ *packet);		/* 読み込み		*/
extern W    	write_wconsole(ID caller, DDEV_WRI_REQ *packet);	/* 書き込み		*/
extern W    	control_wconsole(ID caller, DDEV_CTL_REQ *packet);	/* コントロール		*/
extern W 	posix_read_wconsole(ID caller, ID tskid, DDEV_REQ *packet);
extern W 	relay_wconsole(ID caller, DDEV_RLY_REQ *packet);

/* main.c */
extern void set_curpos (W x, W y, W winid);
extern void move_curpos (W x, W y, W winid);
extern void goto_cursol (W x, W y, W winid);
extern void erase_cursor(W winid);
extern int write_char (UB ch, W winid);
extern void clear_wconsole (W winid);
extern void clear_rest_line(W winid);
extern void clear_rest_screen(W winid);
#endif /* __WCONSOLE_INTERNAL_H__ */
