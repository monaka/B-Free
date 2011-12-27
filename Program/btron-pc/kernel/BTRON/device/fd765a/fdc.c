/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/fd765a/fdc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/fd765a/fdc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "fd.h"

#undef USE_EVENTFLAG

#define printf	dbg_printf

struct status
{
  unsigned char		status_reg;
  unsigned char		status_data[8];

  int			motor;
};

struct status		fd_status;

static int		motor_goal = 0;
static int		motor_status = 0;

extern W		wait_int_with_timeout (W *flag, W timeout);

/*
 *	FDC �����椹�뤿��� LOW-LEVEL �ʴؿ�
 */


/*************************************************************************
 * write_fdc --- FDC �ؤΥ��ޥ�ɽ񤭹���
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
W
write_fdc (UW value)
{
  int i;
  unsigned long	status;

#ifdef FDDEBUG
  printf ("write_fdc: 0x%x\n", value);
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
#ifdef FDDEBUG
	  printf ("write_fdc: timeout!! status = 0x%x\n", status);
#endif
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
 * intr_fd
 *
 *	�ե�åԥ��ǥ������γ����߽����ؿ�
 *
 */
void
intr_fd (void)
{
  B	status;
  
  dis_int ();
#ifdef FDDEBUG
  printf ("!!interrupt!!\n");
#endif
#ifdef USE_EVENTFLAG
  set_flg (waitflag, 1);
#else
  intr_flag = TRUE;
#endif
  ena_int ();
/*  signal (FD_EVENT); */
}

/*************************************************************************
 * sense_interrupt --- �������װ��Υ����å�
 *
 * ������	�ʤ�
 *
 * ���͡�	
 *
 * ������
 *
 */
W
sense_interrupt (W drive, UW *result)
{
  write_fdc (FDC_SENSE);
  if (read_result (drive, result, 2) == FALSE)
    {
#ifdef FDDEBUG
      printf ("sense interupt: mal function.\n");
#endif
      return (FALSE);
    }
  
  if ((result[0] & ST0_IC) == ST0_IC_NT)
    {
#if FDDEBUG
      printf ("sense interupt: normal end.\n");
#endif
      return (TRUE);
    }

#if FDDEBUG
  printf ("sense interupt: mal function.\n");
#endif
  return (FALSE);
}

/***************************************************************************
 * ready_check --- 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
W
ready_check (void)
{
  int	i;
  int	status;
  
  for (i = 0; i < MAX_RETRY; i++)
    {
      status = read_fdc ();
#ifdef notdef
      status = status & (FDC_BUSY | FD0_BUSY | FD1_BUSY | FD2_BUSY | FD3_BUSY);
#endif
      status = status & FDC_BUSY;
      if (status == 0)
	break;
#ifdef notdef
      busywait (1000);
      dly_tsk(5); /* wait 50 ms */
#endif
    }
#ifdef FDDEBUG
  printf ("FD not ready.\n");
#endif
  return (status);
}

/*************************************************************************
 * read_fdc
 *
 * ������	�ʤ�
 *
 * ���͡�	FDC �Υ��ơ������쥸���������ơ�
 *
 * ������	FDC �Υ��ơ������쥸���������Ƥ��֤���
 *
 */
W
read_fdc (void)
{
  return (inb (FDC_STAT));
}

/*************************************************************************
 * reset_fdc --- FDC �Υꥻ�åȤ�Ԥ���
 *
 * ������	�ɥ饤���ֹ�
 *
 * ���͡�	�ʤ�
 *
 * ������	FDC ����ӥɥ饤�Фν������Ԥ���
 *
 */
W
reset_fdc (W drive)
{
  W	data;
  UW	result[8];
  W 	count;
  W	i;

#ifdef notdef
  dis_int ();
  intr_flag = FALSE;
  outb (FDC_WCNTL, 0);
  outb (FDC_WCNTL, 0x0c | 0x04);
  ena_int ();
  wait_int (&intr_flag);
  printf ("collect interrupt sense data.\n");
  for (i = 0; i < 4; i++)
    {
      fdc_isense ();
    }
#endif

#ifdef FDDEBUG
  printf ("reset_fdc: start\n");
  printf ("  srt = 0x%x\n", fd_data[drive]->srt);
  printf ("  hut = 0x%x\n", fd_data[drive]->hut);
  printf ("  hlt = 0x%x\n", fd_data[drive]->hlt);
#endif
  specify (drive,
	   fd_data[drive]->srt,
	   fd_data[drive]->hut,
	   fd_data[drive]->hlt,
	   fd_data[drive]->nd);
#if 1
  outb (FDC_DCR, 0x00);
#endif
  return (TRUE);
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
	      printf ("\n");
#endif
	      break;	/* too many results */
	    }
	  fd_status.status_data[result_nr++] = inb (FDC_DATA);
#ifdef FDDEBUG
	  printf ("sense: 0x%x ", 
		  fd_status.status_data[result_nr - 1]);
#endif
	}
      status = inb (FDC_STAT) & (FDC_MASTER | FDC_DIN | FDC_BUSY);
      if (status == FDC_MASTER) 
	{	/* all read */
	  return (TRUE);
	}
    }
/*  need_reset = TRUE;		/* controller chip must be reset */
  return (FALSE);
}

/*************************************************************************
 * 
 *
 * ������
 *
 * ���͡�
 *
 * ������
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
      printf ("FD NOT READY.\n");
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
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
#define NO_FIFO 0x20
#define FIFO_DEP 0x00
W
specify (W drive, UW srt, UW hut, UW hlt, UW nd)
{
  ready_check ();
#ifdef notdef
  write_fdc(FDC_CONFIGURE);
  write_fdc(0x00);
  write_fdc(0x10 | (NO_FIFO & 0x02) | (FIFO_DEP & 0x0F));
  write_fdc(0x00);
#endif
  write_fdc (FDC_SPECIFY);
#if 1
  write_fdc (0xdf);
  write_fdc (0x02);
#else
  write_fdc (((srt << 4) | (hut & 0x0f)) & 0xff);
  write_fdc (((hlt << 1) | (nd & 0x01)) & 0xff);
#endif
  return (0);
}


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
	      printf ("\n");
#endif
	      break;	/* too many results */
	    }
	  fd_status.status_data[result_nr++] = inb (FDC_DATA);
#ifdef FDDEBUG
	  printf ("sense: 0x%x ", 
		  fd_status.status_data[result_nr - 1]);
#endif
	}
      status = inb (FDC_STAT) & (FDC_MASTER | FDC_DIN | FDC_BUSY);
      if (status == FDC_MASTER) 
	{	/* all read */
	  return (TRUE);
	}
    }
/*  need_reset = TRUE;		/* controller chip must be reset */
  return (FALSE);
}

/*************************************************************************
 * read_result --- �ꥶ��ȥ��ơ������쥸���������Ƥ��ɤ߼�롣
 *
 * ������	drive	�ɥ饤���ֹ�
 *
 * ���͡�	TRUE	���ｪλ
 *		FALSE	���ơ������쥸�����˥ǡ��������äƤ��ʤ���
 *
 * ������	
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
	  printf ("read_result:status code = 0x%x\n", result[i]);
#endif
	}
      return (TRUE);
    }
  return (FALSE);
}

/*************************************************************************
 * recalibrate --- recalibrate ���ޥ�ɤ�¹Ԥ��롣
 *
 * ������	�ɥ饤���ֹ�
 *
 * ���͡�	
 *
 * ������
 *
 */
W
recalibrate (W drive)
{
  UW	result[2];
  UINT	rflag;
  
  ready_check ();

  dis_int ();
  intr_flag = FALSE;
#ifdef USE_EVENTFLAG
  clr_flg (waitflag, 0);
#endif
  write_fdc (FDC_RECALIBRATE);
  write_fdc (drive & 0x03);
  ena_int ();

#ifdef USE_EVENTFLAG
  wai_flg (&rflag, waitflag, 1, TWF_ORW);
#else
  wait_int (&intr_flag);
#endif
  fdc_isense ();
  if (fd_status.status_data[0] != 0)
    {
      return (FALSE);
    }
  return (TRUE);
}

/*************************************************************************
 * on_motor
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
on_motor (W drive)
{
  int motor_bit, running;

  motor_bit = 1 << drive;		/* ư�������� FD drive �ֹ��
					   �ӥåȥޥåפ��Ѵ� */
  running = motor_status & motor_bit;	/* nonzero if this motor is running */
  motor_goal = motor_status | motor_bit;/* want this drive running too */

  outb (FDC_WCNTL, (motor_goal << MOTOR_SHIFT) | 0x0c | drive);
  motor_status = motor_goal;

  /* If the motor was already running, we don't have to wait for it. */
  if (running) {
    def_alm(0, (VP) NADR);	/* remove alarm which stop motor */
    return;			/* motor was already running */
  }
#ifdef notdef
  busywait (1000);
#endif
  dly_tsk(25); /* sleep 250 ms */
}

/************************************************************************
 *
 */

void sm_handler(VP drive)
{
  motor_goal = 0;
  if (motor_goal != motor_status) {
	outb (FDC_WCNTL, (motor_goal << MOTOR_SHIFT) | 0x0c);
	motor_status = motor_goal;
  }
}

void
stop_motor (W drive)
{
#ifdef notdef
  motor_goal = 0;
  if (motor_goal != motor_status) {
	outb (FDC_WCNTL, (motor_goal << MOTOR_SHIFT) | 0x0c);
	motor_status = motor_goal;
  }
#endif
  T_DALM pk_dalm;
  pk_dalm.exinf = (VP) drive;
  pk_dalm.almhdr = (FP) sm_handler;
  pk_dalm.tmmode = TTM_REL;
  pk_dalm.almtim.utime = 0;
  pk_dalm.almtim.ltime = 300; /* 3 s */
  def_alm(0, &pk_dalm);
}

/*************************************************************************
 * seek ---
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
W
seek (W head, W drive, W cylinder)
{
  int	i;
  int	result;
  UINT	rflag;

  ready_check ();
#ifdef FDDEBUG
  printf ("fd_seek:write to FDC.\n");
#endif

  dis_int ();
#ifdef USE_EVENTFLAG
  clr_flg (waitflag, 0);
#endif
  intr_flag = FALSE;
  fd_status.status_data[0] = 0;
  fd_status.status_data[1] = 0;
  fd_status.status_data[2] = 0;
  write_fdc (FDC_SEEK);
  write_fdc ((head << 2) | (drive & 0x03));
  write_fdc (cylinder);
  ena_int ();

#ifdef FDDEBUG
  printf ("fd_seek:writed to FDC.\n");
#endif

#ifdef USE_EVENTFLAG
  wai_flg (&rflag, waitflag, 1, TWF_ORW);
#else
  wait_int (&intr_flag);
#endif
  fdc_isense ();
#ifdef notdef
  result = fd_status.status_data[0];
#else
  if ((fd_status.status_data[0] & 0xF8) != ST0_SE) result = E_DEV;
  else if (fd_status.status_data[1] != cylinder) result = E_DEV;
  else result = E_OK;
#endif

#ifdef notdef
  busywait (100);
  dly_tsk(3); /* wait 30 ms */
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
 * get_data ---
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
W
get_data (W drive, W head, W cylinder, W sector, B *buff)
{
  W	i;
  int	ret;
  int	s;
  UINT	rflag;
  
#define LEN	(fd_data[drive]->length)
#define CHAN	(fd_data[drive]->dmachan)

/*
  reset_fdc (drive);
*/
  on_motor (drive);
  ready_check ();
/*
  outb (FDC_DCR, 1);
*/

#ifdef FDDEBUG
  printf ("get_data: <%d> [%d, %d, %d] DMA = %d, buff = 0x%x\n", drive, head, cylinder, sector, CHAN, buff);
#endif

#ifdef notdef
  busywait (100);		/* */
  dly_tsk(3); /* wait 30 ms */
#endif

/*
  recalibrate (drive);
  outb (FDC_DCR, 0);
*/
  ret = seek (head, drive, cylinder);
#if 1
  if (ret != E_OK) return (ret);
#endif
  ready_check ();

  dis_int ();
#ifdef USE_EVENTFLAG
  clr_flg (waitflag, 0);
#endif
  intr_flag = FALSE;
#if 1
  setup_dma (CHAN, (void *)(((W)buff) & 0x7fffffff), DMA_READ, LEN, DMA_MASK & 0x02);
#else
  setup_dma (CHAN, (void *)(((W)buff) & 0x7fffffff), DMA_READ, BLOCK_SIZE, DMA_MASK);
#endif
  write_fdc (FDC_READ);
  write_fdc ((head << 2) | drive);
  write_fdc (cylinder);
  write_fdc (head);
  write_fdc (sector);
  write_fdc (fd_data[drive]->lencode);
  write_fdc (fd_data[drive]->sector);
  write_fdc (fd_data[drive]->gapskip);
  write_fdc (fd_data[drive]->dtl);
  ena_int ();

#ifdef USE_EVENTFLAG
  wai_flg (&rflag, waitflag, 1, TWF_ORW);
#else
  wait_int (&intr_flag);		/* busy wait �Ԥ� */
#endif
  ret = fdc_sense ();
  if (ret != TRUE) return (E_DEV);
  stop_motor (drive);
#ifdef FDDEBUG
  printf ("fd_status.status_data[0] = 0x%x\n", fd_status.status_data[0]);	/* */
#endif
#ifdef notdef
  if ((fd_status.status_data[0] & ST0_IC) == ST0_IC_NT)
    {
      return (E_OK);
    }
  else
    {
#ifdef FDDEBUG
      printf ("FD: st[0] = 0x%x\n", fd_status.status_data[0]);
      printf ("get_data is abnormal end.\n");
#endif
      return (E_DEV);
    }
#else
  if ((fd_status.status_data[0] & 0xF8) != 0x00) {
    return (E_DEV);
  }
  if (fd_status.status_data[1] | fd_status.status_data[2]) return (E_DEV);
  s = (fd_status.status_data[3] - cylinder)
    * fd_data[drive]->head * fd_data[drive]->sector;
  s += (fd_status.status_data[4] - head) * fd_data[drive]->sector;
  s += (fd_status.status_data[5] - sector);
  if (s * LEN != BLOCK_SIZE) return (E_DEV);
  else 
    return (E_OK);
#endif

#undef LEN
#undef CHAN
}

/*
 * put_data ---
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
W
put_data (W drive, W head, W cylinder, W sector, B *buff)
{
  W	i;
  int	ret;
  int   s;
  UINT	rflag;
  
#define LEN	(fd_data[drive]->length)
#define CHAN	(fd_data[drive]->dmachan)

  on_motor (drive);
/*
  outb (FDC_DCR, 0);
*/
#ifdef FDDEBUG
  printf ("put_data: <%d> [%d, %d, %d] DMA = %d, buff = 0x%x\n", drive, head, cylinder, sector, CHAN, buff);	/* */
#endif
  ret = seek (head, drive, cylinder);
#if 1
  if (ret != E_OK) return (ret);
#endif
  ready_check ();

  dis_int ();
#ifdef USE_EVENTFLAG
  clr_flg (waitflag, 0);
#endif
  intr_flag = FALSE;
#if 1
  setup_dma (CHAN, (void *)(((W)buff) & 0x7fffffff), DMA_WRITE, LEN, DMA_MASK & 0x02);
#else
  setup_dma (CHAN, (void *)(((W)buff) & 0x7fffffff), DMA_WRITE, BLOCK_SIZE, DMA_MASK);
#endif
  write_fdc (FDC_WRITE);
  write_fdc ((head << 2) | drive);
  write_fdc (cylinder);
  write_fdc (head);
  write_fdc (sector);
  write_fdc (fd_data[drive]->lencode);
  write_fdc (fd_data[drive]->sector);
  write_fdc (fd_data[drive]->gapskip);
  write_fdc (fd_data[drive]->dtl);
  ena_int ();

#ifdef USE_EVENTFLAG
  wai_flg (&rflag, waitflag, 1, TWF_ORW);
#else
  wait_int (&intr_flag);		/* busy wait �Ԥ� */
#endif
  ret = fdc_sense ();
  if (ret != TRUE) return (E_DEV);
  stop_motor (drive);
#ifdef FDDEBUG
  printf ("fd_status.status_data[0] = 0x%x\n", fd_status.status_data[0]);	/* */
#endif
#ifdef notdef
  if ((fd_status.status_data[0] & ST0_IC) == ST0_IC_NT)
    {
      return (E_OK);
    }
  else
    {
#ifdef FDDEBUG
      printf ("FD: st[0] = 0x%x\n", fd_status.status_data[0]);
      printf ("get_data is abnormal end.\n");
#endif
      return (E_DEV);
    }
#else
  if ((fd_status.status_data[0] & 0xF8) != 0x00) return (E_DEV);
  if (fd_status.status_data[1] | fd_status.status_data[2]) return (E_DEV);
  s = (fd_status.status_data[3] - cylinder)
    * fd_data[drive]->head * fd_data[drive]->sector;
  s += (fd_status.status_data[4] - head) * fd_data[drive]->sector;
  s += (fd_status.status_data[5] - sector);
  if (s * LEN != BLOCK_SIZE) return (E_DEV);
  else return (E_OK);
#endif

#undef LEN
#undef CHAN
}


/*
 * �����ߥϥ�ɥ��Ʊ����Ȥ뤿��δؿ�
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


/*
 * �����ߥϥ�ɥ��Ʊ����Ȥ뤿��δؿ�
 *
 */
W
wait_int_with_timeout (W *flag, W timeout)
{
  W	i;

  for (i = 0; i < timeout; i++)
    {
      if (*flag != FALSE)
	break;
#ifdef notdef
      busywait (100);
#endif
    }

  if (*flag == FALSE)
    {
      printf ("interrupt timeout error\n");
      return (*flag);
    }

  dis_int ();
  *flag = FALSE;
  ena_int ();
  return (*flag);
}


