#include "fd.h"
#include "macros.h"
#include "file.h"
#include "page.h"
#include "types.h"
#include "errno.h"
#include "sfs.h"
#include "asm.h"
#include "lib.h"
#include "dma.h"
#include "config.h"
#include "keyboard.h"

int			debug_flag = 0;

#define TRAD_OPERATOR(name, f)  static int (f) (char* argv[]);
#define HOOK_OPERATOR(name, f)  static int (f##_func) (char* argv[]);
#define EXTN_OPERATOR(name, f)  extern int (f) (char* argv[]);
#include "commands.h"
#undef EXTN_OPERATOR
#undef HOOK_OPERATOR
#undef TRAD_OPERATOR

struct func_entry
{
  char	name[15];
  FUNCP	func;
};

struct func_entry func_table[] =
{
#define EXTN_OPERATOR(name, f) { name, f, },
#define HOOK_OPERATOR(name, f) { name, f##_func, },
#define TRAD_OPERATOR(name, f) { name, f, },
#include "commands.h"
#undef TRAD_OPERATOR
#undef HOOK_OPERATOR
#undef EXTN_OPERATOR
};

static char	**setargs (char *line, char **args);

/**********************************************************************
 *
 */
static char **
setargs (char *line, char **args)
{
  int	i;

  for (i = 0; i < 10 - 1; i++)
    {
      args[i] = line;
      if (*line == '\0')
	goto escape;
      while (*line != '\0')
	{
	  if (*line == ' ')
	    {
	      *line++ = '\0';
	      break;
	    }
	  line++;
	}
      while (*line == ' ')	/* 空白の取り除き */
	{
	  line++;
	}
    }
escape:
  args[i] = NULL;
  return (args);
}

/**********************************************************************
 *
 */
int
evaluate (char *line)
{
  int i;
  char *args[10];		/* いい加減に決めた値 */

  bzero ((void *)args, sizeof (char *) * 10);
  setargs(line, args);

  for (i = 0; i < TABLE_SIZE(func_table); i++)
    {
      if (strncmp (args[0], func_table[i].name, 15) == 0)
	{
	  func_table[i].func (args);
	  break;
	}
    
    }
  if (i >= TABLE_SIZE(func_table))
    {
      boot_printf ("boot: %s: command not found\n", args[0]);
    }

  return E_OK;
}


/**********************************************************************
 * help --- 
 */
static int
help_func (char *argv[])
{
  int	i;
  
  boot_printf ("help:\n");
  boot_printf ("COMMAND:\n");
  for (i = 0; i < TABLE_SIZE(func_table); i++)
    {
      char disp[21];
      int funcname_len;
      strcpy(disp, "                    ");
      funcname_len = strlen(func_table[i].name);
      if (funcname_len > 15)
	{
	  funcname_len = 15;
	}
      strncpy(disp, func_table[i].name, funcname_len);
      boot_printf ("%s", disp);
      if (i % 3 == 2)
	{
	  boot_printf("\n");
	}
    }
  if (i % 3 != 0)
    {
      boot_printf("\n");
    }

  return E_OK;
}

/***************************************************************************
 */
static int
cpu_reset_func (char *argv[])
{
  /* IBM PC の場合、キーボードに対し、パルスを送信することによって
   * リセットする。
   */
  while ((inb (0x64) & 0x02) == 0x02)
    ;
  outb (0x64, 0xf0);

  return E_OK;
}

/**************************************************************************
 *
 */
static int
dump (BYTE *addr, int length)
{
  int	i;
  int	j;

  for (i = 1; i <= length; i += 16)
    {
      boot_printf ("%x: ", (unsigned int)addr);
      for (j = 0; j < 16; j++)
	{
	  boot_printf ("%x ", (*addr++) & 0xff);
	}
      boot_printf ("\n");
      if (((i / 16) % 20) == 0)
	{
	  boot_printf ("-- more -- ");
	  read_keyboard ();
	  boot_printf ("\n");
	}
    }
  return E_OK;
}

static int
run_func (char* argv[])
{
  void	(*func)();

  func = (void (*)())0x80010000;
  (*func)();

  return E_OK;
}

/*************************************************************************
 * boot_func --- とりあえずの boot
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	FD の最初から kernel を読み込む
 *		正式版では、/system/btron386 から読み込むようになる予定
 *
 */
static int
boot_func (char *argv[])
{
  char	tmp[BLOCK_SIZE];
  extern  int	read_multi_module ();
  extern  int	read_a_out ();

  boot_printf ("booting kernel...\n");
  init_fd ();
  fd_reset ();
  on_motor (0);
  fd_recalibrate(0);
  fd_read (0, 0, tmp);
  boot_printf ("exec data read...\n");
  boot_printf ("magic number %d\n", tmp[0]);
  if (tmp[0] == 0x0001)
    {
      boot_printf ("read multiple module.\n");
      read_multi_module ();
      boot_printf ("boot end\n");
    }
  else
    {
      read_a_out ();
    }

  stop_motor (0);
  return E_OK;
}

/**************************************************************************
 *
 */
static int
dump_func (char **args)
{
  int	base;
  int	length;

  base = atoi (args[1]);
  length = atoi (args[2]);
  boot_printf ("dump_func: base = 0x%x, length = %d\n", base, length);
  dump ((BYTE *)base, length);

  return E_OK;
}

/**********************************************************************
 *
 *
 */
static int
fd_recalibrate_func (char* argv[])
{
  boot_printf ("recalibrate status = 0x%x\n", fd_recalibrate (0));

  return E_OK;
}

/**********************************************************************
 *
 *
 */
static int
seek_func (char **args)
{
  int	drive;
  int	cylinder;

  drive = atoi (args[1]);
  cylinder = atoi (args[2]);
  boot_printf ("seek drive = %d, cylinder = %d\n",
	  drive, cylinder);
  boot_printf ("seek status = 0x%x\n", fd_seek (0, drive, cylinder, 1));

  return E_OK;
}

/****************************************************************************
 *
 */
static int
debug_func (char **argv)
{
  int	level;

  if (argv[1] == 0)
    level = 0;
  else
    level = atoi (argv[1]);

  debug_flag = level;

  return E_OK;
}

/*****************************************************************************
 * print_map
 *
 */
static int
print_map_func (char **av)
{
  ULONG vaddr = av[1] ? atoi (av[1]) : 0;
  struct page_table_entry	*page;

  page = get_page_entry (vaddr * PAGE_SIZE);
  boot_printf ("virtual  address: 0x%x\n", vaddr * PAGE_SIZE);
  boot_printf ("physical address: 0x%x\n", page->frame_addr * 4096);
  boot_printf ("user/supervisor:  0x%x\n", page->u_and_s);
  boot_printf ("read_write:       0x%x\n", page->read_write);
  boot_printf ("present bit:      0x%x\n", page->present);

  return E_OK;
}

/**********************************************************************
 *
 */
static int
fd_status_func (char **args)
{
/*
  ULONG in[1];
  ULONG out[1];

  in[0] = FDC_DEVICE_STAT;
  cmd_fdc (1, in, 1, out);
*/
  return E_OK;
}

/**********************************************************************
 *
 */
static int
fd_read_func (char **args)
{
  int drive;
  int blockno;
  int stat;

  drive = atoi (args[1]);
  blockno = atoi (args[2]);
  boot_printf ("fd_read_func: drive = %d, blockno = %d\n", drive, blockno);
  stat = fd_read (drive, blockno, (char *)DMA_BUFF);
  if (stat != E_OK)
    {
      boot_printf ("can't read device. errno = %d\n", stat);
      return (stat);
    }
  boot_printf ("read ok.\n");
  
  dump ((BYTE *)DMA_BUFF, 512);
#ifdef DEBUG
#endif /* DEBUG */

  return E_OK;
}

/*
 *
 */
int
boot_hd (ULONG devid)
{
  struct sfs_inode	ip;
  int			fd;
  struct sfs_superblock	*sb;
  int			errno;
  char			buf[BLOCK_SIZE];
  extern int		f_sfs_mountroot (int);
  extern int		sfs_lookup_file (int, struct sfs_superblock *, struct sfs_inode *, char *, struct sfs_inode *);
  extern int		vga_text (void);
  extern int		multi_boot (int, struct sfs_superblock *, struct sfs_inode *, int);


  if (f_sfs_mountroot (devid) == -1)
    {
      boot_printf ("mount failure\n");
      return (E_PAR);
    }

  fd = fs_table[0].devid;
  sb = &(fs_table[0].sb);

  errno = sfs_lookup_file (fd, sb, rootdirp, BOOT_PATH, &ip);
  if (errno)
    {
      vga_text ();
      return (E_SYS);
    }

  errno = sfs_read_file (fd, sb, &ip, 0, 2, buf);
  if (errno)
    {
      vga_text ();
      return (E_SYS);
    }

  if (buf[0] == 0x01)
    {
      multi_boot (fd, sb, &ip, 1);
    }
  else
    {
      vga_text ();
      boot_printf ("Unknown file type (0x%x)\n", buf[0]);
      return (E_SYS);
    }

  return (E_OK);
}

int
boot_btron (char **av)
{
  int devid = 0;
  struct sfs_inode	ip;
  int			fd;
  struct sfs_superblock	*sb;
  int			errno;
  char			buf[BLOCK_SIZE];
  char			*btron_os = BOOT_PATH;
  extern int		f_sfs_mountroot (int);
  extern int		sfs_lookup_file (int, struct sfs_superblock *, struct sfs_inode *, char *, struct sfs_inode *);
  extern int		multi_boot (int, struct sfs_superblock *, struct sfs_inode *, int);
  
  if (av[2] == NULL)
    {
      boot_printf ("usage: boot hd drive partition\n");
      boot_printf ("       boot fd drive\n");
      return E_PAR;
    }

  if (strcmp (av[1], "hd") == 0)
    {
      int pn;
      if (av[3] == NULL)
	{
	  boot_printf ("usage: boot hd drive partition [btron file]\n");
	  boot_printf ("       boot fd drive [btron file]\n");
	  return E_PAR;
	}
      pn = atoi(av[3]);
      if (pn <= 0)
	{
	  boot_printf ("usage: boot hd drive partition\n");
	  boot_printf ("partition number should start from 1\n");
	  return E_PAR;
	}
      pn = pn - 1;
      devid = 0x010000 | (atoi(av[2]) << 8) | pn;

      if (av[4])
	{
	  btron_os = av[4];
	}
    }
  else if (strcmp (av[1], "fd") == 0)
    {
      devid = 0x000000 | (atoi(av[2]) << 8);
      if (av[3])
	{
	  btron_os = av[3];
	}
    }
  else
    {
      boot_printf ("unknown device. Valid device is \"fd\" or \"hd\"\n");
      return 1;
    }

  boot_printf ("devid = 0x%x (device = %d, partition = %d)\n", devid, (devid >> 8) & 0xff, (devid+1) & 0xff);

  if (f_sfs_mountroot (devid) == -1)
    {
      boot_printf ("mount failure\n");
      return (E_PAR);
    }

  fd = fs_table[0].devid;
  sb = &(fs_table[0].sb);

  errno = sfs_lookup_file (fd, sb, rootdirp, btron_os, &ip);
  if (errno)
    {
      boot_printf ("cannot find OS (\"%s\")\n", btron_os);
      return (E_SYS);
    }

  errno = sfs_read_file (fd, sb, &ip, 0, 2, buf);
  if (errno)
    {
      boot_printf ("cannot read OS file.\n");
      return (E_SYS);
    }

  if (buf[0] == 0x01)
    {
      boot_printf ("read multiple module.\n");
      multi_boot (fd, sb, &ip, 0);
      boot_printf ("boot end\n");
    }
  else
    {
      boot_printf ("Unknown file type (0x%x)\n", buf[0]);
      return (E_SYS);
    }

  return (E_OK);
}

#define HOOK_OPERATOR(name, f) \
static int f##_func (char *argv[]) \
{ \
  f (); \
  return E_OK; \
}
#define EXTN_OPERATOR(name, f)
#define TRAD_OPERATOR(name, f)
#include "commands.h"
#undef TRAD_OPERATOR
#undef EXTN_OPERATOR
#undef HOOK_OPERATOR

