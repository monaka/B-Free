/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/interpret.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/interpret.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "init.h"

#ifdef nodef

/*
 * ���ꤷ���ե���������Ƥ򸫤Ƽ¹Ԥ��롣
 *
 * ���δؿ������򤹤�ե���������Ƥϼ��Τ褦�ˤʤäƤ��롣
 *
 * command argument
 *
 * ñ�˥��ޥ�ɤȰ������¤Ӥˤ����ʤ���
 *
 */
interpret (char *fname)
{
  int	fd;
  char	cmd[MAXCMD];
  char	args[MAXARGS][MAXARGLEN];
  char	line[MAXLINE];

  fd = ropen (fname);
  if (fd < 0)
    {
      printf ("init: cannot open file (%s)\n");
      stop ();
    }

  /* 
   * �ե���������Ƥ��Ԥ����ɤ߼�ꡢ
   * �¹Ԥ��Ƥ���.
   */
  while (read_line (fd, line, sizeof (line)) >= 0)
    {
      if (line[0] != '#')	/* ���ʳ��ιԤΤ߲�᤹�� */
	{
	  parser (line, &cmd, args);
	  exec_program (cmd, args);
	  wait ();
	}
    }
}

#endif /* notdef */
