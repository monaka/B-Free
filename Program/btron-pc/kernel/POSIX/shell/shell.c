/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
 * main() 関数の更新。
 *
 * Revision 1.1  1996/11/22  16:22:18  night
 * 最初の登録
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
      /* インタラクティブモードで動作する。
       */
      shell_interactive ();
      exit (0);
    }
  
  exec_command (av[1], ac - 2, &av[2]);
  exit (0);
}



