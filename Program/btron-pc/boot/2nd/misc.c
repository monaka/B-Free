/*

B-Free Project ¤ÎÀ¸À®Êª¤Ï GNU Generic PUBLIC LICENSE ¤Ë½¾¤¤¤Þ¤¹¡£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT misc routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/misc.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $
 *
 * $Log: misc.c,v $
 * Revision 1.2  2011/12/30 00:57:06  liu1
 * ã‚³ãƒ³ãƒ‘ã‚¤ãƒ«ã‚¨ãƒ©ãƒ¼ã®ä¿®æ­£ã€‚
 *
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-01-30 18:50:12  kishida0
 * use same keyboard define file & you can use BS key
 *
 * Revision 1.2  1999/03/31 07:57:07  monaka
 * Minor fixes.
 *
 * Revision 1.1  1996/05/11 10:45:06  night
 * 2nd boot (IBM-PC ÈÇ B-FREE OS) ¤Î¥½¡¼¥¹¡£
 *
 * 
 * ------------------------------------------------------------------------
 * 
 * Revision 1.3  1995/09/21  15:50:41  night
 * ¥½¡¼¥¹¥Õ¥¡¥¤¥ë¤ÎÀèÆ¬¤Ë Copyright notice ¾ðÊó¤òÄÉ²Ã¡£
 *
 * Revision 1.2  1994/07/30  17:37:18  night
 * ¥Õ¥¡¥¤¥ëÃæ¤ÎÆüËÜ¸ìÊ¸»ú¤ò¤¹¤Ù¤Æ EUC ¥³¡¼¥É¤ËÊÑ¹¹¡£
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
 * ¿§¡¹¤Ê´Ø¿ô¤Î½¸¹ç
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/misc.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $";

#include "console.h"
#include "keyboard.h"
#include "types.h"
#include "misc.h"

#include "keycode.h"


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
int getch(void)
{
  int ch;
  ch = read_keyboard ();
  return ch;
}

/***************************************************************************
 *
 */
char *
gets (char *line)
{
  int	ch;
  int   x,y;
  char	*p;
  int   len;
  
  p = line;
  len = 0;
  ch = getch ();
  while (ch != '\n')
    {
      get_coursor(&x,&y);
      if(ch == DEL){
        if(len>0){
          set_coursor(x-1,y);
          putchar(' ');
          set_coursor(x-1,y);
          p--;
          len--;
        }
      }else{
        *p++ = ch;
        len++;
        putchar(ch);
      }
      ch = getch ();
    }
  *p = '\0';
  putchar('\n');
  return (line);
}

void
__stack_chk_fail(void)
{
  boot_printf("stack over flow\n");
  for(;;) {
    ;
  }
}
