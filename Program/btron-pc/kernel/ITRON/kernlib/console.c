/*

B-Free Project ¤ÎÀ¸À®Êª¤Ï GNU Generic PUBLIC LICENSE ¤Ë½¾¤¤¤Þ¤¹¡£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/console.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/console.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: console.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-12-18 15:55:01  kishida0
 * $B%^%$%J!<$J=$@5(B
 *
 * Revision 1.1  1999/04/18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.1  1996/07/22 23:52:05  night
 * ºÇ½é¤ÎÅÐÏ¿
 *
 *
 */

#include <types.h>
#include <itron.h>
#include <config.h>
#include <errno.h>
#include "device.h"
#include "port-manager.h"

#define CONSOLE_DRIVER	"driver.console"


/* ¥³¥ó¥½¡¼¥ë¥Ç¥Ð¥¤¥¹¥É¥é¥¤¥Ð¤ËÂÐ¤·¤Æ¥á¥Ã¥»¡¼¥¸¤ò½ÐÎÏ¤¹¤ë¤¿¤á¤Î´Ø¿ô·² 
 *
 */

/* ----------------------------------------------------------- */
/*                  ¥Õ¥¡¥¤¥ëÆâ static ÊÑ¿ô                     */
/* ----------------------------------------------------------- */
 
static ID	console;
static ID	recv;


/* open_console
 *
 * ¥³¥ó¥½¡¼¥ë¥Ç¥Ð¥¤¥¹¥É¥é¥¤¥Ð¤ò¥ª¡¼¥×¥ó¤¹¤ë¡£
 *
 * °ú¿ô		¤Ê¤·
 *
 * ÊÖ¤êÃÍ	¥¨¥é¡¼ÈÖ¹æ
 *
 */
ER
open_console (void)
{
  recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recv <= 0)
    {
      return (E_SYS);
    }

  if (find_port (CONSOLE_DRIVER, &console) != E_PORT_OK)
    {
      return (E_SYS);
    }
  
  return (E_OK);
}


/* write_console
 *
 * ¥³¥ó¥½¡¼¥ë¤Ø¤ÎÊ¸»ú½ÐÎÏ
 *
 *
 */
ER
write_console (B *buf, W length)
{
  DDEV_REQ		req;		/* Í×µá¥Ñ¥±¥Ã¥È */
  DDEV_RES		res;		/* ÊÖÅú¥Ñ¥±¥Ã¥È */
  W			rsize;
  ER			error;
  W			i;
  
  req.header.mbfid = recv;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.size = length;
  bcopy (buf, req.body.wri_req.dt, length);
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      return (error);
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (res.body.wri_res.errcd);
    }      
  return (E_OK);
}
