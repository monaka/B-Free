/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/string.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/string.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: string.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-02-07 18:23:58  kishida0
 * add tolower toupper functions
 *
 * Revision 1.3  2000/01/15 15:21:54  naniwa
 * changeg to EUC code
 *
 * Revision 1.2  1999/12/18 15:53:08  kishida0
 * 関数追加 strnlen
 *
 * Revision 1.1  1999/04/18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.1  1996/07/22 23:52:05  night
 * 最初の登録
 *
 * Revision 1.7  1995/12/05 15:13:16  night
 * strncmp() の変更。
 * 引数で指定した文字列の最後まで比較を行ったら、無条件で 0 を返すように
 * した。
 * 以前は、最後に比較した文字の次の文字の比較した結果を返していた。そのた
 * め、正しくない値になる場合がある。
 *
 * Revision 1.6  1995/10/01  13:07:10  night
 * strcmp() の処理の変更。
 * (ループの終了条件が間違っていた)
 *
 * Revision 1.5  1995/09/21  15:51:42  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.4  1995/02/26  14:14:20  night
 * strlen() 関数の宣言を追加。
 *
 * Revision 1.3  1995/01/14  14:20:57  night
 * strcpy(), strncpy() を作成した。
 *
 * Revision 1.2  1995/01/14  14:13:51  night
 * ヘッダファイル <types.h> のインクルードの指定を追加。
 *
 * Revision 1.1  1995/01/14  13:59:46  night
 * strcmp(), strncmp() を作成。
 *
 *
 */

/*
 * 文字列操作関数
 * これらの関数は、周辺核 (サーバ) で使用する。
 * 通常のアプリケーションでは、libc.a で宣言した関数を使う。
 *
 * このファイルで宣言する関数は以下のとおり。
 *
 *	strcat		文字列を結合 (concat) する。
 *	strchr		文字列の中で指定した文字が入っている位置をみつける。
 *	strcmp		文字列を比較する。
 *	strncmp		文字列を比較する(最大長の指定つき)
 *	strcpy		文字列のコピー
 *	strncpy		文字列のコピー(最大長の指定つき)
 *	strlen		文字列の長さをバイト単位で返す
 *      strnlen         文字列の長さをバイト単位で返す(制限値付き)
 *
 */


/*
 * ヘッダファイルの読み込み
 * ヘッダファイルのパスは、Makefile に指定してある。
 */
#include <types.h>


/*
 * 2 つの文字列を比較する。
 * 文字列が等しい場合には、 0 を返す。
 * 等しくない場合には、以下の規則にとおりの値を返す。
 *
 * (s1) > (s2) : 0 以下の値を返す。
 * (s1) < (s2) : 0 以上の値を返す。
 *
 */
W
strcmp (B *s1, B *s2)
{
  while (*s1 && (*s1 == *s2))
    {
      s1++;
      s2++;
    }

  /*
   * 返り値は、第 2 引数から第 1 引数の最後の文字の値を引くことによっ
   * て得る。もし、両方との等しい文字ならば、0 が返る。
   */
  return (*s2 - *s1);
}


/*
 * 2 つの文字列を比較する。
 * 文字列が等しい場合には、 0 を返す。
 * 等しくない場合には、以下の規則にとおりの値を返す。
 *
 * (s1) > (s2) : 0 以下の値を返す。
 * (s1) < (s2) : 0 以上の値を返す。
 *
 * この関数では文字列の最大長を指定する。
 */
W
strncmp (B *s1, B *s2, W length)
{
  W	counter;

  /*
   * 文字列を一文字ずつ比較していく。
   * 文字列の最大長まで比較するか、等しくない文字が出た時点でループは
   * 終了する。 
   */
  for (counter = 0;
       (*s1 == *s2) && (counter < length) && (*s1 != '\0');
       counter++)
    {
      s1++;
      s2++;
    }

  if (counter >= length)
    return (0);

  /*
   * 返り値は、第 2 引数から第 1 引数の最後の文字の値を引くことによっ
   * て得る。もし、両方との等しい文字ならば、0 が返る。
   */
  return (*s2 - *s1);
}


/*
 * 文字列のコピー。
 *
 * 第2引数から第1引数へ文字列をコピーする。
 * 文字列の終りは、文字の値が 0 かどうかで判別する。
 */
void
strcpy (B *s1, B *s2)
{
  while (*s2 != '\0')
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
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
void
strncpy (B *s1, B *s2, W size)
{
  W	counter;		/* コピーするバイト数をカウントするた */
				/* めの変数                           */

  for (counter = 0; (counter < (size - 1)) && (*s2); counter++)
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
}


/*
 * 文字列の長さをバイト単位で返す関数。
 *
 */
int
strlen (char *s)
{
  int	count;

  for (count = 0; s[count]; count++)
    ;
  return (count);
}
/*
 * 文字列の長さをバイト単位で返す関数(制限値付き)
 *
 */
int strnlen(char * s, int count)
{
  char *sc;

  for (sc = s; count-- && *sc != '\0'; ++sc)
    ;
  return sc - s;
}

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


