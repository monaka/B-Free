/*

B-Free Project ¤ÎÀ¸À®Êª¤Ï GNU Generic PUBLIC LICENSE ¤Ë½¾¤¤¤Þ¤¹¡£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT memory allocate/deallocate routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/memory.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $
 *
 * $Log: memory.c,v $
 * Revision 1.2  2011/12/30 00:57:06  liu1
 * ã‚³ãƒ³ãƒ‘ã‚¤ãƒ«ã‚¨ãƒ©ãƒ¼ã®ä¿®æ­£ã€‚
 *
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  1999-03-15 01:35:30  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.5  1998/11/20 08:02:36  monaka
 * *** empty log message ***
 *
 * Revision 1.4  1996/08/12 19:34:42  night
 * ÊªÍý¥á¥â¥ê¥µ¥¤¥º¤ò¥Á¥§¥Ã¥¯¤¹¤ë¤È¤­¤ÎºÇÂç¥µ¥¤¥º¤ò 16MB ¤«¤é 256 MB ¤ËÊÑ¹¹¤·¤¿¡£
 *
 * Revision 1.3  1996/07/24  14:02:13  night
 * Í¾Ê¬¤Ê print Ê¸¤òºï½ü¡£
 *
 * Revision 1.2  1996/07/22  13:35:08  night
 * A20 ¤ò¥¤¥Í¡¼¥Ö¥ë¤Ë¤¹¤ë½èÍý¤òÄÉ²Ã¡£
 *
 * Revision 1.1  1996/05/11  10:45:05  night
 * 2nd boot (IBM-PC ÈÇ B-FREE OS) ¤Î¥½¡¼¥¹¡£
 *
 * Revision 1.5  1995/09/21  15:50:41  night
 * ¥½¡¼¥¹¥Õ¥¡¥¤¥ë¤ÎÀèÆ¬¤Ë Copyright notice ¾ðÊó¤òÄÉ²Ã¡£
 *
 * Revision 1.4  1995/09/20  15:32:20  night
 * malloc ÍÑ¤Î¥á¥â¥ê¤ò 640K - 100K Ê¬¤ËÊÑ¹¹¡£
 *
 * Revision 1.3  1995/06/28  14:11:07  night
 * ¥á¥â¥ê¥¢¥í¥±¡¼¥È¤Î´ÉÍýÎÎ°è¤ò (640K - 100K) ¡Á 640K ¤«¤é (2M - 100K) ¡Á
 * 2M ¤ËÊÑ¹¹¤·¤¿¡£
 *
 * Revision 1.2  1995/06/26  15:06:12  night
 * malloc ´Ø¿ô¤ÎÄÉ²Ã¡£
 *
 * Revision 1.1  1993/10/11  21:29:33  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:24  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:27  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/memory.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $";

#include "types.h"
#include "location.h"
#include "config.h"
#include "memory.h"
#include "asm.h"

struct alloc_entry
{
  struct alloc_entry	*next;
  int size;
  BYTE body[0];
};

static struct alloc_entry	*alloc_reg;


extern int	end;

static void	init_malloc (void *last, int size);
void		*last_addr;
UWORD32	base_mem, ext_mem, real_mem;

#define TRUE_SIZE(size)	(size + sizeof (struct alloc_entry))

static void
enable_A20 ()
{
  for (;;)
    {
      if ((inb (0x64) & 0x02) == 0)
	break;
    }
  outb (0x64, 0xD1);
  for (;;)
    {
      if ((inb (0x64) & 0x02) == 0)
	break;
    }
  outb (0x60, 0xdf);
}


/**************************************************************************
 * init_memory
 *
 *
 */
void
init_memory (void)
{
  volatile int	*p;

  last_addr = (void *)&end;
#ifdef	PC9801
  outb (0x00f2, 0);	/* 1M °Ê¾å¤ÎÎÎ°è¤ò»ÈÍÑ¤Ç¤­¤ë¤è¤¦¤Ë¤¹¤ë¡£*/
#elif IBMPC
  /* IBMPC (¸ß´¹µ¡) ¤Ç A20 ¤ò¥¤¥Í¡¼¥Ö¥ë¤Ë¤¹¤ë¡£
   * 0xD1 -> out (0x64)
   * 0xDF -> out (0x60)
   */
  enable_A20 ();
#endif
  for (p = (int *)0x100000; (int)p < 0xf000000; p = (int *)((int)p + 0x100000))
    {
      *p = 0;
      *p = 0xAA;
      if (*p != 0xAA)
	break;
    }
  boot_printf ("Extended Memory = %d K bytes\n", ((int)p - 0x100000) / 1000);
  boot_printf ("USE Memory      = %d bytes\n", last_addr);

  ext_mem = ((int)p - 0x100000);
  base_mem = BASE_MEM;
  real_mem = ext_mem + BASE_MEM;

#ifdef nodef
  /* malloc µ¡¹½¤Î½é´ü²½ */
  init_malloc ((void *)(2 * 1024 * 1024), MALLOC_SIZE);
#endif
  /* 640K ¥Ð¥¤¥È¤«¤é²¼°Ì 100 K ¥Ð¥¤¥È¤ÎÎÎ°è¤ò malloc ÍÑ¤Ë»ÈÍÑ¤¹¤ë */
  init_malloc ((void *)(640 * 1024), MALLOC_SIZE);
}

/*
 * boot ¤Ç¥á¥â¥ê¤ò»ÈÍÑ¤¹¤ëµ¡¹½¤Î½é´ü²½¡£
 *
 * Âè£±°ú¿ô¤Ç»ØÄê¤·¤¿¥á¥â¥ê¤ÎºÇ¸å¤«¤é¡¢Âè£²°ú¿ô¤Ç»ØÄê¤·¤¿Ê¬¤À¤±¤ò 
 * malloc ¤Ç»ÈÍÑ¤¹¤ë¡£
 *
 */
static void
init_malloc (void *last, int size)
{
  alloc_reg = (struct alloc_entry *)(last - size);
  alloc_reg->size = size;
  alloc_reg->next = alloc_reg;
#ifdef nodef
  boot_printf ("init_malloc: last = 0x%x\n", last);
  boot_printf ("init_malloc: alloc_reg = 0x%x\n", alloc_reg);
  boot_printf ("init_malloc: alloc_reg->size = %d\n", alloc_reg->size);
  boot_printf ("init_malloc: alloc_reg->next = 0x%x\n", alloc_reg->next);
#endif
}

/*
 * malloc --- »ØÄê¤·¤¿¥µ¥¤¥º¤Î¥á¥â¥ê¤ò¼èÆÀ¤¹¤ë
 *
 * alloc_reg ¤Ë¤Ï¡¢¥¢¥í¥±¡¼¥È¤Ç¤­¤ë¥Õ¥ê¡¼¥á¥â¥ê¤Î¥¨¥ó¥È¥ê¤¬¤Ä¤Ê¤¬¤Ã¤Æ
 * ¤¤¤ë¡£¤³¤ì¤é¤ÎÃæ¤«¤éºÇ½é¤Ë¥¢¥í¥±¡¼¥È¤Ç¤­¤ë¥µ¥¤¥º¤ò¤â¤Ã¤¿¤â¤Î¤ò¼è¤ê
 * ¤À¤¹¡£
 */
void *
malloc (int size)
{
  int			true_size;	/* ´ÉÍýÍÑÎÎ°è¤â´Þ¤ó¤À"¿¿¤Î"¥¢¥í¥±¡¼¥È */
					/* ¥µ¥¤¥º */
  struct alloc_entry	*p, *prev;
  struct alloc_entry	*alloced;


  if (alloc_reg == NULL)	/* ¼èÆÀ¤¹¤Ù¤­¥Õ¥ê¡¼¥á¥â¥ê¤¬¤Ê¤¤ */
    {
      return (NULL);
    }

  true_size = TRUE_SIZE (size);

/*  boot_printf ("alloc_reg = 0x%x, alloc_reg->next = 0x%x\n", alloc_reg, */
/*  alloc_reg->next); */

  for (prev = alloc_reg, p = alloc_reg->next;
       p->size < true_size;
       prev = p, p = p->next)
    {
      if (p == alloc_reg)	/* ¼èÆÀ¤Ç¤­¤ë¥¨¥ó¥È¥ê¤¬¤Ê¤«¤Ã¤¿ */
	{
	  return (NULL);
	}
    }

  if (p->size == true_size)	/* ÃúÅÙ¤Î¥µ¥¤¥º¤À¤Ã¤¿ */
    {
      if (p->next == p)	/* ¤Ä¤Ê¤¬¤Ã¤Æ¤¤¤ë¥¨¥ó¥È¥ê¤Ï¤Ò¤È¤Ä¤À¤±¤À¤Ã¤¿ */
	{
	  alloc_reg = NULL;
	  p->next = NULL;
	}
      else
	{
	  /* ¥Õ¥ê¡¼¥ê¥¹¥È¤«¤é p ¤Ç»ØÄê¤µ¤ì¤Æ¤¤¤ë¥¨¥ó¥È¥ê¤ò³°¤¹ */
	  prev->next = p->next;
	}
      
      return ((void *)(p->body));	/* ¼èÆÀ¤·¤¿¥á¥â¥ê¤Ø¤Î¥Ý¥¤¥ó¥¿ */
					/* ¤òÊÖ¤¹¡£¼èÆÀ¤·¤¿ÎÎ°è¤Ï¡¢ */
					/* alloc_reg ¹½Â¤ÂÎ¤Î body Í× */
					/* ÁÇ¤Ç¤¢¤ë¤³¤È¤ËÃí°Õ */
    }

  alloced = (struct alloc_entry *)(((char *)p) + (p->size - true_size));
  p->size -= true_size;
  alloced->size = true_size;
  return (alloced->body);
}

/*
 * °ú¿ô¤ËÅÏ¤µ¤ì¤¿¥Ý¥¤¥ó¥¿¤¬»Ø¤¹ÎÎ°è¤ò¥Õ¥ê¡¼¥ê¥¹¥È¤ËÆþ¤ì¤ë¡£
 * ¤â¤·¡¢ÎÎ°è¤¬°ÊÁ°¥Õ¥ê¡¼¥ê¥¹¥È¤Ë¤Ê¤¤¤â¤Î¤À¤Ã¤¿¤é¡¢¤Ê¤Ë¤â¤·¤Ê¤¤¡£
 */
void
free (void *ptr)
{
  struct alloc_entry	*current, *prev;
  struct alloc_entry	*new_entry;

  new_entry = (struct alloc_entry *)((char *)ptr - sizeof (struct alloc_entry));

  /* ÁÞÆþ¤ò»ØÄê¤µ¤ì¤¿ÎÎ°è¤Ï malloc ¤Ç´ÉÍý¤·¤Æ¤¤¤ëÎÎ°è¤Ç¤Ï¤Ê¤¤. */
  if (((char *)new_entry < (char *)(void *)ext_mem 
                            + (1024 * 1024) - MALLOC_SIZE) 
      || ((char *)new_entry > (char *)(void *)ext_mem + (1024 * 1024)))
    {
      return;
    }

  /* ¥Õ¥ê¡¼¥ê¥¹¥È¤Ë¥¨¥ó¥È¥ê¤Ï¤Ò¤È¤Ä¤·¤«¤Ê¤¤ */
  if (alloc_reg == alloc_reg->next)
    {
      if (((char *)new_entry + new_entry->size) == (char *)alloc_reg)
	{
	  new_entry->size += alloc_reg->size;
	  new_entry->next = new_entry;
	  alloc_reg = new_entry;
	}
      else if (((char *)alloc_reg + alloc_reg->size) == (char *)new_entry)
	{
	  alloc_reg->size += new_entry->size;
	}
      else
	{
	  alloc_reg->next = new_entry;
	  new_entry->next = alloc_reg;
	}
      return;
    }

  /* ¥Õ¥ê¡¼¥ê¥¹¥È¤òÀèÆ¬¤«¤éÃ©¤ê¡¢¥ê¥¹¥È¤ËÁÞÆþ¤¹¤ë¥Ý¥¤¥ó¥È¤ò·è¤á¤ë */
  /* ¥Õ¥ê¡¼¥ê¥¹¥È¤Ï¥¢¥É¥ì¥¹½ç¤Ë¤Ê¤Ã¤Æ¤ª¤ê¡¢current ¤Î¥¢¥É¥ì¥¹¤¬ÄÉ²Ã¤¹ */
  /* ¤ëÎÎ°è¤ÎºÇ¸å¤è¤ê¤âÂç¤­¤¯¤Ê¤Ã¤¿¤é¡¢¤½¤ÎÁ°¤ËÁÞÆþ¤¹¤ë¡£*/
  for (prev = alloc_reg, current = alloc_reg->next;
       current != alloc_reg;
       prev = current, current = current->next)
    {
      /* current ¤ËÎÙÀÜ¤·¤Æ¤¤¤ë¡£current ¤ÈÎÎ°è¤ò¤Ä¤Ê¤²¤ë */
      if ((char *)current == ((char *)new_entry + new_entry->size))
	{
	  new_entry->size += current->size;
	  new_entry->next = current->next;
	  current = new_entry;
	  prev->next = current;
	}
      else
	{
	  /* current ¤Ë¤ÏÎÙÀÜ¤·¤Æ¤¤¤Ê¤¤¡£¥ê¥¹¥È¤ËÁÞÆþ¤¹¤ë */
	  new_entry->next = current;
	  prev->next = new_entry;
	}
      
      /* prev ¤ËÎÙÀÜ¤·¤Æ¤¤¤ë¾ì¹ç¡£prev ¤Î¥µ¥¤¥º¤òÁý¤ä¤¹¡£*/
      /* ¤³¤Î¤È¤­¡¢current ¤ÈÊ»¹ç¤·¤Æ¤¤¤è¤¦¤¬¡¢Ã±¤ËÁÞÆþ¤·¤Æ¤¤¤è¤¦¤¬¡¢ */
      /* ¤É¤Á¤é¤Ë¤·¤Æ¤â prev->next ¤Ï¡¢new_entry¤ò»Ø¤·¤Æ¤¤¤ë¤³¤È¤ËÃí°Õ */
      if ((char *)new_entry == ((char *)prev + prev->size))
	{
	  prev->size += new_entry->size;
	  prev->next = new_entry->next;
	}
    }
}
