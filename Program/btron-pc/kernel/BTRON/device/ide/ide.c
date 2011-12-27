/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ide/ide.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ide/ide.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: ide.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.15  2000-01-22 10:59:30  naniwa
 * fixed to be able to write disk
 *
 * Revision 1.14  1999/04/13 04:14:43  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.13  1999/03/15 05:58:23  monaka
 * modified some debug messages.
 *
 * Revision 1.12  1998/06/10 15:46:24  night
 * �ѡ��ƥ���������Ȥ��ơ�CHS �ǤϤʤ���˥��֥�å��ֹ��Ȥ��褦��
 * �ѹ�������
 *
 * Revision 1.11  1998/05/07 16:47:42  night
 * �ѡ��ƥ������κǽ�Υ֥�å��ֹ��׻�����Ȥ��ˡ�
 * ��ĥ�ѡ��ƥ������Τ��Ȥ��θ����褦�ˤ�����
 *
 * Revision 1.10  1998/02/23 14:39:38  night
 * get_req �ΰ������ְ�äƤ������ὤ����
 * ���ޤǤΥ����ɤ��ȡ����۶��֤���Ƭ��ʬ���˲����Ƥ�����
 *
 * Revision 1.9  1997/10/11 16:18:51  night
 * IDE HD �ؤν񤭹��߽����������ư���褦�ˤ���������
 *
 * Revision 1.8  1997/07/07 14:43:14  night
 * IDE �ɥ饤�Ф� write ������ѡ��ƥ�������ռ�����褦���ѹ�������
 *
 * Revision 1.7  1997/07/06 11:46:24  night
 * �֥�å����ե��åȤη׻���ˡ���ѹ���
 *
 * Revision 1.6  1997/07/04 15:03:58  night
 * �ѡ��ƥ������λ�����ˡ���ѹ���
 * dd �� 0 �ΤȤ��ˤϡ��ǥ��������Τ򼨤��褦�ˤ�����
 * (����ϡ�dd �� 0 �ΤȤ��ˤϥѡ��ƥ������ 0 �򼨤��Ƥ���)
 * �����ѹ��ˤ�ꡢ�ǽ�Υѡ��ƥ������ϡ�dd �� 1 �ǻ��ꤹ�뤳�Ȥˤʤ롣
 *
 * Revision 1.5  1997/07/02 13:08:31  night
 * �����ߤ�Ȥ�ʤ��褦�˽�����
 *
 * Revision 1.4  1997/05/11 12:41:12  night
 * read/write ���˥ѡ��ƥ�����������θ����褦���ѹ�������
 *
 * Revision 1.3  1997/05/08 15:07:12  night
 * IDE �ɥ饤�֤ξ����Ͽ�����ΰ�򿷤������ꤷ����
 *
 * Revision 1.2  1997/05/06 12:40:55  night
 * IDE_GET_STAT �ɲá�
 *
 * Revision 1.1  1996/11/05  18:42:35  night
 * �ǽ����Ͽ
 *
 *
 *
 */

/*
 * IDE HD �ǥХ����ɥ饤��
 *
 *
 */
#include "ide.h"


/*********************************************************************
 *	 ����ѿ��������
 *
 */
W			ide_intr_flag;
struct ide_id		ide_spec[MAXIDE];
struct ide_partition	ide_partition[MAXIDE][IDE_MAX_PARTITION];


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
static void	init_ide_driver (void);
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
  init_ide_driver ();

  /*
   * Ω���夲��å�����
   */
  dbg_printf ("IDE disk driver started. receive port is %d\n", recvport);

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
/*      printf ("ide: get_req\n");	/* */
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* ���辰���� */
/*	  printf ("ide: receive packet type = %d\n", req.header.msgtyp);	/* */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  printf ("ide: get_req() Unknown error(error = %d)\n", sys_errno);
	  printf ("IDE driver is halt.\n");
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
      open_ide (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_ide (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_ide (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_ide (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_ide (packet->header.mbfid, &(packet->body.ctl_req));
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
init_ide_driver (void)
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
      printf ("IDE: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (IDE_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

  init_log ();
  init_ide ();
}

/*
 * init_ide --- IDE �ɥ饤�Фν����
 *
 */
W
init_ide (void)
{
  W		status;
  T_DINT	pkt;
  ER		err;

  pkt.intatr = ATR_INTR;
  pkt.inthdr = (FP)intr_ide;
#ifdef notdef
  err = def_int (INT_IDE0, &pkt);
  if (err != E_OK)
    {
      dbg_printf ("ide: error on def_int (errno = %d)\n", err);
      return (err);
    }
  reset_intr_mask (IDE0_INTR_MASK);
#endif

  outb (IDE_CONTROL_REG, 0x0e);
  busywait (10000);
  outb (IDE_CONTROL_REG, 0x0a);
  busywait (10000);
  status = inb (IDE_STAT_REG);
  dbg_printf ("IDE init status = 0x%x\n", status);
  reset_ide (0);
/*  reset_idec (1); */

  read_stat (0, &ide_spec[0]);
  read_partition (0);
}

/************************************************************************
 * open_ide --- IDE �Υ����ץ�
 *
 * ������	dd	IDE �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	
 *
 * ������	���ꤷ�� IDE �Υ��ơ�������Ĵ�١��⤷�Ȥ��Ƥ��ʤ���С�
 *		���Ѿ��� (DRIVE_USING) �ˤ��롣
 *
 */
W
open_ide (ID caller, DDEV_OPN_REQ *packet)
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
 * ide_close --- �ɥ饤�ФΥ�����
 *
 * ������	dd	IDE �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	
 *
 * ������	���ꤷ�� IDE �Υ��ơ�������Ĵ�١��⤷�Ȥ��Ƥ����
 *		���Ѿ��� (DRIVE_USING) �Υե饰��ä���
 *
 */
W
close_ide (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_ide --- 
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
read_ide (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;
  W		blockno;	/* ʪ���֥�å��ֹ� */
  W		bcount;		/* �����֥�å���ʪ���֥�å�����礭�����˻��Ѥ��롣*/
  				/* ʪ���֥�å����ɤߤȤ�Ȥ��β���Ȥʤ� 	       */
  W		cylinder;
  W		head;
  W		sector;
  W		drive;		/* �ɥ饤���ֹ� */
  UW		partition;	/* �ѡ��ƥ�������ֹ� */
  W		i, try;
  W		done_length;	/* �������ɤ߹�����Х��ȿ� */
  static B	buff[BLOCK_SIZE * 2];
  UW		bp;
  ER		ret;
  ER		error;
  UW		bufstart;
  UW		buflength;
  UW		parstart;

  drive = IDE_GET_DRIVE(packet->dd);
  partition = IDE_GET_PARTITION(packet->dd);
  if (partition == 0)
    {
      parstart = 0;
    }
  else
    {
      parstart = ide_partition[drive][partition - 1].start * BLOCK_SIZE;
    }

  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);
#ifdef DEBUG
  printf ("partition = %d, bufstart = %d, buflength = %d\n", partition, bufstart, buflength);	/* */
#endif

  done_length = 0;
  error = E_OK;
  ret = E_OK;

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
#ifdef DEBUG
      printf ("read_ide: bp = %d, length = %d\n", bp, BLOCK_SIZE); 	/* */
#endif
      /* �Х��ȥ��ե��åȤ���ʪ���֥�å��ֹ�ؤ��Ѵ� */
      blockno = ((UW)(bp + bufstart + parstart) / BLOCK_SIZE) * (BLOCK_SIZE / IDE_BLOCK_SIZE);
      if (BLOCK_SIZE >= (IDE_BLOCK_SIZE))
	{
	  bcount = BLOCK_SIZE / IDE_BLOCK_SIZE;
	}
  
#define H	(ide_spec[IDE_GET_DRIVE (packet->dd)].n_head)
#define S	(ide_spec[IDE_GET_DRIVE (packet->dd)].n_sector)
#define LEN	IDE_BLOCK_SIZE

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef DEBUG
	  printf ("read_ide: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < IDE_RETRY; try++)
	    {
#ifdef DEBUG
	      printf ("get_data (%d, %d, %d, %d, 0x%x)\n",
		      drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
#endif

	      ret = get_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]), LEN / IDE_BLOCK_SIZE);
	      if (ret > 0)
		break;
	    }
	  if (ret > 0)
	    {
	      done_length += ret;
	      blockno++;
	    }
	  else
	    {
	      error = E_SYS;
	      goto bad;
	    }
	}
    }

#ifdef DEBUG
  printf ("bcopy(): 0x%x, 0x%x, %d\n",
	  diff,
	  res.body.rea_res.dt,
	  (done_length < packet->size) ? done_length : packet->size);	/* */
#endif

/*  done_length -= packet->start - bufstart; */
  if (((done_length < packet->size) ? done_length : packet->size) > BLOCK_SIZE * 2)
    {
      printf ("buffer overflow (size = %d)\n", (done_length < packet->size) ? done_length : packet->size);
      error = E_SYS;
      goto bad;
    }

  bcopy (&buff[packet->start - bufstart], 
	 res.body.rea_res.dt, 
	 (done_length < packet->size) ? done_length : packet->size);
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = (done_length < packet->size) ? done_length : packet->size;
  res.body.rea_res.errcd = E_OK;
  res.body.rea_res.errinfo = 0;
  snd_mbf (caller, sizeof (res), &res);
#ifdef DEBUG
  printf ("ide: snd_mbuf done.\n");
#endif
  return (E_OK);

bad:
  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.a_size = done_length;
  res.body.rea_res.errcd = error;
  res.body.rea_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
  
#undef H
#undef S
#undef LEN
}


/************************************************************************
 *	write_ide
 */
W
write_ide(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  W		blockno;	/* ʪ���֥�å��ֹ� */
  W		bcount;		/* �����֥�å���ʪ���֥�å�����礭�����˻��Ѥ��롣*/
  				/* ʪ���֥�å����ɤߤȤ�Ȥ��β���Ȥʤ� 	       */
  W		cylinder;
  W		head;
  W		sector;
  W		drive;		/* �ɥ饤���ֹ� */
  UW		partition;	/* �ѡ��ƥ�������ֹ� */
  W		i, try;
  W		done_length;	/* �������ɤ߹�����Х��ȿ� */
  static B	buff[BLOCK_SIZE * 2];
  UW		bp;
  ER		ret;
  ER		error;
  UW		bufstart;
  UW		buflength;
  UW		parstart;

  drive = IDE_GET_DRIVE(packet->dd);
  partition = IDE_GET_PARTITION(packet->dd);
  if (partition == 0)
    {
      parstart = 0;
    }
  else
    {
      parstart = ide_partition[drive][partition - 1].start * BLOCK_SIZE;
    }

  bufstart = ROUNDDOWN (packet->start, BLOCK_SIZE);
  buflength = ROUNDUP (packet->start + packet->size, BLOCK_SIZE);
#ifdef DEBUG
  printf ("partition = %d, bufstart = %d, buflength = %d\n", partition, bufstart, buflength);	/* */
#endif

  /* �ޤ�����������֥�å��κǽ�˥ǥ���������Ȥ��ɤ߼��
   */
  done_length = 0;
  error = E_OK;
  ret = E_OK;

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
#ifdef DEBUG
      printf ("write_ide: bp = %d, length = %d\n", bp, BLOCK_SIZE); 	/* */
#endif
      /* �Х��ȥ��ե��åȤ���ʪ���֥�å��ֹ�ؤ��Ѵ� */
      blockno = ((UW)(bp + bufstart + parstart) / BLOCK_SIZE) * (BLOCK_SIZE / IDE_BLOCK_SIZE);
      if (BLOCK_SIZE >= (IDE_BLOCK_SIZE))
	{
	  bcount = BLOCK_SIZE / IDE_BLOCK_SIZE;
	}
  
#define H	(ide_spec[IDE_GET_DRIVE (packet->dd)].n_head)
#define S	(ide_spec[IDE_GET_DRIVE (packet->dd)].n_sector)
#define LEN	IDE_BLOCK_SIZE

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef DEBUG
	  printf ("write_ide: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < IDE_RETRY; try++)
	    {
#ifdef DEBUG
	      printf ("get_data (%d, %d, %d, %d, 0x%x)\n",
		      drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
#endif

	      ret = get_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]), LEN / IDE_BLOCK_SIZE);
	      if (ret > 0)
		break;
	    }
	  if (ret > 0)
	    {
	      done_length += ret;
	      blockno++;
	    }
	  else
	    {
	      error = E_SYS;
	      goto bad;
	    }
	}
    }

  /* �ɤ߼�ä��ǡ������ɬ�פ���ʬ�ˡ��񤭴��������ǡ������񤭤���
   */
  bcopy (packet->dt, &buff[packet->start - bufstart], packet->size);

#ifdef DEBUG
  {
    int	i;

    printf ("buff = 0x%x\n", buff);
    for (i = 0; i < 20 - 1; i++)
      {
	printf ("0x%x, ", buff[i]);
      }
    printf ("0x%x\n", buff[i]);
  }
#endif

  /* �񤭴������ǡ�������٥ǥ�������˽񤭹���
   */
  done_length = 0;
  error = E_OK;
  ret = E_OK;

  for (bp = 0; bp < (buflength - bufstart); bp += BLOCK_SIZE)
    {
#ifdef DEBUG
      printf ("write_ide: bp = %d, length = %d\n", bp, BLOCK_SIZE); 	/* */
#endif
      /* �Х��ȥ��ե��åȤ���ʪ���֥�å��ֹ�ؤ��Ѵ� */
      blockno = ((UW)(bp + bufstart + parstart) / BLOCK_SIZE) * (BLOCK_SIZE / IDE_BLOCK_SIZE);
      if (BLOCK_SIZE >= (IDE_BLOCK_SIZE))
	{
	  bcount = BLOCK_SIZE / IDE_BLOCK_SIZE;
	}
  
#define H	(ide_spec[IDE_GET_DRIVE (packet->dd)].n_head)
#define S	(ide_spec[IDE_GET_DRIVE (packet->dd)].n_sector)
#define LEN	IDE_BLOCK_SIZE

      for (i = 0; i < bcount; i++)
	{
	  head = (blockno % (H * S)) / S;
	  cylinder = (blockno / (H * S));
	  sector = (blockno % S) + 1;

#ifdef DEBUG
	  printf ("write_ide: (H = %d, C = %d, S = %d)\n", head, cylinder, sector);	/* */
#endif
	  for (try = 0; try < IDE_RETRY; try++)
	    {
#ifdef DEBUG
	      printf ("write_data (%d, %d, %d, %d, 0x%x)\n",
		      drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]));
#endif

	      ret = put_data (drive, head, cylinder, sector, (void *)&(buff[bp + (i * LEN)]), LEN / IDE_BLOCK_SIZE);
	      if (ret > 0)
		break;
	    }
	  if (ret > 0)
	    {
	      done_length += ret;
	      blockno++;
	    }
	  else
	    {
	      error = E_SYS;
	      goto bad;
	    }
	}
    }

/*  done_length -= packet->start - bufstart; */
  if (((done_length < packet->size) ? done_length : packet->size) > BLOCK_SIZE * 2)
    {
      printf ("buffer overflow (size = %d)\n", (done_length < packet->size) ? done_length : packet->size);
      error = E_SYS;
      goto bad;
    }

  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.a_size = (done_length < packet->size) ? done_length : packet->size;
  res.body.wri_res.errcd = E_OK;
  res.body.wri_res.errinfo = 0;
  snd_mbf (caller, sizeof (res), &res);
#ifdef DEBUG
  printf ("ide: snd_mbuf done.\n");
#endif
  return (E_OK);

bad:
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.a_size = done_length;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
  
#undef H
#undef S
#undef LEN
}

/************************************************************************
 *	control_ide
 */
W
control_ide (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES		res;
  ER			error = E_OK;
  W			drive;
  struct ide_partition	*p;
  W			i;

  drive = packet->dd >> 3;
  switch (packet->cmd)
    {
    case IDE_GET_STAT:
      /* IDE �ξ������Ф�
       */
      error = read_stat (drive, &ide_spec[drive]);
      break;

    case IDE_GET_GEOMETRIC:
      printf ("IDE_GET_GEOMETRIC: start. (drive = %d)\n", drive);
      error = read_partition (drive);
      p = (struct ide_partition *)res.body.ctl_res.res;
      for (i = 0; i < IDE_MAX_PARTITION; i++)
	{
	  p[i].boot_flag = ide_partition[drive][i].boot_flag;
#ifdef notdef
	  p[i].start_head = ide_partition[drive][i].start_head;
	  p[i].start_sector = ide_partition[drive][i].start_sector;
	  p[i].start_cylinder = ide_partition[drive][i].start_cylinder;
#endif
	  p[i].length = ide_partition[drive][i].length;
	  p[i].type = ide_partition[drive][i].type;
	  p[i].start = ide_partition[drive][i].start;
	}
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


ER
read_partition (W drive)
{
  static UB		buf[BLOCK_SIZE];
  W			rlength;
  UW			*ip;
  W			i;
  ER			error = E_OK;
  struct ide_partition	*table;

  error = read_stat (drive, &ide_spec[0]);
  if (error)
    {
      return (error);
    }

  rlength = get_data (drive, 0, 0, 1, buf, 1);	/* H = 0, C = 0, S = 1 */
  printf ("IDE: get_data length = %d\n", rlength);
  bcopy (&buf[PARTITION_OFFSET], ide_partition[drive], sizeof (struct ide_partition) * IDE_MAX_PARTITION);

#ifdef notdef
  if (rlength > 0)
    {
      table = ide_partition[drive];
      for (i = 0; i < IDE_MAX_PARTITION; i++)
	{
	  table[i].boot_flag = buf[(PARTITION_OFFSET + BOOT_FLAG_OFFSET + (PARTITION_SIZE * i))];
#ifdef notdef
	  table[i].start_head = buf[(PARTITION_OFFSET + HEAD_OFFSET + (PARTITION_SIZE * i))];
	  table[i].start_sector = buf[(PARTITION_OFFSET + SECTOR_OFFSET + (PARTITION_SIZE * i))];
	  table[i].start_cylinder = buf[(PARTITION_OFFSET + CYLINDER_OFFSET + (PARTITION_SIZE * i))];
#endif
	  (char *)ip = &buf[(PARTITION_OFFSET + TOTAL_SECTOR_OFFSET + (PARTITION_SIZE * i))];
	  table[i].length = *ip;
	  table[i].type = buf[(PARTITION_OFFSET + TYPE_OFFSET + (PARTITION_SIZE * i))];
#ifdef notdef
	  table[i].start = (table[i].start_sector - 1)
	    + (table[i].start_head * ide_spec[drive].n_sector)
	      + (table[i].start_cylinder
		 * ide_spec[drive].n_head
		 * ide_spec[drive].n_sector);
#else
	  table[i].start = (table[i].start_sector - 1)
	    + (table[i].start_head * ide_spec[drive].now_sector_track)
	      + (table[i].start_cylinder
		 * ide_spec[drive].now_head
		 * ide_spec[drive].now_sector_track);
#endif
	  printf ("ide partition: drive = %d, partition = %d, start = %d, size = %d\n",
		  drive, i, table[i].start, table[i].length);
	}
      return (E_OK);
    }
#endif /* notdef */

  return (E_OK);
}


void
intr_ide (void)
{
  dis_int ();
  ide_intr_flag = TRUE;
  reset_intr_mask (IDE0_INTR_MASK);
  ena_int ();
}



reset_ide (W driver)
{
}

void
busywait(W x)
{
  int i,j;
  int	tmp;

  for (i = 0; i < x; i++)
     for (j = 0; j < 100; j++)
       tmp = j;
}

