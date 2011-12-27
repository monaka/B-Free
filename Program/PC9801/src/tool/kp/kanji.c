/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/kanji.c,v $
 *
 * $Date: 2011/12/27 17:13:35 $
 * $Author: liu1 $
 * $Log: kanji.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:53:49  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1994/07/30  17:38:30  night
 * ファイル中の日本語文字をすべて EUC コードに変更。
 *
 * Revision 1.2  1994/07/05  17:22:11  night
 * *** empty log message ***
 *
 * Revision 1.1  1993/10/11  21:49:17  btron
 * btron/386 first coding.
 *
 * Revision 1.1.1.1  93/01/14  12:30:55  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:03  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1  93/01/09  02:45:36  btron
 * Initial version
 * 
 *
 * Discription
 *
 */

static char	version_id[] = "$Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/kanji.c,v $ $Revision: 1.1 $";

#include "../../kernel/itron-3.0/h/types.h"

/**************************************************************************
 * shift_to_jis --- シフトＪＩＳからＪＩＳへの変換
 *
 */
UH
shift_to_jis (UH shift)
{
  UH low, high;

  high = (shift >> 8) & 0xff;
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
UH
jis_to_shift (UH jis)
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
UB *
string_shift_to_jis (UB *s)
{
  UB 	*p;
  UH	kanji;

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
UB *
string_jis_to_shift (B *s)
{
  UB	*p;
  UH	kanji;

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
UB *
string_shift_to_tron (UB * s)
{
  UB 	*p;
  UH	kanji;

  for (p = (UB *)s; *p != '\0'; p++)
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
UB *
string_tron_to_shift (UB *s)
{
  UB 	*p;
  UH	kanji;

  for (p = (UB *)s; *p != '\0'; p += 2)
    {
      kanji = (*p << 8) | *(p + 1);
      kanji = jis_to_shift (kanji);
      *p = (kanji >> 8) & 0xff;
      *(p + 1) = (kanji & 0xff);
    }
  return (s);
}
