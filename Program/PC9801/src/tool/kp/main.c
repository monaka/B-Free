/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/main.c,v $
 *
 * $Date: 2011/12/27 17:13:35 $
 * $Author: liu1 $
 *
 *
 * description:
 *
 */

static char	version_id[] = "$Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/tool/kp/main.c,v $ $Revision: 1.1 $";

#include "kp.h"

char	*input_language = "euc";
char	*language_table[] =
{
  "sjis",
  "SJIS",
  "euc",
  0
};


void
main (int ac, char **av)
{
  int	i;
  char	*input_file = NULL;
  char	*output_file = NULL;
  
  for (i = 1; i < ac; i++)
    {
      if (!strcmp (av[i], "-h") || !strcmp (av[i], "-?"))
	{
	  short_help ();
	  exit (0);
	}
      else if (!strcmp (av[i], "-help"))
	{
	  long_help ();
	  exit (0);
	}
      else if (!strcmp (av[i], "-input-code") || !strcmp (av[i], "-ic"))
	{
	  if (check_language (av[++i]) == NG)
	    {
	      fprintf (stderr, "Language is unknown. (%s)\n", av[i]);
	      exit (0);
	    }
	  input_language = av[i];
	}
      else if (!strcmp (av[i], "-input") || !strcmp (av[i], "-i"))
	{
	  input_file = av[++i];
	}
      else if (!strcmp (av[i], "-output") || !strcmp (av[i], "-o"))
	{
	  output_file = av[++i];
	}
    }

  top_level (input_file, output_file);
  exit (0);
}

int
check_language (char *lang)
{
  int	i;

  for (i = 0; language_table[i] != NULL; i++)
    {
      if (!strcmp (lang, language_table[i]))
	{
	  return (OK);
	}
    }
  return (NG);
}
