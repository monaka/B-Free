/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.1  1995/03/18  14:15:13  night
 * 最初の登録
 *
 *
 */

#include "sh.h"


static int	internal_chdir (int ac, char **av);
static int	internal_echo (int ac, char **av);


/* 
 * POSIX 環境用のシェルコマンド
 * (内部コマンドの宣言を行う)
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
   * cd コマンドに引数が何も指定されていない場合には、ホームディレクトリに
   * 移動する。
   */
  if (ac <= 1)
    {
      homedir = getenv ("HOME");
      if (homedir == NULL)
	{
	  /* /etc/passwd ファイルから 該当するユーザのエントリを取得し、  */
	  /* ホームディレクトリ名を得る。 */
	  
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
      /* 1つ以上引数がある場合には、最初の引数に従って chdir システム */
      /* コールを実行する。 */
      if (chdir (av[1]) < 0)
	{
	  perror ("cd");
	}
    }
  return (errno);
}

/*
 * 引数の内容をそのまま出力する。
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
