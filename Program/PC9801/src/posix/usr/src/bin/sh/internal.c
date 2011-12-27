/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/internal.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/internal.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: internal.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:08  night
/* �������ե��������Ƭ�� Copyright notice ������ɲá�
/*
 * Revision 1.1  1995/03/18  14:15:13  night
 * �ǽ����Ͽ
 *
 *
 */

#include "sh.h"


static int	internal_chdir (int ac, char **av);
static int	internal_echo (int ac, char **av);


/* 
 * POSIX �Ķ��ѤΥ����륳�ޥ��
 * (�������ޥ�ɤ������Ԥ�)
 */
struct internal_command	command_table[] =
{
  { "cd",	internal_chdir },
  { "echo",	internal_echo },
  { NULL,	NULL },
};


/*
 * chdir
 */
static int
internal_chdir (int ac, char **av)
{
  int	errno;
  char	*homedir;
  struct passwd *pass;

  /*
   * cd ���ޥ�ɤ˰�����������ꤵ��Ƥ��ʤ����ˤϡ��ۡ���ǥ��쥯�ȥ��
   * ��ư���롣
   */
  if (ac <= 1)
    {
      homedir = getenv ("HOME");
      if (homedir == NULL)
	{
	  /* /etc/passwd �ե����뤫�� ��������桼���Υ���ȥ���������  */
	  /* �ۡ���ǥ��쥯�ȥ�̾�����롣 */
	  
	}
      else
	{
	  if (chdir (homedir) < 0)
	    {
	      perror ("cd");
	    }
	}
    }
  else
    {
      /* 1�İʾ������������ˤϡ��ǽ�ΰ����˽��ä� chdir �����ƥ� */
      /* �������¹Ԥ��롣 */
      if (chdir (av[1]) < 0)
	{
	  perror ("cd");
	}
    }
  return (errno);
}

/*
 * ���������Ƥ򤽤Τޤ޽��Ϥ��롣
 */
int
internal_echo (int ac, char **av)
{
  int	i;

  for (i = 1; i < ac; i++)
    {
      sh_printf ("%s\n", av[i]);
    }
  return (0);
}
