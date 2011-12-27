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
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.11  1996-01-06 15:57:52  night
 * SCSI の情報を SRAM から取得するようにした。
 *
 * Revision 1.10  1995/09/21  15:50:40  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.9  1995/08/05  12:04:03  night
 * ロード時のメッセージの変更。
 *
 * Revision 1.8  1995/06/26  15:05:48  night
 * DOS 関係のコマンド (dosls/doscat) を追加。
 *
 * Revision 1.7  1994/11/20  10:39:37  night
 * 1) ロード時に余分な busy loop を行っていたのを削除した。
 * 2) 余分な註釈を削除した。
 * 3) モジュールファイルが実行ファイルではない時の return 文を削除した(そ
 *    の前で無限ループを実行しているので return 文は実行しないため)。
 *
 * Revision 1.6  1994/11/19  07:42:05  night
 * モジュール情報の登録を行う処理を追加。
 * ブート時のブロック番号の計算を変更。
 *   モジュール情報の length / BLOCK_SIZE ではブロック番号がひとつ大きかっ
 *   た。ブロック番号の計算値から -1 を引くようにした。
 *
 * Revision 1.5  1994/11/13  15:02:57  night
 * 複数モジュールの boot のための変更 [ 機能追加 ]
 *
 * Revision 1.4  1994/09/04  13:23:02  night
 * オープニングメッセージの変更 (Welcme -> Welcome)
 *
 * Revision 1.3  1994/07/30  17:37:18  night
 * ファイル中の日本語文字をすべて EUC コードに変更。
 *
 * Revision 1.2  1994/07/05  17:19:36  night
 * Change FD format; 512bytes/block.
 *
 * Revision 1.1  1993/10/11  21:29:30  btron
 * btron/386
 *
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "boot.h"
#include "types.h"
#include "errno.h"
#include "config.h"
#include "location.h"
#include "memory.h"
#include "console.h"
#include "date.h"
#include "strings.h"
#include "macros.h"
#include "dma.h"
#include "fd.h"
#include "file.h"
#include "vram.h"
#include "page.h"
#include "a.out.h"		/* for gcc header */
#include "dos/dosfs.h"

#define MAX_LINE	100

static	int	ls_func ();
static  int	cd_func ();
static  int	pwd_func ();
static	int	help ();
static	int	cpu_reset ();
	int	beep ();
static	int	status_memory ();
static	int	seek_func ();
static	int	fd_status_func ();
static	int	fd_recalibrate_func ();
static	int	fd_read_func ();
static	int	dump_func ();
static	int	mount_func ();
static	int	dump ();
static	int	print_map ();
static	int	debug ();
static	int	read_func ();
static	int	boot_func ();
static	int	run_func ();
static  int	dosls ();
static  int	doscat ();
static	int	dosfat ();
static	int	scsi ();

static char	**setargs (char *line, char **args);

struct func_entry
{
  char	*name;
  FUNCP	func;
};

struct func_entry	func_table[] =
{
  "ls",		ls_func,
  "cd",		cd_func,
  "pwd",	pwd_func,
  "help",	help,
  "beep",	beep,
  "reset",	cpu_reset,
  "memory",	status_memory,
  "fdinit",	init_fd,
  "fdstatus",	fd_status_func,
  "fdseek",	seek_func,
  "fdreset",	fd_reset,
  "fdrecalib",	fd_recalibrate_func,
  "fdstat",	fd_get_status,
  "fdread",	fd_read_func,
  "dump",	dump_func,
  "mount",	mount_func,
  "map",	print_map,
  "debug",	debug,
  "read",	read_func,
  "boot",	boot_func,
  "run",	run_func,
  "dosls",	dosls,
  "doscat",	doscat,
  "dosfat",	dosfat,

  "scsi",	scsi,
};

struct file_system	root_fs;
struct file		root;
struct file		cwd;
int			debug_flag = 0;
struct dos_fsinfo	dosfs;

#ifdef PG_DEBUG
static void
check_paging ()
{
  char	*n;
  int	i;

  printf ("check pageing.\n");
  for (i = 0; i < 1000; i++)
    {
      n = (char *)0x8000C000 + (i * 1024);
      printf ("check 0x%x    ", n);
      *n = 0x12;
      if (*n == 0x12)
        {
          printf ("check paging ok.\n");
        }
      else
        {
          printf ("check paging invalid.\n");
        }
    }
}
#endif PG_DEBUG

/****************************************************************************
 *
 */
static int
debug (char **argv)
{
  int	level;

  if (argv[1] == 0)
    level = 0;
  else
    level = atoi (argv[1]);

  debug_flag = level;
}
 
/******************************************************************************
 *
 *
 */
void
_main ()
{
  struct date_t	d;
  int	i;
  int	tmp;
  int	ch;
  char	line[MAX_LINE];
  char	*args[10];		/* いい加減に決めた値 */
  
  beep ();
  init_8259A ();
  init_idt ();
  init_console ();
  init_memory ();
  init_keyboard ();
  init_vm ();
  enable_page ();
#ifdef PG_DEBUG
  check_paging ();
#endif
  
  banner ();
  open_interrupt ();
  for (;;)
    {
      bzero (args, sizeof (char *) * 10);
      printf ("boot> ");
      gets (line);
      if (strlen (line) > 0)
	{
	  setargs (line, args);
	  evaluate (args);
	}
    }
/* DON'T REACHED */
}

/**********************************************************************
 *
 */
static int
fd_status_func (char **args)
{
  fd_get_status (0, atoi (args[1]));
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
  printf ("fd_read_func: drive = %d, blockno = %d\n", drive, blockno);
  stat = fd_read (drive, blockno, (char *)DMA_BUFF);
  if (stat != E_OK)
    {
      printf ("can't read device. errno = %d\n", stat);
      return (stat);
    }
  printf ("read ok.\n");
  
#ifdef DEBUG
  dump (DMA_BUFF, 1024);
#endif /* DEBUG */
}

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
evaluate (char *args[])
{
  int	i;

  for (i = 0; i < TABLE_SIZE(func_table); i++)
    {
      if (strcmp (args[0], func_table[i].name) == 0)
	{
	  func_table[i].func (args);
	  break;
	}
    }
}

/**********************************************************************
 *
 */
static char *
file_type (unsigned short type)
{
  static char	types[17];
  unsigned short	mask;
  int			i;
  
  strcpy (types, "1111xxxxBAPOxREW");
  mask = 0x0001;
  for (i = 0; i < 16; i++)
    {
      if ((mask & type) == 0)
	{
	  types[15 - i] = '-';
	}
      mask = mask << 1;
    }
  return (types);
}
  

/**********************************************************************
 */
static int
ls_func (char **av)
{
  int	i;
  union record_entry	rec;
  struct file		tmp;

  printf ("RECORD %d\n", cwd.f_nrec);
  printf ("INDEX LEVEL %d\n", cwd.f_index);

  if (av[1] != NULL)
    {
      printf ("ls: record = %d\n", atoi (av[1]));
      if (get_record (&cwd, atoi (av[1]), &rec) == NULL)
	{
	  printf ("can't get record.\n");
	}
      else
	{
	  bzero (&tmp, sizeof (tmp));
	  open_file (0, rec.link_record.file_id, &root_fs, &tmp);
	  printf ("0x%x\t%d\t<%d>\t%40t\n",
		  tmp.f_type,
		  tmp.f_size,
		  rec.link_record.file_id,
		  tmp.fname);
	}
      return 0;
    }
      
/*  rec = cwd.record_table; */
  for (i = 0; i < cwd.f_nrec; i++)
    {
      if (get_record (&cwd, i, &rec) == NULL)
	break;
      if (rec.normal_record.type == 0x80)
	{
	  bzero (&tmp, sizeof (tmp));
	  open_file (0, rec.link_record.file_id, &root_fs, &tmp);
	  printf ("0x%x\t%d\t<%d>\t%40t\n",
		  tmp.f_type,
		  tmp.f_size,
		  rec.link_record.file_id,
		  tmp.fname);
	}
      else
	{
	  printf ("0xXXXX\t%d\t0x%x\t%d\t\n",
		  rec.normal_record.size,
		  rec.normal_record.type,
		  rec.normal_record.start_offset);
	}
    }
}

/**********************************************************************
 * cd_func --- ディレクトリ移動の機能の実行
 *
 */
static int
cd_func (char **argv)
{
  int	id;
  
  if (argv[1] == 0)
    {
      bcopy (&root, &cwd, sizeof (root));
      printf ("cd %t\n", cwd.fname);
      return (E_OK);
    }
  else
    id = atoi (argv[1]);

  open_file (0, id, &root_fs, &cwd);
  printf ("cd %t\n", cwd.fname);
  return (E_OK);
}

/**********************************************************************
 *
 */
static int
pwd_func ()
{
  printf ("%t\n", cwd.fname);
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
  printf ("seek drive = %d, cylinder = %d\n",
	  drive, cylinder);
  printf ("seek status = 0x%x\n", fd_seek (drive, cylinder));
}

/**********************************************************************
 *
 *
 */
static int
fd_recalibrate_func ()
{
  fd_recalibrate (0);
}

/**********************************************************************
 * help --- 
 */
static int
help ()
{
  int	i;
  
  printf ("help:\n");
  printf ("COMMAND:\n");
  for (i = 0; i < TABLE_SIZE(func_table); i++)
    {
      printf ("      %s\n", func_table[i].name);
    }
}

/***************************************************************************
 */
static int
cpu_reset ()
{
  outb (0x00f0, 0);
}

/***************************************************************************
 * beep -- BEEP routine.
 */
int
beep ()
{
  outb (0x37, 0x06);
  busywait (100);
  outb (0x37, 0x07);
}


/***************************************************************************
 * status_memory --- メモリ情報の表示
 */
int
status_memory ()
{
  extern void	*last_addr;
  volatile int	*p;
  
#ifdef notdef
  for (p = (int *)0x100000; (int)p < 0xf00000; (int)p += 0x100000)
    {
      *p = 0;
      *p = 0xAA;
      if (*p != 0xAA)
	break;
    }
#endif /* notdef */
  printf ("Extended Memory = %d K bytes\n", ext_mem / 1024);
  printf ("USE Memory      = %d bytes\n", last_addr);
}

/***************************************************************************
 *
 */
void
print_binary (int n)
{
  unsigned int	mask = 0x00000080;
  int	i;

  for (i = 0; i < 8; i++)
    {
      if (mask & n)
	printf ("1");
      else
	printf ("0");
      mask = mask >> 1;
    }
}

  
void
banner (void)
{
  printf ("BTRON/386 2nd BOOT Version %d.%d\n\n", MAJOR_VER, MINOR_VER);
  printf ("%s\n", rcsid);
  printf ("START 32 BIT MODE.\n");
  printf ("Welcome BTRON/386.\n");
}

/**************************************************************************
 * panic.
 */
void
panic (char *s)
{
  printf ("panic: %s\n", s);
  for (;;)
    ;
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
  printf ("dump_func: base = 0x%x, length = %d\n", base, length);
  dump (base, length);
}

/**************************************************************************
 *
 */
static int
dump (unsigned char *addr, int length)
{
  int	i;
  int	j;

  for (i = 1; i <= length; i += 16)
    {
      printf ("%x: ", addr);
      for (j = 0; j < 16; j++)
	{
	  printf ("%x ", (*addr++) & 0xff);
	}
      printf ("\n");
      if (((i / 16) % 20) == 0)
	{
	  printf ("-- more -- ");
	  read_keyboard ();
	  printf ("\n");
	}
    }
}

/***************************************************************************
 * mount_func ---
 *
 */
static int
mount_func (char **args)
{
  int	result;
  struct file_system	*fs;
  struct file_system_header	*header;

  if (args[1] == NULL)
    {
      printf ("drive is 0\n");
      fs = attach_filesystem (0, &root_fs);
    }
  else
    {
      printf ("drive is %d\n", atoi (args[1]));
      fs = attach_filesystem (atoi (args[1]), &root_fs);
    }

  if (fs == NULL)
    {
      printf ("can't mount.\n");
      return E_DEV;
    }
  header = &(fs->fs_block.header);
  printf ("os_disk_id = 0x%x\n", header->os_disk_id);
  printf ("disk_id    = 0x%x\n", header->disk_id);
  printf ("block size = 0x%x\n", header->sysblock_size);
  printf ("file id table size = 0x%x\n", header->file_id_table_size);
  printf ("file system name = %40t\n", header->file_system_name);
  printf ("device name      = %40t\n", header->device_name);
  printf ("root file name   = %40t\n", root.fname);
  bcopy (&root, &cwd, sizeof (root));
  return (E_OK);
}

/*****************************************************************************
 * print_map
 *
 */
int
print_map (char **av)
{
  unsigned long	vaddr = av[1] ? atoi (av[1]) : 0;
  struct page_table_entry	*page;

  page = get_page_entry (vaddr * PAGE_SIZE);
  printf ("virtual  address: 0x%x\n", vaddr * PAGE_SIZE);
  printf ("physical address: 0x%x\n", page->frame_addr * 4096);
  printf ("user/supervisor:  0x%x\n", page->u_and_s);
  printf ("read_write:       0x%x\n", page->read_write);
  printf ("present bit:      0x%x\n", page->present);
}


cat (char *buf, int size)
{
  int	i;

  for (i = 0; i < size; i += 2)
    {
      printf ("%2t", &buf[i]);
    }
}


  
/*************************************************************************
 *
 */
static int
read_func (char **av)
{
  struct file	*fp;
  int		i, j;
  int		recordno = av[1] ? atoi(av[1]) : 0;
  char		buf[1024];
  int		size;
  
  fp = &cwd;
  size = read_file (0, fp, recordno, 0, 1024, buf);
  printf ("read size = %d\n", size);
  cat (buf, size);
}  


static int
run_func (VOID)
{
  void	(*func)();

  func = (void (*)())0x80010000;
  (*func)();
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
boot_func (VOID)
{
  char	buf[BLOCK_SIZE];
  char	tmp[BLOCK_SIZE];
  int	*p;
  int	i;
  int	bn;
  void	(*func)();
  struct exec	*exec_info;
  int	errno;
  volatile int	a, b;
  
  printf ("booting kernel...\n");
  init_fd ();
  fd_reset ();
  fd_read (0, 0, tmp);
  printf ("exec data readed...\n");
  printf ("magic number %d\n", tmp[0]);
  if (tmp[0] == 0x0001)
    {
      printf ("read multiple module.\n");
      read_multi_module ();
      printf ("boot end\n");
    }
  else
    {
      read_a_out ();
    }
}

/*
   複数のモジュールを読み込む。

   最初のモジュールのみ ITRON カーネルと仮定している。
   そのため、読み込む位置は、0x00010000 と決めている。
   (ITRON カーネルは、仮想アドレス 0x80010000 に読み込むことにしている。
   0x00010000 というのは、0x80010000 に対応している物理アドレスである)

   2 番目以降のモジュールは、ITRON カーネルにの後に続けて読み込むことになる。
   そのため、ITRON カーネルの大きさによって物理アドレスは変わることになる。
   2 番目以降のモジュールについては、boot はロードするだけで、仮想アドレス
   へのマッピングなどは ITRON カーネルの起動後に行う。

*/ 
read_multi_module ()
{
  static char	buf[BLOCK_SIZE];
  static char	first_block[BLOCK_SIZE];
  int	*p;
  int	i;
  int	bn;
  struct boot_header	*info;
  void	*paddr;
  void  (*entry)();

  printf ("Multiple module boot.\n");
  info = (struct boot_header *)MODULE_TABLE;
  fd_read (0, 0, info);
  info->machine.base_mem = base_mem;
  info->machine.ext_mem = ext_mem;
  info->machine.real_mem = real_mem;
  printf ("Module %d\n", info->count);
  bn = 1;	/* 最初のモジュールが入っているブロック番号 */
  entry = (void (*)())(info->modules[0].entry);
  for (i = 0; i < info->count; i++)
    {
      printf ("[%d] %d  ", i, bn);
      read_single_module (bn, info->modules[i].paddr, &(info->modules[i]));

      bn += ((info->modules[i].length / BLOCK_SIZE) - 1);
    }
  printf ("load done.\n");
  printf ("exec_info->a_entry = 0x%x\n", entry); 
  (*entry)();
}

read_single_module (int start_block, void *paddr, struct module_info *info)
{
  char	buf[BLOCK_SIZE];
  char	tmp[BLOCK_SIZE];
  int	*p;
  int	i, j;
  int	bn;
  struct exec	*exec_info;
  int	errno;

  bn = start_block;
  fd_read (0, bn, tmp);
  exec_info = (struct exec *)tmp;
  if (N_BADMAG (*exec_info))
    {
      printf ("This object is not exec format (%d).\n", *exec_info);
      for (;;)
	;
      /* STOP HERE */
    }
  
  if ((N_MAGIC(*exec_info) == 0413) || (N_MAGIC(*exec_info) == NMAGIC))
    {
      bn += 1;	/* a.out のヘッダの大きさだけカウントアップする */

      for (i = 0;
	   i < (ROUNDUP (exec_info->a_text, PAGE_SIZE) 
		 / BLOCK_SIZE);
	   i++, bn++)
	{
	  printf (".");
	  fd_read (0, bn, buf);
	  bcopy (buf,
		 (unsigned long)(paddr + i * BLOCK_SIZE),
		 BLOCK_SIZE);
	}
/*      printf ("\nText region is readed.\n"); */
      for (j = 0;
	   j <= (ROUNDUP (exec_info->a_data, PAGE_SIZE)
		 / BLOCK_SIZE);
	   j++ , bn++)
	{
	  printf (".");
	  fd_read (0, bn, buf);
	  bcopy (buf,
		 (unsigned long)(paddr
				 + (ROUNDUP (exec_info->a_text, PAGE_SIZE)) 
				 + j * BLOCK_SIZE),
		 BLOCK_SIZE);
	}
    }
  else
    {
      printf ("I am unknown a.out image.(0x%x)\n", N_MAGIC(*exec_info));
      for (;;)
	;
    }
/*  printf ("\nload done.\n"); */
  printf ("\n");
}


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

  fd_read (0, 0, tmp);
  exec_info = (struct exec *)tmp;
  if (N_BADMAG (*exec_info))
    {
      printf ("This object is not exec format.\n");
      return (0);
    }
  
  printf ("*** Single Module boot. ***\n");
  printf ("text size = %d\n", exec_info->a_text);
  printf ("data size = %d\n", exec_info->a_data);
  printf (" bss size = %d\n", exec_info->a_bss);
#ifdef linux
  if (N_MAGIC(*exec_info) == 0413)
#else
  if (exec_info->a_magic == 0413)
#endif
    {
      printf ("demand loading object. (page size alignemnt)\n");
      printf ("load address = 0x%x\n", KERNEL_ADDR);
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
#ifdef notdef
	  printf (".");
#else
	  putchar ('.');
#endif
	  fd_read (0, bn, buf);
#ifdef linux
	  bcopy (buf,
		 (unsigned long)(KERNEL_ADDR + ((bn - 1) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#else
	  bcopy (buf,
		 (unsigned long)(KERNEL_ADDR + ((bn - 4) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#endif /* linux */
	}
      printf ("\nText region is readed.\n");
      for (i = 0;
	   i <= (ROUNDUP (exec_info->a_data, PAGE_SIZE)
		 / BLOCK_SIZE);
	   i++, bn++)
	{
#ifdef notdef
	  printf (".");
#else
	  putchar ('.');
#endif
	  fd_read (0, bn, buf);
#ifdef linux
	  bcopy (buf,
		 (unsigned long)(KERNEL_ADDR + ((bn - 1) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#else
	  bcopy (buf,
		 (unsigned long)(KERNEL_ADDR + ((bn - 4) * BLOCK_SIZE)),
		 BLOCK_SIZE);
#endif
	}
    }
  else
    {
      printf ("load address = 0x%x\n", (KERNEL_ADDR - N_TXTOFF (*exec_info)));
      for (i = 0;
	   i <= ((ROUNDUP (exec_info->a_text, BLOCK_SIZE) 
	      + ROUNDUP (exec_info->a_data, BLOCK_SIZE)
	      + ROUNDUP (sizeof (struct exec), BLOCK_SIZE)) / BLOCK_SIZE + 10);
	   i++)
	{
#ifdef notdef
	  printf (".");
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
		 (unsigned long)((KERNEL_ADDR - N_TXTOFF(*exec_info))
				 + (i * BLOCK_SIZE)),
		 BLOCK_SIZE);
	}
    }
  p = (int *)(exec_info->a_entry);
  printf ("load done.\n");
/*  printf ("exec_info->a_entry = 0x%x\n", p); */
  func = (void (*)())(exec_info->a_entry);
  (*func)();
}


/*
 *
 */
dosls (char **av)
{
  struct dos_fsinfo	fs;

  init_filesystem (&fs, "0");
  list_dir (&fs, av[1]);
  printf ("done.\n");
}

doscat (char **av)
{
  struct dos_fsinfo	fs;

  init_filesystem (&fs, "0");
  cat_file (&fs, av[1]);
  printf ("done.\n");
}

dosfat (char **av)
{
  struct dos_fsinfo	fs;

  init_filesystem (&fs, "0");
  print_fat (&fs);
}

scsi ()
{
  int	i;
  unsigned char	*flag;
  
  flag = (unsigned char*)0x80000482;
  printf ("SCSI ID BITS: 0x%x\n", *flag);
}
