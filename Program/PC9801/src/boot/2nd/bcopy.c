/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 * bcopy.c
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/bcopy.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: bcopy.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:34  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:28:35  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:13  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:31  btron
 * BTRON SYSTEM 1.0
 * 
 *
 */

#include "types.h"


/*************************************************************************
 * bcopy --- copy string.
 */
void
bcopy (char *src, char *dest, int length)
{
  while (length-- > 0)
    {
      *dest++ = *src++;
    }
}

/************************************************************************
 * bzero
 */
void
bzero (char *src, int length)
{
  while (length-- > 0)
    {
      *src = 0;
    }
}
