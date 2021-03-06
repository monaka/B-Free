/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT atoi routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/atoi.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: atoi.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:33  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:28:34  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:13  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:34  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/atoi.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"

#define isdigit(c,base)	((c >= '0') && (c <= '9'))


/**************************************************************************
 *
 */
int
atoi (char *s)
{
  return (string_to_number (s, 10));
}

/***************************************************************************
 *
 */
int
string_to_number (char *s, int base)
{
  int	total = 0;
  

  while (*s != '\0')
    {
      if (!isdigit (*s, base))
	return (0);
      total = (total * base) + (*s - '0');
      s++;
    }
  return (total);
}


