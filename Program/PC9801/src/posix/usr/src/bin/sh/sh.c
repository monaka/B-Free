/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/sh.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/sh.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: sh.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:08  night
/* �������ե��������Ƭ�� Copyright notice ������ɲá�
/*
 * Revision 1.1  1995/03/18  14:15:12  night
 * �ǽ����Ͽ
 *
 *
 */

/* 
 * POSIX �Ķ��ѤΥ����륳�ޥ��
 *
 */

#include "sh.h"

/*
 * ����ѿ������ (�ºݤ˾���Ȥ�)
 */
int	stdin_fd  = 0;
int	stdout_fd = 1;
int	stderr_fd = 2;


static void	init_shell (int ac, char **av, char *ev);



/*
 * �����륳�ޥ�ɤμ¹Ԥγ���
 *
 * ���Υ�����ϰʲ��Υ��ץ�������:
 *
 *	-v		+verbose ��Ʊ��ư��򤹤롣
 *	
 *	+stdin device	ɸ�����ϥǥХ����λ���
 *	+stdout device	ɸ����ϥǥХ����λ���
 *	+stderr device	ɸ�२�顼���ϥǥХ����λ���
 *	+verbose	-v ��Ʊ��ư��򤹤롣
 *
 */
void
main (ac, av, ev)
     int	ac;
     char	**av;
     char	*ev;
{
  char	line[MAXLINE];

  /*
   * ������ץ����ν����
   */
  init_shell (ac, av, ev);

  /*
   * ���Υ�����ϡ�����Ū�� �ץ��ץ�ɽ�� - ������ - �¹� �Υ롼�פ�
   * ��ʤꤿ�äƤ��롣
   */
  for (;;)
    {
      print_prompt ();
      if (read_line (line) == EOF)
	{
	  exit_shell ();
	}
      eval_line (line);
    }
}

/*
 *
 */
static void
init_shell (ac, av, ev)
     int	ac;
     char	**av;
     char	*ev;
{
  int	i;

  /*
   * ���ޥ�ɥ��ץ����β���
   */
  for (i = 1; i < ac; i++)
    {
      if (strcmp (av[i], "+stdin") == 0)
	{
	  int	fd;

	  fd = open (av[++i], O_RDONLY);
	  if (fd == -1)
	    {
	      fatal_error ("cannot device open %s\n", av[i]);
	    }
	  stdin_fd = fd;
	}
      else if (strcmp (av[i], "+stdout") == 0)
	{
	  int	fd;

	  fd = open (av[++i], O_WRONLY);
	  if (fd == -1)
	    {
	      fatal_error ("cannot device open %s\n", av[i]);
	    }
	  stdout_fd = fd;
	}
      else if (strcmp (av[i], "+stderr") == 0)
	{
	  int	fd;

	  fd = open (av[++i], O_WRONLY);
	  if (fd == -1)
	    {
	      fatal_error ("cannot device open %s\n", av[i]);
	    }
	  stderr_fd = fd;
	}
    }

  /*
   * �����ʥ�ϥ�ɥ�λ���
   */

  

}
