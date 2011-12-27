/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/ide.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: ide.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.16  1999/03/18 16:34:07  night
 * Add cast.
 *
 * Revision 1.15  1999/03/15 01:35:23  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.14  1998/11/20 08:02:28  monaka
 * *** empty log message ***
 *
 * Revision 1.13  1998/06/11 15:06:45  night
 * IDE HD ������ɤ߹��ߤκݡ��������ֹ�ξ�� 2 �ӥåȤ�ͭ���ˤʤäƤ�
 * �ʤ��ä���
 *   outb (IDE_CYL_HIGH_REG, (cyl << 8) & 0xff);
 * �Ȥ��Ƥ������ᡢcyl �ξ�� 2 �ӥåȤϾ�� 0 �Ȥ��ƻ��ꤷ�Ƥ��ޤäƤ�����
 * �����
 *   outb (IDE_CYL_HIGH_REG, (cyl >> 8) & 0xff);
 * ��ľ�����Ȥˤ�äơ���� 2 �ӥåȤ��ͤ�ͭ���ˤ�����
 *
 * Revision 1.12  1998/06/10 15:42:22  night
 * HD ������ɤ߼����ˡ��֥�å��ֹ�λ��꤬������������
 * �ѡ��ƥ������ơ��֥�����ɽ������褦���ѹ�������
 *
 * Revision 1.11  1998/06/10 13:41:18  night
 * �С��ƥ������������Ϥ���Ȥ��ˡ����ϥ�����/�إå�/��������
 * ��λ������/�إå�/����������Ϥ���褦�ˤ�����
 *
 * Revision 1.10  1998/06/09 14:03:25  night
 * �ǥ������Υѡ��ƥ���������Ȥ��ơ�CHS ��ȤäƤ�����ʬ��
 * ���٤ƥ�˥��֥�å��ֹ����Ѥ���褦���ѹ�������
 *
 * Revision 1.9  1998/04/27 15:19:53  night
 * IDE HD �Υ�����ȥ꤬ BIOS �Ѵ�����Ƥ�����ν������ɲá�
 *
 * 1) �ѡ��ƥ������ơ��֥�Υ��������������ȥ֥�å��ֹ��׻�����Ȥ��ˡ�
 *    BIOS ���Ѵ����Ƥ��른����ȥ�Ƿ׻�����褦���ѹ�������
 *
 * 2) BIOS ���Ѵ������Ǥ���������ȥ���ͤ�ǥ������ѥ�᡼����
 *    now_cylinder��now_head��now_sector_track �������褦�ˤ�����
 *    (��������ν������ɲ�)
 *
 * 3) ide_id() �ǡ�IDE �ǥ������Υ�ǥ�̾��¾�˥�����ȥ�������Ϥ����
 *    ���˽������ѹ�������512MB �ʾ�Υǥ������ˤĤ��Ƥϡ�BIOS �ǥ������
 *    ���Ѵ������������Ϥ���褦�ˤ�����
 *
 * Revision 1.8  1998/04/17 11:48:01  night
 * calc_N ���֤��ͤ��ͤ��ְ�äƤ����Τǽ���������
 *
 * Revision 1.7  1998/04/16 20:28:55  night
 * 1024 �ʾ�Υ������ֹ���äƤ���Ȥ��ˡ�BIOS �Υ�����ȥ��Ѵ���
 * ��θ���� HD �˥�����������褦�˽������ѹ�������
 *
 * Revision 1.6  1998/02/25 13:06:48  night
 * ide_read_dump ���ޥ�ɤ��ɲá�
 *
 * Revision 1.5  1997/08/31 14:24:30  night
 * ide �ǥ������� ID ������ɤ߼���Ԥ��������ɲá�
 *
 * Revision 1.4  1997/06/29 13:13:45  night
 * HD �� SFS �ե����ޥåȤ��줿�ѡ��ƥ�����󤫤�� BTRON OS ����ɤǤ���
 * ��ǽ���ɲá�
 *
 * Revision 1.3  1997/05/11 12:40:08  night
 * �ѡ��ƥ������������Ϥ���褦�ˤ�����
 *
 * Revision 1.2  1997/04/24 15:29:21  night
 * ��ư���� IDE �Υѡ��ƥ������������Ϥ��Ƥ����Τ����᤿��
 *
 * Revision 1.1  1996/08/11  15:11:56  night
 * �ǽ����Ͽ
 *
 *
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/ide.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "types.h"
#include "macros.h"
#include "config.h"
#include "boot.h"
#include "memory.h"
#include "interrupt.h"
#include "idt.h"
#include "dma.h"
#include "errno.h"
#include "file.h"
#include "page.h"
#include "a.out.h"
#include "asm.h"
#include "lib.h"
#include "ide.h"
#include "date.h"

int			ide_intr_flag;
struct ide_id		ide_parameter;
struct ide_partition	partition_table[4];	/* ��ĥ�ѡ��ƥ������Ϲͤ��Ƥ��ʤ� */
struct ide_partition	backup_partition_table[4];	/* ��ĥ�ѡ��ƥ������Ϲͤ��Ƥ��ʤ� */


void
intr_ide ()
{
  boot_printf ("IDE:interrupt!\n");
  lock ();
  ide_intr_flag = TRUE;
  unlock ();
}

static int
ide_check_ready ()
{
  int result;
  result = inb (IDE_STAT_REG) & 0x40; 
  return result;
}

static int
ide_wait_while_busy ()
{
  while (inb (IDE_STAT_REG) & 0x80)
    {
      busywait (5);
    }

  return E_OK;
}

static int
ide_chk_drq ()
{
  return (inb (IDE_STAT_REG) & 0x08);
}

static int
ide_enable_int ()
{
  outb (IDE_CONTROL_REG, 0x00);

  return E_OK;
}

static int
ide_disable_int ()
{
  outb (IDE_CONTROL_REG, 0x02);

  return E_OK;
}

static int
ide_send_command (BYTE cmd)
{
  while (!ide_check_ready ())
    ;
#ifdef nodef
boot_printf ("%s:%d\n", __FILE__, __LINE__);
  outb (IDE_CONTROL_REG, 0x00);
#endif

  ide_wait_while_busy ();

  outb (IDE_COMMAND_REG, cmd & 0xff);
  ide_wait_while_busy ();
  return (inb (IDE_ERROR_REG));
}


void
ide_id (void)
{
  int i;

  get_ide_parameter (&ide_parameter);

  for (i = 0; i < 20; i++)
    {
      boot_printf ("%c", ide_parameter.model[i * 2 + 1]);
      boot_printf ("%c", ide_parameter.model[i * 2]);
    }
  boot_printf ("\n");
  boot_printf ("C/H/S: %d/%d/%d\n", 
	  ide_parameter.n_cylinder,
	  ide_parameter.n_head,
	  ide_parameter.n_sector);

  for (i = 0; i < IDE_MAX_PARTITION; i++)
    {
      boot_printf ("partition[%d] type = 0x%x, start = %d, length = %d, bootable = %d\n",
	      i, 
	      partition_table[i].type,
	      (int)partition_table[i].start,
	      (int)partition_table[i].length,
	      (int)partition_table[i].boot_flag);
#ifdef nodef
      boot_printf ("begin(CHS): %d/%d/%d\n",
	      partition_table[i].begin[2] | ((partition_table[i].begin[1] & 0xc0) << 2),
	      partition_table[i].begin[0],
	      partition_table[i].begin[1] & 0x3f);
      boot_printf ("end(CHS): %d/%d/%d\n",
	      partition_table[i].end[2] | ((partition_table[i].end[1] & 0xc0) << 2),
	      partition_table[i].end[0],
	      partition_table[i].end[1] & 0x3f);
#endif
    }

}

void
ide_read_dump (char **av)
{
  int	cylinder, sector, head;
  BYTE  buf[512];
  int	i, j;

  cylinder = atoi (av[1]);
  head  = atoi (av[2]);
  sector = atoi (av[3]);

  boot_printf ("read: c/h/s = %d/%d/%d\n", cylinder, head, sector);
  ide_read_low (0, cylinder, head, sector, buf, 1);
  for (i = 0; i < 128; )	
    {
      for (j = 0; j < 8; j++, i++)
	{
	  if (buf[i] < 0x10)
	    {
	      boot_printf("0x0%x ", buf[i]);
	    }
	  else
	    {
	      boot_printf("0x%x ", buf[i]);
	    }
	}
      boot_printf ("\n");
    }	      
}

void
ide_init (void)
{
  int	status;

#ifdef nodef
  set_idt (INT_IDE, 0x08, (int)int46_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (14);

  set_idt (INT_IDE2, 0x08, (int)int47_handler, INTERRUPT_DESC, 0);	/* 2nd */
  reset_intr_mask (15);							/* 2nd */
#endif

  outb (IDE_DRIVE_REG, 0xA0);			/* �ɥ饤�� 0 ����� */

  outb (IDE_CONTROL_REG, 0x08);			/* �ɥ饤�֤Υꥻ�å� */
  busywait (1000);

  outb (IDE_CONTROL_REG, 0x0a);
  busywait (1000);

  status = inb (IDE_STAT_REG);
  boot_printf ("IDE init status = 0x%x\n", status);

  get_ide_parameter (&ide_parameter);

  read_partition_table (0, partition_table);
}

/* IDE HD �Υƥ��ȴؿ� 
 *
 * �ǥ������ѥ�᡼������Ϥ���
 */
int get_ide_parameter (struct ide_id *id)
{
  int	i;
  UWORD16 *p;

  lock ();
  outb (IDE_DRIVE_REG, 0xA0);			/* �ɥ饤�� 0 ����� */
  busywait (1000);
  ide_send_command (IDE_CMD_READ_INFO);
  p = (UWORD16 *)id;
  if (ide_chk_drq ())
    {
      for (i = 0; i < sizeof (struct ide_id) / 2; i++)
	{
	  *p = inw (IDE_DATA_REG);
	  p++;
	}
    }
  else
    {
      boot_printf ("drive busy. (%d)\n", __LINE__);
    }

  unlock ();

#ifdef nodef
  for (i = 0; i < 20; i++)
    {
      boot_printf ("%c", id->model[i * 2 + 1]);
      boot_printf ("%c", id->model[i * 2]);
    }
#endif
  boot_printf ("\n");

  return E_OK;
}


int
ide_read_low (int drive, int cyl_b, int head_b, int sec_b, char *buf, int length)
{
  int	stat;
  int	i;
  UWORD16 *tmpbuf;
  int	cyl, head, sec;

  cyl = cyl_b;
  head = head_b;
  sec = sec_b;

  length *= IDE_BLOCK_SIZE;

  ide_intr_flag = FALSE;
  while (inb (IDE_STAT_REG) & 0x80)
    ;

  lock ();
#ifdef nodef
  boot_printf ("IDE set parameter.\n");
#endif
  outb (IDE_DRIVE_REG, 0xA0 | ((drive << 4) | head));
  outb (IDE_SCOUNT_REG, 1);
  outb (IDE_CYL_LOW_REG, cyl & 0xff);
  outb (IDE_CYL_HIGH_REG, (cyl >> 8) & 0xff);
  outb (IDE_SNUMBER_REG, sec & 0xff);

#ifdef IDEDEBUG
  boot_printf ("IDE send command.\n");
#endif
  ide_send_command (IDE_CMD_READ);
  unlock ();

  if (!(inb (IDE_STAT_REG) & IDE_DRQ))
    {
      busywait (1);

#ifdef nodef
      wait_int (&ide_intr_flag);
#endif
    }

  while ((stat = inb (IDE_STAT_REG)) & 0x80)
    ;

#ifdef nodef
  boot_printf ("IDE read: stat = 0x%x\n", stat);
#endif
  
  if (stat & 0x01)
    {
      boot_printf ("IDE read error.\n");
      return (0);
    }

  tmpbuf = (UWORD16 *)buf;
  for (i = 0; i < length / 2; i++)
    {
      tmpbuf[i] = inw (IDE_DATA_REG);
    }

  ide_enable_int (drive);
  return (length);
}


int
read_partition_table (int drive, struct ide_partition *table)
{
  BYTE buf[IDE_BLOCK_SIZE];
  int	i;

  ide_read_low (0, 0, 0, 1, buf, 1);
  bcopy (&buf[PARTITION_OFFSET], table, sizeof (struct ide_partition) * 4);

  for (i = 0; i < IDE_MAX_PARTITION; i++)
    {
      boot_printf ("partition[%d] type = 0x%x, start = %d, length = %d, bootable = %d\n",
	      i, 
	      table[i].type,
	      table[i].start,
	      table[i].length,
	      table[i].boot_flag);
      boot_printf ("begin(CHS): %d/%d/%d\n",
	      table[i].begin[2] | ((table[i].begin[1] & 0xc0) << 2),
	      table[i].begin[0],
	      table[i].begin[1] & 0x3f);
      boot_printf ("end(CHS): %d/%d/%d\n",
	      table[i].end[2] | ((table[i].end[1] & 0xc0) << 2),
	      table[i].end[0],
	      table[i].end[1] & 0x3f);
    }

  return E_OK;
}

int
ide_read_block_1k (int drive, int partition, unsigned int block, BYTE *buf, int length)
{
  int	err;

  err = ide_read (drive, partition, block * (BLOCK_SIZE / IDE_BLOCK_SIZE), buf, length * (BLOCK_SIZE / IDE_BLOCK_SIZE));
  return (err);
}


int
ide_read (int drive, int partition, ULONG block, BYTE *buf, int length)
{
  unsigned int	true_block;
  int		i;
  int		cylinder, sector, head;

#ifdef nodef
  bzero ((void *)buf, length * IDE_BLOCK_SIZE);
#endif

  if ((block > partition_table[partition].length) || (block < 0))
    {
      boot_printf ("Block numer is invalid (blockno = %d)\n", (int)block);
      boot_printf ("Total block of partition is %d\n", partition_table[partition].length);
      boot_printf ("Buffer address = 0x%x, length = %d\n", (int)buf, length);
      return (E_PAR);
    }

  true_block = partition_table[partition].start + block;

  if (true_block < 0)
    {
      int	i;

      boot_printf ("ERROR!! read block number is invalid!.\n");
      boot_printf ("true_block = %d, partition = %d, startblock = %d, block = %d\n",	/*  */
	      true_block, partition, partition_table[partition].start, block);
      boot_printf ("Buffer address = 0x%x, length = %d\n", buf, length);

      for (i = 0; i < IDE_MAX_PARTITION; i++)
	{
	  boot_printf ("partition[%d] type = 0x%x, start = %d, length = %d, bootable = %d\n",
		  i, 
		  partition_table[i].type,
		  partition_table[i].start,
		  partition_table[i].length,
		  partition_table[i].boot_flag);
	  boot_printf ("begin(CHS): %d/%d/%d\n",
		  partition_table[i].begin[2] | ((partition_table[i].begin[1] & 0xc0) << 2),
		  partition_table[i].begin[0],
		  partition_table[i].begin[1] & 0x3f);
	  boot_printf ("end(CHS): %d/%d/%d\n",
		  partition_table[i].end[2] | ((partition_table[i].end[1] & 0xc0) << 2),
		  partition_table[i].end[0],
		  partition_table[i].end[1] & 0x3f);
	}
      for (;;)
	;
      return (E_PAR);
    }

  for (i = 0; i < length; i++)
    {
      cylinder = (true_block + i) / (ide_parameter.n_head * ide_parameter.n_sector);
      head =     ((true_block + i) % (ide_parameter.n_head * ide_parameter.n_sector)) / ide_parameter.n_sector;
      sector =   ((true_block + i) % ide_parameter.n_sector) + 1;

      ide_read_low (drive, cylinder, head, sector, (buf + (i  * IDE_BLOCK_SIZE)), 1);
    }

  return (E_OK);
}

int
disp_string (char *s, int length)
{
  int	i;

  for (i = 0; i < (length / 2); i += 2)
    {
      if ((s[i + 1]) == 0)
	putchar (' ');
      else
	putchar (s[i + 1]);

      if ((s[i]) == 0)
	putchar (' ');
      else
	putchar (s[i]);
    }

  return E_OK;
}


int
ide_boot (char **arg)
{
  int			drive = 0;
  int			partition = 0;
  BYTE buf[IDE_BLOCK_SIZE];
  struct boot_header	*infop;
  int			*second_boot_size;
  

  if (ide_read (0, partition, 1, buf, 1) != 1)
    {
      boot_printf ("cannot read from IDE HD.\n");
      return (-1);
    }

  second_boot_size = (int *)buf;
  boot_printf ("2nd boot size (512 block) = %d\n", *second_boot_size);
  boot_printf ("kernel start block = %d\n", 2 + *second_boot_size);

  infop = (struct boot_header *)MODULE_TABLE;
  ide_read (drive, partition, 2 + *second_boot_size, (char *)infop, 1);
  if (infop->cookie == 0x0001)
    {
      boot_printf ("read multiple module.\n");
      ide_read_multi_module (drive, partition, 2 + *second_boot_size, infop);
      boot_printf ("boot end\n");
    }
  else
    {
      boot_printf ("Unknown kernel module.(magic ID = %d)\n");
    }

  return E_OK;
}


/*
   ʣ���Υ⥸�塼����ɤ߹��ࡣ

   �ǽ�Υ⥸�塼��Τ� ITRON �����ͥ�Ȳ��ꤷ�Ƥ��롣
   ���Τ��ᡢ�ɤ߹�����֤ϡ�0x00010000 �ȷ��Ƥ��롣
   (ITRON �����ͥ�ϡ����ۥ��ɥ쥹 0x80010000 ���ɤ߹��ळ�Ȥˤ��Ƥ��롣
   0x00010000 �Ȥ����Τϡ�0x80010000 ���б����Ƥ���ʪ�����ɥ쥹�Ǥ���)

   2 ���ܰʹߤΥ⥸�塼��ϡ�ITRON �����ͥ�ˤθ��³�����ɤ߹��ळ�Ȥˤʤ롣
   ���Τ��ᡢITRON �����ͥ���礭���ˤ�ä�ʪ�����ɥ쥹���Ѥ�뤳�Ȥˤʤ롣
   2 ���ܰʹߤΥ⥸�塼��ˤĤ��Ƥϡ�boot �ϥ��ɤ�������ǡ����ۥ��ɥ쥹
   �ؤΥޥåԥ󥰤ʤɤ� ITRON �����ͥ�ε�ư��˹Ԥ���

*/ 
int
ide_read_multi_module (int drive, int partition, int offset, struct boot_header *info)
{
  int	i;
  int	bn;
  void  (*entry)();

  boot_printf ("Multiple module boot.\n");
  info->machine.base_mem = base_mem;
  info->machine.ext_mem = ext_mem;
  info->machine.real_mem = real_mem;

  boot_printf ("Module %d\n", info->count);
  bn = offset + (BLOCK_SIZE / IDE_BLOCK_SIZE);	/* �ǽ�Υ⥸�塼�뤬���äƤ���֥�å��ֹ� */
  entry = (void (*)())(info->modules[0].entry);
  for (i = 0; i < info->count; i++)
    {
      boot_printf ("[%d] Baddr = %d, paddr = 0x%x, vaddr = 0x%x, length = %d\n", 
	      i,
	      bn,
	      info->modules[i].paddr,
	      info->modules[i].vaddr,
	      info->modules[i].length);

      ide_read_module (drive, partition, bn, &(info->modules[i]));
      bn += (info->modules[i].length / IDE_BLOCK_SIZE) - 2;
    }
  boot_printf ("load done.\n");
  boot_printf ("exec_info->a_entry = 0x%x\n", entry); 
  (*entry)();

  return E_OK;
}


int
ide_read_module (int drive, int partition, int bn, struct module_info *moduleinfo)
{
  char	buf[BLOCK_SIZE];
  char	tmp[BLOCK_SIZE];
  int	i, j;
  struct exec	*exec_info;
  void  *paddr;

  paddr = (void *)(moduleinfo->paddr);
  ide_read (drive, partition, bn, tmp, 1);
  exec_info = (struct exec *)tmp;
  if (N_BADMAG (*exec_info))
    {
      boot_printf ("This object is not exec format (%d).\n", *exec_info);
      for (;;)
	;
      /* STOP HERE */
    }
  
  if ((N_MAGIC(*exec_info) == 0413) || (N_MAGIC(*exec_info) == NMAGIC))
    {
      bn += (BLOCK_SIZE / IDE_BLOCK_SIZE);	/* a.out �Υإå����礭������������ȥ��åפ��� */

      for (i = 0;
	   i < (ROUNDUP (exec_info->a_text, PAGE_SIZE) 
		 / IDE_BLOCK_SIZE);
	   i++, bn++)
	{
	  boot_printf (".");
	  ide_read (drive, partition, bn, buf, 1);
	  bcopy (buf,
		 (ULONG)(paddr + i * IDE_BLOCK_SIZE),
		 IDE_BLOCK_SIZE);
	}
/*      boot_printf ("\nText region is readed.\n"); */
      for (j = 0;
	   j <= (ROUNDUP (exec_info->a_data, PAGE_SIZE)
		 / IDE_BLOCK_SIZE);
	   j++ , bn++)
	{
	  boot_printf (".");
	  ide_read (drive, partition, bn, buf, 1);
	  bcopy (buf,
		 (ULONG)(paddr
				 + (ROUNDUP (exec_info->a_text, PAGE_SIZE)) 
				 + j * IDE_BLOCK_SIZE),
		 IDE_BLOCK_SIZE);
	}
    }
  else
    {
      boot_printf ("I am unknown a.out image.(0x%x)\n", N_MAGIC(*exec_info));
      for (;;)
	;
    }
/*  boot_printf ("\nload done.\n"); */
  boot_printf ("\n");

  return E_OK;
}


