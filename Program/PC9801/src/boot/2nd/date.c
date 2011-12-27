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
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/date.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: date.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:36  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:28:43  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:17  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:30  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/date.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "location.h"
#include "date.h"

void
busywait(int x)
{
  int i,j;
  int	tmp;

  for (i = 0; i < x; i++)
     for (j = 0; j < 100; j++)
       tmp = j;
}

static int	bcd_to_bin (int bcd);
 
void
init_clock (void)
{
  /* no work */
}

int
read_clock (struct date_t *date)
{
  unsigned char	data[6];		/* 40 bit */
  int		i,j;
  int		tmp;
  
  outb (0x20, 0x03);
  busywait (2);
  outb (0x20, 0x0b);
  busywait (4);
  outb (0x20, 0x03);
  busywait (20);

  outb (0x20, 0x01);
  busywait (2);
  outb (0x20, 0x09);
  busywait (4);
  outb (0x20, 0x01);
  busywait (40);

  for (i = 0; i < 6; i++)
    {
      data[i] = 0;
      for (j = 0; j < 8; j++)
	{
	  tmp = 0;
	  tmp = inb (0x33);
	  data[i] |= ((tmp & 0x1) << j);
	  outb (0x20, 0x11);
	  busywait (2);
	  outb (0x20, 0x01);
	  busywait (2);
	}
    }
  date->year = bcd_to_bin (data[5]);
  date->month = 0;
  date->day = bcd_to_bin (data[3]);
  date->week = 0;
  date->hour = bcd_to_bin (data[2]);
  date->minute = bcd_to_bin (data[1]);
  date->second = bcd_to_bin (data[0]);
  printf ("%d%d:%d%d:%d%d\n",
	  (data[2] >> 4) & 0xf, data[2] & 0xf,
	  (data[1] >> 4) & 0xf, data[1] & 0xf,
	  (data[0] >> 4) & 0xf, data[0] & 0xf);
  return (1);
}

static int
bcd_to_bin (int bcd)
{
  unsigned int	tmp;
  
  tmp = ((unsigned)bcd) >> 4;
  return ((tmp * 10) + (bcd & 0xf));
}


  
