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
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/console.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: console.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:35  night
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

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/console.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "location.h"
#include "memory.h"
#include "console.h"
#include "macros.h"

static	int	x;
static	int	y;

void	write_cr ();
void	write_tab ();

/************************************************************************
 *
 */     
int
init_console (void)
{
  console_clear ();
  x = y = 0;
  set_cursor_pos (x, y);
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
	  write_vram (x, y, ' ', 0xe1);
	  x++;
	}
    }
}

static int	back = 0;

/***********************************************************************
 *
 */
int
putchar (int ch)
{
  short	*addr;
  unsigned short skanji;

  ch = ch & 0xff;
  if (back != 0)
    {
      skanji = ((back << 8) | ch) & 0xffff;
      write_kanji_vram (x++, y, skanji, 0xe1);
      back = 0;
    }
  else if (ch & 0x80)
    {
      back = ch;
      return;
    }
  else if (ch == '\n')
    {
      write_cr ();
      return;
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
}


