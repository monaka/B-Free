/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/wconsole/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/wconsole/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-02-27 16:09:30  naniwa
 * minor fix
 *
 * Revision 1.4  2000/02/27 15:29:25  naniwa
 * to work as multi task OS
 *
 * Revision 1.3  2000/02/19 03:00:12  naniwa
 * to support multi window environment
 *
 * Revision 1.2  1999/05/29 02:55:18  naniwa
 * adjusted window design
 *
 * Revision 1.1  1999/05/15 09:56:04  naniwa
 * first version
 *
 */


#include "wconsole_internal.h"

#ifdef notdef
/* keyboard.h で定義済み */
#define C(x)	((x) & 0x1f)
#endif

static void caridge_return (W winid);
static void next_tab (W winid);
static void backspace (W winid);
static void write_vram_xy (W x, W y, W ch, W attr, W winid);
static void scroll_up (W winid);

extern W video_mode;

struct window w[] = {
  {80, 25, 7, 14, 40, 50, 80*7+2, 25*14+2, "B-Free", {0, 0}, 1, NULL},
  {80, 12, 7, 14, 40, 30, 80*7+2, 12*14+2, "Win 1", {0, 0}, 0, NULL},
  {80, 12, 7, 14, 40, 270, 80*7+2, 12*14+2, "Win 2", {0, 0}, 0, NULL}
};

int focus = 0, max_win = 3;

static struct position	curpos;

void
set_curpos (W x, W y, W winid)
{
  if (video_mode == GRAPHIC_MODE) {
    w[winid].curpos.x = x;
    w[winid].curpos.y = y;
  }
  else {
    curpos.x = x;
    curpos.y = y;
  }
  goto_cursol (x, y, winid);
}

void
move_curpos (W x, W y, W winid)
{
  if (video_mode == GRAPHIC_MODE) {
    w[winid].curpos.x += x;
    if (w[winid].curpos.x < 0) w[winid].curpos.x = 0;
    else if (w[winid].curpos.x >= w[winid].column)
      w[winid].curpos.x = w[winid].column-1;
    w[winid].curpos.y += y;
    if (w[winid].curpos.y < 0) w[winid].curpos.y = 0;
    else if (w[winid].curpos.y >= w[winid].line)
      w[winid].curpos.y = w[winid].line-1;
    goto_cursol (w[winid].curpos.x, w[winid].curpos.y, winid);
  }
  else {
    curpos.x += x;
    if (curpos.x < 0) curpos.x = 0;
    else if (curpos.x >= MAX_COLUMN) curpos.x = MAX_COLUMN-1;
    curpos.y += y;
    if (curpos.y < 0) curpos.y = 0;
    else if (curpos.y >= MAX_LINE) curpos.y = MAX_LINE-1;
    goto_cursol (curpos.x, curpos.y, winid);
  }
}

/*************************************************************************
 * goto_cursol --- カーソル位置の設定.
 *
 * 引数：	x, y	座標
 *
 * 返値：	なし
 *
 * 処理：	指定した座標にカーソルを移動させる。
 *
 */
void
goto_cursol (W x, W y, W winid)
{
  W	addr;

  if (video_mode == GRAPHIC_MODE && winid == focus) {
    draw_istring(w[winid].font_w*x + w[winid].x + 1,
		 w[winid].font_h*y + w[winid].y + 1,
		 " ", 0, 8, 0);
    return;
  }
  dis_int ();
  addr = x + (y * 80);
  
  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  outb (GDC_DATA, addr & 0xff);
  ena_int ();
}

void erase_cursor(W winid)
{
  if (video_mode == GRAPHIC_MODE) {
    draw_istring(w[winid].font_w*w[winid].curpos.x + w[winid].x + 1,
		 w[winid].font_h*w[winid].curpos.y + w[winid].y + 1,
		 " ", 0, 15, 0);
  }
}

/*
 *
 */
write_char (UB	ch, W winid)
{
  if (ch < 0x20)	/* コントロールコード */
    {
      switch (ch)
	{
	case '\n':
	  caridge_return (winid);
	  break;

	case '\t':
	  next_tab (winid);
	  break;

	case '\b':
	  backspace (winid);
	  break;
	}
    }
  else if (ch <= 0x7f)
    {
      if (video_mode == GRAPHIC_MODE) {
	write_vram_xy (w[winid].curpos.x, w[winid].curpos.y, ch, 0, winid);
	w[winid].curpos.x++;
	if (w[winid].curpos.x >= w[winid].column) {
	  w[winid].curpos.y++;
	  w[winid].curpos.x = 0;
	  if (w[winid].curpos.y >= w[winid].line) {
	    w[winid].curpos.y = w[winid].line - 1;
	    scroll_up (winid);
	  }
	}
      }
      else {
	write_vram_xy (curpos.x, curpos.y, ch, 0, winid);
	curpos.x++;
	if (curpos.x >= MAX_COLUMN) {
	  curpos.y++;
	  curpos.x = 0;
	  if (curpos.y >= MAX_LINE) {
	    curpos.y = MAX_LINE - 1;
	    scroll_up (winid);
	  }
	}
      }
    }
  if (video_mode == GRAPHIC_MODE)
    goto_cursol (w[winid].curpos.x, w[winid].curpos.y, winid);
  else
    goto_cursol (curpos.x, curpos.y, winid);
  return (E_OK);
}

static void
caridge_return (W winid)
{
  if (video_mode == GRAPHIC_MODE) {
    w[winid].curpos.x = 0;
    w[winid].curpos.y++;
    if (w[winid].curpos.y >= w[winid].line) {
      w[winid].curpos.x = 0;
      w[winid].curpos.y = w[winid].line - 1;
      scroll_up (winid);
    }
  }
  else {
    curpos.x = 0;
    curpos.y++;
    if (curpos.y >= MAX_LINE) {
      curpos.x = 0;
      curpos.y = MAX_LINE - 1;
      scroll_up (winid);
    }
  }
}


static void
next_tab (W winid)
{
  if (video_mode == GRAPHIC_MODE) {
    if ((w[winid].curpos.x % 8) == 0) {
      w[winid].curpos.x += 8;
    }
    else {
      w[winid].curpos.x += (8 - (w[winid].curpos.x % 8));
    }
    if (w[winid].curpos.x >= w[winid].column) {
      w[winid].curpos.y++;
      if (w[winid].curpos.y >= w[winid].line) {
	w[winid].curpos.x = 0;
	w[winid].curpos.y = w[winid].line - 1;
	scroll_up (winid);
      }
    }
  }
  else {
    if ((curpos.x % 8) == 0) {
      curpos.x += 8;
    }
    else {
      curpos.x += (8 - (curpos.x % 8));
    }
    if (curpos.x >= MAX_COLUMN) {
      curpos.y++;
      if (curpos.y >= MAX_LINE) {
	curpos.x = 0;
	curpos.y = MAX_LINE - 1;
	scroll_up (winid);
      }
    }
  }
}


static void
backspace (W winid)
{
  if (video_mode == GRAPHIC_MODE) {
    if (w[winid].curpos.x <= 0) {
      if (w[winid].curpos.y > 0) {
	w[winid].curpos.x = w[winid].column - 1;
	w[winid].curpos.y--;
      }
    }
    else {
      w[winid].curpos.x--;
    }
    write_vram_xy (w[winid].curpos.x, w[winid].curpos.y, ' ', 0, winid);
    goto_cursol (w[winid].curpos.x, w[winid].curpos.y, winid);
  }
  else {
    if (curpos.x <= 0) {
      if (curpos.y > 0) {
	curpos.x = MAX_COLUMN - 1;
	curpos.y--;
      }
    }
    else {
      curpos.x--;
    }
    write_vram_xy (curpos.x, curpos.y, ' ', 0, winid);
    goto_cursol (curpos.x, curpos.y, winid);
  }
}

/*************************************************************************
 * write_vram_xy --- VRAM に文字を書き込む
 *
 * 引数：	x, y	座標
 *		ch	出力する文字
 *		attr	文字属性
 *
 * 返値：	なし
 *
 * 処理：	引数 x, y で指定した座標に文字を書き込む.
 *		この関数は、ASCII 文字用になっている。
 *
 * 注意：	現在、attr は無視している。
 *
 */
static void
write_vram_xy (W x, W y, W ch, W attr, W winid)
{
  short	*addr;
  short *attr_addr;
  char str[2] = {0, 0};

  if (video_mode == GRAPHIC_MODE) {
    str[0] = ch;
    draw_istring(w[winid].font_w*x + w[winid].x + 1,
		 w[winid].font_h*y + w[winid].y + 1,
		 str, 0, 15, 0);
  }
  else {
#define CURSOR_POS(x,y)		(x + y * 80)
    addr = (short *)TEXT_VRAM_ADDR;
    ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
    addr [CURSOR_POS (x, y)] = ch;
  }
}

/*************************************************************************
 * scroll_up --- 画面全体をスクロールアップする
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	画面全体を一行スクロールする。
 *
 */
static void
scroll_up (W winid)
{
  UH	*addr;
  W		i;
  
  if (video_mode == GRAPHIC_MODE) {
#ifdef notdef
    /* 画面を消去しカーソルをホームへ移動 */
    fillbox_vga(40, 50, 565, 352, 15);
    set_curpos (0, 0, winid);
#else
    scroll_vga(SCROLL_UP, w[winid].x, w[winid].y+1, w[winid].width,
	       w[winid].height-1, w[winid].font_h, 15);
#endif
  }
  else {
    addr = (unsigned short *)TEXT_VRAM_ADDR;
    for (i = 0; i < TEXT_VRAM_SIZE - 80; i++) {
      addr[i] = addr[i + 80];
    }
    for (i = 0; i <= MAX_COLUMN; i++) {
      write_vram_xy (i, MAX_LINE - 1, ' ', 0xe1, winid);
    }
  }
}


void
clear_wconsole (W winid)
{
  W	x, y;
  if (video_mode == GRAPHIC_MODE) {
    fillbox_vga(w[winid].x, w[winid].y, w[winid].width, w[winid].height, 15);
  }
  else {
    for (y = 0; y < MAX_LINE; y++)
      for (x = 0; x < MAX_COLUMN; x++) {
	write_vram_xy (x, y, ' ', 0xE1, winid);
      }
  }
  set_curpos (0, 0, winid);
}

void
clear_rest_line(W winid)
{
  W x, y;

  if (video_mode == GRAPHIC_MODE) {
    fillbox_vga(w[winid].font_w*w[winid].curpos.x+w[winid].x+1,
		w[winid].font_h*w[winid].curpos.y+w[winid].y+1,
		(w[winid].column-w[winid].curpos.x)*w[winid].font_w,
		w[winid].font_h, 15);
  }
  else {
    y = curpos.y;
    for(x = curpos.x+1; x < MAX_COLUMN; x++) {
      write_vram_xy (x, y, ' ', 0xE1, winid);
    }
  }
}

void
clear_rest_screen(W winid)
{
  W x, y;
  clear_rest_line(winid);

  if (video_mode == GRAPHIC_MODE) {
    fillbox_vga(w[winid].x+1, w[winid].font_h*w[winid].curpos.y+w[winid].y+1,
		w[winid].column*w[winid].font_w,
		(w[winid].line-w[winid].curpos.y)*w[winid].font_h, 15);
  }
  else {
    for (y = curpos.y+1; y < MAX_LINE; y++)
      for (x = 0; x < MAX_COLUMN; x++) {
	write_vram_xy (x, y, ' ', 0xE1, winid);
      }
  }
}

