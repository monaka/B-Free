/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.1  1995/03/18  14:15:12  night
 * 最初の登録
 *
 *
 */

/* 
 * POSIX 環境用のシェルコマンド
 *
 */

#include "sh.h"

/*
 * 大域変数の宣言 (実際に場所をとる)
 */
int	stdin_fd  = 0;
int	stdout_fd = 1;
int	stderr_fd = 2;


static void	init_shell (int ac, char **av, char *ev);



/*
 * シェルコマンドの実行の開始
 *
 * このシェルは以下のオプションをもつ:
 *
 *	-v		+verbose と同じ動作をする。
 *	
 *	+stdin device	標準入力デバイスの指定
 *	+stdout device	標準出力デバイスの指定
 *	+stderr device	標準エラー出力デバイスの指定
 *	+verbose	-v と同じ動作をする。
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
   * シェルプログラムの初期化
   */
  init_shell (ac, av, ev);

  /*
   * このシェルは、基本的に プロンプト表示 - 行入力 - 実行 のループか
   * らなりたっている。
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
   * コマンドオプションの解析
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
   * シグナルハンドラの指定
   */

  

}
