/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include <stdio.h>
#include <a.out.h>

main (int ac, char **av)
{
  struct exec	info;
  FILE		*fp;
  
  fp = fopen (av[1], "rb");
  if (fp == NULL)
    {
      exit (-1);
    }

  fread (&info, sizeof (info), 1, fp);
  printf ("0x%x\n", info.a_entry);
}
