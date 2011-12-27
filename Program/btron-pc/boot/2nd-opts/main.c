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
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 *
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-05-05 17:25:45  kishida0
 * for debug functions
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
 * 関数 cat() の中で、文字を出力するときに使用している boot_printf の指定子が
 * 間違っていた (%2t) ので、正しい (%c) に変更した。
 *
 * Revision 1.19  1998/11/20 08:02:34  monaka
 * *** empty log message ***
 *
 * Revision 1.18  1998/06/11 15:49:53  night
 * func_table 中の ide_init, ide_boot, ide_id, ide_read_dump, vga_test 関
 * 数をキャストするよう変更した。
 * また、sfsread、mount、dir、cat の各コマンドを有効にした。
 *
 * Revision 1.17  1998/02/25 13:07:34  night
 * ide_read_dump コマンドの追加。
 *
 * Revision 1.16  1997/08/31 14:25:47  night
 * BOOT 時の処理の変更。
 * (root ファイルシステムを OS に教える処理の追加)
 *
 * Revision 1.15  1997/06/29 13:38:39  night
 * 従来型の OS のロードの処理を有効にした。
 *
 * Revision 1.14  1997/06/29 13:13:46  night
 * HD の SFS フォーマットされたパーティションからの BTRON OS をロードできる
 * 機能を追加。
 *
 * Revision 1.13  1997/04/24 15:30:34  night
 * 註釈の変更。
 *
 * Revision 1.12  1996/11/14  13:14:16  night
 * cat コマンドの追加。
 *
 * Revision 1.11  1996/11/13  16:28:37  night
 * "sfsread", "mount", "dir" のコマンドの追加。
 *
 * Revision 1.10  1996/08/11  15:11:34  night
 * IDE HD の操作関係の関数を ide.c に移動した。
 * ideboot コマンドを追加した。
 *
 * Revision 1.9  1996/07/31  19:27:38  night
 * IDE HD 用関数を改造。
 * IDE HD に対してデータ読み取りを行うとき、データがキャッシュ
 * されていると割り込みを待つ動作の前に読み取りが終了してしまう。
 * その結果、割り込みを待ち続けてしまう。
 * データ読み取りの要求を出すときに、割り込みを待つ前に HD の読み
 * 取り動作が終了しているかどうかを判定するようにした。
 *
 * Revision 1.8  1996/07/30  18:23:54  night
 * IDE HD 用関数類を追加。
 * これらの関数は一時的なものなので、将来は ide.c と ide.h などの別のファ
 * イルに移す必要がある。
 *
 * Revision 1.7  1996/07/25  16:59:56  night
 * OS を読み込むときに、モジュールが入っている最初の FD のブロック番号を
 * 表示していた部分を削除した。
 *
 * Revision 1.6  1996/07/24  13:59:49  night
 * インターバルタイマの初期化の処理を削除
 *
 * Revision 1.5  1996/07/23  17:05:06  night
 * インターバルタイマ用の割り込み処理を追加。
 *
 * Revision 1.4  1996/07/22  13:34:47  night
 * CPU リセットをキーボードコントローラを介して行うように修正。
 * (以前は、リセットができなかった)
 *
 * Revision 1.3  1996/07/08  14:53:14  night
 * IBM-PC 互換機用の変更。
 *
 * Revision 1.2  1996/05/11  15:51:33  night
 * beep 関数の中身を削除
 *
 * Revision 1.1  1996/05/11  10:45:05  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 *
 * 
 * ----------------------------------------------------------------------------------
 *
 * Revision 1.11  1996/01/06 15:57:52  night
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

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

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

#define MAX_LINE	100

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
  vga_graph ();
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
 * status_memory --- メモリ情報の表示
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
  offset = BLOCK_SIZE;	/* 最初のモジュールが入っているオフセット(バイト) */
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
int
read_multi_module ()
{
  int	i;
  int	bn;
  struct boot_header	*info;
  void  (*entry)();

  boot_printf ("Multiple module boot.\n");
  info = (struct boot_header *)MODULE_TABLE;
  fd_read (0, 0, (char *)info);
  info->machine.base_mem = base_mem;
  info->machine.ext_mem = ext_mem;
  info->machine.real_mem = real_mem;
  info->machine.rootfs = 0xffffffff;
  boot_printf ("Module %d\n", info->count);
  bn = 1;	/* 最初のモジュールが入っているブロック番号 */
  entry = (void (*)())(info->modules[0].entry);
  for (i = 0; i < info->count; i++)
    {
      boot_printf ("[%d] ", i);
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
  fd_read (0, bn, tmp);
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
      bn += 1;	/* a.out のヘッダの大きさだけカウントアップする */

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
	  fd_read (0, bn, buf);
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

