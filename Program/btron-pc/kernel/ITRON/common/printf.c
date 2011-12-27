/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* printk.c --- printk 関係の関数
 *
 */

#include "itron.h"
#include "func.h"


#define INC(p,x)	(((W)p) = (((W)p) + sizeof (x *)))


static void	print_string (B *);
static void	print_digit (UW, UW);
static void	print_digit0 (UW, UW);

/*
 *
 */
W
printk (B *fmt,...)
{
  VP arg0;

  arg0 = (VP)&fmt;
  INC (arg0, B *);
  return (vprintk (fmt, (VP)arg0));
}


/*
 *
 */
W
vprintk (B *fmt, VP arg0)
{
  VP *ap;

  for (ap = (VP *)arg0; *fmt != '\0'; fmt++)
    {
      if (*fmt == '%')
	{
	  switch (*++fmt)
	    {
	    case 's':
	      print_string ((B*)(*ap));
	      INC (ap, B *);
	      break;

	    case 'd':
	      if ((W)*ap < 0)
		{
		  ((W)*ap) = -((W)*ap);
		}
	      print_digit ((W)*ap, 10);
	      INC (ap, W);
	      break;

	    case 'x':
	      print_digit ((UW)*ap, 16);
	      INC (ap, W);
	      break;

	    default:
	      putchar ('%');
	      break;
	    }
	}
      else
	{
	  putchar (*fmt);
	}
    }
  return 0; /* dummy */
}

static void
print_string (B *s)
{
  while (*s != '\0')
    {
      putchar (*s);
      s++;
    }
}

static void
print_digit (UW d, UW base)
{
  print_digit0 (d, base);
}

static void
print_digit0 (UW d, UW base)
{
  static B digit_table[] = "0123456789ABCDEF";

  if (d < base)
    {
      putchar (digit_table[d]);
    }
  else
    {
      print_digit0 (d / base, base);
      putchar (digit_table[d % base]);
    }
}
