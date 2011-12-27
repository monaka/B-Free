/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * 文字列の連結関数群
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strcat.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Header";


/* 
 * $Log: strcat.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-02-17 14:29:01  naniwa
 * minor fix on log comment
 *
 * Revision 1.3  2000/02/01 08:11:22  naniwa
 * not to include malloc
 *
 * Revision 1.2  2000/01/15 15:26:30  naniwa
 * minor fix
 *
 * Revision 1.1  1996/11/11 13:33:42  night
 * 最初の登録
 *
 * Revision 1.3  1995/09/21  15:52:52  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/07/06  15:05:20  night
 * strncat の関数名が strcat になっていたので、正しく直した。
 *
 * Revision 1.1  1995/03/18  14:21:12  night
 * 最初の登録
 *
 *
 */

/*
 * 2 つの文字列を連結するための関数
 *
 */
#if 0
char *
strcat (char *s1, char *s2)
{
  char *buf;
  char *bufp;	/* バッファの先頭をおぼえておくためのポインタ */

  buf = malloc (strlne (s1) + strlen (s2) + 1);
  if (buf == NULL)
    {
      return (NULL);	/* 失敗した */
    }

  bufp = buf;
  while (*s1)
    {
      *buf++ = *s1++;
    }

  while (*s2)
    {
      *buf++ = *s2++;
    }
  *buf = '\0';
  return (bufp);
}
#endif

char *strcat(char *dest, char *src)
{
  char *tmp = dest;

  while (*tmp) tmp++;
  while ((*tmp++ = *src++) != '\0') ;
  return(dest);
}



/*
 * 2 つの文字列を連結するための関数
 *
 *
 */
#if 0
char *
strncat (char *s1, char *s2, int n)
{
  int	i;
  int	length;
  char	*buf;
  char	*bufp;	/* バッファの先頭をおぼえておくためのポインタ */

  length = strlen (s1) + strlen (s2) + 1;
  if (length > n)
    length = n - 1;
  
  buf = malloc (length);
  if (buf == NULL)
    {
      return (NULL);	/* 失敗した */
    }

  bufp = buf;
  for (i = 0; (*s1) && (i < length); i++)
    {
      *buf++ = *s1++;
    }

  for (; (*s2) && (i < length); i++)
    {
      *buf++ = *s2++;
    }
  *buf = '\0';
  return (bufp);
}
#endif

char *strncat(char *dest, char *src, int count)
{
  char *tmp = dest;

  if (count) {
    while (*tmp) tmp++;
    while ((*tmp++ = *src++)) {
      if (--count == 0) {
	*tmp = '\0';
	break;
      }
    }
  }
  return(dest);
}
