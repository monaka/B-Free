/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


/* 
 * $Log: message.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1997-07-06 11:55:08  night
 * putc() の処理を変更。
 *
 * Revision 1.2  1997/06/29 15:44:32  night
 * コメントの変更
 *
 * ・漢字コードを SJIS から EUC に変更した。
 * ・RCS の Log マクロ入りのコメントを追加した(まだ入れていなかったファイルのみ)。
 *
 * Revision 1.1  1997/06/29 13:18:13  night
 * BTRON メモリ manager の最初の登録。
 *
 */


#include "memory_manager.h"


static ID	log_port;
static ID	dev_recv;

extern W	put_string (B *line, ID port);
extern W 	putc (int ch, ID port);

static ER	vprintf (B *fmt, VP arg0);




/* init_log - ログ出力機構を初期化する
 *
 *
 */
void
init_log (void)
{
  if (find_port (CONSOLE_DRIVER, &log_port) != E_PORT_OK)
    {
      dbg_printf ("MM: Cannot open console device.\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

  dev_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (dev_recv <= 0)
    {
      dbg_printf ("MM: Cannot allocate port\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }
}


void
print_digit (UW d, UW base)
{
  static B digit_table[] = "0123456789ABCDEF";

  if (d < base)
    {
      putc ((W)(digit_table[d]), log_port);
    }
  else
    {
      print_digit (d / base, base);
      putc ((W)(digit_table[d % base]), log_port);
    }
}

#define INC(p,x)	(((W)p) = (((W)p) + sizeof (x)))

/*
 *
 */
W
printf (B *fmt,...)
{
  VP arg0;
  ER err;

  arg0 = (VP)&fmt;
  INC (arg0, B *);
  err = vprintf (fmt, (VP)arg0);
  return (err);
}


static ER
vprintf (B *fmt, VP arg0)
{
  VP *ap;

  for (ap = (VP *)arg0; *fmt != '\0'; fmt++)
    {
      if ((*fmt) == '%')
	{
	  ++fmt;
	  switch (*fmt)
	    {
	    case 's':
	      put_string (*(B **)ap, log_port);
	      INC (ap, B *);
	      break;

	    case 'd':
	      if ((W)*ap < 0)
		{
		  ((W)*ap) = -((W)*ap);
		  putc ('-', log_port);
		}
	      print_digit ((W)*ap, 10);
	      INC (ap, W);
	      break;

	    case 'x':
	      print_digit ((UW)*ap, 16);
	      INC (ap, W);
	      break;

	    default:
	      putc ('%', log_port);
	      break;
	    }
	}
      else
	{
	  putc (*fmt, log_port);
	}
    }
}



W
put_string (B *line, ID port)
{
  W i;

  putc ('"', port);
  for (i = 0; line[i] != '\0'; i++)
    {
      putc (line[i], port);
    }
  putc ('"', port);
  return (i);
}


W 
putc (int ch, ID port)
{
  static DDEV_REQ	req;		/* 要求パケット */
  static DDEV_RES	res;		/* 返答パケット */
  W			rsize;
  ER			error;
  W			i;
  
  
  req.header.mbfid = dev_recv;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0x00;
  req.body.wri_req.size = 1;
  req.body.wri_req.dt[0] = (char)(ch & 0xff);
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      dbg_printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, dev_recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      dbg_printf ("%d\n", res.body.wri_res.errcd);
      return (0);
    }      
  return (1);
}




