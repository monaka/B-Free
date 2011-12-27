/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/vga.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: vga.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-02-27 03:18:12  monaka
 * Graphics functions are deleted.
 *
 * Revision 1.4  1999/03/15 01:35:34  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.3  1998/11/20 08:02:40  monaka
 * *** empty log message ***
 *
 * Revision 1.2  1997/10/12 10:17:29  night
 * オープニング画面のイメージファイルを hanko.xbm から opening.xbm に変更。
 *
 * Revision 1.1  1997/08/31 14:22:45  night
 * 最初の登録
 *
 *
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/vga.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "errno.h"
#include "types.h"
#include "location.h"
#include "memory.h"
#include "console.h"
#include "macros.h"
#include "misc.h"
#include "vram.h"
#include "asm.h"
#include "vga.h"

#ifdef BOOT_USE_VGA_GRAPH
#include "images/opening.xbm"
#endif


/* VGA system の制御
 *
 */

struct vga_setup_info
{
  BYTE attr[21];
  BYTE misc;
  UWORD16 sequencer[6];
  UWORD16 gctrol[9];
  UWORD16 crtc[25];
};


struct palette
{
  BYTE red[16];
  BYTE green[16];
  BYTE blue[16];
};


/****************************************
  MODE 12Hの初期データ
****************************************/
struct vga_setup_info mode12 =
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
  {0, 0,  42, 42,  0,  0, 21, 42, 21, 21, 63, 63, 21, 21, 63, 63},
  {0, 42, 0,  42,  0,  42, 0, 42, 21, 63, 21, 63, 21, 63, 21, 63}
};


/* Text mode
 */
struct vga_setup_info text_mode =
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



int poly[] =
{
  80,112,400,112,144,288,80,192, 0,
  82,114,402,114,146,290,82,194, -1
};

void clear_vga(int color);
void save_register (struct vga_setup_info *);
void set_palette (struct palette *);
void get_palette (struct palette *);
void change_mode (struct vga_setup_info *info);
void box_vga (int x, int y, int width, int height, int color, int restop);
void poly_line (int *table, int color, int mode);
void write_vga_port (UWORD16 *data, int length, ULONG port);

/* グラフィックシステムの操作関数(テスト用)
 *
 */
static UWORD16 buf[TEXT_VRAM_SIZE];
static BYTE fontbuf[8 * 1024];
static BYTE fontbuf2[8 * 1024];


int
vga_text (void)
{
  int	i;
  UWORD16 *p;
  BYTE *q;


  clear_vga (0);

  inb (STATREG);
  outb (ATTRREG, 0x00);

  outb (0x3CE, 0x01);		/* graphic register addr */
  outb (0x3CF, 0x00);		/* graphic register data */

  outb (0x3C4, 0x02);		/* VGASEQ_ADDR */
  outb (0x3C5, 0x04);		/* VGASEQ_DATA */
  q = (BYTE *)0xA0000;
  for (i = 0; i < 8 * 1024; i++)
    {
      q[i] = fontbuf[i];
    }
  q = (BYTE *)0xA8000;
  for (i = 0; i < 8 * 1024; i++)
    {
      q[i] = fontbuf2[i];
    }

  set_palette (&text_palette);
  change_mode (&text_mode);

  inb (STATREG);
  outb (ATTRREG, 0x20);

  p = (BYTE *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE; i++)
    {
      p[i] = buf[i];
    }

  return E_OK;
}



/* 引数で指定した画面モードへ変更
 */
void
change_mode (struct vga_setup_info *info)
{
  int    i;

  lock ();

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

  unlock ();
}



/* 全画面の塗りつぶし（消去）
 */
void
clear_vga(int color)
{
  ULONG i;
  BYTE *gvram = (BYTE *)VGA_VRAM_ADDRESS;

  /* 書込モードを０にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM0 << 8);

  /* 塗りつぶす色を設定 */
  outw (VGA_GR_INDEX_PORT, VGA_SET_RESET | (color & 0x0F) << 8);

  /* Set/Reset レジスタは全プレーン有効 */
  outw (VGA_GR_INDEX_PORT, VGA_ENABLE_SET_RESET | (VGA_PLANE1 | VGA_PLANE2 | VGA_PLANE3 | VGA_PLANE4)<<8);

  /* ビットマスクは無し */
  outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | 0xFF << 8);

  /* 描画時の演算は上書き */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | 0 << 8);

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

/* VGA のレジスタへ情報を書き込む 
 */
void
write_vga_port (UWORD16 *data, int length, ULONG port)
{
  int	i;

  for (i = 0; i < length; i++)
    {
      outw (port, data[i]);
    }
}




#ifdef BOOT_USE_VGA_GRAPH

void
vga_graph ()
{
  int	i;
  UWORD16 *p;
  BYTE *q;


  save_register (&text_mode);

  p = (UWORD16 *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE; i++)
    {
      buf[i] = p[i];
    }

  get_palette (&text_palette);
  set_palette (&graph_palette);
  change_mode (&mode12);		/* VGA mode 12 に変更 */

  outw (0x3CE, 0x0204);		/* VGAGRP_ADDR */
  q = (BYTE *)0xA0000;
  for (i = 0; i < 8 * 1024; i++)
    {
      fontbuf[i] = q[i];
    }
  q = (BYTE *)0xA8000;
  for (i = 0; i < 8 * 1024; i++)
    {
      fontbuf2[i] = q[i];
    }

  clear_vga (0);

#ifdef nodef
  for (j = 0; j < 16; j++)
    {
      for (i = 0; i < 200; i++)
	{
	  point_vga (i, j * 10 + 0, j, VGA_OVERWRITE);
	  point_vga (i, j * 10 + 1, j, VGA_OVERWRITE);
	  point_vga (i, j * 10 + 2, j, VGA_OVERWRITE);
	  point_vga (i, j * 10 + 3, j, VGA_OVERWRITE);
	}
    }

  poly_line (poly, 4, VGA_OVERWRITE);
#endif

  stick_bitmap (150, 50, opening_width, opening_height, opening_bits, 4, 7);
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


  lock ();

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

  unlock ();
}


/* stick_bitmap - ビットマップ画像を画面に張りつける
 *
 */
void
stick_bitmap (int x, int y, int x_length, int y_length, BYTE *bitmap, int color, int bkcolor)
{
  int	xi, yi;
  int	width;	/* 本当の x 幅 */
  int	mask;


  width = ROUNDUP (x_length, 8);
  for (xi = 0; xi < width; xi++)
    {
      for (yi = 0; yi < y_length; yi++)
	{
	  mask = 1 << (xi + (yi * width)) % 8;
	  if (bitmap[(xi + (yi * width)) / 8] & mask)
	    {
	      point_vga (xi + x, yi + y, color, VGA_OVERWRITE);
	    }
	  else
	    {
	      point_vga (xi + x, yi + y, bkcolor, VGA_OVERWRITE);
	    }
	}
    }
}




/* point_vga - 任意のドットの描画
 *
 * x = 		表示するＸ座標（０〜６３９）
 * y =		表示するＹ座標（０〜４７９）
 * color =  	色
 * rastop = 	演算パラメータ
 * VGA_OVERWRITE 画面に上書き
 * VGA_AND       画面と論理積演算する
 * VGA_OR	画面と論理和演算する
 * VGA_XOR	画面と排他的論理和演算する
 */
void
point_vga (ULONG x, ULONG y, int color, int rastop)
{
  BYTE *gvram = (BYTE *)(VGA_VRAM_ADDRESS + P2G (x / 8, y));
  BYTE dummy, mask = MakeMask(7 - (x % 8));


  /* 書込モードを２にする */
  outw (VGA_GR_INDEX_PORT, VGA_MODE | VGA_WM2 << 8);

  /* ビットマスクの設定 */
  outw (VGA_GR_INDEX_PORT, VGA_BIT_MASK | mask << 8);

  /* 描画時の演算の設定 */
  outw (VGA_GR_INDEX_PORT, VGA_FUNCTION | rastop << 8);
  
  /* 描画するプレーンは全て有効 */
  outw (VGA_SR_INDEX_PORT, SR_MAP_MASK | 0xFF << 8);

  /* 描画 */
  dummy = *gvram;			/* ラッチに VRAM の内容を読み込む */
  *gvram = color;
}

void
box_vga (int x, int y, int width, int height, int color, int restop)
{
  int	i, j;

  for (i = x; i <= (x + width); i++)
    {
      for (j = y; j <= (y + height); j++)
	{
	  point_vga (i, j, color, restop);
	}
    }
}


/*
 * ラインルーチン
 *
 */
void
line_vga (int x1, int y1, int x2, int y2, int color, int mode)
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

#endif /* ifdef BOOT_USE_VGA_GRAPH */
