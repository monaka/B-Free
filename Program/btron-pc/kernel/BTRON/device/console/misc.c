/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char misc_rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  2000-07-09 16:44:48  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.6  2000/02/07 18:04:36  kishida0
 * minor fix
 *
 * Revision 1.5  1999/03/21 12:49:24  night
 * 浪花(naniwa@mechgw.mech.yamaguchi-u.ac.jp) からの指摘による変更。
 * ------------------
 * エスケープ・シーケンスを実装している途中に気が付いたんですが，
 * console は既にバックスペースには対応されていたんですね (^^;)．frtm
 * の入力には '\b' が使えれば十分なので，エスケープシーケンスを作る必
 * 要は無くなってしまったのですが，折角なので最後まで実装しました．
 *
 * 実装したエスケープ・シーケンスの仕様です．先の mail のミスを訂正し
 * ました．
 *
 * ESC[Pl;PcH	カーソルを (Pc,Pl) へ移動．デフォルトはそれぞれ 1．
 * ESC[PnA		カーソルを Pn 行上へ移動．デフォルトは 1．
 * 		画面上端より上への移動は無視．
 * ESC[PnB		カーソルを Pn 行上へ移動．デフォルトは 1．
 * 		画面下端より下への移動は無視．
 * ESC[PnC		カーソルを Pn 桁右へ移動．デフォルトは 1．
 * 		画面右端より右への移動は無視．
 * ESC[PnD		カーソルを Pn 桁左へ移動．デフォルトは 1．
 * 		画面右端より左への移動は無視．
 * ESC[2J		画面を消去し，カーソルを (1,1) へ．
 * ESC[J		表示の終りまでを消去．
 * ESC[K		カーソル位置から行の終りまでを消去．
 *
 * カーソルの座標は左上隅が (1,1)．右下が (MAX_COLUMN, MAX_LINE)．
 *
 * kernel/BTRON/device/console の下で patch -p1 であてるようにパッチ
 * を作りました．
 * ------------------
 *
 * Revision 1.4  1997/08/31 14:00:36  night
 * console_internal.h のインクルードを追加。
 *
 * Revision 1.3  1996/07/24  16:43:23  night
 * マクロ CURSOL_POS を CURSOR_POS に名前を変更。
 *
 * Revision 1.2  1996/07/24  16:42:25  night
 * テキスト VRAM に書き込むときの処理を PC9801 のものから IBM PC 用に変更
 * した。
 *
 * Revision 1.1  1996/07/24  16:03:59  night
 * 最初の登録
 *
 * Revision 1.4  1995/12/05 14:17:30  night
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


#include "console_internal.h"

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

void
move_curpos (W x, W y)
{
  curpos.x += x;
  if (curpos.x < 0) curpos.x = 0;
  else if (curpos.x >= MAX_COLUMN) curpos.x = MAX_COLUMN-1;
  curpos.y += y;
  if (curpos.y < 0) curpos.y = 0;
  else if (curpos.y >= MAX_COLUMN) curpos.y = MAX_LINE-1;
  goto_cursol (curpos.x, curpos.y);
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
  W	addr;
  
  dis_int ();
  addr = x + (y * 80);

  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  outb (GDC_DATA, addr & 0xff);
  ena_int ();
}

/*
 *
 */
ER
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
	  curpos.x = 0;
	  if (curpos.y >= MAX_LINE)
	    {
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
  
#define CURSOR_POS(x,y)		(x + y * 80)
  addr = (short *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
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

void
clear_rest_line(void)
{
  W x, y;
  y = curpos.y;
  for(x = curpos.x+1; x < MAX_COLUMN; x++) {
    write_vram_xy (x, y, ' ', 0xE1);
  }
}

void
clear_rest_screen(void)
{
  W x, y;
  clear_rest_line();
  for (y = curpos.y+1; y < MAX_LINE; y++)
    for (x = 0; x < MAX_COLUMN; x++)
      {
	write_vram_xy (x, y, ' ', 0xE1);
      }
}
