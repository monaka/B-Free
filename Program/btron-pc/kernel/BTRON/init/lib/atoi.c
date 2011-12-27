/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: atoi.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: atoi.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include <types.h>
#include <string.h>

int
atoi (char *s)
{
  int	result;
  BOOL	minus = FALSE;

  result = 0;
  if (*s == '-')
    {
      s++;
      minus = TRUE;
    }

  while (isnum(*s))
    {
      result = (result * 10) + (*s - '0');
      s++;
    }
  return (minus ? -result: result);
}




