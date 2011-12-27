/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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

  /* とりあえず、サポートしていないというエラーで返す
   */
  put_response (req, EP_NOSUP, 0, 0, 0);

  return (FAIL);
}  
