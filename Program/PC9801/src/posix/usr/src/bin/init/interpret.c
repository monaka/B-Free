/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/interpret.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/interpret.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "init.h"

#ifdef nodef

/*
 * 指定したファイルの内容を見て実行する。
 *
 * この関数が理解するファイルの内容は次のようになっている。
 *
 * command argument
 *
 * 単にコマンドと引数の並びにすぎない。
 *
 */
interpret (char *fname)
{
  int	fd;
  char	cmd[MAXCMD];
  char	args[MAXARGS][MAXARGLEN];
  char	line[MAXLINE];

  fd = ropen (fname);
  if (fd < 0)
    {
      printf ("init: cannot open file (%s)\n");
      stop ();
    }

  /* 
   * ファイルの内容を一行ずつ読み取り、
   * 実行していく.
   */
  while (read_line (fd, line, sizeof (line)) >= 0)
    {
      if (line[0] != '#')	/* 註釈以外の行のみ解釈する */
	{
	  parser (line, &cmd, args);
	  exec_program (cmd, args);
	  wait ();
	}
    }
}

#endif /* notdef */
