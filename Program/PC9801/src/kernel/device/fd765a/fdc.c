/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/fdc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/fd765a/fdc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "fd.h"

/*
 *	FDC を制御するための LOW-LEVEL な関数
 */


/*************************************************************************
 * write_fdc --- FDC へのコマンド書き込み
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
write_fdc (UW value)
{
  W	status;
  
  status = inb (FDC_STAT);
  while ((status & (FDC_MASTER | FDC_DIN)) == (FDC_MASTER | FDC_DIN))
    {
      inb (FDC_DATA);
      status = inb (FDC_STAT);
    }
/*  dbg_printf ("write_fdc: 0x%x\n", value);	/* */
  outb (FDC_CMD, value);
  busywait (50);
#ifdef FDDEBUG
  dbg_printf ("write_fdc: end\n");
#endif
  return (TRUE);
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
  B	status;
  
#ifdef FDDEBUG
  dbg_printf ("!!interrupt!!\n");
#endif /* FDDEBUG */
  intr_flag = TRUE;
/*  signal (FD_EVENT); */
}

/*************************************************************************
 * sense_interrupt --- 割り込み要因のチェック
 *
 * 引数：	なし
 *
 * 返値：	
 *
 * 処理：
 *
 */
W
sense_interrupt (W drive, UW *result)
{
  write_fdc (FDC_SENSE);
  if (read_result (drive, result, 2) == FALSE)
    {
#ifdef FDDEBUG
      dbg_printf ("sense interupt: mal function.\n");
#endif
      return (FALSE);
    }
  
  if ((result[0] & ST0_IC) == ST0_IC_NT)
    {
#if FDDEBUG
      dbg_printf ("sense interupt: normal end.\n");
#endif
      return (TRUE);
    }

#if FDDEBUG
  dbg_printf ("sense interupt: mal function.\n");
#endif
  return (FALSE);
}

/***************************************************************************
 * ready_check --- 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
ready_check (void)
{
  W	i;
  W	status;
  
  status = inb (FDC_STAT);
#ifdef FDDEBUG
  dbg_printf ("read_check fdc status = 0x%x\n", status);
#endif
  for (i = 0; i < MAX_FD_RETRY; i++)
    {
      status = status & (FDC_BUSY | FD0_BUSY | FD1_BUSY | FD2_BUSY | FD3_BUSY);
      if (status == 0)
	{
#ifdef FDDEBUG
	  dbg_printf ("read_check:end\n");
#endif
	  return 0;
	}
      busywait (1000);
      status = inb (FDC_STAT);
    }
#ifdef FDDEBUG
  dbg_printf ("FD not ready.\n");
#endif
  return (status);
}

/*************************************************************************
 * read_fdc
 *
 * 引数：	なし
 *
 * 返値：	FDC のステータスレジスタの内容。
 *
 * 処理：	FDC のステータスレジスタの内容を返す。
 *
 */
W
read_fdc (void)
{
  return (inb (FDC_STAT));
}

/*************************************************************************
 * reset_fdc --- FDC のリセットを行う。
 *
 * 引数：	ドライブ番号
 *
 * 返値：	なし
 *
 * 処理：	FDC およびドライバの初期化を行う。
 *
 */
W
reset_fdc (W drive)
{
  W	data;
  UW	result[8];
  W 	count;

#ifdef FDDEBUG
  dbg_printf ("reset_fdc: start\n");
#endif
  dis_int ();
  outb (FDC_WCNTL, 0x90);
  busywait (100);
  outb (FDC_WCNTL, 0x10);
  ena_int ();
  for (count = 0; count < (1024 * 1024 * 3); count++)
    {
      if (intr_flag == TRUE)
	break;
    }
  if (intr_flag == FALSE)
    return (FALSE);

  if ((inb (FDC_EXTERNAL) & 0x3) != HD_EXTERN)
    {
      outb (FDC_EXTERNAL, HD_EXTERN);
    }
  read_result (drive, result, 8);
  if (sense_fdc (drive) == FALSE)
    {
/*      dbg_printf ("fd: reset_fdc(): sense_fdc is FALSE\n");	/* */
      return ;
    }

  specify (drive,
	   fd_data[drive]->srt,
	   fd_data[drive]->hut,
	   fd_data[drive]->hlt,
	   fd_data[drive]->nd);
  recalibrate (drive);
  on_motor (drive);
  return (TRUE);
}

/*************************************************************************
 * 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
sense_fdc (W drive)
{
  UW	result [1];
  
  ready_check ();
  intr_flag = FALSE;
  write_fdc (FDC_DEVSTAT);
  write_fdc (drive);
  if (read_result (drive, result, 1) == FALSE)
    {
#ifdef FDDEBUG
      dbg_printf ("FD NOT READY.\n");
#endif
      return (FALSE);
    }
  if ((result[0] & ST3_FT) || 
      (result[0] & ST3_RY) == 0)
    {
      return (FALSE);
    }
  return (TRUE);
}


/************************************************************************
 * specify ---
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
specify (W drive, UW srt, UW hut, UW hlt, UW nd)
{
  ready_check ();
  write_fdc (FDC_SPECIFY);
  write_fdc (0xdf);
  write_fdc (0x1e);
  return (0);
}

/*************************************************************************
 * read_result --- リザルトステータスレジスタの内容を読み取る。
 *
 * 引数：	drive	ドライブ番号
 *
 * 返値：	TRUE	正常終了
 *		FALSE	ステータスレジスタにデータが入っていない。
 *
 * 処理：	
 *
 */
W
read_result (W drive, UW *result, W n)
{
  W	i;
  W	sense;
  
  sense = read_fdc ();
  if (sense & FDC_DIN)
    {
      for (i = 0; i < n; i++)
	{
	  result[i] = inb (FDC_DATA);
#ifdef FDDEBUG
	  dbg_printf ("read_result:status code = 0x%x\n", result[i]);
#endif
	}
      return (TRUE);
    }
  return (FALSE);
}

/*************************************************************************
 * recalibrate --- recalibrate コマンドを実行する。
 *
 * 引数：	ドライブ番号
 *
 * 返値：	
 *
 * 処理：
 *
 */
W
recalibrate (W drive)
{
  UW	result[2];
  
  ready_check ();
  intr_flag = FALSE;
  write_fdc (FDC_RECALIBRATE);
  write_fdc (drive);
  wait_int (&intr_flag);
  if (sense_interrupt (drive, result) == FALSE)
    {
      return (FALSE);
    }
  return (TRUE);
}

/*************************************************************************
 * on_motor
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
on_motor (W drive)
{
  outb (FDC_WCNTL, FDC_MTON | FDC_DMAE);

#ifdef notdef
  if (fd_data[drive]->motor == OFF_MOTOR)
    {
      dbg_printf ("motor on.\n");
      outb (FDC_WCNTL, FDC_MTON | FDC_DMAE);
    }
  fd_data[drive]->motor = ON_MOTOR;
#endif /* notdef */
}

/************************************************************************
 *
 */
void
stop_motor (W drive)
{
}

/*************************************************************************
 * seek ---
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
seek (W drive, W cylinder)
{
  UW	result[2];

  recalibrate (0);
#ifdef FDDEBUG
  dbg_printf ("fd_seek:write to FDC.\n");
#endif
  write_fdc (FDC_SEEK);
  write_fdc (drive & 0x03);
  write_fdc (cylinder);
  wait_int (&intr_flag);			/* */
  if (sense_interrupt (drive, result) == FALSE)
    return (FALSE);
  return (TRUE);
}

/************************************************************************
 * get_data ---
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
get_data (W drive, W head, W cylinder, W sector, B *buff)
{
  W	i;
  W	result[7];
  
#define LEN	(fd_data[drive]->length)
#define CHAN	(fd_data[drive]->dmachan)

  on_motor (drive);
  setup_dma (CHAN, (void *)(((W)buff) & 0x7fffffff), DMA_READ, LEN, DMA_MASK & 0x02);
#ifdef FDDEBUG
  dbg_printf ("get_data: <%d> [%d, %d, %d] DMA = %d, buff = 0x%x\n", drive, head, cylinder, sector, CHAN, buff);	/* */
#endif
  seek (drive, cylinder);
  ready_check ();
  intr_flag = FALSE;
  write_fdc (FDC_READ);
  write_fdc ((head << 2) | drive);
  write_fdc (cylinder);
  write_fdc (head);
  write_fdc (sector);

  write_fdc (fd_data[drive]->lencode);
  write_fdc (fd_data[drive]->sector);
  write_fdc (fd_data[drive]->gapskip);
  write_fdc (fd_data[drive]->dtl);
  reset_intr_mask (11);
  wait_int (&intr_flag);		/* busy wait 待ち */
  read_result (drive, result, 7);
  if ((result[0] & ST0_IC) == ST0_IC_NT)
    {
#ifdef FDDEBUG
      dbg_printf ("get_data is normal end.\n");
#endif
      return (E_OK);
    }
  else
    {
#ifdef FDDEBUG
      dbg_printf ("FD: st[0] = 0x%x\n", result[0]);
      dbg_printf ("get_data is abnormal end.\n");
#endif
      return (E_DEV);
    }

#undef LEN
#undef CHAN
}

/*
 * get_data ---
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
W
put_data (W drive, W head, W cylinder, W sector, B *buff)
{
  W	i;
  W	result[7];
  
#define LEN	(fd_data[drive]->length)
#define CHAN	(fd_data[drive]->dmachan)

  on_motor (drive);
  setup_dma (CHAN, (void *)(((W)buff) & 0x7fffffff), DMA_WRITE, LEN, DMA_MASK & 0x02);
#ifdef FDDEBUG
  dbg_printf ("put_data: <%d> [%d, %d, %d] DMA = %d, buff = 0x%x\n", drive, head, cylinder, sector, CHAN, buff);	/* */
#endif
  seek (drive, cylinder);
  ready_check ();
  intr_flag = FALSE;
  dis_int();
  write_fdc (FDC_WRITE);
  write_fdc ((head << 2) | drive);
  write_fdc (cylinder);
  write_fdc (head);
  write_fdc (sector);

  write_fdc (fd_data[drive]->lencode);
  write_fdc (fd_data[drive]->sector);
  write_fdc (fd_data[drive]->gapskip);
  write_fdc (fd_data[drive]->dtl);
  ena_int();
  reset_intr_mask (11);
  wait_int (&intr_flag);		/* busy wait 待ち */
  read_result (drive, result, 7);
  if ((result[0] & ST0_IC) == ST0_IC_NT)
    {
      return (E_OK);
    }
  else
    {
      return (E_DEV);
    }

#undef LEN
#undef CHAN
}

/*
 * 割り込みハンドラと同期をとるための関数
 *
 */
W
wait_int (W *flag)
{
  while (*flag == FALSE)
    ;
  dis_int ();
  *flag = FALSE;
  ena_int ();
  return (*flag);
}


