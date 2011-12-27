/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * 文字列のコピー関数群
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strcpy.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strcpy.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: strcpy.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-17 14:29:03  naniwa
 * minor fix on log comment
 *
 * Revision 1.2  2000/01/15 15:26:32  naniwa
 * minor fix
 *
 * Revision 1.1  1996/11/11 13:33:42  night
 * 最初の登録
 *
 * Revision 1.2  1995/09/21  15:52:53  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/03/18  14:21:13  night
 * 最初の登録
 *
 *
 */

/*
 * 文字列のコピー
 *
 */
char *
strcpy (char *s1, char *s2)
     /* s1	コピー先のバッファを指すポインタ */
     /* s2	コピー元のバッファを指すポインタ */
{
  char *s;
  s = s1;
  while (*s2 != '\0')
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
  return (s);
}

/*
 * 文字列のコピー。
 *
 * 第2引数から第1引数へ文字列をコピーする。
 * 文字列の終りは、文字の値が 0 かどうかで判別する。
 *
 * この関数は、コピーする文字列の最大長を指定する。もし、指定した最大
 * 長よりも長い文字列をコピーしようとした場合には、最大長 - 1 でコピー
 * を中止する。
 * 最大長は、バイト単位で指定する。
 *
 */
char *
strncpy (char *s1, char *s2, int size)
{
  int	counter;		/* コピーするバイト数をカウントするた */
				/* めの変数                           */
  char *s;

  s = s1;
  for (counter = 0; (counter < (size - 1)) && (*s2); counter++)
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
  return(s);
}
