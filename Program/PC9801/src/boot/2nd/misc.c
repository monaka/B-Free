/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT misc routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:50:41  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1994/07/30  17:37:18  night
 * ファイル中の日本語文字をすべて EUC コードに変更。
 *
 * Revision 1.1  1993/10/11  21:29:38  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:25  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:23  btron
 * BTRON SYSTEM 1.0
 * 
 *
 * 色々な関数の集合
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "misc.h"

/***************************************************************************
 *
 */
int
getchar (void)
{
  int	ch;
  
  ch = read_keyboard ();
  putchar (ch);
  return (ch);
}

/***************************************************************************
 *
 */
char *
gets (char *line)
{
  int	ch;
  char	*p;
  
  p = line;
  ch = getchar ();
  while (ch != '\n')
    {
      *p++ = ch;
      ch = getchar ();
    }
  *p = '\0';
  return (line);
}

/**************************************************************************
 * shift_to_jis --- シフトＪＩＳからＪＩＳへの変換
 *
 */
unsigned short
shift_to_jis (unsigned short shift)
{
  unsigned short low, high;

  high = (shift >> 8);
  low = (shift & 0xff);
  
  if (shift == 0x8520)	return (0x2121);
  if (high == 0x80)
    return (shift);
  else
    {
      if (high < 0xa0)
	high -= 0x70;
      else if (high < 0xf0)
	high -= 0xb0;
      else
	return (shift);

      if (low >= 0x80)
	low--;

      high *= 2;
      if (low >= 0x9e)
	low -= 0x5e;
      else
	high--;

      low -= 0x1f;

      if (high == 0x2a)
	return (low | 0x80);
      else if (high == 0x29)
	high = 0;

      return ((((high & 0xff) << 8) + (low & 0xff)));
    }
}

/***************************************************************************
 *
 *
 */
unsigned short
jis_to_shift (unsigned short jis)
{
  unsigned int	low;
  unsigned int	high;

  if (jis == 0x2121)
    return (0x8140);

  low = (jis & 0xff) + 0x1f;
  high = (jis >> 9);
  if ((jis & 0x100) == 0)
    {
      high--;
      low += 0x5e;
    }
  if (low >= 0x7f)
    low++;
  if (high < 0x2f)
    high += 0x71;
  else
    high += 0xb1;
  return (((high & 0xff) << 8) + (low & 0xff));
}

/********************************************************************
 *
 */
char *
string_shift_to_jis (char *s)
{
  unsigned char *p;
  unsigned short	kanji;

  for (p = (unsigned char *)s; *p != '\0'; p++)
    {
      if (*p >= 0x80)	/* kanji */
	{
	  kanji = (*p << 8) | *(p + 1);
	  kanji = shift_to_jis (kanji);
	  *p = (kanji >> 8) & 0xff;
	  *(p + 1) = (kanji & 0xff);
	  p++;
	}
    }
  return (s);
}

/********************************************************************
 *
 */
char *
string_jis_to_shift (char *s)
{
  unsigned char *p;
  unsigned short	kanji;

  for (p = (unsigned char *)s; *p != '\0'; p += 2)
    {
      kanji = (*p << 8) | *(p + 1);
      kanji = jis_to_shift (kanji);
      *p = (kanji >> 8) & 0xff;
      *(p + 1) = (kanji & 0xff);
    }
  return (s);
}

/********************************************************************
 *
 */
char *
string_shift_to_tron (char *s)
{
  unsigned char *p;
  unsigned short	kanji;

  for (p = (unsigned char *)s; *p != '\0'; p++)
    {
      if (*p >= 0x80)	/* kanji */
	{
	  kanji = (*p << 8) | *(p + 1);
	  kanji = shift_to_jis (kanji);
	  *p = (kanji & 0xff);
	  *(p + 1) = (kanji >> 8) & 0xff;
	  p++;
	}
    }
  return (s);
}

/********************************************************************
 *
 */
char *
string_tron_to_shift (char *s)
{
  unsigned char *p;
  unsigned short	kanji;

  for (p = (unsigned char *)s; *p != '\0'; p += 2)
    {
      kanji = *p | (*(p + 1)) << 8;
      kanji = jis_to_shift (kanji);
      *p = (kanji >> 8) & 0xff;
      *(p + 1) = (kanji & 0xff);
    }
  return (s);
}

/**************************************************************************
 * print_record_type --- 
 *
 */
void
print_record_type (int	type)
{
  switch (type)
    {
    default:
      printf ("normal type\n");
      break;
    case 0:
      printf ("unused record\n");
      break;
    case 0x80:
      printf ("link record\n");
      break;
    }
}

      
  
