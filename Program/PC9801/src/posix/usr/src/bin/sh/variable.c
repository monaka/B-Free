/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/variable.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/variable.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * シェル変数の設定をおこなうモジュール
 */

#include "sh.h"


struct variable
{
  char	*name;
  char	*value;
};


/*
 *
 */
int
var_set (char *name, char *value)
{
  struct variable	*var;

  var = fetch_var (name);
  if (var == NULL)
    {
      var = var_make ();
    }
  else
    {
      if (var->value)
	free (var->value);
    }
  var->value = value;
  return (0);
}
