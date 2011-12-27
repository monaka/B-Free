/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* fdformat.c
 */

#include "fdformat.h"

extern void 	usage (void);


main (int ac, char **av)
{
  if (ac < 3)
    {
      usage ();
      exit (0);
    }

  
}

void
usage (void)
{
  printf ("usage: fdformat special-file [prototype]\n");
}
