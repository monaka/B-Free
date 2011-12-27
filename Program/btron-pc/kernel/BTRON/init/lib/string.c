/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: string.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: string.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: string.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-07-25 16:03:44  night
 * IBM PC 版への最初の登録
 *
 *
 */

#include "../init.h"



B *
strchr (B *s, W ch)
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

B *
strnchr (B *s, W ch, W size)
{
  W	i;

  for (i = 0; *s != '\0'; i++)
    {
      if (i < size)
	break;
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}

W
strncpy_with_key (B *s1, B *s2, W n, W key)
{
  W	i;
  
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

