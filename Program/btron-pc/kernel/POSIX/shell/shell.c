/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/shell/shell.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Id: shell.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: shell.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1996-11-25 14:04:13  night
 * main() �ؿ��ι�����
 *
 * Revision 1.1  1996/11/22  16:22:18  night
 * �ǽ����Ͽ
 *
 *
 */


/*
 *
 */
main (int ac, char **av)
{
  int	i;

  if (ac < 2)
    {
      /* ���󥿥饯�ƥ��֥⡼�ɤ�ư��롣
       */
      shell_interactive ();
      exit (0);
    }
  
  exec_command (av[1], ac - 2, &av[2]);
  exit (0);
}



