/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * ドライバー等でも使用する場合、テンポラリ変数を使ったマクロで作成すると
 * 問題になるので、関数にて定義した
 *
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/ctype.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: ctype.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-02-17 14:29:00  naniwa
 * minor fix on log comment
 *
 * Revision 1.1  2000/02/07 18:14:20  kishida0
 * マクロでは問題になりそうな tolower toupper などを作成した
 *
 *
 *
 */

/*
 *
 */
char
tolower (char c)
{
  if(c>='A'){
    if(c<='Z'){
      return c-('A'-'a');
    }
  }
  return c;
}

char
toupper(char c)
{
  if(c>='a'){
    if(c<='z'){
      return c-('a'-'A');
    } 
  }
  return c;
}

