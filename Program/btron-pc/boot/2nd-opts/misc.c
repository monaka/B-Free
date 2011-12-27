/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT misc routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.1  1996/05/11 10:45:06  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * 
 * ------------------------------------------------------------------------
 * 
 * Revision 1.3  1995/09/21  15:50:41  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1994/07/30  17:37:18  night
 * ファイル中の日本語文字をすべて EUC コードに変更。
 *
 * Revision 1.1  1993/10/11  21:29:38  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:25  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:23  btron
 * BTRON SYSTEM 1.0
 * 
 *
 * 色々な関数の集合
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "types.h"
#include "misc.h"

/***************************************************************************
 *
 */
int
getchar (void)
{
  int	ch;
  
  ch = read_keyboard ();
  putchar (ch);
  return (ch);
}

/***************************************************************************
 *
 */
char *
gets (char *line)
{
  int	ch;
  char	*p;
  
  p = line;
  ch = getchar ();
  while (ch != '\n')
    {
      *p++ = ch;
      ch = getchar ();
    }
  *p = '\0';
  return (line);
}

