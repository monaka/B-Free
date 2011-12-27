/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: lib.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "$Id: lib.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: lib.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-12-05 14:33:48  night
 * 擬似標準入出力関数の追加。
 *
 * Revision 1.1  1995/10/10  16:21:47  night
 * 最初の登録.
 * システム起動後、プロンプトを出力してコマンド入力 - 実行ができるところ
 * まで出来た。ただし、コマンドは echo だけ。
 *
 *
 */

#include "init.h"



/* Stub routine */
__main ()
{
}


/*
 */
start ()
{
  main (0, NULL);
}


void
print_digit (UW d, UW base)
{
  static B digit_table[] = "0123456789ABCDEF";

  if (d < base)
    {
      putc (digit_table[d], stdout);
    }
  else
    {
      print_digit (d / base, base);
      putc (digit_table[d % base], stdout);
    }
}

W
printf (B *fmt,...)
{
  void	*bp;

  bp = &fmt;
  bp += sizeof (B *);

  while (*fmt)
    {
      if (*fmt == '%')
	{
	  switch (*++fmt)
	    {
	    case '\0':
	      goto out;
	      break;

	    case 's':
	      fputs (*(B **)bp, stdout);
	      bp += sizeof (B *);
	      break;

	    case 'c':
	      putc (*(B *)bp, stdout);
	      bp += sizeof (W);
	      break;

	    case 'd':
	      if (*(W *)bp < 0)
		{
		  (*(W*)bp) = -(*(W*)bp);
		}
	      print_digit (*(W*)bp, 10);
	      bp += sizeof (W);
	      break;

	    case '%':
	      putc ('%', stdout);
	      break;
	    }
	}
      else
	{
	  putc (*fmt, stdout);
	}
      fmt++;
    }

out:;
  fflush (stdout);
  return ((W)fmt);
}


W
putc (W ch, FILE *port)
{
  port->buf[port->count] = ch;
  port->count++;

  if (ch == '\n')
    {
      writechar (port->device, dev_recv, port->buf, port->count);
      port->count = 0;
    }
  else if (port->count >= port->bufsize)
    {
      writechar (port->device, dev_recv, port->buf, port->count);
      port->count = 0;
    }

  return (ch);
}

W
__putc (W ch, FILE *port)
{
  UB	buf[1];

  buf[0] = ch;
  writechar (port->device, dev_recv, buf, 1);
  return (ch);
}


fflush (FILE *port)
{
  writechar (port->device, dev_recv, port->buf, port->count);
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
  return (i);
}


W 
writechar (ID port, ID resport, UB *buf, W length)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  W			i;
  
  req.header.mbfid = resport;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0xAA;
  req.body.wri_req.size = length;
  bcopy (buf, req.body.wri_req.dt, length);
  error = snd_mbf (port, sizeof (req), &req);
  if (error != E_OK)
    {
      dbg_printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, resport);
  if (res.body.wri_res.errcd != E_OK)
    {
      dbg_printf ("%d\n", res.body.wri_res.errcd);
      return (0);
    }      
  return (1);
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
getc (FILE *port)
{
  W ch;

  ch = readchar (port->device, dev_recv);
  __putc (ch, stdout);
  return (ch);
}


W
readchar (ID port, ID resport)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  W			i;
  
  req.header.mbfid = resport;
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
  rcv_mbf (&res, (INT *)&rsize, resport);
  if (res.body.rea_res.dt[0] != 0)
    {
      return (res.body.rea_res.dt[0]);
    }
  return (0);
}




void *
malloc (W size)
{
}


void *
realloc (void *buf, W size)
{
}


void
free (void *buf)
{
}



B *
strchr (B *s, W ch)
{
  while (*s != '\0')
    {
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}

B *
strnchr (B *s, W ch, W size)
{
  W	i;

  for (i = 0; *s != '\0'; i++)
    {
      if (i < size)
	break;
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}

W
strncpy_with_key (B *s1, B *s2, W n, W key)
{
  W	i;
  
  for (i = 0; *s2; i++)
    {
      if ((i >= n) || (*s2 == key))
	{
	  *s1 = '\0';
	  return (i);
	}
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}
