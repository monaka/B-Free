/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT main routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/printf.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: printf.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:43  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:29:51  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:26  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:30  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/printf.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "location.h"
#include "memory.h"
#include "console.h"
#include "macros.h"


static void	write_string ();
static void	write_digit ();
static void	write_digit2 ();
static void	write_jis (char *s, int width);
static void	write_tron (char *s, int width);


#define INC(x,type)	(((type *)x) += 1)

void
printf (char *fmt, ...)
{
  int	i;
  void	**sp = (void **)&fmt;
  int	width;

  INC(sp, char*);
  for (i = 0; fmt[i] != '\0'; i++)
    {
      if (fmt[i] == '%')
	{
	  width = MAXINT;
	  i++;
	  if (ISDIGIT (fmt[i]))
	    {
	      width = 0;
	      while (ISDIGIT (fmt[i]))
		{
		  width = (fmt[i] - '0') + (width * 10);
		  i++;
		}
	    }
	  switch (fmt[i])
	    {
	    case 's':
	      write_string ((char *)*sp, width);
	      INC (sp, char*);
	      break;
	    case 'd':
	      write_digit ((int)*sp, 10, width);
	      INC (sp, int);
	      break;
	    case 'x':
	      write_digit ((int)*sp, 16, width);
	      INC (sp, int);
	      break;
	    case 'c':
	      putchar ((int)*sp);
	      INC (sp, int);
	      break;
	    case 'j':
	      write_jis ((char *)*sp, width);
	      INC (sp, char *);
	      break;
	    case 't':
	      write_tron ((char *)*sp, width);
	      INC (sp, char *);
	      break;
	    }
	}
      else
	{
	  switch (fmt[i])
	    {
	    default:
	      putchar (fmt[i]);
	      break;
	    case '\n':
	      write_cr ();
	      break;
	    case '\t':
	      write_tab ();
	      break;
	    }
	}
    }
}

/***********************************************************************
 *
 */
static void
write_string (char *s, int width)
{
  int	i;
  
  for (i = 0; (*s != '\0') && (i < width); i++)
    {
      switch (*s)
	{
	default:
	  putchar (*s);
	  break;
	case '\n':
	  write_cr ();
	  break;
	case '\t':
	  write_tab ();
	  break;
	}
      s++;
    }
}

/***********************************************************************
 *
 */
static void
write_digit (int n, int base, int width)
{
  if (n == 0)
    {
      putchar ('0');
    }
  else if ((n < 0) && (base == 10))
    {
      putchar ('-');
      write_digit2 (-n, base, width);
    }
  else
    {
      write_digit2 (n, base, width);
    }
}

static void
write_digit2 (unsigned int n, int base, int width)
{
  int	ch;

  if (--width < 0) return;

  if (n > 0)
    {
      write_digit2 (n / base, base, width);
      ch = n % base;
      if ((ch >= 0) && (ch <= 9))
	ch += '0';
      else
	ch = (ch - 10) + 'a';
      putchar (ch);
    }
}


/************************************************************************
 *
 *
 */
static void
write_jis (char *s, int width)
{
  static unsigned char buf[100];
  unsigned char	*p;
  
  bcopy (s, buf, strnlen (s, width) + 1);
  buf[strnlen (s, width)] = '\0';
  string_jis_to_shift (buf);
  for (p = buf; *p != '\0'; p++)
    {
      putchar (*p);
    }
}

/************************************************************************
 *
 *
 */
static void
write_tron (char *s, int width)
{
  static unsigned char	buf[100];
  unsigned char	*p;
  int	i;
  unsigned char	c;
  int	n;
  
  bzero (buf, TABLE_SIZE (buf));
  if (strnlen (s, width) == width)
    {
      bcopy (s, buf, width);
    }
  else
    {
      bcopy (s, buf, strlen (s) + 1);
    }
  for (i = 0; i < strnlen (buf, width); i += 2)
    {
      c = buf[i];
      buf[i] = buf[i + 1];
      buf[i + 1] = c;
/*      printf ("write_tron: 0x%x%x\n", buf[i], buf[i + 1]); */
    }
  string_jis_to_shift (buf);
  for (p = buf; *p != '\0'; p++)
    {
      putchar (*p);
    }
}
