/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/spec.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/spec.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "fd.h"

struct spec	fd_spec[] =
{
  /* name    head  sector  length    dtl  lencode  gap3  gapskip    srt    hut    hlt     nd  dmachan  retry  motor            */
  {  "2HD",     2,      8,   1024,  0xff,       3,  116,      53,  0x0d,  0x06,  0x0f,   0x0,       2,     3,     0 },	/* 2HD */
  {  "2HC",     2,     15,    512,  0xff,       2,   84,      27,  0x0d,  0x06,  0x0e,   0x0,       2,     3,     0 },	/* 2HC */
  {   NULL,     0,      0,      0,     0,       0,    0,       0,     0,     0,     0,     0,       0,     0,     0 }
};


/*
 * FD の種類名をキーにして、Specification (の入った構造体へのポインタ)を返す。
 */
struct spec *
get_fdspec (B *name)
{
  W	i;

  for (i = 0; fd_spec[i].typename != NULL; i++)
    {
      dbg_printf ("compare: <%s>, <%s>\n", name, fd_spec[i].typename);	/* */
      if (strcmp (name, (B *)(fd_spec[i].typename)) == 0)
	{
	  return (&(fd_spec[i]));
	}
    }
  dbg_printf ("fd: get_fdspec error.(name = %s)\n", name);
  return (NULL);	
}

