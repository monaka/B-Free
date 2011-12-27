/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT VRAM routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/vram.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: vram.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:44  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:30:03  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:28  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:27  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/vram.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "location.h"
#include "console.h"
#include "misc.h"
#include "vram.h"

/* static void	scroll_up (); */


/***********************************************************************
 * 
 */
void
write_vram (int x, int y, int ch, int attr)
{
  short	*addr;
  short *attr_addr;
  
  addr = (short *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = ch & 0x00ff;
/*
  attr_addr = (short *)TEXT_ATTR_ADDR;
  attr_addr [CURSOR_POS (x, y)] = (short)attr & 0xff;
*/
}

/***********************************************************************
 * 
 */
void
write_kanji_vram (int x, int y, unsigned int ch, int attr)
{
  unsigned short	*addr;
  short *attr_addr;
  unsigned short first, second;
  unsigned short	kanji;

/*  printf ("write_kanji_vram: 0x%x\n", ch); */
  kanji = shift_to_jis (ch);
  
  first = ((kanji >> 8) & 0xff) - 0x20;
  second = (kanji << 8) & 0xff00;

  addr = (unsigned short *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = second | first;
  addr [CURSOR_POS (x + 1, y)] = second | (first | 0x80);
/*
  attr_addr = (short *)TEXT_ATTR_ADDR;
  attr_addr [CURSOR_POS (x, y)] = (short)attr & 0xff;
/*
  printf ("first = 0x%x\n", first);
  printf ("second =0x%x\n", second);
  printf ("addr = 0x%x\n", &addr[CURSOR_POS (x,y)]);
*/
}
/***********************************************************************
 * 
 */
void
write_kanji_vram2 (int x, int y, unsigned int ch, int attr)
{
  short	*addr;
  short *attr_addr;
  short first, second;
  
  first = (ch >> 8) & 0xff;
  second = ch & 0xff;
  
  addr = (short *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = ((first - 0x20) << 8) | second;
  addr [CURSOR_POS (x + 1, y)] = ((first - 0x20) << 8) | second | 0x8000;
/*
  attr_addr = (short *)TEXT_ATTR_ADDR;
  attr_addr [CURSOR_POS (x, y)] = (short)attr & 0xff;
*/
}

/**************************************************************************
 * set_cursor_pos --- 
 */
void
set_cursor_pos (int x, int y)
{
  int	adr;
  
  adr = x + (y * 80);
  while ((inb (GDC_STAT) & 0x04) == 0)
    ;
  outb (GDC_COMMAND, 0x49);
  outb (GDC_STAT, adr & 0xff);
  outb (GDC_STAT, (adr >> 8) & 0xff);
}

void
scroll_up ()  
{
  unsigned short	*addr;
  int			i;
  
  addr = (unsigned short *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE - 80; i++)
    {
      addr[i] = addr[i + 80];
    }
  for (i = 0; i <= MAX_WIDTH; i++)
    {
      write_vram (i, MAX_HEIGHT, ' ', 0xe1);
    }
}

