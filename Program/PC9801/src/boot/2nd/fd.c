/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT floppy driver routines.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/fd.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: fd.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:50:37  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1994/07/30  17:37:15  night
 * ファイル中の日本語文字をすべて EUC コードに変更。
 *
 * Revision 1.2  1994/07/05  17:19:17  night
 * Change FD format; 512bytes/block.
 *
 * Revision 1.1  1993/10/11  21:28:57  btron
 * btron/386
 *
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/fd.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "interrupt.h"
#include "idt.h"
#include "fd.h"
#include "dma.h"
#include "errno.h"
#include "file.h"

#define ON	1
#define OFF	0

#define MAX_RETRY	100

/* Write control registers bit. IO port address 0x94 */
#define FDC_RESET	0x80
#define FDC_FRY		0x40
#define FDC_DMAE	0x10
#define FDC_MTON	0x08

/* External status bit. IO port address 0x94 */
#define FDC_FINT1	0x80
#define FDC_FINT0	0x40
#define FDC_DMACH	0x20
#define FDC_PSTB	0x10

#define DMA_MASK	0x02

struct spec
{
  unsigned int		srt;
  unsigned int		hut;
  unsigned int		hlt;
  unsigned int		nd;
};

struct status
{
  unsigned char		status_reg;
  unsigned char		status_data[8];

  int			motor;
};

static struct status	fd_status;
static int		intr_flag;
static struct spec	fd_spec[] =
{
  { 0x0d, 0x06, 0x0f, 0x0 },		/* HD */
  { 0x0d, 0x06, 0x0f, 0x0 },
  { 0x0d, 0x06, 0x0e, 0x0 }
};


/************************************************************************
 *
 */
int
write_fdc (unsigned int value)
{
  unsigned int	status;
  int		i;

  for (i = 0; i < 20; i++)
    {
      status = inb (FDC_STAT);
      if ((status & (FDC_MASTER | FDC_DIN)) == (FDC_MASTER | FDC_DIN))
	{
	  int data;

#ifdef FDDEBUG
	  printf ("request read from FDC.\n");
#endif
	  data = inb (FDC_DATA);
#ifdef FDDEBUG
	  printf ("data is = 0x%x\n", data);
#endif
	}
      else if ((status & (FDC_MASTER | FDC_DIN)) == FDC_MASTER)
	{
#ifdef FDDEBUG
	  printf ("write to FDC. -- 0x%x\n", value);
#endif /* DEBUG */
	  outb (FDC_CMD, value);
	  return 0;
	}
      busywait (100);
    }
#ifdef FDDEBUG
  printf ("can't send to FDC.\n");
  printf ("must be reset FDC!!\n");
#endif
  return (1);
}

/************************************************************************
 *
 */
int
init_fd (void)
{
  int status;
  
#ifdef FDDEBUG
  printf ("init_fd: ");
#endif

  set_idt (43, 0x08, (int)int43_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (11);
  
  fd_reset ();
}

/************************************************************************
 * intr_fd
 *
 *	フロッピィディスクの割り込み処理関数
 *
 */
void
intr_fd (void)
{
  unsigned char	status;
  int		i;
  
  lock ();
  intr_flag = 1;
#ifdef FDDEBUG
  printf ("!! fd interrupt!!\n");
#endif
  unlock ();

#ifdef notdef
  status = inb (FDC_STAT);
  fd_status.status_reg = status;
  if (status & FDC_MASTER)
    {
      beep ();
      for (i = 0; i < 8; i++)
	{
	  fd_status.status_data[i] = inb (FDC_DATA);
	  status = inb (FDC_STAT);
	  if ((status & (FDC_MASTER | FDC_BUSY)) == 0)
	    break;
	}
    }
#endif
}

/***************************************************************************
 * fd_ready_check --- 
 */
int
fd_ready_check (void)
{
  int	i;
  int	status;
  
  for (i = 0; i < MAX_RETRY; i++)
    {
      status = inb (FDC_STAT);
      status = status & (FDC_BUSY | FD0_BUSY | FD1_BUSY | FD2_BUSY | FD3_BUSY);
      if (status == 0)
	return 0;
      busywait (1000);
    }
#ifdef FDDEBUG
  printf ("FD not ready.\n");
#endif
  return (status);
}

/************************************************************************
 *
 */
int fd_get_status (int drive, int datan)
{
  int	status;
  int	status_count = 0;
  int	i;
  int	tmp;

  for (i = 0; i < MAX_RETRY; i++)
    {
      status = inb (FDC_STAT);
      status &= (FDC_MASTER | FDC_DIN | FDC_BUSY);
      if (status == (FDC_MASTER | FDC_DIN | FDC_BUSY))
	{
	  tmp = inb (FDC_DATA);
	  fd_status.status_data[status_count] = tmp;
#ifdef FDDEBUG
	  printf ("stauts (%d) = 0x%x\n",
		  status_count,
		  fd_status.status_data[status_count]);
#endif
	  status_count++;
	  i = 0;
	}
      else if (status == FDC_MASTER)
	{
	  return (status);
	}
    }
#ifdef FDDEBUG
  printf ("Can't result from FDC.\n");
#endif
  return (-1);
}

/************************************************************************
 * fd_recalibrate
 */
int
fd_recalibrate (int drive)
{
  fd_ready_check ();
  write_fdc (FDC_RECALIBRATE);
  write_fdc (drive);
  wait_int (&intr_flag);
  return (0);
}

/************************************************************************
 * fd_specify ---
 */
int
fd_specify (unsigned int srt,
	    unsigned int hut,
	    unsigned int hlt,
	    unsigned int nd)
{
  fd_ready_check ();
  write_fdc (FDC_SPECIFY);
  write_fdc (0xd6);
  write_fdc (0x1e);
  
/*
  write_fdc (((srt << 4) | hut) & 0xff);
  write_fdc (((hlt << 1) | nd) & 0xff);
*/
  return (0);
}

/************************************************************************
 *
 */
int
on_motor (void)
{
  if (fd_status.motor == OFF)
    {
#ifdef FDDEBUG
      printf ("motor on.\n");
#endif
      outb (FDC_WCNTL, FDC_MTON | FDC_DMAE);
    }
  fd_status.motor = ON;
}

/************************************************************************
 *
 */
int
stop_motor (int drive)
{
}

/************************************************************************
 *
 */
int
fd_read (int drive,
	 int blockno,
	 unsigned char *buff)
{
  int	cylinder;
  int	head;
  int	sector;
  int	length;
  int	i;
  int	ret;
  int	readcount;

  blockno *= 2;
  for (readcount = 0; readcount < (BLOCK_SIZE / HD_LENGTH); readcount++, blockno++, buff += HD_LENGTH)
    {
      /* calculate parameter. */
      head = (blockno % (HD_HEAD * HD_SECTOR)) / HD_SECTOR;
      cylinder = (blockno / (HD_HEAD * HD_SECTOR));
      sector = (blockno % HD_SECTOR) + 1;

#if FDDEBUG
      printf ("head = %d, cylinder = %d, sector = %d\n",
	      head, cylinder, sector);
#endif

      for (i = 0; i < MAX_RETRY; i++)
	{
	  setup_dma ((void *)buff, DMA_READ, HD_LENGTH, DMA_MASK);
	  fd_seek (drive, cylinder);
	  ret = fd_get_data (drive, head, cylinder, sector, buff);
	  if (ret == E_OK)
	    {
#ifdef FDDEBUG
	      printf ("READ SUCCESS\n");
#endif
	      break;
	    }
	}
      if (ret != E_OK)
	return (E_DEV);
    }
  return (E_OK);
}

/************************************************************************
 *
 */
fd_seek (int drive, int cylinder)
{
  fd_recalibrate (0);
/*  fd_ready_check (); */
#ifdef FDDEBUG
  printf ("fd_seek:write to FDC.\n");
#endif
  write_fdc (FDC_SEEK);
  write_fdc (drive & 0x03);
  write_fdc (cylinder);
#ifdef FDDEBUG
  printf ("fd_seek:writed to FDC.\n");
#endif
  wait_int (&intr_flag);			/* */
/*  fd_ready_check ();  	*/
#ifdef FDDEBUG
  printf ("fd_seek: write FD_SENSE to FDC.\n");
#endif
  write_fdc (FDC_SENSE);
#ifdef FDDEBUG
  printf ("fd_seek: writed FD_SENSE to FDC.\n");
#endif
  fd_get_status (drive, 2);
#ifdef FDDEBUG
  printf ("seek status:\n");
  printf ("status is = 0x%x\n", fd_status.status_data[0]);
#endif
  return (fd_status.status_data[0]);
}

/************************************************************************
 * fd_get_data
 */
int
fd_get_data (int drive, int head, int cylinder, int sector, void *buff)
{
  int	i;
  
  lock();
  write_fdc (FDC_READ);
  write_fdc ((head << 2) | drive);
  write_fdc (cylinder);
  write_fdc (head);
  write_fdc (sector);
  write_fdc (HD_LENCODE);
  write_fdc (HD_SECTOR);
  write_fdc (HD_GAP);
  write_fdc (HD_DTL);
  unlock ();
  reset_intr_mask (11);
  wait_int (&intr_flag);
  fd_get_status (drive, 0);
#ifdef FDDEBUG
  printf ("read:FDC status = 0x%x\n", fd_status.status_data[0]);
#endif
  if ((fd_status.status_data[0] & 0xC0) == 0)
    return (E_OK);
  else
    return (E_DEV);
}

/************************************************************************
 *
 */
int
fd_reset (void)
{
  int	data;

  lock ();
  outb (FDC_WCNTL, 0x90);
  busywait (10);
  outb (FDC_WCNTL, 0x10);
  unlock ();
  
  wait_int (&intr_flag);
  if ((inb (FDC_EXTERNAL) & 0x3) != HD_EXTERN)
    {
      outb (FDC_EXTERNAL, HD_EXTERN);
    }

  write_fdc (FDC_SENSE);
  fd_get_status (0, 2);
  fd_specify (fd_spec[HD_TYPE].srt,
	      fd_spec[HD_TYPE].hut,
	      fd_spec[HD_TYPE].hlt,
	      fd_spec[HD_TYPE].nd);
  fd_status.motor = OFF;
  on_motor ();
}
