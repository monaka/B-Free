/*

B-Free Project §Œ¿∏¿Æ ™§œ GNU Generic PUBLIC LICENSE §ÀΩæ§§§ﬁ§π°£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: lib.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $
 *
 * $Log: lib.c,v $
 * Revision 1.2  2011/12/30 00:57:06  liu1
 * „Ç≥„É≥„Éë„Ç§„É´„Ç®„É©„Éº„ÅÆ‰øÆÊ≠£„ÄÇ
 *
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.9  1999-03-31 07:17:30  monaka
 * Added cast for bcopy().
 *
 * Revision 1.8  1999/03/15 01:35:26  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.7  1998/11/20 08:02:33  monaka
 * *** empty log message ***
 *
 * Revision 1.6  1998/06/11 15:42:33  night
 * bzero §Œ•–•∞§ŒΩ§¿µ°£
 * (ŒŒ∞Ë§Ú•π•§°º•◊§π§Î•›•§•Û•ø§Ú•§•Û•Ø•Í•·•Û•»§∑§∆§§§ §´§√§ø)
 *
 * Revision 1.5  1996/11/13 16:41:02  night
 * strcat, strncat §Œ•–•∞Ω§¿µ°£
 * •≥•‘°º∏µ§Œ∞˙øÙ§Œ∆‚Õ∆§Ú•≥•‘°º§∑§∆§§§ §§°£
 *
 * Revision 1.4  1996/11/13  16:28:14  night
 * strcat, strncat §Œ TPTR §Œ∑øªÿƒÍ§Ú BPTR §À —ππ§∑§ø°£
 *
 * Revision 1.3  1996/05/17  14:21:18  night
 * •´°º•Ω•Î∞Ã√÷§ÚªÿƒÍ§π§Îª˛°¢•´°º•Ω•Î∞Ã√÷§Úº®§π√Õ§Ú 2 «‹§À§∑§∆§§§øΩËÕ˝§Ú∫ÔΩ¸§∑§ø°£
 *
 * Revision 1.2  1996/05/11  15:50:47  night
 * •´°º•Ω•Î∞Ã√÷§Œ¿ﬂƒÍ ˝À°§Ú IBM-PC Õ—§À —ππ°£
 *
 * Revision 1.1  1996/05/11  10:45:02  night
 * 2nd boot (IBM-PC »« B-FREE OS) §Œ•Ω°º•π°£
 *
 *
 */


static char	rcsid[] = "$Id: lib.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $";

#include "types.h"
#include "location.h"
#include "memory.h"
#include "console.h"
#include "macros.h"
#include "misc.h"
#include "vram.h"
#include "asm.h"
#include "lib.h"

static void	write_string ();
static void	write_digit ();
static void	write_digit2 ();

#define  _AUPBND                (sizeof (int) - 1)
#define  _ADNBND                (sizeof (int) - 1)

#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define INC(ap,T)           	((T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
/* (((t *)x) += 1) */

void
boot_printf (char *fmt, ...)
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
write_digit2 (ULONG n, int base, int width)
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



/*  ∏ª˙ŒÛ¥ÿ∑∏§Œ•’•°•§•Î */

/*************************************************************************
 * strlen
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
WORD
strlen (BPTR s)
{
  int	i;

  for (i = 0; *s != '\0'; i++, s++)
    ;
  return (i);
}

/*************************************************************************
 * strnlen ---
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
WORD
strnlen (BPTR s, WORD len)
{
  WORD	i;

  for (i = 0; (*s != '\0') && (i < len); i++, s++)
    ;
  return (i);
}

/*************************************************************************
 * strcpy --- 
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
WORD
strcpy (BPTR s1, BPTR s2)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      *s1++ = *s2++;
    }
  *s1 = *s2;
  return (i);
}

/*************************************************************************
 * strncpy --- 
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
WORD
strncpy (BPTR s1, BPTR s2, WORD n)
{
  WORD i;
  
  for (i = 0; *s2; i++)
    {
      if (i > n)
	{
/*	  *s1 = '\0';*/
	  return (i);
	}
      *s1++ = *s2++;
    }
/*  *s1 = *s2;*/
  return (i);
}

/*************************************************************************
 * strncpy_with_key --- 
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
WORD
strncpy_with_key (BPTR s1, BPTR s2, WORD n, int key)
{
  WORD i;
  
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

/***********************************************************************
 * strcat -- £≤§ƒ§Œ ∏ª˙ŒÛ§Œœ¢∑Î
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
BPTR
strcat (BPTR s1, BPTR s2)
{
  TPTR ret_val = s1;
  
  while (*s1)
    s1++;

  while (*s2)
    {
      *s1++ = *s2++;
    }
  *s1++ = *s2++;
  return ret_val;
}

/***********************************************************************
 * strncat -- £≤§ƒ§Œ ∏ª˙ŒÛ§Œœ¢∑Î
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
BPTR
strncat (BPTR s1, BPTR s2, WORD n)
{
  TPTR	ret_val = s1;
  WORD	i;
  
  while (*s1)
    s1++;

  for (i = 0; *s2; i++)
    {
      if (i >= n)
	break;
      *s1++ = *s2++;
    }
  *s1++ = *s2++;
  return ret_val;
}

/***********************************************************************
 * strcmp --- £≤§ƒ§Œ ∏ª˙ŒÛ§Œ»Ê≥”
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
ULONG
strcmp (BPTR s1, BPTR s2)
{
  while (*s1 && *s2)
    {
      if (*s1 != *s2)
        {
          return (*s2 - *s1);
        }
      s1++;
      s2++;
    }
  return (*s1 - *s2);
}

/***********************************************************************
 * strchr --  ∏ª˙ŒÛ§Œ√Ê§ÀªÿƒÍ§∑§ø ∏ª˙§¨¥ﬁ§ﬁ§Ï§∆§§§ §§§´§Úƒ¥§Ÿ§Î°£
 *
 *
 */
char *
strchr (char *s, int ch)
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

/***********************************************************************
 * strnchr --  ∏ª˙ŒÛ§Œ√Ê§ÀªÿƒÍ§∑§ø ∏ª˙§¨¥ﬁ§ﬁ§Ï§∆§§§ §§§´§Úƒ¥§Ÿ§Î°£
 *
 *
 */
char *
strnchr (char *s, int ch, int n)
{
  int	i;

  for (i = 0; *s != '\0'; i++)
    {
      if (i < n)
	break;
      if (ch == *s)
	{
	  return (s);
	}
      s++;
    }
  return (NULL);
}


/***********************************************************************
 * strncmp -- £≤§ƒ§Œ ∏ª˙ŒÛ§Œ»Ê≥”
 *
 * ∞˙øÙ°ß
 *
 *  ÷√Õ°ß
 *
 * ΩËÕ˝°ß
 *
 */
ULONG
strncmp (BPTR s1, BPTR s2, WORD n)
{
  WORD	i;

  for (i = 0; (i < n) && *s1 && *s2; i++)
    {
      if (*s1 != *s2)
        return (*s2 - *s1);
      s1++;
      s2++;
    }
  if (i == n)
    return 0;
  return (*s1 - *s2);
}

int
toupper (int ch)
{
  if ((ch >= 'a') && (ch <= 'z'))
    {
      ch = ch + ('A' - 'a');
    }
  return (ch);
}


/* VRAM ¥ÿ∑∏§Œ¡‡∫Ó */

/***********************************************************************
 * 
 */
void
write_vram (int x, int y, int ch, int attr)
{
  short	*addr;
  
  addr = (short *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
/*  addr [CURSOR_POS (x, y) * 2 + 1] = attr;
 */
}

/**************************************************************************
 * set_cursor_pos --- 
 */
void
set_cursor_pos (int x, int y)
{
  int	addr;

  lock ();
  addr = CURSOR_POS (x, y);
/*  addr *= 2; */

  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  outb (GDC_DATA, addr & 0xff);
  unlock ();
}

void
scroll_up ()
{
  UWORD16 *addr;
  int i;
  
  addr = (UWORD16 *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE - 80; i++)
    {
      addr[i] = addr[i + 80];
    }
  for (i = 0; i <= MAX_WIDTH; i++)
    {
      write_vram (i, MAX_HEIGHT, ' ', 0xe1);
    }
}


/*  ∏ª˙ŒÛ -> øÙ√Õ§ÿ§Œ —¥π¥ÿøÙ */

#define isdigit(c,base)	((c >= '0') && (c <= '9'))


/**************************************************************************
 *
 */
int
atoi (char *s)
{
  int result;
  result = string_to_number (s, 10);

  return result;
}

/***************************************************************************
 *
 */
int
string_to_number (char *s, int base)
{
  int	total = 0;
  

  while (*s != '\0')
    {
      if (!isdigit (*s, base))
	return (0);
      total = (total * base) + (*s - '0');
      s++;
    }
  return (total);
}


/* bcopy ¥ÿ∑∏§Œ¥ÿøÙ */


/*************************************************************************
 * bcopy --- copy string.
 */
void
bcopy (const char *src, char *dest, int length)
{
  while (length-- > 0)
    {
      *dest++ = *src++;
    }
}

/************************************************************************
 * bzero
 */
void
bzero (char *src, int length)
{
  while (length-- > 0)
    {
      *src++ = 0;
    }
}

