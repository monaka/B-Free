/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* log.c - �������Ѥδؿ�
 *
 *
 *
 * $Log: log.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  1999-03-24 03:54:45  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.7  1997/10/24 13:56:58  night
 * ɬ�פ� return ʸ���ɲá�
 *
 * Revision 1.6  1997/08/31 13:30:09  night
 * ʸ������ϻ��ˤĤ��Ƥ���������ν��Ϥ���������
 * (�����ν��ϤȤޤ���路������)
 *
 * Revision 1.5  1997/05/08 15:11:29  night
 * �ץ����ξ�������ꤹ�뵡ǽ���ɲá�
 * (syscall misc �� proc_set_info ���ޥ��)
 *
 * Revision 1.4  1997/04/28 15:28:04  night
 * ���󥯥���ȥ������η׻��� sizeof(type *) �Ȥ�äƤ����Ȥ����
 * sizeof(type) ���ѹ�������
 *
 * Revision 1.3  1997/04/24 15:40:30  night
 * mountroot �����ƥॳ����μ�����Ԥä���
 *
 * Revision 1.2  1996/11/11  13:36:54  night
 * Assert �¹Ի���ɽ��������ѹ�
 *
 * Revision 1.1  1996/11/05  15:13:46  night
 * �ǽ����Ͽ
 *
 */

#include "posix.h"


static ID	log_port;
static ID	dev_recv;

extern W	put_string (B *line, ID port);
extern W 	putc (int ch, ID port);

static ER	vprintk (B *fmt, VP arg0);




/* init_log - �����ϵ�������������
 *
 *
 */
void
init_log (void)
{
  if (find_port (CONSOLE_DRIVER, &log_port) != E_PORT_OK)
    {
      dbg_printf ("POSIX: Cannot open console device.\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

  dev_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (dev_recv <= 0)
    {
      dbg_printf ("POSIX: Cannot allocate port\n");
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
printk (B *fmt,...)
{
  VP arg0;
  ER err;

  arg0 = (VP)&fmt;
  INC (arg0, B *);
  err = vprintk (fmt, (VP)arg0);
  return (err);
}

static ER
vprintk (B *fmt, VP arg0)
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

  return (E_OK);
}



W
put_string (B *line, ID port)
{
  W i;

  for (i = 0; line[i] != '\0'; i++)
    {
      putc (line[i], port);
    }
  return (i);
}


W 
putc (int ch, ID port)
{
  DDEV_REQ		req;		/* �׵�ѥ��å� */
  DDEV_RES		res;		/* �����ѥ��å� */
  W			rsize;
  ER			error;
  
  
  req.header.mbfid = dev_recv;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0xAA;
  req.body.wri_req.size = 1;
  req.body.wri_req.dt[0] = (char)(ch & 0xff);
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      dbg_printf ("cannot send packet. %d\n", error);
      return (EP_IO);
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, dev_recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      dbg_printf ("%d\n", res.body.wri_res.errcd);
      return (EP_IO);
    }      
  return (EP_OK);
}




/* _assert - ASSERT �ޥ���ˤ�äƸƤӽФ����ؿ�
 *
 * ��å���������Ϥ����ץ�����λ���롣
 *
 */ 
void
_assert (B *msg)
{
  printk ("ASSERT: ");
  printk ("%s\n", msg);
  for (;;)
    {
      slp_tsk ();
    }
}


