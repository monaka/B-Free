/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Id: main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.15  2000-01-15 15:17:02  naniwa
 * no more depend on libc.a
 *
 * Revision 1.14  1999/11/10 10:23:57  naniwa
 * to support execve
 *
 * Revision 1.13  1999/05/10 02:56:04  naniwa
 * add read_line_edit()
 *
 * Revision 1.12  1999/03/30 13:15:23  monaka
 * Now we connect to lowlib.posix by default.
 *
 * Revision 1.11  1999/03/15 06:00:45  monaka
 * modified some debug messages.
 *
 * Revision 1.10  1999/02/17 09:43:09  monaka
 * Modified for 'device descriptor'.
 *
 * Revision 1.9  1998/12/08 05:38:35  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.8  1998/12/02 01:03:39  monaka
 * Major modified for implementing scripting language.
 * But there are a partly connection to user interface.
 *
 * Revision 1.7  1998/11/26 11:14:29  monaka
 *
 * Command evaluator has moved to libMitten/evaluator.c
 * This modify is for Scripting Environment but has not implemented yet.
 *
 * Revision 1.6  1998/11/26 10:19:07  monaka
 *
 * Parser routines has moved to libMitten/parse.c and modified.
 * This modify makes you use multi word string and some escape character on
 * init scripting interpreter.
 * Note that you can't use Mitten language yet. The command syntax is still
 * traditional.
 *
 * Revision 1.5  1997/08/31 14:04:33  night
 * BOOT �� HD ����Ԥ�줿���ˤϡ���ưŪ�� POSIX �� root file system ��
 * ���ꤹ��褦�˽������ѹ�������
 *
 * Revision 1.4  1997/05/06 12:44:26  night
 * ���ϹԤ򤽤Τޤ޽��Ϥ��� print ʸ����������
 *
 * Revision 1.3  1997/04/24 15:37:32  night
 * parse_line() ����ʸ����ΥХåե��κǸ������å������ͤȤ��ơ�
 * ���ϸ������(�ƥ��ޥ�ɤΰ����Ȥ��ƻ���)�Υ���������Ѥ��Ƥ�����
 * ���Τ��ᡢ����Υ�����(10) ����Ĺ���Ԥ����Ϥ���ȡ�ư���������
 * �ʤä���
 *
 * Revision 1.2  1996/07/25  16:58:52  night
 * �ɤ߹���Ԥ򥪡��С��ե�����Ȥ��ν������ѹ���
 * �����ϡ��Хåե��Υ������򥪡��С��ե����������
 * �Х������ꡢ�����С��ե��������Ȥ�õ�ΤǤ��ʤ��ä���
 *
 * Revision 1.1  1996/07/25  16:02:00  night
 * IBM PC ���Ѥؤκǽ����Ͽ
 *
 * Revision 1.3  1996/01/06 12:16:35  night
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
ID	console;
ID	dev_recv;	/* �ǥХ����ɥ饤�Ф���������� */


/*
 *
 */
W main (W ac, B **av)
{
  static B	line[100];
  UW		rootfs;
  ER		errno;


  init_device ();
  MitnStack_Init ();
  InitMittenHook ();

  banner ();

  errno = vsys_inf (1, 0, &rootfs);
  if ((errno == E_OK) && (rootfs != 0xffffffff))
    {
      errno = posix_init (rootfs);
    }
  else
    {
      for (;;)
	{
	  printf ("Root file system is... [fd]/hd: ");
	  read_line_edit (line, sizeof (line));
	  if (*line == '\0')
	    {
	      if (posix_init (0x80000000) == E_OK)
		break;
	    }
	  else if (strcmp (line, "fd") == 0)
	    {
	      if (posix_init (0x80000000) == E_OK)
		break;
	    }
	  else if (strcmp (line, "hd") == 0)
	    {
	      if (posix_init (0x80010002) == E_OK)
		break;
	    }
	  printf ("Error on boot. retry...\n");
	}
    }

  main_loop ();
}


void
banner (void)
{
  printf ("BTRON OS Initialize program (powered by Mitten Scripting Language).\n");
  printf ("Version %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
}


void main_loop ()
{
  static B	line[100];

  eval ("(lowlib.posix) load");

  for (;;)
    {
/*      MitnStack_Status();*/
      eval ("(init>) ==");
      read_line_edit (line, sizeof (line));
      if (*line != '\0')
	{
	  eval (line);
	  MitnStack_GarbageCollect ();
	}
    }
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

W read_line_edit(B *line, W length)
{
  W i;
  W ch;
  
  for (i = 0; i < length-1;) {
    ch = getc(stdin);
    if ((ch == C('m')) || (ch == C('j'))) break;
    else if (ch == 0x0b) {
      if (i > 0) {
        --i; line[i] = 0;
        printf("\b");
      }
    }
    else if (isprint (ch)) {
        line[i++] = ch;
    }
  }
  line[i] = '\0';
  return (i);
}

