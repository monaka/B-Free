/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT main routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/console.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: console.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  2000-01-30 18:50:07  kishida0
 * use same keyboard define file & you can use BS key
 *
 * Revision 1.4  1998/11/20 08:02:21  monaka
 * *** empty log message ***
 *
 * Revision 1.3  1997/04/24 15:28:53  night
 * コンソールの初期化時に、画面行数分スクロールを行う処理を追加した。
 *
 * 文字属性もクリアするため。console_clear() では、文字属性はクリアしない。
 * 機種によっては、起動時に文字属性が特殊(色付きなど)の場合がある。
 *
 * Revision 1.2  1996/05/11  15:49:51  night
 * コンソール表示時にアトリビュートをセットしない。
 *
 * Revision 1.1  1996/05/11  10:45:00  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 *
 * ------------------------------------------------------------------------
 *
 * Revision 1.2  1995/09/21 15:50:35  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:28:41  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:17  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:24  btron
 * BTRON SYSTEM 1.0
 * 
 *
 *	init_console
 *	console_clear
 *	write_cr
 *	write_tab
 *	putchar
 *
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/console.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "lib.h"
#include "errno.h"
#include "location.h"
#include "memory.h"
#include "console.h"
#include "macros.h"

static	int	x;
static	int	y;

void	write_cr (void);
void	write_tab (void);

/************************************************************************
 *
 */     
int
init_console (void)
{
  int	i;

  console_clear ();
  for (i = 0; i < MAX_HEIGHT; i++)
    {
      scroll_up ();
    }

  x = y = 0;
  set_cursor_pos (x, y);

  return E_OK;
}

void
console_clear (void)
{
  int	x, y;
  
  for (y = 0; y <= MAX_HEIGHT; y++)
    for (x = 0; x <= MAX_WIDTH; x++)
      {
	write_vram (x, y, ' ', 0xE1);
      }
}
/***********************************************************************
 *
 */
void
write_cr ()
{
  x = 0;
  if (y >= MAX_HEIGHT)
    {
      scroll_up ();
    }
  else
    {
      y++;
    }
  set_cursor_pos (x, y);
}

/***********************************************************************
 *
 */
void
write_tab ()
{
  int	tmp;
  if (x < MAX_WIDTH)
    {
      tmp = ROUNDUP (x + 1, 8) - 1;
      while (x < tmp)
	{
	  write_vram (x, y, ' ', 0);
	  x++;
	}
    }
}


/***********************************************************************
 *
 */
int
putchar (int ch)
{
  ch = ch & 0xff;
  if (ch == '\n')
    {
      write_cr ();
      return E_OK;
    }
  else
    {
      write_vram (x, y, ch, 0xE1);
    }
  
  if (++x >= MAX_WIDTH)
    {
      y++;
      x = 0;
      if (y > MAX_HEIGHT)
	{
	  scroll_up ();
	  y = MAX_HEIGHT;
	}
    }
  set_cursor_pos (x, y);

  return E_OK;
}

void set_coursor(int sx, int sy)
{
  x = sx;
  y = sy;
  set_cursor_pos (x,y);
}
void get_coursor(int *gx,int *gy)
{
  *gx = x;
  *gy = y;
}
