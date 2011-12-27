/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* <strings.c> --- 文字列操作関係の関数定義
 *
 * $Date: 2011/12/27 17:13:36 $
 * $Version$
 * $Author: liu1 $
 *
 * Discription
 *
 */

#include "types.h"
#include "strings.h"

extern WORD	tron_to_number_1 (TPTR s, WORD d);
extern WORD	tcode_to_number (TCODE s);

/*************************************************************************
 * strlen
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
WORD
strlen (BPTR s)
{
  int	i;

  for (i = 0; *s != '\0'; i++, s++)
    ;
  return (i);
}

/*************************************************************************
 * strnlen ---
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
WORD
strnlen (BPTR s, WORD len)
{
  WORD	i;

  for (i = 0; (*s != '\0') && (i < len); i++, s++)
    ;
  return (i);
}

/*************************************************************************
 * strcpy --- 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
WORD
strcpy (BPTR s1, BPTR s2)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/*************************************************************************
 * strncpy --- 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
WORD
strncpy (BPTR s1, BPTR s2, WORD n)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      if (i >= n)
	{
	  *s1 = '\0';
	  return (i);
	}
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/*************************************************************************
 * strncpy_with_key --- 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
WORD
strncpy_with_key (BPTR s1, BPTR s2, WORD n, int key)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      if ((i >= n) || (*s2 == key))
	{
	  *s1 = '\0';
	  return (i);
	}
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/***********************************************************************
 * strcat -- ２つの文字列の連結
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
TPTR
strcat (TPTR s1, TPTR s2)
{
  TPTR ret_val = s1;
  
  while (*s1++)
    ;
  while (*s2)
    {
      *s1++ = *s2++;
    }
  return ret_val;
}

/***********************************************************************
 * strncat -- ２つの文字列の連結
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
TPTR
strncat (TPTR s1, TPTR s2, WORD n)
{
  TPTR	ret_val = s1;
  WORD	i;
  
  while (*s1++)
    ;
  for (i = 0; *s2; i++)
    {
      if (i >= n)
	break;
      *s1++ = *s2++;
    }
  return ret_val;
}

/***********************************************************************
 * strcmp --- ２つの文字列の比較
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
ULONG
strcmp (BPTR s1, BPTR s2)
{
  while (*s1 && *s2)
    {
      if (*s1 != *s2)
        {
          return (*s2 - *s1);
        }
      s1++;
      s2++;
    }
  return (*s1 - *s2);
}

/***********************************************************************
 * strchr -- 文字列の中に指定した文字が含まれていないかを調べる。
 *
 *
 */
char *
strchr (char *s, int ch)
{
  while (*s != '\0')
    {
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}

/***********************************************************************
 * strnchr -- 文字列の中に指定した文字が含まれていないかを調べる。
 *
 *
 */
char *
strnchr (char *s, int ch, int n)
{
  int	i;

  for (i = 0; *s != '\0'; i++)
    {
      if (i < n)
	break;
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}


/***********************************************************************
 * strncmp -- ２つの文字列の比較
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
ULONG
strncmp (BPTR s1, BPTR s2, WORD n)
{
  WORD	i;

  for (i = 0; (i < n) && *s1 && *s2; i++)
    {
      if (*s1 != *s2)
        return (*s2 - *s1);
      s1++;
      s2++;
    }
  if (i == n)
    return 0;
  return (*s1 - *s2);
}

/*************************************************************************
 * tron_to_number 
 *
 * 引数：	s
 *
 * 返値：	変換後の数値
 *
 * 処理：
 *
 */
GLOBAL WORD
tron_to_number (TPTR s)
{
  WORD	retval = 0;
  LOCAL TPTR minus = (TPTR)"−";

  string_shift_to_tron (minus);
  if (strncmp ((BPTR)s, (BPTR)minus, sizeof (TCODE)) == 0)
    {
      retval = tron_to_number_1 (++s, 0);
      retval = 0 - retval;
    }
  else
    {
      retval = tron_to_number_1 (s, 0);
    }
  string_tron_to_shift (minus);
  return (retval);
}

/*************************************************************************
 * tron_to_number_1 -- tron_to_number の下請けルーチン
 *
 * 引数：	s
 *
 * 返値：	
 *
 * 処理：
 *
 */
LOCAL WORD
tron_to_number_1 (TPTR s, WORD d)
{
  WORD i;

  if (*s == TNULL)
    {
      return (d);
    }

  i = tcode_to_number (*s);
  return (tron_to_number_1 (++s, d * 10 + i));
}

/*************************************************************************
 * tcode_to_number --- ＴＲＯＮコード1文字を数値に変換する
 *
 * 引数：	s	tronコードの文字
 *
 * 返値：	-1	変換できない
 *		!= -1	変換後の数値
 *
 * 処理：      
 *
 */
LOCAL WORD
tcode_to_number (TCODE s)
{
  WORD i;
  LOCAL TPTR number_char = (TPTR)"０１２３４５６７８９";

  string_shift_to_tron (number_char);
  for (i = 0; i <= 9; i++)
    {
      if (strncmp ((BPTR)&s, (BPTR)(number_char[i]), sizeof (TCODE)) == 0)
	{
	  string_tron_to_shift (number_char);
	  return (i);
	}
    }
  
  string_tron_to_shift (number_char);
  return (-1);
}
  

toupper (int ch)
{
  if ((ch >= 'a') && (ch <= 'z'))
    {
      ch = ch + ('A' - 'a');
    }
  return (ch);
}
