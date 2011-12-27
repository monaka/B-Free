/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
 * 入力した文字列を配列に変換する処理の変更。
 * (parse_line ())
 *
 * Revision 1.2  1995/12/05 14:34:56  night
 * o デバイスの初期化処理を init_device() 関数に移動する。
 * o (擬似)標準入出力関数への対応。
 *
 * Revision 1.1  1995/10/10  16:21:46  night
 * 最初の登録.
 * システム起動後、プロンプトを出力してコマンド入力 - 実行ができるところ
 * まで出来た。ただし、コマンドは echo だけ。
 *
 *
 */

#include "init.h"


/*
 * 入出力を行う。
 */
ID	keyboard;
ID	console;
ID	dev_recv;	/* デバイスドライバからの返答用 */


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
 * Init プログラムの初期化
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
	  /* コマンドをみつけた */
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

