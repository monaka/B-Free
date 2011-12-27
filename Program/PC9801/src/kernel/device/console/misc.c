/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-12-05 14:17:30  night
 * 画面をスクロールする時に最下行を消去する処理で最下行 + 1 を消去していた。
 * そのため、最下行は消去できず文字が残ってしまった。
 * 正しく最下行を消去するように変更した。
 *
 * Revision 1.3  1995/09/21  15:50:55  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/17  16:51:06  night
 * 文字列出力用の関数を実装した。
 *
 * Revision 1.1  1995/09/14  04:29:20  night
 * 最初の登録
 *
 *
 */


#include "console.h"

/*  定数の設定
 */
#define NORM_ATTR	0x1e		/* 通常文字 			*/

#define TEXT_VRAM_ADDR	0x800A0000	/* TEXT VRAM のアドレス		*/
#define TEXT_VRAM_SIZE	2000

#define GDC_STAT	0x0060
#define GDC_COMMAND	0x0062

#define C(x)	((x) & 0x1f)


static void	scroll_up (void);
static void	write_vram_xy (W x, W y, W ch, W attr);
static void	caridge_return (void);
static void	next_tab (void);
static void	backspace (void);

struct position
{
  W	x;
  W	y;
};

static struct position	curpos;


void
set_curpos (W x, W y)
{
  curpos.x = x;
  curpos.y = y;
  goto_cursol (x, y);
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
goto_cursol (W x, W y)
{
  W	adr;
  
  dis_int ();
  adr = x + (y * 80);
  while ((inb (GDC_STAT) & 0x04) == 0)
    ;
  outb (GDC_COMMAND, 0x49);
  outb (GDC_STAT, adr & 0xff);
  outb (GDC_STAT, (adr >> 8) & 0xff);
  ena_int ();
}

/*
 *
 */
write_char (UB	ch)
{
  if (ch < 0x20)	/* コントロールコード */
    {
      switch (ch)
	{
	case '\n':
	  caridge_return ();
	  break;

	case '\t':
	  next_tab ();
	  break;

	case '\b':
	  backspace ();
	  break;
	}
    }
  else if (ch <= 0x7f)
    {
      write_vram_xy (curpos.x, curpos.y, ch, 0);
      curpos.x++;
      if (curpos.x >= MAX_COLUMN)
	{
	  curpos.y++;
	  if (curpos.y >= MAX_LINE)
	    {
	      curpos.x = 0;
	      curpos.y = MAX_LINE - 1;
	      scroll_up ();
	    }
	}
    }
  goto_cursol (curpos.x, curpos.y);
  return (E_OK);
}

static void
caridge_return (void)
{
  curpos.x = 0;
  curpos.y++;
  if (curpos.y >= MAX_LINE)
    {
      curpos.x = 0;
      curpos.y = MAX_LINE - 1;
      scroll_up ();
    }
}


static void
next_tab ()
{
  if ((curpos.x % 8) == 0)
    {
      curpos.x += 8;
    }
  else
    {
      curpos.x += (8 - (curpos.x % 8));
    }

  if (curpos.x >= MAX_COLUMN)
    {
      curpos.y++;
      if (curpos.y >= MAX_LINE)
	{
	  curpos.x = 0;
	  curpos.y = MAX_LINE - 1;
	  scroll_up ();
	}
    }
}


static void
backspace ()
{
  if (curpos.x <= 0)
    {
      if (curpos.y > 0)
	{
	  curpos.x = MAX_COLUMN - 1;
	  curpos.y--;
	}
    }
  else
    {
      curpos.x--;
    }
  write_vram_xy (curpos.x, curpos.y, ' ', 0);
  goto_cursol (curpos.x, curpos.y);
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
write_vram_xy (W x, W y, W ch, W attr)
{
  short	*addr;
  short *attr_addr;
  
#define CURSOL_POS(x,y)		(x + y * 80)
  addr = (short *)TEXT_VRAM_ADDR;
  addr [CURSOL_POS (x, y)] = ch & 0x00ff;
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
scroll_up (void)  
{
  UH	*addr;
  W		i;
  
  addr = (unsigned short *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE - 80; i++)
    {
      addr[i] = addr[i + 80];
    }
  for (i = 0; i <= MAX_COLUMN; i++)
    {
      write_vram_xy (i, MAX_LINE - 1, ' ', 0xe1);
    }
}


void
clear_console (void)
{
  W	x, y;
  
  for (y = 0; y < MAX_LINE; y++)
    for (x = 0; x < MAX_COLUMN; x++)
      {
	write_vram_xy (x, y, ' ', 0xE1);
      }
  set_curpos (0, 0);
}
