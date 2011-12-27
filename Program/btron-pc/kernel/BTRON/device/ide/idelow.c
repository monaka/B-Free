/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ide/idelow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ide/idelow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#include "ide.h"


static int
ide_check_ready ()
{
  return (inb (IDE_STAT_REG) & 0x40);
}

static int
ide_wait_while_busy ()
{
  while (inb (IDE_STAT_REG) & 0x80);
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
}

static int
ide_disable_int ()
{
  outb (IDE_CONTROL_REG, 0x02);
}

static int
ide_send_command (int cmd)
{
  ide_check_ready ();
  outb (IDE_CONTROL_REG, 0x00);
  ide_wait_while_busy ();
  outb (IDE_COMMAND_REG, cmd);
  ide_wait_while_busy ();
  return (inb (IDE_ERROR_REG));
}



/* read_stat - 
 *
 */
ER
read_stat (W drive, struct ide_id *stat)
{
  UH		*p;
  W		i;
  W		N;

#ifdef notdef
  dbg_printf ("IDE:read_stat start\n");
#endif
  ide_send_command (IDE_CMD_READ_INFO);
  p = (unsigned short *)stat;
  if (ide_chk_drq ())
    {
      for (i = 0; i < sizeof (struct ide_id) / 2; i++)
	{
	  *p = inw (IDE_DATA_REG);
	  p++;
	}
    }
  
#ifdef notdef
  printf ("IDE ID\n");
  printf ("  CYL:  %d\n", stat->n_cylinder);
  printf ("  HEAD: %d\n", stat->n_head);
  printf ("  SEC:  %d\n", stat->n_sector);
#endif
/*
  printf ("  CYL(EIDE):  %d\n", stat.now_cylinder);
  printf ("  HEAD(EIDE): %d\n", stat.now_head);
  printf ("  SEC(EIDE):  %d\n", stat.now_sector);
*/

#ifdef notdef
  N = calc_N (stat->n_cylinder);
  stat->now_cylinder = (stat->n_cylinder) / N;
  stat->now_head = (stat->n_head) * N;
  stat->now_sector_track = (stat->n_sector);
#endif

  return (E_OK);
}


calc_N (int cylinder)
{
  int	N, N2;

  if (cylinder > 1024)
    {
      cylinder = ROUNDUP (cylinder, 1024);
      N = ((cylinder - 1) / 1024) + 1;

      for (N2 = 1; N2 < N; N2 = N2 * 2)
	{
	  ;
	}
      return (N2);
    }

  return (1);
}


ER
get_data (W drive, UW head, UW cyl, UW sec, B *buf, W length)
{
  W	stat;
  W	i;
  UH	*tmpbuf;

  length *= IDE_BLOCK_SIZE;

  ide_intr_flag = FALSE;
  while (inb (IDE_STAT_REG) & 0x80)
    ;

  dis_int ();
  outb (IDE_DRIVE_REG, (drive << 4) | head);
  outb (IDE_SCOUNT_REG, 1);
  outb (IDE_CYL_LOW_REG, cyl & 0xff);
  outb (IDE_CYL_HIGH_REG, (cyl >> 8) & 0xff);
  outb (IDE_SNUMBER_REG, sec & 0xff);

  ide_send_command (IDE_CMD_READ);
  ena_int ();

  if (!(inb (IDE_STAT_REG) & IDE_DRQ))
    {
#ifdef notdef
      wait_int (&ide_intr_flag);
#endif
    }

  while ((stat = inb (IDE_STAT_REG)) & 0x80)
    ;

  if (stat & 0x01)
    {
      dbg_printf ("IDE read error. stat = 0x%x\n", stat);
      return (0);
    }

  tmpbuf = (unsigned short *)buf;
  for (i = 0; i < length / 2; i++)
    {
      tmpbuf[i] = inw (IDE_DATA_REG);
    }

  ide_enable_int (drive);
  return (length);
}




ER
put_data (W drive, UW head, UW cyl, UW sec, B *buf, W length)
{
  W	stat;
  W	i;
  UH	*tmpbuf;

  length *= IDE_BLOCK_SIZE;

  ide_intr_flag = FALSE;
  while (inb (IDE_STAT_REG) & 0x80)
    ;

  dis_int ();
  outb (IDE_DRIVE_REG, (drive << 4) | head);
  outb (IDE_SCOUNT_REG, 1);
  outb (IDE_CYL_LOW_REG, cyl & 0xff);
  outb (IDE_CYL_HIGH_REG, (cyl << 8) & 0xff);
  outb (IDE_SNUMBER_REG, sec & 0xff);

  ide_send_command (IDE_CMD_WRITE);
  ena_int ();

  if (!(inb (IDE_STAT_REG) & IDE_DRQ))
    {
#ifdef notdef
      wait_int (&ide_intr_flag);
#endif
    }

  while ((stat = inb (IDE_STAT_REG)) & 0x80)
    ;

  if (stat & 0x01)
    {
      dbg_printf ("IDE read error. stat = 0x%x\n", stat);
      return (0);
    }

  tmpbuf = (unsigned short *)buf;
  for (i = 0; i < length / 2; i++)
    {
      outw (IDE_DATA_REG, tmpbuf[i]);
    }

  ide_enable_int (drive);
  return (length);
}


/**************************************************************************
 *
 */
int
wait_int (int *flag)
{
  ena_int ();
  while (*flag == FALSE)
    ;
  dis_int ();
  *flag = FALSE;
  ena_int ();
  return (*flag);
}
