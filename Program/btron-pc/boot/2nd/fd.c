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
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/fd.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: fd.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.16  2000-06-29 08:36:48  naniwa
 * to discard some busywait calls
 *
 * Revision 1.15  2000/01/18 14:32:47  naniwa
 * modified fd_seek
 *
 * Revision 1.14  2000/01/08 09:03:39  naniwa
 * minor tune
 *
 * Revision 1.13  1999/12/21 10:53:54  naniwa
 * minor fix
 *
 * Revision 1.12  1999/12/19 10:57:03  naniwa
 * made disk read check strict
 *
 * Revision 1.11  1999/05/04 12:51:37  naniwa
 * modified to speed up boot from FD
 *
 * Revision 1.10  1999/03/15 01:35:21  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.9  1998/11/20 08:02:26  monaka
 * *** empty log message ***
 *
 * Revision 1.8  1997/06/29 13:13:45  night
 * HD の SFS フォーマットされたパーティションからの BTRON OS をロードできる
 * 機能を追加。
 *
 * Revision 1.7  1997/05/14 15:27:43  night
 * specify コマンドのパラメータ変更。
 * (linux の fd ドライバの値に合わせた)
 * 速度向上のための修正。
 *
 * Revision 1.6  1996/07/24  16:45:27  night
 * 読み込み時のリトライ回数を 2 回から 10 回に増やした。
 *
 * Revision 1.5  1996/07/24  14:02:05  night
 * 余分な print 文を削除。
 *
 * Revision 1.4  1996/07/06  17:33:21  night
 * デバッグ用の boot_printf を FDDEBUG で囲んだ
 *
 * Revision 1.3  1996/07/06  13:09:02  night
 * Floppy disk 関連の処理を PC 互換機用に変更
 *
 * Revision 1.2  1996/06/11  16:40:32  night
 * 当面必要としない on_motor (FD のモーターを制御する関数) の呼び出しを
 * 行わないようにした。
 *
 * Revision 1.1  1996/05/11  10:45:02  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * ------------------------------------------------------------------
 *
 * Revision 1.4  1995/09/21 15:50:37  night
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

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/fd.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "types.h"
#include "lib.h"
#include "interrupt.h"
#include "idt.h"
#include "fd.h"
#include "dma.h"
#include "errno.h"
#include "file.h"
#include "asm.h"

#define ON	1
#define OFF	0

#define MAX_RETRY	10

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

/* #define DMA_MASK	0x02 */

struct spec
{
  UWORD32 srt;
  UWORD32 hut;
  UWORD32 hlt;
  UWORD32 nd;
};

struct status
{
  BYTE status_reg;
  BYTE status_data[8];

  WORD32 motor;
};

struct status		fd_status;
static int		intr_flag = FALSE;
static struct spec	fd_spec[] =
{
  { 0x0C,  0x0f,   0x08, 0x0 },		/* 1.44M */
#ifdef nodef
  { 0x04,  128,   12, 0x0 },		/* 1.44M */
#endif
  { 0x0d, 0x06, 0x0f, 0x0 },		/* HD */
  { 0x0d, 0x06, 0x0f, 0x0 },
  { 0x0d, 0x06, 0x0e, 0x0 }
};
static int motor_goal = 0;
static int motor_status;

int fd_get_data (BYTE drive, int head, int cylinder, int sector, void *buff);

/************************************************************************
 *
 */
int
write_fdc (BYTE value)
{
  int i;
  UWORD32 status;

#ifdef FDDEBUG
  boot_printf ("write_fdc: 0x%x\n", value);
#endif  
#ifdef notdef
  busywait (1000);
#endif
  i = 1000;
  status = inb (FDC_STAT);
  while ((status & (FDC_MASTER | FDC_DIN)) != (FDC_MASTER | 0)) 
    {
      if (i < 0)
	{
	  boot_printf ("write_fdc: timeout!! status = 0x%x\n", status);
	  break;
/*	  return (0); */
	}
      i--;
      status = inb (FDC_STAT);
    }

  outb(FDC_DATA, value);
  return (1);
}

/************************************************************************
 *
 */
int
init_fd (void)
{
#ifdef FDDEBUG
  boot_printf ("init_fd: ");
#endif

  set_idt (38, 0x08, (int)int38_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (6);
  
  fd_reset ();

  return E_OK;
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
  lock ();
  intr_flag = 1;
#ifdef FDDEBUG
  boot_printf ("!! fd interrupt!!\n");
#endif
  unlock ();
}


BOOL
fdc_isense ()
{
  int	result_nr = 0;
  int	status;

  write_fdc (FDC_SENSE);

  status = inb (FDC_STAT);
  for (;;)
    {
      if (status == (FDC_MASTER | FDC_DIN | FDC_BUSY)) 
	{
	  if (result_nr >= 8) 
	    {
#ifdef FDDEBUG
	      boot_printf ("\n");
#endif
	      break;	/* too many results */
	    }
	  fd_status.status_data[result_nr++] = inb (FDC_DATA);
#ifdef FDDEBUG
	  boot_printf ("isense: 0x%x ", 
		  fd_status.status_data[result_nr - 1]);
#endif
	}
      status = inb (FDC_STAT) & (FDC_MASTER | FDC_DIN | FDC_BUSY);
      if (status == FDC_MASTER) 
	{	/* all read */
	  return (TRUE);
	}
    }
#if 0
  need_reset = TRUE;		/* controller chip must be reset */
#endif
  return (FALSE);
}

BOOL
fdc_sense ()
{
  int	result_nr = 0;
  int	status;

  status = inb (FDC_STAT);
  for (;;)
    {
      if (status == (FDC_MASTER | FDC_DIN | FDC_BUSY)) 
	{
	  if (result_nr >= 8) 
	    {
#ifdef FDDEBUG
	      boot_printf ("\n");
#endif
	      break;	/* too many results */
	    }
	  fd_status.status_data[result_nr++] = inb (FDC_DATA);
#ifdef FDDEBUG
	  boot_printf ("isense: 0x%x ", 
		  fd_status.status_data[result_nr - 1]);
#endif
	}
      status = inb (FDC_STAT) & (FDC_MASTER | FDC_DIN | FDC_BUSY);
      if (status == FDC_MASTER) 
	{	/* all read */
	  return (TRUE);
	}
    }
#if 0
  need_reset = TRUE;		/* controller chip must be reset */
#endif
  return (FALSE);
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
#ifdef notdef
      status = status & (FDC_BUSY | FD0_BUSY | FD1_BUSY | FD2_BUSY | FD3_BUSY);
#else
      status = status & FDC_BUSY;
#endif
      if (status == 0)
	return E_OK;

#ifdef notdef
      busywait (1000);
#endif
    }
#ifdef FDDEBUG
  boot_printf ("FD not ready.\n");
#endif
  return (status);
}

/************************************************************************
 *
 */
int fd_get_status (BYTE drive, int datan)
{
  int	status;
  int	status_count = 0;
  int	i;
  int	tmp;

  for (i = 0; i < MAX_RETRY; i++)
    {
      status = inb (FDC_STAT);
      status &= (FDC_MASTER | FDC_DIN | FDC_BUSY);
      if ((status & (FDC_MASTER | FDC_DIN)))
	{
	  tmp = inb (FDC_DATA);
	  fd_status.status_data[status_count] = tmp;
#ifdef FDDEBUG
	  boot_printf ("stauts (%d) = 0x%x\n",
		  status_count,
		  fd_status.status_data[status_count]);
#endif
	  status_count++;
	  i = 0;
	  if (status_count >= datan)
	    return (status);
	}
      else if (status == FDC_MASTER)
	{
	  return (status);
	}
    }
#ifdef FDDEBUG
  boot_printf ("Can't result from FDC.\n");
#endif
  return (-1);
}

/************************************************************************
 * fd_recalibrate
 */
int
fd_recalibrate (BYTE drive)
{
  fd_ready_check ();
  write_fdc (FDC_RECALIBRATE);
  write_fdc (drive);
#ifdef FDDEBUG
  boot_printf ("fd_recalib: wait_int\n");
#endif  
  wait_int (&intr_flag);
  fdc_isense ();

  return (fd_status.status_data[0]);
}

/************************************************************************
 * fd_specify ---
 */
int
fd_specify (UWORD32 srt,
	    UWORD32 hut,
	    UWORD32 hlt,
	    UWORD32 nd)
{
  fd_ready_check ();
  write_fdc (FDC_SPECIFY);
#if 1
  write_fdc (0xdf);
  write_fdc (0x02);

#else
#ifdef nodef
  write_fdc (0xc8);
  write_fdc (0x18);
#endif
  
  write_fdc (((srt << 4) | (hut & 0x0f)) & 0xff);
  write_fdc (((hlt << 1) | (nd & 0x01)) & 0xff);
#endif
  outb(FDC_DCR, 0x00);

  return E_OK;
}

/************************************************************************
 *
 */
int
on_motor (BYTE drive)
{
  int motor_bit, running;

  motor_bit = 1 << drive;		/* 動かしたい FD drive 番号を
					   ビットマップに変換 */
  running = motor_status & motor_bit;	/* nonzero if this motor is running */
  motor_goal = motor_status | motor_bit;/* want this drive running too */

  outb (FDC_WCNTL, (motor_goal << MOTOR_SHIFT) | 0x0c | drive);
  motor_status = motor_goal;

  /* If the motor was not running yet, we have to wait for it. */
  if (!running)
    {
      busywait (1000);
    }

  return E_OK;
}


/************************************************************************
 *
 */
int
stop_motor (BYTE drive)
{
  motor_goal = 0;
  if (motor_goal != motor_status) {
	outb (FDC_WCNTL, (motor_goal << MOTOR_SHIFT) | 0x0c);
	motor_status = motor_goal;
  }

  return E_OK;
}


int
fd_read_with_partition (int drive, int part, int blockno, BYTE *buff, int length)
{
  int result;
  result = fd_read (drive, blockno, buff) ;

  return result;
}

/************************************************************************
 *
 */
int
fd_read (BYTE drive,
	 int blockno,
	 BYTE *buff)
{
  int	cylinder;
  int	head;
  int	sector;
  int	i;
  int	ret;
  int	readcount;
  int   s;

  on_motor (0);
  blockno *= 2;
  for (readcount = 0; readcount < (BLOCK_SIZE / HD_LENGTH); readcount++, blockno++, buff += HD_LENGTH)
    {
      /* calculate parameter. */
      head = (blockno % (HD_HEAD * HD_SECTOR)) / HD_SECTOR;
      cylinder = (blockno / (HD_HEAD * HD_SECTOR));
      sector = (blockno % HD_SECTOR) + 1;

#if FDDEBUG
      boot_printf ("head = %d, cylinder = %d, sector = %d\n",
	      head, cylinder, sector);
#endif

      intr_flag = FALSE;
      for (i = 0; i < MAX_RETRY; i++)
	{
#ifdef notdef
	  outb (FDC_DCR, 0);
#endif
	  ret = fd_seek (drive, head, cylinder, 0);
	  if (ret != TRUE) continue;
	  setup_dma ((void *)buff, DMA_READ, HD_LENGTH, DMA_MASK);
	  fd_get_data (drive, head, cylinder, sector, buff);
	  wait_int (&intr_flag);
	  ret = fdc_sense ();
	  if (ret != TRUE) continue;
#ifdef notdef
	  if ((fd_status.status_data[0] & 0x40) == 0x40)
	    {
	      boot_printf ("Read failed.\n");
	    }
#endif
	  if ((fd_status.status_data[0] & 0xF8) != 0x00) continue;
	  if (fd_status.status_data[1] | fd_status.status_data[2] != 0x00)
	    continue;
	  s = (fd_status.status_data[3] - cylinder) * HD_HEAD * HD_SECTOR;
	  s += (fd_status.status_data[4] - head) * HD_SECTOR;
	  s += (fd_status.status_data[5] - sector);
	  if (s * 512 != BLOCK_SIZE) continue;
	  else
	    {
#ifdef FDDEBUG
	      boot_printf ("READ SUCCESS\n");
#endif
	      ret = E_OK;
	      break;
	    }
	}
      if (i >= MAX_RETRY)
	{
	  boot_printf ("Read failed.\n");
	  stop_motor (0);
	  return (E_DEV);
	}
    }
  /*  stop_motor (0);*/
  return (E_OK);
}

/************************************************************************
 *
 */
int
fd_seek (BYTE drive, int head, int cylinder, int motor)
{
  int	result;

  on_motor (0);
  fd_ready_check ();
#ifdef FDDEBUG
  boot_printf ("fd_seek:write to FDC.\n");
#endif

  intr_flag = FALSE;
  write_fdc (FDC_SEEK);
  write_fdc ((head << 2) | (drive & 0x03));
  write_fdc (cylinder);

#ifdef FDDEBUG
  boot_printf ("fd_seek:wrote to FDC.\n");
#endif
  wait_int (&intr_flag);
  fdc_isense ();
#ifdef notdef
  result = fd_status.status_data[0];
#endif
  result = TRUE;
  if ((fd_status.status_data[0] & 0xF8) != 0x20) result = FALSE;
  if (fd_status.status_data[1] != cylinder) result = FALSE;

#ifdef notdef
  busywait (100);
#endif

/*
  if (motor)
    stop_motor (0);
  wait_int (&intr_flag);
  fdc_isense ();
*/
  return (result);
}

/************************************************************************
 * fd_get_data
 */
int
fd_get_data (BYTE drive, int head, int cylinder, int sector, void *buff)
{
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
/*  reset_intr_mask (6); */
  return (TRUE);
}

/************************************************************************
 *
 */
int
fd_reset (void)
{
#ifdef nodef
  lock ();
  intr_flag = FALSE;
  outb (FDC_WCNTL, 0);
  outb (FDC_WCNTL, 0x0c | 0x04);
  unlock ();
  wait_int (&intr_flag);

  boot_printf ("collect interrupt sense data.\n");
  for (i = 0; i < 4; i++)
    {
      fdc_isense ();
    }

#endif

  on_motor (0);

#ifdef FDDEBUG
  boot_printf ("fd_reset: wait...\n");
#endif  

  fd_specify (fd_spec[HD_TYPE].srt,
	      fd_spec[HD_TYPE].hut,
	      fd_spec[HD_TYPE].hlt,
	      fd_spec[HD_TYPE].nd);

  stop_motor (0);

  return E_OK;
}
