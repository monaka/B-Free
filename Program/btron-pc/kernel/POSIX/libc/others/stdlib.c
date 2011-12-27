/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: stdlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: stdlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: stdlib.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  2000-06-04 04:10:15  naniwa
 * to support octal
 *
 * Revision 1.5  2000/01/29 16:23:52  naniwa
 * to clean up
 *
 * Revision 1.4  2000/01/18 14:39:25  naniwa
 * to make independent of libkernel.a
 *
 * Revision 1.3  2000/01/15 15:25:16  naniwa
 * to use read/write systemcalls
 *
 * Revision 1.2  1999/11/10 10:44:37  naniwa
 * correction
 *
 * Revision 1.5  1999/05/10 02:57:17  naniwa
 * add fprintf() and modified to user vfprintf()
 *
 * Revision 1.4  1998/12/08 05:38:39  monaka
 * I init has been included the Mitten Scripting Language since this release.
 * But, it's just a first preview. So you can't use many convenience
 * features. Mitten works extension scripting environment like Tcl, Guile,
 * Windows Scripting Host, and so on. If you wished, you may connect your
 * function written in C to Mitten operator. If you wished, you may call
 * Mitten from C.
 *
 * Revision 1.3  1997/07/06 11:50:02  night
 * �ǥХå�ʸ�ΰ������꤬�ޤ����äƤ����Τǡ�����������
 *
 * Revision 1.2  1997/04/24 15:38:06  night
 * printf() �ؿ��ν����� kernlib �� dbg_printf() ��Ʊ�ͤΤ�Τˤ�����
 *
 * Revision 1.1  1996/07/25  16:03:44  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 *
 */

#include "stdlib.h"

static ER	vfprintf (FILE *port, B *fmt, VP arg0);


static void
print_digit (FILE *port, UW d, UW base)
{
  static B digit_table[] = "0123456789ABCDEF";

  if (d < base)
    {
      putc (digit_table[d], port);
    }
  else
    {
      print_digit (port, d / base, base);
      putc (digit_table[d % base], port);
    }
}

#define INC(p,x)	(((W)p) = (((W)p) + sizeof (x *)))

/*
 *
 */
W
printf (B *fmt,...)
{
  VP *arg0;
  ER err;

  arg0 = (VP *)&fmt;
  INC (arg0, B *);
  err = vfprintf (stdout, fmt, (VP)arg0);
  fflush (stdout);
  return (err);
}

W
fprintf (FILE *port, B *fmt,...)
{
  VP *arg0;
  ER err;

  arg0 = (VP *)&fmt;
  INC (arg0, B *);
  err = vfprintf (port, fmt, (VP)arg0);
  fflush (port);
  return (err);
}

static ER
vfprintf (FILE *port, B *fmt, VP arg0)
{
  VP *ap;

  for (ap = (VP *)arg0; *fmt != '\0'; fmt++)
    {
      if (*fmt == '%')
	{
	  switch (*++fmt)
	    {
	    case 's':
	      fputs ((B*)(*ap), port);
	      INC (ap, B *);
	      break;

	    case 'd':
	      if ((W)*ap < 0)
		{
		  ((W)*ap) = 0 - ((W)*ap);
		  putc ('-', port);
		}
	      print_digit (port, (W)*ap, 10);
	      INC (ap, W);
	      break;

	    case 'x':
	      print_digit (port, (W)*ap, 16);
	      INC (ap, W);
	      break;

	    case 'o':
	      print_digit (port, (W)*ap, 8);
	      INC (ap, W);
	      break;

	    default:
	      putc ('%', port);
	      break;
	    }
	}
      else
	{
	  putc (*fmt, port);
	}
    }
}


W
putc (W ch, FILE *port)
{
  port->buf[port->count] = ch;
  port->count++;

  if (ch == '\n')
    {
      writechar (port->device, port->buf, port->count);
      port->count = 0;
    }
  else if (port->count >= port->bufsize)
    {
      writechar (port->device, port->buf, port->count);
      port->count = 0;
    }

  return (ch);
}

W
__putc (W ch, FILE *port)
{
  UB	buf[1];

  buf[0] = ch;
  writechar (port->device, buf, 1);
  return (ch);
}


fflush (FILE *port)
{
  writechar (port->device, port->buf, port->count);
  port->count = 0;
}

W
fputs (B *line, FILE *port)
{
  W i;

  for (i = 0; line[i] != '\0'; i++)
    {
      putc (line[i], port);
    }
  fflush (port);
  return (i);
}


W 
writechar (ID port, UB *buf, W length)
{
#ifdef notdef
  DDEV_REQ		req;		/* �׵�ѥ��å� */
  DDEV_RES		res;		/* �����ѥ��å� */
  W			rsize;
  ER			error;
  W			i;
  
  req.header.mbfid = dev_recv;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0xAA;
  req.body.wri_req.size = length;
  bcopy (buf, req.body.wri_req.dt, length);
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
#ifdef DEBUG
      dbg_printf ("cannot send packet. %d(%s, %d)\n", error, __FILE__, __LINE__);
#endif
      return;
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, dev_recv);
  if (res.body.wri_res.errcd != E_OK)
    {
#ifdef DEBUG
      dbg_printf ("%d\n", res.body.wri_res.errcd);
#endif
      return (0);
    }      
  return (1);
#else
  write(port, buf, length);
#endif
}


W
isprint (W ch)
{
  if ((ch < 0x20) || (ch >= 0x7f))
    {
      return (FALSE);
    }
  return (TRUE);
}

W
isspace (W ch)
{
  if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
    return (TRUE);
  return (FALSE);
}

W
isnum (W ch)
{
  ch -= '0';

  if (ch < 0 || ch > 9)
    {
      return FALSE;
    }

  return TRUE;
}

W
getc (FILE *port)
{
  W ch;

  ch = readchar (port->device);
  __putc (ch, stdout);
  return (ch);
}


W
readchar (ID port)
{
#ifdef notdef
  DDEV_REQ		req;		/* �׵�ѥ��å� */
  DDEV_RES		res;		/* �����ѥ��å� */
  W			rsize;
  ER			error;
  W			i;
  
  req.header.mbfid = dev_recv;
  req.header.msgtyp = DEV_REA;
  req.body.rea_req.dd = 0xAA;
  req.body.rea_req.size = 1;
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, dev_recv);
  if (res.body.rea_res.dt[0] != 0)
    {
      return (res.body.rea_res.dt[0]);
    }
  return (0);
#else
  B buf[1];
  read(port, buf, 1);
  return(buf[0]);
#endif
}
