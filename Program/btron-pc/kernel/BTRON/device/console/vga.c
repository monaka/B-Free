/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/vga.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: vga.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.18  2000-07-09 16:44:49  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.17  2000/02/17 14:21:04  naniwa
 * fixed scroll_vga
 *
 * Revision 1.16  2000/02/13 13:23:40  kishida0
 * add $(SRCS) Makefile
 *
 * Revision 1.15  2000/02/07 18:04:37  kishida0
 * minor fix
 *
 * Revision 1.14  2000/01/30 19:02:44  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.13  2000/01/25 15:57:53  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.12  2000/01/23 15:46:31  kishida0
 * グラフィック画面の scroll 機能を追加した (まだバギーなので注意)
 *
 * Revision 1.11  1999/05/16 13:54:46  naniwa
 * modified ellipse_vga(), fillellipse_vga()
 *
 * Revision 1.10  1999/05/15 05:01:19  naniwa
 * add ellipse_vga(), fillellipse_vga()
 *
 * Revision 1.9  1999/05/04 14:46:53  naniwa
 * minor fix on fillbox_vga()
 *
 * Revision 1.8  1999/04/28 13:11:38  naniwa
 * Speed up of fillbox_vga() and stick_bitmap(). Addition of stipple_bitmap().
 *
 * Revision 1.7  1999/03/08 14:05:37  night
 * パレットテーブルの内容変更。
 * 浪花(naniwa@mechgw.mech.yamaguchi-u.ac.jp) からの指摘による修正。
 * ---------------------------------------
 * frtm にグラフィックス用のワードを追加していて気が付いたのですが，
 * b-free-bc.0.0.40 では kernel/BTRON/device/console/vga.c の中で
 *
 * struct palette text_palette, graph_palette =
 * {
 *   {0, 0,   0,  0, 42, 42, 42, 42, 21, 21, 21, 21, 63, 63, 63, 63},
 *   {0, 0,  42, 42,  0,  0, 21, 42, 21, 21, 63, 63, 21, 21, 63, 63},
 *   {0, 42, 0,  42,  0,  42, 0, 42, 21, 63, 21, 63, 21, 63, 21, 63}
 * };
 *                           ↑
 * によって色の定義を行っているのだと思いますが，矢印の部分の暗い黄色
 * の定義が間違っているみたいです．
 *
 * struct palette text_palette, graph_palette =
 * {
 *   {0, 0,   0,  0, 42, 42, 42, 42, 21, 21, 21, 21, 63, 63, 63, 63},
 *   {0, 0,  42, 42,  0,  0, 42, 42, 21, 21, 63, 63, 21, 21, 63, 63},
 *   {0, 42, 0,  42,  0,  42, 0, 42, 21, 63, 21, 63, 21, 63, 21, 63}
 * };
 *
 * ではないでしょうか? 既に修正されていたら申し訳ありません．
 * ---------------------------------------
 *
 * Revision 1.6  1999/02/15 09:14:30  monaka
 * Circle draw code support (but not yet tested enough...).
 *
 * Revision 1.5  1999/01/14 02:59:03  monaka
 * Commented out reference to 'hanko.xbm'
 *
 * Revision 1.4  1998/11/30 13:22:20  night
 * バックグラウンドの描画やビットマップの描画時に alloca および malloc を
 * 使わないように変更。
 * その代わりに static 変数 copy_buffer を宣言し、描画時にはこの変数を使
 * 用する。copy_buffer のサイズは、マクロ MAX_PATTERN_SIZE で指定している。
 * (マクロ MAX_PATTERN_SIZE は、console_internal.h で定義している)
 *
 * Revision 1.3  1997/10/22 14:30:34  night
 * ビットマップ描画時、malloc を使わないようにした。
 *
 * Revision 1.2  1997/09/21 13:31:25  night
 * kernlib の中の malloc ライブラリを使用しないようにした。
 * (malloc ライブラリを使ってしまうと、console ドライバ用に確保しているメ
 * モリ領域をオーバーしてしまうため)
 *
 * Revision 1.1  1997/08/31 14:01:35  night
 * 最初の登録
 *
 *
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/vga.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "console_internal.h"

/*#include "hanko.xbm"*/


static unsigned char copy_buffer[MAX_PATTERN_SIZE];	/* バックグラウンドやパターンの情報のコピー用 */


/* VGA system の制御
 *
 */

struct vga_setup_info
{
  unsigned char		attr[21];
  unsigned char		misc;
  unsigned short	sequencer[6];
  unsigned short	gctrol[9];
  unsigned short	crtc[25];
};


struct palette
{
  unsigned char	red[16];
  unsigned char	green[16];
  unsigned char	blue[16];
};


/****************************************
  MODE 12Hの初期データ
****************************************/
static struct vga_setup_info mode12 =
{
  /* attr */
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x01, 0x00, 0x0f, 0x00, 0x00 
  },

  /* misc */
  0xe3,
  
  /* sequencer */      
  {
    0x0100, 0x0101, 0x0f02, 0x0003, 0x0604, 0x0300,
  },

  /* graphic controller */
  {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0506, 0x0f07, 0xff08 
  },

  /* crtc */
  {
    0x5f00, 0x4f01, 0x5002, 0x8203, 0x5404, 0x8005,
    0x0b06, 0x3e07, 0x0008, 0x4009, 0x000a, 0x000b,
    0x000c, 0x000d, 0x000e, 0x000f, 0xea10, 0x8c11,
    0xdf12, 0x2813, 0x0014, 0xe715, 0x0416, 0xe317,
    0xff18 
  }
};  

struct palette text_palette, graph_palette =
{
  {0, 0,   0,  0, 42, 42, 42, 42, 21, 21, 21, 21, 63, 63, 63, 63},
  {0, 0,  42, 42,  0,  0, 42, 42, 21, 21, 63, 63, 21, 21, 63, 63},
  {0, 42, 0,  42,  0,  42, 0, 42, 21, 63, 21, 63, 21, 63, 21, 63}
};


/* Text mode
 */
static struct vga_setup_info text_mode =
{
  /* attr */
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x00, 0x03, 0x00, 0x00 
  },

  /* misc */
  0xe3,
  
  /* sequencer */      
  {
    0x0100, 0x0001, 0x0302, 0x0003, 0x0204, 0x0300 
  },

  /* graphic controller */
  {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0e06, 0x0307, 0xff08 
  },

  /* crtc */
  {
    0x5f00, 0x4f01, 0x5002, 0x8203, 0x5504, 0x8105,
    0xbf06, 0x1f07, 0x0008, 0x4f09, 0x0d0a, 0x0e0b,
    0x000c, 0x000d, 0x040e, 0x040f, 0x9c10, 0x8e11,
    0xff12, 0x2813, 0x1f14, 0x9615, 0x6916, 0xa317,
    0xff18 
  }

};  


static unsigned short	buf[TEXT_VRAM_SIZE];
static unsigned char  fontbuf[8 * 1024];
static unsigned char  fontbuf2[8 * 1024];



/* グラフィックシステムの操作関数(テスト用)
 *
 */
void
vga_graph (void)
{
  int	i;
  unsigned short	*p;
  unsigned char		*q;


  save_register (&text_mode);

  p = (unsigned short *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE; i++)
    {
      buf[i] = p[i];
    }

  get_palette (&text_palette);
  set_palette (&graph_palette);
  change_mode (&mode12);		/* VGA mode 12 に変更 */

  outw (0x3CE, 0x0204);		/* VGAGRP_ADDR */
  q = (unsigned char *)0x800A0000;
  for (i = 0; i < 8 * 1024; i++)
    {
      fontbuf[i] = q[i];
    }
  q = (unsigned char *)0x800A8000;
  for (i = 0; i < 8 * 1024; i++)
    {
      fontbuf2[i] = q[i];
    }

  clear_vga (0);
}


W
vga_text (void)
{
  int	i;
  unsigned short	*p;
  unsigned char		*q;


  clear_vga (0);

  inb (STATREG);
  outb (ATTRREG, 0x00);

  outb (0x3CE, 0x01);		/* graphic register addr */
  outb (0x3CF, 0x00);		/* graphic register data */

  outb (0x3C4, 0x02);		/* VGASEQ_ADDR */
  outb (0x3C5, 0x04);		/* VGASEQ_DATA */
  q = (unsigned char *)0x800A0000;
  for (i = 0; i < 8 * 1024; i++)
    {
      q[i] = fontbuf[i];
    }
  q = (unsigned char *)0x800A8000;
  for (i = 0; i < 8 * 1024; i++)
    {
      q[i] = fontbuf2[i];
    }

  set_palette (&text_palette);
  change_mode (&text_mode);

  inb (STATREG);
  outb (ATTRREG, 0x20);

  p = (unsigned short *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE; i++)
    {
      p[i] = buf[i];
    }

  return (E_OK);
}



W
graphic_command (ID caller, DDEV_CTL_REQ *packet)
{
  union gr_cmd_t	*p;
  W			errno;

  p = (union gr_cmd_t *)packet->param;
  switch (packet->cmd)
    {
    case GR_POINT:
      point_vga (p->point.x, p->point.y, p->point.color, p->point.mode);
      return (E_OK);

    case GR_LINE:
      line_vga (p->line.x1, p->line.y1, p->line.x2, p->line.y2, p->line.color, p->line.mode);
      return (E_OK);

    case GR_BOX:
      box_vga (p->box.x, p->box.y, p->box.width, p->box.height, p->box.color, VGA_OVERWRITE);
      return (E_OK);

    case GR_FILLBOX:
      fillbox_vga (p->fillbox.x, p->fillbox.y, p->fillbox.width, p->fillbox.height, p->fillbox.color, VGA_OVERWRITE);
      return (E_OK);

    case GR_CIRCLE:
      circle_vga (p->box.x, p->box.y, p->box.width, p->box.height, p->box.color, VGA_OVERWRITE);
      return (E_OK);

    case GR_SETBG:
      {
#ifdef notdef
	VP buf;

#ifdef USE_MALLOC
	buf = (VP)malloc (p->setbg.size);
#else
	buf = (VP)alloca (p->setbg.size);
#endif
	if (buf == 0)
	  {
	    return (E_NOMEM);
	  }
#endif

	if (p->setbg.size > MAX_PATTERN_SIZE)
	  {
	    return (E_NOMEM);
	  }

	errno = vget_reg (p->setbg.caller, p->setbg.bitmap, p->setbg.size, copy_buffer);
	if (errno)
	  {
	    return (errno);
	  }

	setbg_vga (p->setbg.width, p->setbg.height, p->setbg.size, copy_buffer, p->setbg.frcolor, p->setbg.bgcolor);
#ifdef USE_MALLOC
	free (buf);
#endif
      }
      break;

    case GR_SETBITMAP:
      {
#ifdef notdef
	VP buf;

#ifdef USE_MALLOC
	buf = (VP)malloc (p->setbitmap.size);
#else
	buf = (VP)alloca (p->setbitmap.size);
#endif

	if (buf == 0)
	  {
	    return (E_NOMEM);
	  }
#endif
	if (p->setbg.size > MAX_PATTERN_SIZE)
	  {
	    return (E_NOMEM);
	  }

	errno = vget_reg (p->setbitmap.caller, p->setbitmap.bitmap, p->setbitmap.size, copy_buffer);
	if (errno)
	  {
	    return (errno);
	  }

	stick_bitmap (p->setbitmap.x, p->setbitmap.y, p->setbitmap.width, p->setbitmap.height, copy_buffer, p->setbitmap.frcolor, p->setbitmap.bgcolor);
#ifdef USE_MALLOC
	free (buf);
#endif
      }
      break;

    case GR_SPLBITMAP:
      {
#ifdef notdef
	VP buf;

#ifdef USE_MALLOC
	buf = (VP)malloc (p->splbitmap.size);
#else
	buf = (VP)alloca (p->splbitmap.size);
#endif

	if (buf == 0)
	  {
	    return (E_NOMEM);
	  }
#endif
	if (p->setbg.size > MAX_PATTERN_SIZE)
	  {
	    return (E_NOMEM);
	  }

	errno = vget_reg (p->splbitmap.caller, p->splbitmap.bitmap, p->splbitmap.size, copy_buffer);
	if (errno)
	  {
	    return (errno);
	  }

	stipple_bitmap (p->splbitmap.x, p->splbitmap.y, p->splbitmap.width, p->splbitmap.height, copy_buffer, p->splbitmap.frcolor);
#ifdef USE_MALLOC
	free (buf);
#endif
      }
      break;

    case GR_ELLIPSE:
      ellipse_vga (p->ellipse.x, p->ellipse.y, p->ellipse.width, p->ellipse.height, p->ellipse.color, VGA_OVERWRITE);
      return (E_OK);

    case GR_FILLELLIPSE:
      fillellipse_vga (p->ellipse.x, p->ellipse.y, p->ellipse.width, p->ellipse.height, p->ellipse.color, VGA_OVERWRITE);
      return (E_OK);

    case GR_SCROLL:
      scroll_vga (p->scroll.direc, p->scroll.x, p->scroll.y, p->scroll.width, p->scroll.height, p->scroll.step, p->scroll.color);
      return (E_OK);
    default:
      break;
    }
  return (E_OK);
}


/* VGA のレジスタへ情報を書き込む 
 */
void
write_vga_port (unsigned short *data, int length, unsigned int port)
{
  int	i;

  for (i = 0; i < length; i++)
    {
      outw (port, data[i]);
    }
}


void
set_palette (struct palette *pal)
{
  int	i;

  for (i = 0; i < 16; i++)
    {
      outb (0x3C8, i);		/* DAC order register */
      outb (0x3C9, pal->red[i]);
      outb (0x3C9, pal->green[i]);
      outb (0x3C9, pal->blue[i]);
    }
}

void
get_palette (struct palette *pal)
{
  int	i;

  outb (0x3C7, 0);
  for (i = 0; i < 16; i++)
    {
      pal->red[i] = inb (0x3C9);
      pal->green[i] = inb (0x3C9);
      pal->blue[i] = inb (0x3C9);
    }
}

void
save_register (struct vga_setup_info *info)
{
  int	i;


  dis_int ();

  inb (STATREG);
  outb (ATTRREG, 0x00);

  /* Crt Controlerの設定
   */
  for (i = 0; i < 25; i++)
    {
      outb (0x3D4, i);
      info->crtc[i] = (inb (0x3D5) << 8) | i;
    }

  /* Attribute Registerの設定
   */
  for (i = 0; i < 21; i++)
    {
      inb (STATREG);
      outb (ATTRREG, i);
      info->attr[i] = inb (0x3C1);
    }

  /* Graphics registerの設定
   */
  for (i = 0; i < 9; i++)
    {
      outb (0x3CE, i);
      info->gctrol[i] = (inb (0x3CF) << 8) | i;
    }

  /* Sequence Registerの設定
   */
  for (i = 1; i < 5; i++)
    {
      outb (0x3C4, i);
      info->sequencer[i] = (inb (0x3C5) << 8) | i;
    }

  /* Miscellaneus Registerの設定
   */
  info->misc = inb (0x3CC);

  inb (STATREG);

  ena_int ();
}

/* 引数で指定した画面モードへ変更
 */
void
change_mode (struct vga_setup_info *info)
{
  int    i;

  dis_int ();

  /* Attribute Registerの設定
   */
  inb (STATREG);

  for (i=0; i < 21; i++)
    {
      outb (ATTRREG,i);
      outb (ATTRREG, info->attr[i]);
    }
  
  inb (STATREG);	
  outb (ATTRREG,0x20);

  /* Miscellaneus Registerの設定
   */
  outb (MISCREG, info->misc);

  /* Sequence Registerの設定
   */
  write_vga_port (info->sequencer, 6, SEQUREG);

  /* Graphics registerの設定
   */
  write_vga_port (info->gctrol, 9, GRAPHREG);

  /* Crt Controlerの設定
   */
  outw (CRTCREG, 0x0011);
  write_vga_port (info->crtc, 25, CRTCREG);

  inb (STATREG);
  outb (ATTRREG, 0x20);

  ena_int ();
}





/* 全画面の塗りつぶし（消去）
 */
void
clear_vga(int color)
{
  unsigned int i;
  unsigned char *gvram = (unsigned char *)VGA_VRAM_ADDRESS;

  /* 書込モードを０にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM0 << 8);

  /* 塗りつぶす色を設定 */
  outw (VGA_GR_INDEX_PORT, VGA_SET_RESET | (color & 0x0F) << 8);

  /* Set/Reset レジスタは全プレーン有効 */
  outw (VGA_GR_INDEX_PORT, VGA_ENABLE_SET_RESET | (VGA_PLANE1 | VGA_PLANE2 | VGA_PLANE3 | VGA_PLANE4)<<8);

  /* ビットマスクは無し */
  outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0xFF << 8);

  /* 描画時の演算は上書き */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION |  VGA_OVERWRITE << 8);

  /* 描画するプレーンを選択 */
  outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xFF << 8);

  for (i = 0; i < 80 * 480; i++)
    {
      *gvram++ = 0;		/* ここで書き込んでいる値 0 は、VGA コントローラは参照しない。
				 * Enable Set/Reset register によって、この値は無視するように
				 * なっている。
				 */
    }

}

/* stick_bitmap - ビットマップ画像を画面に張りつける
 *
 */

static int pick_bitmap(unsigned char *bmap, int x, int flag)
{
  static unsigned char *mem, c;
  static int count, bit, skip, n;
  if (flag == 0) {
    mem = bmap;
    c = *mem;
    count = 0;
    n = 0;
    skip = (x-1)/8+1;
    return(0);
  }
  else {
    bit = c & 0x01;
    c >>= 1;
    ++count;
    if (count == x) {
      ++n;
      mem = &(bmap[n*skip]);
      c = *mem;
      count = 0;
    }
    else if ((count%8) == 0) {
      c = *++mem;
    }
    return bit;
  }
}

void
stick_bitmap (int x, int y, int x_length, int y_length, unsigned char *bitmap, int color, int bkcolor)
{
  int i, j, k;
  int w, xl;
  unsigned char *gvram;
  unsigned char dummy, mask, mask2;

  if (x < 0 || y < 0) {
    int	xi, yi;
    int	width;	/* 本当の x 幅 */
    int	mask;

    if (x+x_length < 0) return;
    if (y+y_length < 0) return;
    width = ROUNDUP (x_length, 8);
    for (xi = 0; xi < x_length; xi++) {
      if ((xi+x) >= 0 && (xi + x) < 640) {
	  for (yi = 0; yi < y_length; yi++) {
	      if ((yi+y) >= 0 && (yi + y) < 480) {
		mask = 1 << (xi + (yi * width)) % 8;
		if (bitmap[(xi + (yi * width)) / 8] & mask) {
		  point_vga (xi + x, yi + y, color, VGA_OVERWRITE);
		}
		else {
		  point_vga (xi + x, yi + y, bkcolor, VGA_OVERWRITE);
		}
	      }
	  }
      }
    }
    return;
  }
  if (x >= 640) return;
  if (y >= 480) return;
  if (x+x_length >= 640) xl = 640 - x;
  else xl = x_length;
  if (y+y_length >= 480) y_length = 480 - y;

  /* 書込モードを２にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM2 << 8);
  /* 描画時の演算の設定 */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | VGA_OVERWRITE << 8);
  /* 描画するプレーンは全て有効 */
  outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xFF << 8);

  pick_bitmap(bitmap, x_length, 0);
  for(i = y; i < (y + y_length); i++ ) {
    w = xl;
    gvram = (unsigned char *)(VGA_VRAM_ADDRESS + P2G (x / 8U, i));
    /* 最初の byte の処理 */
    if ((j = x % 8) != 0) {
      mask = 0; mask2 = 0;
      for (k = 7-j; k >= 0; --k) {
	if (--w < 0) break;
	mask <<= 1;
	mask += pick_bitmap(bitmap, xl, 1);
	mask2 <<= 1;
	mask2 += 1;
      }
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      /* 描画 */
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram = color;
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | ((~mask) & mask2) << 8);
      /* 描画 */
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram++ = bkcolor;
    }

    /* 8 bit 単位の書き込み */
    while(w >= 8) {
      mask = 0;
      for(j = 0; j < 8; ++j) {
	mask <<= 1;
	mask += pick_bitmap(bitmap, xl, 1);
      }
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram = color;
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | (~mask) << 8);
      /* 描画 */
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram++ = bkcolor;
      w -= 8;
    }
    
    /* 残りの処理 */
    if (w > 0) {
      mask = 0; mask2 = 0;
      for(j = 0; j < w; ++j) {
	mask <<= 1;
	mask += pick_bitmap(bitmap, xl, 1);
	mask2 <<= 1;
	mask2 += 1;
      }
      mask <<= 8-w; mask2 <<= 8-w;
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram = color;
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | ((~mask) & mask2) << 8);
      /* 描画 */
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram = bkcolor;
    }
  }
}

/* stipple_bitmap - ビットマップ画像を画面に張りつける．背景はそのまま．
 *
 */

void
stipple_bitmap (int x, int y, int x_length, int y_length, unsigned char *bitmap, int color)
{
  int i, j, k;
  int w, xl;
  unsigned char *gvram;
  unsigned char dummy, mask;

  if (x < 0 || y < 0) {
    int	xi, yi;
    int	width;	/* 本当の x 幅 */
    int	mask;

    if (x+x_length < 0) return;
    if (y+y_length < 0) return;
    width = ROUNDUP (x_length, 8);
    for (xi = 0; xi < x_length; xi++) {
      if ((xi+x) >= 0 && (xi + x) < 640) {
	  for (yi = 0; yi < y_length; yi++) {
	      if ((yi+y) >= 0 && (yi + y) < 480) {
		mask = 1 << (xi + (yi * width)) % 8;
		if (bitmap[(xi + (yi * width)) / 8] & mask) {
		  point_vga (xi + x, yi + y, color, VGA_OVERWRITE);
		}
	      }
	  }
      }
    }
    return;
  }
  if (x >= 640) return;
  if (y >= 480) return;
  if (x+x_length >= 640) xl = 640 - x;
  else xl = x_length;
  if (y+y_length >= 480) y_length = 480 - y;

  /* 書込モードを２にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM2 << 8);
  /* 描画時の演算の設定 */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | VGA_OVERWRITE << 8);
  /* 描画するプレーンは全て有効 */
  outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xFF << 8);

  pick_bitmap(bitmap, x_length, 0);
  for(i = y; i < (y + y_length); i++ ) {
    w = xl;
    gvram = (unsigned char *)(VGA_VRAM_ADDRESS + P2G (x / 8U, i));
    /* 最初の byte の処理 */
    if ((j = x % 8) != 0) {
      mask = 0;
      for (k = 7-j; k >= 0; --k) {
	if (--w < 0) break;
	mask <<= 1;
	mask += pick_bitmap(bitmap, xl, 1);
      }
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      /* 描画 */
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram++ = color;
    }

    /* 8 bit 単位の書き込み */
    while(w >= 8) {
      mask = 0;
      for(j = 0; j < 8; ++j) {
	mask <<= 1;
	mask += pick_bitmap(bitmap, xl, 1);
      }
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram++ = color;
      w -= 8;
    }
    
    /* 残りの処理 */
    if (w > 0) {
      mask = 0;
      for(j = 0; j < w; ++j) {
	mask <<= 1;
	mask += pick_bitmap(bitmap, xl, 1);
      }
      mask <<= 8-w;
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram = color;
    }
  }
}



/* point_vga - 任意のドットの描画
 *
 * x = 		表示するＸ座標（０〜６３９）
 * y =		表示するＹ座標（０〜４７９）
 * color =  	色
 * mode  = 	演算パラメータ
 * VGA_OVERWRITE 画面に上書き
 * VGA_AND       画面と論理積演算する
 * VGA_OR	 画面と論理和演算する
 * VGA_XOR	 画面と排他的論理和演算する
 */
void
point_vga (W x, W y, W color, W mode)
{
  unsigned char	*gvram = (unsigned char *)(VGA_VRAM_ADDRESS + P2G (x / 8U, y));
  unsigned char	dummy, mask = MakeMask(7 - (x % 8));


  /* 書込モードを２にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM2 << 8);

  /* ビットマスクの設定 */
  outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);

  /* 描画時の演算の設定 */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | mode << 8);
  
  /* 描画するプレーンは全て有効 */
  outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xFF << 8);

  /* 描画 */
  dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
  *gvram = color;
}

/*
 * ラインルーチン
 *
 */
void
line_vga (W x1, W y1, W x2, W y2, W color, W mode)
{
  int	dx, dy;
  int	n;
  int	e;

#define ABS(d)	(d > 0 ? d : -d) 

  dx = x2 - x1;
  dy = y2 - y1;

  if ((dx < 0) && (dy < 0))
    {
      n = x1; x1 = x2; x2 = n;
      n = y1; y1 = y2; y2 = n;
      dx = x2 - x1;
      dy = y2 - y1;
    }

  if (ABS (dx) > ABS (dy))
    {
      e = -ABS (dx);
      for (;;)
	{
	  point_vga (x1, y1, color, mode);
	  if (x1 == x2)
	    break;
	  if (dx > 0)
	    x1++;
	  else
	    x1--;

	  e = e + (2 * ABS (dy));
	  if (e >= 0)
	    {
	      if (dy > 0)
		y1++;
	      else
		y1--;

	      e = e - (2 * ABS (dx));
	    }
	}
    }
  else
    {
      e = -ABS (dy);
      for (;;)
	{
	  point_vga (x1, y1, color, mode);
	  if (y1 == y2)
	    break;
	  if (dy > 0)
	    y1++;
	  else
	    y1--;

	  e = e + (2 * ABS (dx));
	  if (e >= 0)
	    {
	      if (dx > 0)
		x1++;
	      else
		x1--;

	      e = e - (2 * ABS (dy));
	    }
	}
    }
}


void
poly_line (int *table, int color, int mode)
{
  int	i;

  for (i = 0; table[i + 2] >= 0; i += 2)
    {
      if (table[i + 2] == 0)
	{
	  i += 3;
	}

      line_vga (table[i], table[i + 1], table[i + 2], table[i + 3], color, mode);
    }
}


void
box_vga (W x, W y, W width, W height, W color, W restop)
{
  line_vga (x, y, x, y + height, color, restop);
  line_vga (x, y, x + width, y, color, restop);
  line_vga (x + width, y, x + width, y + height, color, restop);
  line_vga (x, y + height, x + width, y + height, color, restop);
}



void
fillbox_vga (W x, W y, W width, W height, W color, W restop)
{
  int	i, j, k, w;
  unsigned char *gvram;
  unsigned char dummy, mask;

  if (x >= 640 || y >= 480) return;
  if (x < 0) {
    width += x;
    x = 0;
    if (width <= 0) return;
  }
  else if (x+width >= 640) {
    width = 640 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
    if (height <= 0) return;
  }
  else if (y+height >= 480) {
    height = 480 - y;
  }
  
  /* 書込モードを２にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM2 << 8);
  /* 描画時の演算の設定 */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | restop << 8);
  /* 描画するプレーンは全て有効 */
  outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xFF << 8);

  for(i = y; i < (y + height); i++ ) {
    w = width;
    gvram = (unsigned char *)(VGA_VRAM_ADDRESS + P2G (x / 8U, i));
    /* 最初の byte の処理 */
    if ((j = x % 8) != 0) {
      mask = 0;
      for (k = 7-j; k >= 0; --k) {
	if (--w < 0) break;
	mask |= MakeMask(k);
      }
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      /* 描画 */
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram++ = color;
    }

    /* 8 bit 単位の書き込み */
    mask = 0xFF;
    /* ビットマスクの設定 */
    outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
    while(w >= 8) {
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram++ = color;
      w -= 8;
    }
    
    /* 残りの処理 */
    if (w > 0) {
      mask = 0;
      for(; w > 0; --w) {
	mask |= MakeMask(8-w);
      }
      /* ビットマスクの設定 */
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);
      dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
      *gvram = color;
    }
  }
}


void
circle_vga (W x, W y, W rx, W ry, W color, W restop)
{
  int dx, dy;
  int xx, yy;
  int ddx, ddy;
  int dd;

  if (rx > ry)
    {
      dx = rx;
      dy = 0;
      xx = 0;
      yy = ry;
      ddx = rx;
      ddy = rx;
      dd  = rx;
      while (dx >= dy)
	{
	  point_vga (x + dx, y + xx, color, restop);
	  point_vga (x + dy, y + yy, color, restop);
	  point_vga (x - dx, y + xx, color, restop);
	  point_vga (x - dy, y + yy, color, restop);
	  point_vga (x - dx, y - xx, color, restop);
	  point_vga (x - dy, y - yy, color, restop);
	  point_vga (x + dx, y - xx, color, restop);
	  point_vga (x + dy, y - yy, color, restop);
	  dd -= (dy << 1) - 1;
	  ++dy;
	  ddx -= ry;
	  if (ddx < 0)
	    {
	      ddx += rx;
	      ++xx;
	    }
	  if (dd < 0)
	    {
	      dd += (dx << 1);
	      --dx;
	      ddy -= ry;
	    }
	  if (ddy < 0)
	    {
	      ddy += rx;
	      --yy;
	    }
	}
    } else {
      dx = 0;
      dy = ry;
      xx = rx;
      yy = 0;
      ddx = ry;
      ddy = ry;
      dd  = ry;
      while (dx <= dy)
	{
	  point_vga (x + xx, y + dx, color, restop);
	  point_vga (x + yy, y + dy, color, restop);
	  point_vga (x - xx, y + dx, color, restop);
	  point_vga (x - yy, y + dy, color, restop);
	  point_vga (x - xx, y - dx, color, restop);
	  point_vga (x - yy, y - dy, color, restop);
	  point_vga (x + xx, y - dx, color, restop);
	  point_vga (x + yy, y - dy, color, restop);
	  dd -= (dx << 1) - 1;
	  ++dy;
	  ddy -= rx;
	  if (ddy < 0)
	    {
	      ddy += ry;
	      ++yy;
	    }
	  if (dd < 0)
	    {
	      dd += (dy << 1);
	      --dy;
	      ddx -= rx;
	    }
	  if (ddx < 0)
	    {
	      ddx += ry;
	      --xx;
	    }
	}
    }
}


void
setbg_vga (W width, W height, W size, VP bitmap, W color, W bgcolor)
{
  W	x, y;

  for (x = 0; x < 640; x += width)
    {
      for (y = 0; y < 480; y += height)
	{
	  stick_bitmap (x, y, width, height, bitmap, color, bgcolor);
	}
    }
}

void
ellipse_vga(W x, W y, W width, W height, W color, W restop)
{
  int cx, cy, px, py;
  int a, b, x1, y1, x2, y2, r, r1, r2, r3;

  a = width/2; b = height/2; r = a*a*b*b;
  cx = x + a;
  cy = y + b;
  px = a;
  py = 0;
  while (px >= 0) {
    point_vga(cx+px, cy+py, color, restop);
    point_vga(cx-px, cy+py, color, restop);
    point_vga(cx+px, cy-py, color, restop);
    point_vga(cx-px, cy-py, color, restop);
    x1 = b*b*px*px; y1 = a*a*py*py;
    x2 = b*b*(px-1)*(px-1); y2 = a*a*(py-1)*(py-1);
    r1 = ABS((x2+y1 - r));
    r2 = ABS((x2+y2 - r));
    r3 = ABS((x1+y2 - r));
    if (r1 < r2) {
      if (r1 < r3) {
	--px;
      }
      else {
	--py;
      }
    }
    else {
      if (r2 < r3) {
	--px; --py;
      }
      else {
	--py;
      }
    }
  }
}

void
fillellipse_vga(W x, W y, W width, W height, W color, W restop)
{
  int cx, cy, px, py;
  int a, b, x1, y1, x2, y2, r, r1, r2, r3;

  a = width/2; b = height/2; r = a*a*b*b;
  cx = x + a;
  cy = y + b;
  px = a;
  py = 0;
  line_vga(x, cy, x+width, cy, color, restop);
  while (px >= 0) {
    line_vga(cx-px, cy+py, cx+px, cy+py,color, restop);
    line_vga(cx-px, cy-py, cx+px, cy-py,color, restop);
    x1 = b*b*px*px; y1 = a*a*py*py;
    x2 = b*b*(px-1)*(px-1); y2 = a*a*(py-1)*(py-1);
    r1 = ABS((x2+y1 - r));
    r2 = ABS((x2+y2 - r));
    r3 = ABS((x1+y2 - r));
    if (r1 < r2) {
      if (r1 < r3) {
	--px;
      }
      else {
	--py;
      }
    }
    else {
      if (r2 < r3) {
	--px; --py;
      }
      else {
	--py;
      }
    }
  }
}

W xor(W m)
{
int i;
W t,r;
  for(r=t=i=0;i<8;i++){
    t = (1<<i);
    if((t&m)!=t){
      r &= t;
    }
  }
  return r;
} 

/* scroll_func */
void
scroll_vga (W direc, W x, W y, W width, W height, W step, W color)
{
  W end,i,j;
  W xdiv8,k;
  W sr,ds, w;
  char *r,*p;
  char d,t,smask,nsmask,emask,nemask;

  if(width <= 0|| height <= 0){
    return;
  }


  if(direc==SCROLL_UP)
    {
      sr=y+step; ds=y;
    }
  else
    {
      sr=y+height - step - 1; ds=y+height - 1;
    }

  end = height - step;
  if(end <= 0){
    return;
  }

  for(smask=0,k = 7 - (x % 8U); k >= 0; --k)
    {
      smask |= MakeMask(k);
    }
  nsmask = ~smask;

  for(emask=0,k = ((x+width) % 8U); k > 0; --k)
    {
      emask |= MakeMask(8-k);
    }
  nemask = ~emask;

  for(i=0;i<end;i++)
    {
      w = width;
      xdiv8 = x / 8U;
      r = (char *)VGA_VRAM_ADDRESS + P2G (xdiv8, sr);
      p = (char *)VGA_VRAM_ADDRESS + P2G (xdiv8, ds);

      outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM0 << 8);
      outw (VGA_GR_INDEX_PORT, VGA_ENABLE_SET_RESET | 0 << 8);
      outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | VGA_OVERWRITE << 8);
      for(j=0;j<4;j++)
        {
          outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0xff << 8);
          outw (VGA_GR_INDEX_PORT, VGA_READ_MAP_SELECT | j << 8);
          d = (*p) & nsmask;
          outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0xff << 8);
          outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | MakeMask(j) << 8);
          t = (*r) & smask;
          d = d | t; 
          *p = d;
        }
      p++;r++;
      w -= 8 - (x % 8);
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0x0ff << 8);
      outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xff << 8);

      outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM1<< 8);
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0xFF << 8);
      for(; w >= 8; w -= 8)
        {
          *p++ = *r++;
        }
      if(w > 0)
        {
          outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM0 << 8);
          outw (VGA_GR_INDEX_PORT, VGA_ENABLE_SET_RESET | 0 << 8);
          outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | VGA_OVERWRITE << 8);
          for(j=0;j<4;j++)
            {
              outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0x0ff << 8);
              outw (VGA_GR_INDEX_PORT, VGA_READ_MAP_SELECT | j << 8);
              d = (*p) & nemask;
              outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0xff << 8);
              outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | MakeMask(j) << 8);
              t = (*r) & emask;
              d = d | t;
              *p = d;
            }
         }
      outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0x0ff << 8);
      outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xff << 8);
      if(direc==SCROLL_UP)
        {
          sr++; ds++;
        }
      else
        {
          sr--; ds--;
        }
    }

  if(direc==SCROLL_UP)
    {
      fillbox_vga(x, y+end, width, step, color, 0);
    }
  else
    {
      fillbox_vga(x, y, width, step, color, 0);
    }
}

/* code_end */
