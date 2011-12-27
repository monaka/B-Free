/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


/* $Header */
static char rcsid[] = "@(#)$Id: init.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* $Log: init.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1998-07-01 13:55:10  night
/* �ե��������ʸ�������ɤ� SJIS ���� EUC ���ѹ���
/*
/* Revision 1.1  1998/01/07 15:31:36  night
/* �ǽ����Ͽ
/*
 *
 */


/*
 * POSIX �Ķ��Ǻǽ��Ω���夬��ץ���� init��
 * 
 */

#include "init.h"



main (int ac, char **av)
{
  int	runlavel;

  if (ac <= 1)
    {
      /* runlevel �ϥǥե���� */
      runlevel = DEFAULT_RUNLEVEL;		/* 0 */
    }
  else
    {
      /* �����ǻ��ꤷ�� runlevel ��¹Ԥ��� */
      runlevel = atoi (av[1]);
    }

  go_runlevel (runlevel);	/* runlevel ���ѹ� */
      
}


go_runlevel (int runlevel)
{
  int	inittab;

  inittab = open (INITTAB, O_RDONLY);
  if (inittab < 0)
    {
      /* inittab �ե����뤬�����ץ�Ǥ��ʤ��ä� - */
      /* �����ƥ����ߤ��� */
    }

  while (read_line (inittab, line) > 0)
    {
      if (parse_runlevel (line) == runlevel)
	{
	  /* �ɤ߹�����Ԥ� runlevel �ϰ����ǻ��ꤵ�줿 runlevel ���� */
	  /* ���� -> ���ιԤǻ��ꤷ�Ƥ��륳�ޥ�ɤ�¹� */
	  exec_command (line);
	}
    }

  close (inittab);
}
