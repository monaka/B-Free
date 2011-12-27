/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
/* ファイル中の文字コードを SJIS から EUC に変更。
/*
/* Revision 1.1  1998/01/07 15:31:36  night
/* 最初の登録
/*
 *
 */


/*
 * POSIX 環境で最初に立ち上がるプログラム init。
 * 
 */

#include "init.h"



main (int ac, char **av)
{
  int	runlavel;

  if (ac <= 1)
    {
      /* runlevel はデフォルト */
      runlevel = DEFAULT_RUNLEVEL;		/* 0 */
    }
  else
    {
      /* 引数で指定した runlevel を実行する */
      runlevel = atoi (av[1]);
    }

  go_runlevel (runlevel);	/* runlevel を変更 */
      
}


go_runlevel (int runlevel)
{
  int	inittab;

  inittab = open (INITTAB, O_RDONLY);
  if (inittab < 0)
    {
      /* inittab ファイルがオープンできなかった - */
      /* システムを停止する */
    }

  while (read_line (inittab, line) > 0)
    {
      if (parse_runlevel (line) == runlevel)
	{
	  /* 読み込んだ行の runlevel は引数で指定された runlevel と等 */
	  /* しい -> その行で指定してあるコマンドを実行 */
	  exec_command (line);
	}
    }

  close (inittab);
}
