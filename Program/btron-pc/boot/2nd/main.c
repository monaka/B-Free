/*

B-Free Project ¤ÎÀ¸À®Êª¤Ï GNU Generic PUBLIC LICENSE ¤Ë½¾¤¤¤Þ¤¹¡£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT main routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/main.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $
 *
 *
 * $Log: main.c,v $
 * Revision 1.2  2011/12/30 00:57:06  liu1
 * ã‚³ãƒ³ãƒ‘ã‚¤ãƒ«ã‚¨ãƒ©ãƒ¼ã®ä¿®æ­£ã€‚
 *
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.28  2000-02-27 03:18:11  monaka
 * Graphics functions are deleted.
 *
 * Revision 1.27  2000/01/30 18:50:10  kishida0
 * use same keyboard define file & you can use BS key
 *
 * Revision 1.26  2000/01/24 23:31:08  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.25  2000/01/23 15:43:23  kishida0
 * boot»þ¤ËmoduleÌ¾¤òÉ½¼¨¤¹¤ëÍÍ¤Ë¤·¤¿
 *
 * Revision 1.24  1999/05/04 12:52:07  naniwa
 * modified to speed up boot from FD
 *
 * Revision 1.23  1999/03/31 07:57:05  monaka
 * Minor fixes.
 *
 * Revision 1.22  1999/03/31 07:17:32  monaka
 * Added cast for bcopy().
 *
 * Revision 1.21  1999/03/15 01:35:28  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.20  1999/02/28 22:13:27  night
 * ´Ø¿ô cat() ¤ÎÃæ¤Ç¡¢Ê¸»ú¤ò½ÐÎÏ¤¹¤ë¤È¤­¤Ë»ÈÍÑ¤·¤Æ¤¤¤ë boot_printf ¤Î»ØÄê»Ò¤¬
 * ´Ö°ã¤Ã¤Æ¤¤¤¿ (%2t) ¤Î¤Ç¡¢Àµ¤·¤¤ (%c) ¤ËÊÑ¹¹¤·¤¿¡£
 *
 * Revision 1.19  1998/11/20 08:02:34  monaka
 * *** empty log message ***
 *
 * Revision 1.18  1998/06/11 15:49:53  night
 * func_table Ãæ¤Î ide_init, ide_boot, ide_id, ide_read_dump, vga_test ´Ø
 * ¿ô¤ò¥­¥ã¥¹¥È¤¹¤ë¤è¤¦ÊÑ¹¹¤·¤¿¡£
 * ¤Þ¤¿¡¢sfsread¡¢mount¡¢dir¡¢cat ¤Î³Æ¥³¥Þ¥ó¥É¤òÍ­¸ú¤Ë¤·¤¿¡£
 *
 * Revision 1.17  1998/02/25 13:07:34  night
 * ide_read_dump ¥³¥Þ¥ó¥É¤ÎÄÉ²Ã¡£
 *
 * Revision 1.16  1997/08/31 14:25:47  night
 * BOOT »þ¤Î½èÍý¤ÎÊÑ¹¹¡£
 * (root ¥Õ¥¡¥¤¥ë¥·¥¹¥Æ¥à¤ò OS ¤Ë¶µ¤¨¤ë½èÍý¤ÎÄÉ²Ã)
 *
 * Revision 1.15  1997/06/29 13:38:39  night
 * ½¾Íè·¿¤Î OS ¤Î¥í¡¼¥É¤Î½èÍý¤òÍ­¸ú¤Ë¤·¤¿¡£
 *
 * Revision 1.14  1997/06/29 13:13:46  night
 * HD ¤Î SFS ¥Õ¥©¡¼¥Þ¥Ã¥È¤µ¤ì¤¿¥Ñ¡¼¥Æ¥£¥·¥ç¥ó¤«¤é¤Î BTRON OS ¤ò¥í¡¼¥É¤Ç¤­¤ë
 * µ¡Ç½¤òÄÉ²Ã¡£
 *
 * Revision 1.13  1997/04/24 15:30:34  night
 * Ãð¼á¤ÎÊÑ¹¹¡£
 *
 * Revision 1.12  1996/11/14  13:14:16  night
 * cat ¥³¥Þ¥ó¥É¤ÎÄÉ²Ã¡£
 *
 * Revision 1.11  1996/11/13  16:28:37  night
 * "sfsread", "mount", "dir" ¤Î¥³¥Þ¥ó¥É¤ÎÄÉ²Ã¡£
 *
 * Revision 1.10  1996/08/11  15:11:34  night
 * IDE HD ¤ÎÁàºî´Ø·¸¤Î´Ø¿ô¤ò ide.c ¤Ë°ÜÆ°¤·¤¿¡£
 * ideboot ¥³¥Þ¥ó¥É¤òÄÉ²Ã¤·¤¿¡£
 *
 * Revision 1.9  1996/07/31  19:27:38  night
 * IDE HD ÍÑ´Ø¿ô¤ò²þÂ¤¡£
 * IDE HD ¤ËÂÐ¤·¤Æ¥Ç¡¼¥¿ÆÉ¤ß¼è¤ê¤ò¹Ô¤¦¤È¤­¡¢¥Ç¡¼¥¿¤¬¥­¥ã¥Ã¥·¥å
 * ¤µ¤ì¤Æ¤¤¤ë¤È³ä¤ê¹þ¤ß¤òÂÔ¤ÄÆ°ºî¤ÎÁ°¤ËÆÉ¤ß¼è¤ê¤¬½ªÎ»¤·¤Æ¤·¤Þ¤¦¡£
 * ¤½¤Î·ë²Ì¡¢³ä¤ê¹þ¤ß¤òÂÔ¤ÁÂ³¤±¤Æ¤·¤Þ¤¦¡£
 * ¥Ç¡¼¥¿ÆÉ¤ß¼è¤ê¤ÎÍ×µá¤ò½Ð¤¹¤È¤­¤Ë¡¢³ä¤ê¹þ¤ß¤òÂÔ¤ÄÁ°¤Ë HD ¤ÎÆÉ¤ß
 * ¼è¤êÆ°ºî¤¬½ªÎ»¤·¤Æ¤¤¤ë¤«¤É¤¦¤«¤òÈ½Äê¤¹¤ë¤è¤¦¤Ë¤·¤¿¡£
 *
 * Revision 1.8  1996/07/30  18:23:54  night
 * IDE HD ÍÑ´Ø¿ôÎà¤òÄÉ²Ã¡£
 * ¤³¤ì¤é¤Î´Ø¿ô¤Ï°ì»þÅª¤Ê¤â¤Î¤Ê¤Î¤Ç¡¢¾­Íè¤Ï ide.c ¤È ide.h ¤Ê¤É¤ÎÊÌ¤Î¥Õ¥¡
 * ¥¤¥ë¤Ë°Ü¤¹É¬Í×¤¬¤¢¤ë¡£
 *
 * Revision 1.7  1996/07/25  16:59:56  night
 * OS ¤òÆÉ¤ß¹þ¤à¤È¤­¤Ë¡¢¥â¥¸¥å¡¼¥ë¤¬Æþ¤Ã¤Æ¤¤¤ëºÇ½é¤Î FD ¤Î¥Ö¥í¥Ã¥¯ÈÖ¹æ¤ò
 * É½¼¨¤·¤Æ¤¤¤¿ÉôÊ¬¤òºï½ü¤·¤¿¡£
 *
 * Revision 1.6  1996/07/24  13:59:49  night
 * ¥¤¥ó¥¿¡¼¥Ð¥ë¥¿¥¤¥Þ¤Î½é´ü²½¤Î½èÍý¤òºï½ü
 *
 * Revision 1.5  1996/07/23  17:05:06  night
 * ¥¤¥ó¥¿¡¼¥Ð¥ë¥¿¥¤¥ÞÍÑ¤Î³ä¤ê¹þ¤ß½èÍý¤òÄÉ²Ã¡£
 *
 * Revision 1.4  1996/07/22  13:34:47  night
 * CPU ¥ê¥»¥Ã¥È¤ò¥­¡¼¥Ü¡¼¥É¥³¥ó¥È¥í¡¼¥é¤ò²ð¤·¤Æ¹Ô¤¦¤è¤¦¤Ë½¤Àµ¡£
 * (°ÊÁ°¤Ï¡¢¥ê¥»¥Ã¥È¤¬¤Ç¤­¤Ê¤«¤Ã¤¿)
 *
 * Revision 1.3  1996/07/08  14:53:14  night
 * IBM-PC ¸ß´¹µ¡ÍÑ¤ÎÊÑ¹¹¡£
 *
 * Revision 1.2  1996/05/11  15:51:33  night
 * beep ´Ø¿ô¤ÎÃæ¿È¤òºï½ü
 *
 * Revision 1.1  1996/05/11  10:45:05  night
 * 2nd boot (IBM-PC ÈÇ B-FREE OS) ¤Î¥½¡¼¥¹¡£
 *
 *
 * 
 * ----------------------------------------------------------------------------------
 *
 * Revision 1.11  1996/01/06 15:57:52  night
 * SCSI ¤Î¾ðÊó¤ò SRAM ¤«¤é¼èÆÀ¤¹¤ë¤è¤¦¤Ë¤·¤¿¡£
 *
 * Revision 1.10  1995/09/21  15:50:40  night
 * ¥½¡¼¥¹¥Õ¥¡¥¤¥ë¤ÎÀèÆ¬¤Ë Copyright notice ¾ðÊó¤òÄÉ²Ã¡£
 *
 * Revision 1.9  1995/08/05  12:04:03  night
 * ¥í¡¼¥É»þ¤Î¥á¥Ã¥»¡¼¥¸¤ÎÊÑ¹¹¡£
 *
 * Revision 1.8  1995/06/26  15:05:48  night
 * DOS ´Ø·¸¤Î¥³¥Þ¥ó¥É (dosls/doscat) ¤òÄÉ²Ã¡£
 *
 * Revision 1.7  1994/11/20  10:39:37  night
 * 1) ¥í¡¼¥É»þ¤ËÍ¾Ê¬¤Ê busy loop ¤ò¹Ô¤Ã¤Æ¤¤¤¿¤Î¤òºï½ü¤·¤¿¡£
 * 2) Í¾Ê¬¤ÊÃð¼á¤òºï½ü¤·¤¿¡£
 * 3) ¥â¥¸¥å¡¼¥ë¥Õ¥¡¥¤¥ë¤¬¼Â¹Ô¥Õ¥¡¥¤¥ë¤Ç¤Ï¤Ê¤¤»þ¤Î return Ê¸¤òºï½ü¤·¤¿(¤½
 *    ¤ÎÁ°¤ÇÌµ¸Â¥ë¡¼¥×¤ò¼Â¹Ô¤·¤Æ¤¤¤ë¤Î¤Ç return Ê¸¤Ï¼Â¹Ô¤·¤Ê¤¤¤¿¤á)¡£
 *
 * Revision 1.6  1994/11/19  07:42:05  night
 * ¥â¥¸¥å¡¼¥ë¾ðÊó¤ÎÅÐÏ¿¤ò¹Ô¤¦½èÍý¤òÄÉ²Ã¡£
 * ¥Ö¡¼¥È»þ¤Î¥Ö¥í¥Ã¥¯ÈÖ¹æ¤Î·×»»¤òÊÑ¹¹¡£
 *   ¥â¥¸¥å¡¼¥ë¾ðÊó¤Î length / BLOCK_SIZE ¤Ç¤Ï¥Ö¥í¥Ã¥¯ÈÖ¹æ¤¬¤Ò¤È¤ÄÂç¤­¤«¤Ã
 *   ¤¿¡£¥Ö¥í¥Ã¥¯ÈÖ¹æ¤Î·×»»ÃÍ¤«¤é -1 ¤ò°ú¤¯¤è¤¦¤Ë¤·¤¿¡£
 *
 * Revision 1.5  1994/11/13  15:02:57  night
 * Ê£¿ô¥â¥¸¥å¡¼¥ë¤Î boot ¤Î¤¿¤á¤ÎÊÑ¹¹ [ µ¡Ç½ÄÉ²Ã ]
 *
 * Revision 1.4  1994/09/04  13:23:02  night
 * ¥ª¡¼¥×¥Ë¥ó¥°¥á¥Ã¥»¡¼¥¸¤ÎÊÑ¹¹ (Welcme -> Welcome)
 *
 * Revision 1.3  1994/07/30  17:37:18  night
 * ¥Õ¥¡¥¤¥ëÃæ¤ÎÆüËÜ¸ìÊ¸»ú¤ò¤¹¤Ù¤Æ EUC ¥³¡¼¥É¤ËÊÑ¹¹¡£
 *
 * Revision 1.2  1994/07/05  17:19:36  night
 * Change FD format; 512bytes/block.
 *
 * Revision 1.1  1993/10/11  21:29:30  btron
 * btron/386
 *
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/main.c,v 1.2 2011/12/30 00:57:06 liu1 Exp $";

#include "boot.h"
#include "lib.h"
#include "types.h"
#include "errno.h"
#include "config.h"
#include "location.h"
#include "memory.h"
#include "console.h"
#include "keyboard.h"
#include "date.h"
#include "strings.h"
#include "macros.h"
#include "dma.h"
#include "fd.h"
#include "file.h"
#include "vram.h"
#include "page.h"
#include "a.out.h"		/* for gcc header */
#include "interrupt.h"
#include "idt.h"
#include "ide.h"
#include "sfs.h"
#include "vga.h"
#include "asm.h"
#include "sfs.h"
#include "misc.h"

#include "main.h"


extern int evaluate(char* line);
int read_single_module (int start_block, void *paddr, struct module_info *info);

struct file_system	root_fs;
struct file		root;
struct file		cwd;


 
/******************************************************************************
 *
 *
 */
void
_main ()
{
  char	line[MAX_LINE];
  extern int setidt ();
  extern int int32_handler ();

  boot_printf ("--2nd boot main--\n");
  
  init_8259A ();
  init_idt ();
  init_console ();
  banner ();
  init_memory ();
  init_keyboard ();
  init_vm ();
  enable_page ();

  init_fd ();
  ide_init ();

  open_interrupt ();

#ifdef HDBOOT
#ifdef BOOT_USE_VGA_GRAPH
  vga_graph ();
#endif
  boot_hd (BOOTOS);
#else
  for (;;)
    {
      boot_printf ("boot> ");
      gets (line);
      if (strlen (line) > 0)
	{
	  evaluate (line);
	}
    }
#endif
/* DON'T REACHED */
}



/***************************************************************************
 * beep -- BEEP routine.
 */
int
beep ()
{

  return E_OK;
}


/***************************************************************************
 * status_memory --- ¥á¥â¥ê¾ðÊó¤ÎÉ½¼¨
 */
int
status_memory ()
{
  extern void	*last_addr;

#ifdef nodef
  volatile int	*p;
  
  for (p = (int *)0x100000; (int)p < 0xf00000; (int)p += 0x100000)
    {
      *p = 0;
      *p = 0xAA;
      if (*p != 0xAA)
	break;
    }
#endif /* nodef */
  boot_printf ("Extended Memory = %d K bytes\n", ext_mem / 1024);
  boot_printf ("USE Memory      = %d bytes\n", last_addr);

  return E_OK;
}

/***************************************************************************
 *
 */
void
print_binary (int n)
{
  ULONG mask = 0x00000080;
  int	i;

  for (i = 0; i < 8; i++)
    {
      if (mask & n)
	boot_printf ("1");
      else
	boot_printf ("0");
      mask = mask >> 1;
    }
}

  
void
banner (void)
{
  boot_printf ("BTRON/386 2nd BOOT Version %d.%d\n\n", MAJOR_VER, MINOR_VER);
  boot_printf ("RCS Version : %s\n", rcsid);
  boot_printf ("START AS 32 BIT MODE.\n");
  boot_printf ("Welcome BTRON/386.\n");
}

/**************************************************************************
 * panic.
 */
void
panic (char *s)
{
  boot_printf ("panic: %s\n", s);
  for (;;)
    ;
}





int
cat (char *buf, int size)
{
  int	i;

  for (i = 0; i < size; i += 2)
    {
      boot_printf ("%c", &buf[i]);
    }

  return E_OK;
}



int
multi_boot (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int silent)
{
  int		i;
  int		offset;
  struct boot_header	*info;
  void  	(*entry)();
  int		errno;

  if (!silent)
    boot_printf ("Multiple module boot.\n");
  info = (struct boot_header *)MODULE_TABLE;

  errno = sfs_read_file (fd, sb, ip, 0, BLOCK_SIZE, (char *)info);
  if (errno)
    {
      if (!silent)
	boot_printf ("Couldn't read OS file.\n");
      vga_text ();
      return (E_SYS);
    }

  info->machine.base_mem = base_mem;
  info->machine.ext_mem = ext_mem;
  info->machine.real_mem = real_mem;
  if (fd & 0x010000)
    {
      /* HD */
      info->machine.rootfs = 0x80010000 | ((fd & 0xff) + 1);
    }
  else
    {
      /* FD */
      info->machine.rootfs = 0x80000000 | (fd & 0xff);
    }

  if (!silent)
    boot_printf ("Module %d\n", info->count);
  offset = BLOCK_SIZE;	/* ºÇ½é¤Î¥â¥¸¥å¡¼¥ë¤¬Æþ¤Ã¤Æ¤¤¤ë¥ª¥Õ¥»¥Ã¥È(¥Ð¥¤¥È) */
  entry = (void (*)())(info->modules[0].entry);

  for (i = 0; i < info->count; i++)
    {
      if (!silent)
	{
	  boot_printf ("[%d] ", i);
	}

      if (load_module (fd, sb, ip, offset, &(info->modules[i]), silent) != E_OK)
	{
	  if (!silent)
	    {
	      boot_printf ("Can't load module(s)...abort.\n");
	    }
	  vga_text ();
	  return (E_SYS);
	}

#ifdef nodef
      offset += (info->modules[i].length -  BLOCK_SIZE);	/* ??? */
#else
      offset += info->modules[i].length + BLOCK_SIZE;	/* ??? */
#endif
    }
  if (!silent) {
    boot_printf ("exec_info->a_entry = 0x%x\n", entry); 
  }
  if (silent)
    {
      vga_text ();
    }

  (*entry)();

  return E_OK;
}


int
load_module (int 			fd,
	     struct sfs_superblock	*sb,
	     struct sfs_inode		*ip,
	     int 			offset,
	     struct module_info		*info, 
	     int			silent)
{
  char	tmp[BLOCK_SIZE];
  struct exec	*exec_info;

  sfs_read_file (fd, sb, ip, offset, sizeof (struct exec), tmp); 
  exec_info = (struct exec *)tmp;
  if (N_BADMAG (*exec_info))
    {
      if (!silent) {
	boot_printf ("This object is not exec format (%d).\n", *exec_info);
      }
      return (E_SYS);
    }
  
  if (!silent)
    {
      boot_printf ("[%s]\n",info->name);
      boot_printf ("Module: exec type = 0x%x, Text size = %d, Data size = %d\n",
	      N_MAGIC(*exec_info),
	      exec_info->a_text,
	      exec_info->a_data);
    }

  if ((N_MAGIC(*exec_info) == 0413) || (N_MAGIC(*exec_info) == NMAGIC))
    {
      if (sfs_read_file (fd, sb, ip, offset + BLOCK_SIZE, exec_info->a_text + exec_info->a_data, (char *)info->paddr))
	{
	  return E_IO;
	}
    }
  else
    {
      if (!silent)
	{
	  boot_printf ("I don't know how to read a.out image.(0x%x)\n", N_MAGIC(*exec_info));
	}
      return E_SYS;
    }
  return E_OK;
}



/*
   Ê£¿ô¤Î¥â¥¸¥å¡¼¥ë¤òÆÉ¤ß¹þ¤à¡£

   ºÇ½é¤Î¥â¥¸¥å¡¼¥ë¤Î¤ß ITRON ¥«¡¼¥Í¥ë¤È²¾Äê¤·¤Æ¤¤¤ë¡£
   ¤½¤Î¤¿¤á¡¢ÆÉ¤ß¹þ¤à°ÌÃÖ¤Ï¡¢0x00010000 ¤È·è¤á¤Æ¤¤¤ë¡£
   (ITRON ¥«¡¼¥Í¥ë¤Ï¡¢²¾ÁÛ¥¢¥É¥ì¥¹ 0x80010000 ¤ËÆÉ¤ß¹þ¤à¤³¤È¤Ë¤·¤Æ¤¤¤ë¡£
   0x00010000 ¤È¤¤¤¦¤Î¤Ï¡¢0x80010000 ¤ËÂÐ±þ¤·¤Æ¤¤¤ëÊªÍý¥¢¥É¥ì¥¹¤Ç¤¢¤ë)

   2 ÈÖÌÜ°Ê¹ß¤Î¥â¥¸¥å¡¼¥ë¤Ï¡¢ITRON ¥«¡¼¥Í¥ë¤Ë¤Î¸å¤ËÂ³¤±¤ÆÆÉ¤ß¹þ¤à¤³¤È¤Ë¤Ê¤ë¡£
   ¤½¤Î¤¿¤á¡¢ITRON ¥«¡¼¥Í¥ë¤ÎÂç¤­¤µ¤Ë¤è¤Ã¤ÆÊªÍý¥¢¥É¥ì¥¹¤ÏÊÑ¤ï¤ë¤³¤È¤Ë¤Ê¤ë¡£
   2 ÈÖÌÜ°Ê¹ß¤Î¥â¥¸¥å¡¼¥ë¤Ë¤Ä¤¤¤Æ¤Ï¡¢boot ¤Ï¥í¡¼¥É¤¹¤ë¤À¤±¤Ç¡¢²¾ÁÛ¥¢¥É¥ì¥¹
   ¤Ø¤Î¥Þ¥Ã¥Ô¥ó¥°¤Ê¤É¤Ï ITRON ¥«¡¼¥Í¥ë¤Îµ¯Æ°¸å¤Ë¹Ô¤¦¡£

*/ 
int
read_multi_module ()
{
  int	i;
  int	bn;
  struct boot_header	*info;
  void  (*entry)();

  boot_printf ("Multiple module boot.\n");
  info = (struct boot_header *)MODULE_TABLE;
  fd_read (0, 0, (BYTE *)info);
  info->machine.base_mem = base_mem;
  info->machine.ext_mem = ext_mem;
  info->machine.real_mem = real_mem;
  info->machine.rootfs = 0xffffffff;
  boot_printf ("Module %d\n", info->count);
  bn = 1;	/* ºÇ½é¤Î¥â¥¸¥å¡¼¥ë¤¬Æþ¤Ã¤Æ¤¤¤ë¥Ö¥í¥Ã¥¯ÈÖ¹æ */
  entry = (void (*)())(info->modules[0].entry);
  for (i = 0; i < info->count; i++)
    { 
      boot_printf ("[%d][%s]\n", i, info->modules[i].name);
      read_single_module (bn, (void *)info->modules[i].paddr, &(info->modules[i]));

#ifdef nodef
      bn += ((info->modules[i].length / BLOCK_SIZE) - 1);
#else
      bn += (info->modules[i].length / BLOCK_SIZE + 1);
#endif
    }
  boot_printf ("load done.\n");
  boot_printf ("exec_info->a_entry = 0x%x\n", entry); 
  stop_motor(0);
  (*entry)();

  return E_OK;
}


int
read_single_module (int start_block, void *paddr, struct module_info *info)
{
  char	buf[BLOCK_SIZE];
  char	tmp[BLOCK_SIZE];
  int	i, j;
  int	bn;
  struct exec	*exec_info;

  bn = start_block;
  fd_read (0, bn, (BYTE *)tmp);
  exec_info = (struct exec *)tmp;
  if (N_BADMAG (*exec_info))
    {
      boot_printf ("This object is not exec format (%d).\n", *exec_info);
      boot_printf ("block number: %d\n", bn);	/* */
      for (;;)
	;
      /* STOP HERE */
    }
  
  if ((N_MAGIC(*exec_info) == 0413) || (N_MAGIC(*exec_info) == NMAGIC))
    {
      bn += 1;	/* a.out ¤Î¥Ø¥Ã¥À¤ÎÂç¤­¤µ¤À¤±¥«¥¦¥ó¥È¥¢¥Ã¥×¤¹¤ë */

      boot_printf ("blocks: text: %d, data: %d, paddr: 0x%x\n",
	      (ROUNDUP (exec_info->a_text, PAGE_SIZE) / BLOCK_SIZE),
	      (ROUNDUP (exec_info->a_data, PAGE_SIZE) / BLOCK_SIZE),
	      paddr);

      for (i = 0;
	   i < (ROUNDUP (exec_info->a_text, PAGE_SIZE) 
		 / BLOCK_SIZE);
	   i++, bn++)
	{
	  boot_printf (".");
	  fd_read (0, bn, (BYTE*)buf);
	  bcopy (buf,
		 (char *)(paddr + i * BLOCK_SIZE),
		 BLOCK_SIZE);
	}
/*      boot_printf ("\nText region is readed.\n"); */
      if (exec_info->a_data > 0)
	{
	  for (j = 0;
	       j <= (ROUNDUP (exec_info->a_data, PAGE_SIZE)
		     / BLOCK_SIZE);
	       j++ , bn++)
	    {
	      boot_printf (",");
	      fd_read (0, bn, buf);
	      bcopy (buf,
		     (char *)(paddr
				     + (ROUNDUP (exec_info->a_text, PAGE_SIZE)) 
				     + j * BLOCK_SIZE),
		     BLOCK_SIZE);
	    }
	}
    }
  else
    {
      boot_printf ("I don't know how to read a.out image.(0x%x)\n", N_MAGIC(*exec_info));
      for (;;)
	;
    }
/*  boot_printf ("\nload done.\n"); */
  boot_printf ("\n");

  return E_OK;
}


int
read_a_out ()
{
  char	buf[BLOCK_SIZE];
  char	tmp[BLOCK_SIZE];
  int	*p;
  int	i;
  int	bn;
  void	(*func)();
  struct exec	*exec_info;
  int	errno;
  struct boot_header	*info;

  fd_read (0, 0, tmp);
  exec_info = (struct exec *)tmp;
  if (N_BADMAG (*exec_info))
    {
      boot_printf ("This object is not exec format.\n");
      return (0);
    }
  
  boot_printf ("*** Single Module boot. ***\n");
  boot_printf ("text size = %d\n", exec_info->a_text);
  boot_printf ("data size = %d\n", exec_info->a_data);
  boot_printf (" bss size = %d\n", exec_info->a_bss);
  info = (struct boot_header *)MODULE_TABLE;
  info->machine.base_mem = base_mem;
  info->machine.ext_mem = ext_mem;
  info->machine.real_mem = real_mem;
  info->machine.rootfs = 0xffffffff;

#ifdef linux
  if (N_MAGIC(*exec_info) == 0413)
#else
  if (exec_info->a_magic == 0413)
#endif
    {
      boot_printf ("demand loading object. (page size alignemnt)\n");
      boot_printf ("load address = 0x%x\n", KERNEL_ADDR);
#ifdef linux
      bn = 1;	/* 1K bytes offset */
#else
      bn = PAGE_SIZE / BLOCK_SIZE;
#endif /* linux */
      for (i = 0;
	   i <= (ROUNDUP (exec_info->a_text, PAGE_SIZE) 
		 / BLOCK_SIZE);
	   i++, bn++)
	{
#ifdef nodef
	  boot_printf (".");
#else
	  putchar ('.');
#endif
	  fd_read (0, bn, buf);
#ifdef linux
	  bcopy (buf,
		 (char *)(KERNEL_ADDR + ((bn - 1) * BLOCK_SIZE)),
 		 BLOCK_SIZE);
#else
	  bcopy (buf,
		 (char *)(KERNEL_ADDR + ((bn - 4) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#endif /* linux */
	}
      boot_printf ("\nText region is read.\n");
      for (i = 0;
	   i <= (ROUNDUP (exec_info->a_data, PAGE_SIZE)
		 / BLOCK_SIZE);
	   i++, bn++)
	{
#ifdef nodef
	  boot_printf (".");
#else
	  putchar ('.');
#endif
	  fd_read (0, bn, buf);
#ifdef linux
	  bcopy (buf,
		 (char *)(KERNEL_ADDR + ((bn - 1) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#else
	  bcopy (buf,
		 (char *)(KERNEL_ADDR + ((bn - 4) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#endif
	}
    }
  else
    {
      boot_printf ("load address = 0x%x\n", (KERNEL_ADDR - N_TXTOFF (*exec_info)));
      for (i = 0;
	   i <= ((ROUNDUP (exec_info->a_text, BLOCK_SIZE) 
	      + ROUNDUP (exec_info->a_data, BLOCK_SIZE)
	      + ROUNDUP (sizeof (struct exec), BLOCK_SIZE)) / BLOCK_SIZE + 10);
	   i++)
	{
#ifdef nodef
	  boot_printf (".");
#else
	  putchar ('.');
#endif
retry:
	  errno = fd_read (0, i, buf);
	  if (errno != E_OK)
	    {
	      goto retry;
	    }
	  bcopy (buf,
		 (char *)((KERNEL_ADDR - N_TXTOFF(*exec_info))
				 + (i * BLOCK_SIZE)),
		 BLOCK_SIZE);
	}
    }
  p = (int *)(exec_info->a_entry);
  boot_printf ("load done.\n");
  boot_printf ("exec_info->a_entry = 0x%x\n", p);
  func = (void (*)())(exec_info->a_entry);
  (*func)();

  return E_OK;
}

