/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include <stdio.h>
#include "../../boot/2nd/boot.h"


main (int ac, char **av)
{
  FILE				*fp;
  int				i;
  struct boot_header		header;
  extern int			nmodule;
  extern struct module_info	module_table[];
  extern char			*module_file[];

  fp = fopen (av[1], "w");
  if (fp == NULL)
    {
      perror ("open");
      exit (-1);
    }
  header.cookie = 0x0001;
  header.count = nmodule;
  fwrite (&header, sizeof (header), 1, fp);
  for (i = 0; i < nmodule; i++)
    {
      fwrite (&module_table[i], sizeof (struct module_info), 1, fp);
    }
  fseek (fp, 1024, 0);
  for (i = 0; i < nmodule; i++)
    {
      printf ("[%d]\t%s\n", i + 1, module_file[i]);
      write_module (fp, module_file[i], module_table[i].length);
    }
  fclose (fp);
}

write_module (FILE *output, char *module, int length)
{
  FILE *input;
  char buf[1024];
  int	i;

  input = fopen (module, "r");
  if (input == NULL)
    {
      perror ("open(module)");
      exit (-1);
    }
  i = 0;
  while (fread (buf, sizeof (buf), 1, input) > 0)
    {
      fwrite (buf, sizeof (buf), 1, output);
      i++;
    }
  printf ("%d\n", i);
  fclose (input);
}
