/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * 文字列の比較関数群
 * (各文字は1バイトと仮定している版)
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/string/strcmp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Header";


/* $Log: strcmp.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:52  night
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.1  1995/03/18  14:21:12  night
 * 最初の登録
 *
 *
 */

#include <types.h>
#include <string.h>


/*
 * 文字列の比較 
 *
 * 引数 s1 と s2 を比較する。
 * どちらかの文字列の内容が異なるまで比較する。
 *
 * 返り値：
 *	0	文字列の内容が等しい
 *	>0	s1 の文字列の方が大きい
 *	<0	s2 の文字列の方が大きい
 *
 */
int
strcmp (char *s1, char *s2)
{
  while (*s1 == *s2)
    {
      s1++;
      s2++;
    }

  return (*s1 - *s2);
}

/*
 * 文字列の比較 
 *
 * 引数 s1 と s2 を比較する。
 * どちらかの文字列の内容が異なるあるいは引数 n で指定した
 * 長さまで等しいか比較する。
 *
 * 返り値：
 *	0	文字列の内容が等しい
 *		あるいは、指定した長さまで比較した。
 *	>0	s1 の文字列の方が大きい
 *	<0	s2 の文字列の方が大きい
 *
 */
int
strncmp (char *s1, char *s2, int n)
{
  int	i;

  for (i = 0; (*s1 == *s2) && (i < n); i++)
    {
      s1++;
      s2++;
    }

  if (i == n)
    return (0);

  return (*s1 - *s2);
}



