/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: noaction.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-03-24 03:54:53  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/*

B-Free Project $B$N@8@.J*$O(B GNU Generic PUBLIC LICENSE $B$K=>$$$^$9!#(B

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: noaction.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-03-24 03:54:53  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

W
psc_noaction_f (struct posix_request *req)
{
  unsigned char	*p;
  W	i;

  p = (B *)0x80291093;
  for (i = 0; i < 10; i++)
    {
      printk ("0x%x ", p[i]);
    }

  /* $B$H$j$"$($:!"%5%]!<%H$7$F$$$J$$$H$$$&%(%i!<$GJV$9(B
   */
  put_response (req, EP_NOSUP, 0, 0, 0);

  return (FAIL);
}  
