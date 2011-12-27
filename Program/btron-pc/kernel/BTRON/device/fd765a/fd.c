/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/fd765a/fd.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/fd765a/fd.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: fd.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  2000-02-06 09:05:06  naniwa
 * to use eventflag
 *
 * Revision 1.7  2000/01/22 11:00:30  naniwa
 * minor fix
 *
 * Revision 1.6  1999/12/19 11:00:04  naniwa
 * made disk read/write check strict
 *
 * Revision 1.5  1999/04/13 04:14:40  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.4  1998/02/23 14:39:28  night
 * get_req �ΰ������ְ�äƤ������ὤ����
 * ���ޤǤΥ����ɤ��ȡ����۶��֤���Ƭ��ʬ���˲����Ƥ�����
 *
 * Revision 1.3  1997/10/18 12:38:28  night
 * �ǥХå�ʸ����ӡ��ޥ��� (S/H/C) ��������֤��ѹ���
 *
 * Revision 1.2  1997/04/24 15:34:17  night
 * �ɤ߽񤭤����ǡ����ΥХ��ȿ����֤��Ȥ��ˡ��⤷���ꤵ�줿�Х��ȿ�����
 * �ɤ߽񤭤����ǡ����ΥХ��ȿ��������礭�����ˤϡ����ꤵ�줿�Х��ȿ���
 * �֤��褦�˽���������
 * ���ϡ�(512 �Х��Ȥ��ڤ�夲��)�ɤ߽񤭤����Х��ȿ����֤��Ƥ�����
 *
 * Revision 1.1  1996/07/28  19:59:33  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 * Revision 1.6  1995/10/03  14:41:48  night
 * �ե�åԥ��ǥХ����ν��������äƤ��ʤ��Ȥ��ˡ����顼�����褦�ˤ�����
 * (�����ϡ��������Ԥ���̵���Ԥ����Ƥ���)
 *
 * Revision 1.5  1995/10/03  14:07:41  night
 * FD �ɥ饤�Ф�ư���ǡ�
 * �ޤ������� 1K �Х��Ȥ��ɤ߽񤭤����Ǥ��ʤ�����Ǥ�դΥ��ե��åȤ�����
 * ����褦�ˤʤä���
 *
 * Revision 1.4  1995/10/01  12:52:32  night
 * FDC �˴ط�������ʬ�� fdc.c �˰�ư��
 *
 * Revision 1.3  1995/09/21  15:50:57  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/06  16:12:26  night
 * ��֥С������
 * start() ����ǵ�ư��å�������Ф��Ȥ���ޤǺ�����
 *
 * Revision 1.1  1995/03/18  14:09:01  night
 * �ǽ����Ͽ
 *
 *
 */

/**************************************************************************
 Discription

 PC98 �� FD �ɥ饤��

	���󥿥ե������̣ӣɡ��Уģ������������Ԥ���
	���ݡ��Ȥ��Ƥ��� PD765 ��̿��ϼ��ΤȤ��ꡣ

	Specify			FDC �ν������Ԥ���
	Recalibrate		���ȥ�å��ܤ˥إåɤ��ư���롣
	Read Data		�ǡ������ɤ߹��ߡ�
	Write Data		�ǡ����ν񤭹��ߡ�
	Seek			�إåɤ򥷡�����
	Sense Interrupt Status	�����߸�����Ĵ�٤롣


�ؿ�����

  ���٥�ؿ�

    �����ˤ������ؿ�����¾�Υ⥸�塼��� I/F ��Ȥ롣

    	  o init_fd	--- �ɥ饤�����Τν������Ԥ���
	  o open_fd	--- ���ꤷ���ɥ饤���ֹ���� FD �򥪡��ץ󤹤롣
	  o close_fd	--- ���ꤷ���ɥ饤���ֹ���� FD �򥯥������롣
	  o read_fd	--- �ǡ������ɤ߼�ꡣ
	  o write_fd	--- �ǡ����ν񤭹��ߡ�
	  o contorl_fd	--- �ɥ饤�Х���ȥ��롣
	  o change_fd	--- ���������⡼���ڤ��ؤ���
	  o status_fd	--- ���ơ����������å���
	  o intr_fd	--- �����ߥϥ�ɥ顣

  ���٥�ؿ� 

    ���δؿ��ϡ�ľ�� FDC �������Ԥ�����̥�٥�δؿ��ϡ����δؿ���
    �Ȥä� FD �����椹�롣�����δؿ��Ϥ��٤� LOCAL �Ȥ����������롣

    	  o write_fdc	--- FDC �˥ǡ��������롣
	  o read_fdc	--- FDC ����ǡ������ɤ߼�롣
	  o reset_fdc	--- FDC �Υꥻ�åȤ�Ԥ���
	  o ready_check	--- FDC �� ready ���֤��ɤ���������å����롣
	  o recalibrate --- ���ꤷ���ɥ饤�֤� recalibrate ��Ԥ���
	  o specify     --- FDC �ν����l (specify) ��Ԥ���
	  o on_motor	--- �⡼������ư���롣
	  o stop_motor	--- �⡼��������ߤ���(���δؿ��ϲ��⤷�ʤ�)��
	  o seek	--- FD �Υ�������Ԥ���
	  o get_data	--- FD ����ǡ������ɤ߼�롣
	  o read_result	--- �ꥶ��ȥ��ơ������쥸���������Ƥ��ɤ߼�롣
	  o sense_interrupt --- �����߾��֤�Ĵ�����롣


	�ؿ����ؿ�

	init_fd
	  reset_fdc
	    write_fdc
	    specify
	    on_motor

	open_fd

	close_fd

	read_fd
	  setup_dma
	  seek
	    write_fdc
	  get_data
	    write_fdc
	
	read_fd
	  setup_dma
	  seek
	    write_fdc
	  get_data
	    write_fdc

	control_fd

	change_fd

	status_fd

	intr_fd
	
	
**********************************************************************/

#include "fd.h"


/*********************************************************************
 *	 ����ѿ��������
 *
 */
struct spec	*fd_data[MAXFD];
W		intr_flag;
ID		waitflag;


/*********************************************************************
 *	 �ɽ��ѿ��������
 *
 */
static W	mydevid;	/* ��ʬ���Ȥ�id */
static ID	recvport;	/* �׵�����Ĥ��ѥݡ��� */
static W	initialized;

/*
 *	�ɽ�ؿ��������
 */
static void	main_loop (void);
static void	init_fd_driver (void);
static void	doit (DDEV_REQ *packet);

/*
 * FD �ǥХ����ɥ饤�Ф� main �ؿ�
 *
 * ���δؿ��ϡ��ǥХ����ɥ饤��Ω���夲���˰������¹Ԥ��롣
 *
 */
start ()
{
  extern char	version[];

  /* 
   * �׵�����ѤΥݡ��Ȥκ���
   */
  init_fd_driver ();

  /*
   * Ω���夲��å�����
   */
  dbg_printf ("floppy disk driver start\n");
  dbg_printf ("  receive port is %d\n", recvport);

  /*
   * �ɥ饤�Ф��������롣
   */
  main_loop ();
}

static void
main_loop ()
{
  DDEV_REQ	req;
  extern ER	sys_errno;
  UW		rsize;

  /*
   * �׵���� - �����Υ롼��
   */
  for (;;)
    {
      /* �׵�μ��� */
/*      dbg_printf ("fd: get_req\n");	/* */
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* ���辰���� */
/*	  dbg_printf ("fd: receive packet type = %d\n", req.header.msgtyp);	/* */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("fd: get_req() Unknown error(error = %d)\n", sys_errno);
	  dbg_printf ("FD driver is halt.\n");
	  slp_tsk ();
	  break;
	}
    }

  /* �����ιԤˤϡ���ʤ� */
}

/*
 *
 */
/************************************************************************
 *
 *
 */
static void
doit (DDEV_REQ *packet)
{
  switch (packet->header.msgtyp)
    {
    case DEV_OPN:
      /* �ǥХ����Υ����ץ� */
      open_fd (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_fd (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_fd (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_fd (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_fd (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/*
 * �����
 *
 * o �ե�����ơ��֥� (file_table) �ν����
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 */
static void
init_fd_driver (void)
{
  int		i;
  ID		root_dev;
  ER		error;

  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recvport <= 0)
    {
      dbg_printf ("FD: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (FD_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

  /* event flag ������ */
  waitflag = get_flag (TA_WSGL, 0);

  fd_data[0] = get_fdspec ("2HD");
  fd_data[1] = get_fdspec ("2HD");
  init_fd ();
}

/*
 * init_fd --- FD �ɥ饤�Фν����
 *
 */
W
init_fd (void)
{
  W		status;
  T_DINT	pkt;
  ER		err;

  pkt.intatr = ATR_INTR;
  pkt.inthdr = (FP)intr_fd;
  err = def_int (INT_FD, &pkt);
  if (err != E_OK)
    {
      dbg_printf ("fd: error on def_int (errno = %d)\n", err);
      return (err);
    }
  reset_intr_mask (6);
  intr_flag = FALSE;
  reset_fdc (0);
/*  reset_fdc (1); */
#ifdef notdef
  fd_test ();
#endif
}

/************************************************************************
 * open_fd --- FD �Υ����ץ�
 *
 * ������	dd	FD �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	
 *
 * ������	���ꤷ�� FD �Υ��ơ�������Ĵ�١��⤷�Ȥ��Ƥ��ʤ���С�
 *		���Ѿ��� (DRIVE_USING) �ˤ��롣
 *
 */
W
open_fd (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

#ifdef notdef
  outb(0x439, (inb(0x439) & 0xfb)); /* DMA Accsess Control over 1MB */
  outb(0x29, (0x0c | 0));	/* Bank Mode Reg. 16M mode */
  outb(0x29, (0x0c | 1));	/* Bank Mode Reg. 16M mode */
  outb(0x29, (0x0c | 2));	/* Bank Mode Reg. 16M mode */
  outb(0x29, (0x0c | 3));	/* Bank Mode Reg. 16M mode */
#endif
  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * fd_close --- �ɥ饤�ФΥ�����
 *
 * ������	dd	FD �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	
 *
 * ������	���ꤷ�� FD �Υ��ơ�������Ĵ�١��⤷�Ȥ��Ƥ��ʤ���С�
 *		���Ѿ��� (DRIVE_USING) �ˤ��롣
 *
 */
W
close_fd (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_fd --- 
 *
 * ������	caller	�ƤӽФ����ؤ��������֤�����Υݡ���
 *		packet	�ɤ߹��ߥǡ����Υѥ�᡼��
 *
 * ���͡�	���顼�ֹ�
 *
 * ������	���δؿ��ϡ��ʲ��ν�����Ԥ���
 *
 *		1) �����֥�å��ֹ� (1024 byte ñ��) ����ʪ���֥�å��ֹ���Ѵ�
 *		2) ʪ���֥�å��ֹ椫�餵��� Head/Cylinder/Sector �γơ��ο����Ѵ�
 *		3) �Ѵ�������������ˤ��� get_data() ��ƤӽФ�
 *
 */
W
read_fd (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;
  W		blockno;	/* ʪ���֥�å��ֹ� */
  W		bcount;		/* �����֥�å���ʪ���֥�å�����礭�����˻��Ѥ��롣*/
  				/* ʪ���֥�å����ɤߤȤ�Ȥ��β���Ȥʤ� 	       */
  W		cylinder;
  W		head;
  W		sector;
  W		drive;
  W		i, try;
  W		done_length;	/* �������ɤ߹�����Х��ȿ� */
  static B	buff[BLOCK_SIZE * 2];
  UW		bp;
  ER		ret;
  ER		error;
  UW		bufstart;
  UW		buflength;

  drive = packet->dd & 0xff;
  if (reset_fdc (drive) == FALSE)
    {
      goto bad;
    }

  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);
  /*  dbg_printf ("bufstart = %d, buflength = %d\n", bufstart, buflength);*/

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
      /* dbg_printf ("read_fd: bp = %d, length = %d\n", bp, BLOCK_SIZE);*/
      /* �Х��ȥ��ե��åȤ���ʪ���֥�å��ֹ�ؤ��Ѵ� */
      blockno = ((UW)(bp + bufstart) / BLOCK_SIZE) * (BLOCK_SIZE / fd_data[packet->dd & 0xff]->length);
#if 1
      if (BLOCK_SIZE >= (fd_data[packet->dd & 0xff]->length))
	{
	  bcount = BLOCK_SIZE / (fd_data[packet->dd & 0xff]->length);
	}
#else
      bcount = 1;
#endif
  
#define H	(fd_data[packet->dd & 0xff]->head)
#define S	(fd_data[packet->dd & 0xff]->sector)
#define LEN	(fd_data[packet->dd & 0xff]->length)
#define CHAN	(fd_data[packet->dd & 0xff]->dmachan)

      done_length = 0;
      error = E_OK;
      ret = E_OK;

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef FDDEBUG
	  dbg_printf ("read_fd: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < (fd_data[packet->dd & 0xff]->retry); try++)
	    {
	      ret = get_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
	      if (ret == E_OK)
		break;
	    }
	  if (ret == E_OK)
	    {
	      done_length += LEN;
	      blockno += 1;
	    }
	  else
	    {
	      error = ret;
	      goto bad;
	    }
	}
    }

/*  dbg_printf ("bcopy(): %d, %d, %d\n", packet->start - bufstart, 0, 
	      (done_length < packet->size) ? done_length : packet->size);	/* */
  bcopy (&buff[packet->start - bufstart], 
	 res.body.rea_res.dt, 
	 (done_length < packet->size) ? done_length : packet->size);
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = (done_length < packet->size) ? done_length : packet->size;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
/*  dbg_printf ("fd: snd_mbuf done.\n"); */
  return (E_OK);

bad:
  stop_motor (drive);
  dbg_printf("fd: read fail. head = %d cylinder = %d sector = %d\n",
	     head, cylinder, sector);
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = done_length;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = error;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
  
#undef H
#undef S
#undef LEN
#undef CHAN
}


/************************************************************************
 *	write_fd
 */
W
write_fd(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  W		blockno;	/* ʪ���֥�å��ֹ� */
  W		bcount;		/* �����֥�å���ʪ���֥�å�����礭�����˻��Ѥ��롣*/
  				/* ʪ���֥�å����ɤߤȤ�Ȥ��β���Ȥʤ� 	       */
  W		cylinder;
  W		head;
  W		sector;
  W		drive;
  W		i, try;
  W		done_length;	/* �������ɤ߹�����Х��ȿ� */
  static B	buff[BLOCK_SIZE * 2];
  ER		ret;
  ER		error;
  UW		bp;
  UW		bufstart;
  UW		buflength;

#define H	(fd_data[packet->dd & 0xff]->head)
#define S	(fd_data[packet->dd & 0xff]->sector)
#define LEN	(fd_data[packet->dd & 0xff]->length)
#define CHAN	(fd_data[packet->dd & 0xff]->dmachan)


  drive = packet->dd & 0xff;
  if (reset_fdc (drive) == FALSE)
    {
      goto bad;
    }

  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);

/*  dbg_printf ("bufstart = %d, buflength = %d\n", bufstart, buflength);	/* */

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
/*      dbg_printf ("read_fd: bp = %d, length = %d\n", bp, BLOCK_SIZE);		/* */
      /* �Х��ȥ��ե��åȤ���ʪ���֥�å��ֹ�ؤ��Ѵ� */
      blockno = ((UW)(bp + bufstart) / BLOCK_SIZE) * (BLOCK_SIZE / fd_data[packet->dd & 0xff]->length);
#if 1
      if (BLOCK_SIZE >= (fd_data[packet->dd & 0xff]->length))
	{
	  bcount = BLOCK_SIZE / (fd_data[packet->dd & 0xff]->length);
	}
#else
      bcount = 1;
#endif
  
      done_length = 0;
      error = E_OK;
      ret = E_OK;

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef FDDEBUG
	  dbg_printf ("read_fd: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < (fd_data[packet->dd & 0xff]->retry); try++)
	    {
	      ret = get_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
	      if (ret == E_OK)
		break;
	    }
	  if (ret == E_OK)
	    {
	      done_length += LEN;
	      blockno += 1;
	    }
	  else
	    {
	      error = ret;
	      goto bad;
	    }
	}
    }

  done_length = 0;
  error = E_OK;
  ret = E_OK;
  bcopy (packet->dt, &buff[packet->start - bufstart], packet->size);
#ifdef notdef
  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);
#endif

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
      /* �Х��ȥ��ե��åȤ���ʪ���֥�å��ֹ�ؤ��Ѵ� */
      blockno = ((bp + bufstart) / BLOCK_SIZE) * (BLOCK_SIZE / fd_data[packet->dd & 0xff]->length);
#if 1
      if (BLOCK_SIZE >= (fd_data[packet->dd & 0xff]->length))
	{
	  bcount = BLOCK_SIZE / (fd_data[packet->dd & 0xff]->length);
	}
#else
      bcount = 1;
#endif

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef FDDEBUG
	  dbg_printf ("write_fd: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < (fd_data[packet->dd & 0xff]->retry); try++)
	    {
	      ret = put_data (drive,
			      head, 
			      cylinder, 
			      sector, 
			      (void *)(W)(buff + (bp + i * LEN)));
	      if (ret == E_OK)
		break;
	    }
	  if (ret == E_OK)
	    {
	      done_length += LEN;
	      blockno += 1;
	    }
	  else
	    {
	      error = ret;
	      goto bad;
	    }
	}
    }
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.a_size = (done_length < packet->size) ? done_length : packet->size;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);

bad:
  stop_motor (drive);
  dbg_printf("fd: write fail. head = %d cylinder = %d sector = %d\n",
	     head, cylinder, sector);
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.a_size = done_length;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = error;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
  
#undef H
#undef S
#undef LEN
#undef CHAN
}

/************************************************************************
 *	control_fd
 */
W
control_fd (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;
  ER		error = E_OK;
  W		drive;

  switch (packet->cmd)
    {
    case CHANGE_MODE:
      switch (packet->param[0])
	{
	case M2HD:
	  fd_data[packet->param[1]] = get_fdspec ("2HD");
	  if (reset_fdc (packet->param[1]) == FALSE)
	    error = E_DEV;
/*	  dbg_printf ("control_fd: change FD type to 2HD\n");	/* */
	  break;

	case M2HC:
	  fd_data[packet->param[1]] = get_fdspec ("2HC");
	  if (reset_fdc (packet->param[1]) == FALSE)
	    error = E_DEV;
/*	  dbg_printf ("control_fd: change FD type to 2HC\n");	/* */
	  break;

	case OTHER_FD:
	  dbg_printf ("control_fd: Unknown FD type.\n");	/* */
	  break;

	default:
	  dbg_printf ("control_fd: unknown FD type (%d)\n", packet->param[0]);
	  error = E_PAR;
	  break;
	}
      break;

    case NOWORK:
      dbg_printf ("control_fd: nowork\n");
      break;

    default:
      error = E_NOSPT;
      break;
    }
  res.body.ctl_res.dd = packet->dd;
  res.body.ctl_res.errcd = error;
  res.body.ctl_res.errinfo = error;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
}
