/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Id: main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "$Id: main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1996-01-06 12:16:35  night
 * ���Ϥ���ʸ�����������Ѵ�����������ѹ���
 * (parse_line ())
 *
 * Revision 1.2  1995/12/05 14:34:56  night
 * o �ǥХ����ν���������� init_device() �ؿ��˰�ư���롣
 * o (����)ɸ�������ϴؿ��ؤ��б���
 *
 * Revision 1.1  1995/10/10  16:21:46  night
 * �ǽ����Ͽ.
 * �����ƥ൯ư�塢�ץ��ץȤ���Ϥ��ƥ��ޥ������ - �¹Ԥ��Ǥ���Ȥ���
 * �ޤǽ��褿�������������ޥ�ɤ� echo ������
 *
 *
 */

#include "init.h"


/*
 * �����Ϥ�Ԥ���
 */
ID	keyboard;
ID	console;
ID	dev_recv;	/* �ǥХ����ɥ饤�Ф���������� */


/*
 *
 */
main (W ac, B **av)
{
  B	*buf;

  init_init ();
  banner ();
  buf = malloc (1024);
  printf ("buf = %d\n", buf);
  main_loop ();
}

void
banner (void)
{
  printf ("BTRON OS Initialize program.\n");
  printf ("Version %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
}

/*
 * Init �ץ����ν����
 */
init_init ()
{
  ER	err;

  init_device ();
  err = init_malloc ();
  printf ("err = %d\n", err);
}


main_loop ()
{
  B	line[100];

  for (;;)
    {
      printf ("init> ");
      read_line (stdin, line, sizeof (line));
      if (*line != '\0')
	eval (line);
    }
}

void
eval (B *line)
{
  W	i;
  B	*av[10];
  B	ac;

  ac = parse_line (line, av, 10);
  for (i = 0; command_table[i].name != NULL; i++)
    {
      if (strcmp (av[0], command_table[i].name) == 0)
	{
	  /* ���ޥ�ɤ�ߤĤ��� */
	  command_table[i].func (ac, av);
	  return;
	}
    }
  printf ("%s: command not found\n", av[0]);
}


W
parse_line (B *line, B **av, W size)
{
  W	i;

  for (i = 0; i < size; line++)
    {
      while (isspace(*line) && (*line != '\0'))
	line++;
      if (*line == '\0')
	break;
      av[i] = line;
      i++;

      while ((!isspace(*line)) && (*line != '\0'))
	line++;
      if (*line == '\0')
	break;
      *line = '\0';
    }
  return (i);
}


W
read_line (FILE *port, B *line, W length)
{
  W	i;
  W	ch;
  
  for (i = 0; i < length; i++)
    {
      ch = getc (port);
      if ((ch == C('m')) || (ch == C('j')))
	break;
      if (isprint (ch))
	{
	  line[i] = ch;
	}
    }
  line[i] = '\0';
  return (i);
}

