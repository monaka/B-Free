/*

Copyright 1994,1995 (C) B-Free Project.


B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

*/
/* $Id: main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/wd33c93/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1995-12-05 15:18:02  night
 * �ǽ����Ͽ��
 * ���ΥС������Ǥϡ��ǥХ��������褿���顼�Υ����å���ԤäƤ��ʤ��Τǡ�
 * ��դ��뤳�ȡ�
 *
 *
 */

#include "scsi.h"

struct diskparam	disk_table[8];



/*********************************************************************
 *	 �ɽ��ѿ��������
 *
 */
static void	doit (DDEV_REQ *packet);

/*********************************************************************
 *	 ����ѿ��������
 *
 */
ID	recvport;
W	initialized = 0;
struct adaptor	adaptor;
W	scsi_bus_status;

/*
 *
 */
start ()
{
  if (!probe ())
    {
      /* SCSI �����ץ���¸�ߤ��ʤ� */
      dbg_printf ("SCSI: SCSI card is not detected.\n");
      dbg_printf ("SCSI: stop.\n");
      for (;;)
	slp_tsk ();
    }

  init_scsi ();			/* SCSI �����ץ��ν���� */

  main_loop ();

#ifdef notdef
  inquery_scsi (0, 0);
  print_disk_param (0, 0);
  inquery_scsi (2, 0);
  print_disk_param (2, 0);
#endif

#ifdef DOS
  {
    UB	buf[1024];
    struct dosdir 
      {
	UB	name[8];
	UB	extention[3];
	UB	type;
	UB	reserve[10];
	UH	mtime;
	UH	mday;
	UH	start_cls;
	UW	size;
      } *dirp;
    struct dosfat
      {
	UB	id;
	UB	reserve[3];
	UH	cluster[0];
      } *dosfatp;
    W	i, j;
    W	count;
    W	fat1;
    W	rootdir_addr;
    struct boot	*bootp;

    bootp = &(disk_table[2].partition_table[1].bootinfo);
    dbg_printf ("version ");
    for (i = 0; i < 8; i++)
      {
	putchar (bootp->ver[i]);
      }
    dbg_printf ("\n");
    dbg_printf ("type:    %d\n", bootp->disk_type);
    dbg_printf ("cls/sec: %d\n", bootp->cls_sec);
    dbg_printf ("sec/byte:%d\n", bootp->sec_byte);
    dbg_printf ("bootsec: %d\n", bootp->bootsec);
    dbg_printf ("nfat:    %d\n", bootp->nfat);
    dbg_printf ("rootsize:%d\n", bootp->rootsize);
    dbg_printf ("fat/sec: %d\n", bootp->fat_sec);
    dbg_printf ("head:    %d\n", bootp->head_disk);
    dbg_printf ("nsector: %d\n", bootp->sec_disk);
    dbg_printf ("dir/sec: %d\n", (bootp->rootsize * 32) / bootp->sec_byte);

    fat1 = bootp->bootsec;
    read_block_in_partition (2, 0, 1, fat1, 1, buf);
    dosfatp = (struct dosfat *)buf;
    dbg_printf ("fat ID = 0x%x\n", dosfatp->id);

    rootdir_addr = bootp->bootsec + (bootp->nfat * bootp->fat_sec);
    dbg_printf ("rootdir = %d\n", rootdir_addr);
    for (count = 0; count < ((bootp->rootsize * 32) / bootp->sec_byte) / 16; count++)
      {
	read_block_in_partition (2, 0, 1, rootdir_addr + count, 1, buf);
	dirp = (struct dosdir *)buf;
	for (i = 0; i < 16; i++)
	  {
	    if (dirp[i].name[0] == ' ')
	      {
		dbg_printf ("dir: end entry.\n");
		goto done;
	      }
	    if ((dirp[i].name[0] != ' ') && (dirp[i].name[0] != NULL))
	      {
		dbg_printf ("%d: ", count * 16 + i);
		for (j = 0; j < 8; j++)
		  putchar (dirp[i].name[j]);
		putchar ('.');
		for (j = 0; j < 3; j++)
		  putchar (dirp[i].extention[j]);
		dbg_printf ("  type = %d\n", dirp[i].type);
		if (strncmp (dirp[i].name, "BAR", 3) == 0)
		  goto done;
	      }
	  }
      }
  }
done:
#endif /* DOS */
  slp_tsk ();
  return (TRUE);
}


get_disk_param (W target, W lun)
{
  UB	buf[1024];
  UB	bootbuf[1024];
  UB	*bufp;
  W	i;
  struct boot	*bootp;

  /* page 3 ���ɤ߼�� */
  /* �⡼�ɥѥ�᡼���إå� + �֥�å��ǥ������ץ꥿ + �ѥ�᡼���ڡ��� */
  /* ���ɤ߼�� */
  mode_sense (target, lun, 3, 4 + 8 + 24, buf);

  disk_table[target].track_zone  = (buf[14] << 8) | buf[15];
  disk_table[target].rsect_zone  = (buf[16] << 8) | buf[17];
  disk_table[target].rtrack_zone = (buf[18] << 8) | buf[19];
  disk_table[target].rtrack_unit = (buf[20] << 8) | buf[21];
  disk_table[target].sec_track = (buf[22] << 8) | buf[23];
  disk_table[target].block_size = (buf[24] << 8) + buf[25]; 
  

  /* page 4 ���ɤ߼�� */
  mode_sense (target, lun, 4, 4 + 8 + 20, buf);

  disk_table[target].cylinder = (buf[14] << 16) | (buf[15] << 8) | buf[16];
  disk_table[target].head = buf[17];

  /* �ѡ��ƥ������ơ��֥���ɤ߼�� */
  read_block (target, lun, 1, 1, buf);
  for (i = 0; i < MAXPART; i++)
    {
      bufp = buf + (32 * i);
      disk_table[target].partition_table[i].start = ((bufp[11] << 8) | bufp[10]) * 
	(disk_table[target].sec_track * disk_table[target].head);
      disk_table[target].partition_table[i].end   = 
	(((bufp[15] << 8) | bufp[14]) + 1) *
	  (disk_table[target].sec_track * disk_table[target].head) - 1;

      disk_table[target].partition_table[i].size = 
	disk_table[target].partition_table[i].end - 
	  disk_table[target].partition_table[i].start + 1;
	
      if (disk_table[target].partition_table[i].start == 0)
	break;

#ifdef DOS
      bootp = &(disk_table[target].partition_table[i].bootinfo);
      read_block_in_partition (target, lun, i, 0, 1, bootbuf);
      bcopy (&buf[3], bootp->ver, 8);
      bootp->sec_byte = (buf[12] << 8) | buf[11];
      bootp->cls_sec  = buf[13];
      bootp->bootsec  = (buf[15] << 8) | buf[14];
      bootp->nfat     = buf[16];
      bootp->rootsize = (buf[18] << 8) | buf[17];
      bootp->sec_disk = (buf[20] << 8) | buf[19];
      bootp->disk_type = buf[21];
      bootp->fat_sec  = (buf[23] << 8) | buf[22];
      bootp->track_sec = (buf[25] << 8) | buf[24];
      bootp->head_disk = (buf[27] << 8) | buf[26];
#endif /* DOS */
    }
}

print_disk_param (W target, W lun)
{
  W	i;

  dbg_printf ("target = %d, lun = %d\n", target, lun);
  dbg_printf ("capacity = %d\n", read_capacity (target, lun));
  dbg_printf ("track/zone  = %d, ", disk_table[target].track_zone);
  dbg_printf ("rsect/zone  = %d, ", disk_table[target].rsect_zone);
  dbg_printf ("rtrack/zone = %d, ", disk_table[target].rtrack_zone);
  dbg_printf ("rtrack/unit = %d\n", disk_table[target].rtrack_unit);
  dbg_printf ("sec/track   = %d, ", disk_table[target].sec_track);
  dbg_printf ("block size  = %d, ", disk_table[target].block_size);
  dbg_printf ("cylinder = %d, ", disk_table[target].cylinder);
  dbg_printf ("head     = %d\n", disk_table[target].head);
  for (i = 0; i < MAXPART; i++)
    {
      if (disk_table[target].partition_table[i].start == 0)
	break;
      dbg_printf ("p[%d] = %d - %d (%d)\n", 
		  i,
		  disk_table[target].partition_table[i].start,
		  disk_table[target].partition_table[i].end,
		  disk_table[target].partition_table[i].size);
    }

  dbg_printf ("\n");
}

/*
 * �׵���� - ���� - ����
 *
 * 
 */
main_loop ()
{
  DDEV_REQ		req;	/* ���������׵�ѥ��å� */
  ER			err;
  extern ER		sys_errno;

  /*
   * �׵���� - �����Υ롼��
   */
  for (;;)
    {
      W	rsize;

      /* �׵�μ��� */
/*      dbg_printf ("call get_req ()\n"); */
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* ���辰���� */
/*	  dbg_printf ("scsi: receive packet type = %d\n", req.header.msgtyp);	/* */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("scsi: get_req() Unknown error(error = %d)\n", err);
	  break;
	}
    }

  /* �����ιԤˤϡ���ʤ� */
}



/*
 * SCSI �����ץ��ν������Ԥ�
 */
W
init_scsi ()
{
  W		i;
  ER		error;
  T_DINT	pkt;
  UB		*scsiflag;
  UW		*info;


  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_REQ), sizeof (DDEV_REQ));
  if (recvport <= 0)
    {
      dbg_printf ("SCSI: cannot make receive port.\n");
      return (E_DEV);
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (SCSI_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("SCSI: cannot regist port (error = %d)\n", error);
      for (;;)
	;
      return (error);
    }
  initialized = 1;

  if (!reset_scsi ())
    {
      unregist_port (SCSI_DRIVER);
      return (E_DEV);
    }

  /* �����ߥϥ�ɥ����Ͽ */
  pkt.intatr = ATR_INTR;
  pkt.inthdr = (FP)intr_scsi;
  def_int (adaptor.intrn, &pkt);

  /* �����ߥޥ�����ꥻ�åȤ��� */
  reset_intr_mask (adaptor.intrn - 32);

  scsiflag = (UB *)0x80000482;
  dbg_printf ("SCSI ID BITS: 0x%x\n", *scsiflag);
  info = (UW *)0x80000460;
  for (i = 0; i < 7; i++)
    {
      if (*scsiflag & (1 << i))
	{
	  disk_table[i].scsiid = i;
	  get_disk_param (i, 0);

#ifdef notdef
	  disk_table[i].sec_track = info[i] & 0xff;
	  disk_table[i].head = (info[i] >> 8) & 0x0f;
	  disk_table[i].cylinder = ((info[i] & 0x0f000) 
				    | ((info[i] >> 16) & 0xff) 
				    | (((info[i] >> 24) & 0xf) << 0x8));
	  if (((info[i] >> 28) & 0x03) == 0)
	    {
	      disk_table[i].block_size = 256;
	    }
	  else if (((info[i] >> 28) & 0x03) == 1)
	    {
	      disk_table[i].block_size = 512;
	    }
	  else if (((info[i] >> 28) & 0x03) == 2)
	    {
	      disk_table[i].block_size = 1024;
	    }
#endif /* notdef */
	}
    }
  
  return (E_OK);
}


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
      if (!initialized)
	{
	  init_scsi ();
	}
      open_scsi (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_scsi (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_scsi (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_scsi (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_scsi (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/************************************************************************
 * open_scsi --- scsi �Υ����ץ�
 *
 * ������	caller	��å������������
 *		packet	�����ץ�ѥ��å�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	E_OK ���å��������������֤���
 *
 */
W
open_scsi (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_scsi --- �ɥ饤�ФΥ�����
 *
 * ������	dd	scsi �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	���󥽡���ϥ������ν����ǤϤʤˤ⤷�ʤ���
 *
 */
W
close_scsi (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_scsi --- 
 *
 * ������	caller
 *		packet
 *			dd	�ǥХ����ɥ饤�Хޥ͡������������
 *			start	�ǥХ�������Ƭ����ΰ��֡�ʪ���֥�å�ñ�̡�
 *			size	�񤭹����̡�ʪ���֥�å�ñ�̡�
 *
 * ���͡�	dd		�ǥХ����ɥ饤�Хޥ͡������������
 *		errcd
 *		errinfo		���顼�ܺپ���
 *		split		ʬ�����
 *		a_size		�ºݤ��ɤ߹�����֥�å���
 *		dt[1024]	�ɤ߹�����ǡ���
 *
 * ����
 *
 */
W
read_scsi (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;
  W		target;
  W		lun;
  W		part;
  UB		buf[1024];
  
  if (packet->size == 512)
    {
      target = (packet->dd >> 7)& 0xff;		/* SCSI ID �μ��Ф� */
      lun = (packet->dd >> 4) & 0x07;		/* SCSI LUN �μ��Ф� */
      part = packet->dd & 0x0f;			/* �ѡ��ƥ�������ֹ� */
      read_block_in_partition (target, lun, part, packet->start, packet->size, buf);
      res.body.rea_res.dd = packet->dd;
      res.body.rea_res.errcd = E_OK;
      res.body.rea_res.errinfo = E_OK;
      res.body.rea_res.a_size = packet->size;
      bcopy (buf, res.body.rea_res.dt, disk_table[target].block_size);
      snd_mbf (caller, sizeof (res), &res);
    }
  else
    {
      res.body.rea_res.dd = packet->dd;
      res.body.rea_res.errcd = E_PAR;
      res.body.rea_res.errinfo = E_NOSPT;
      snd_mbf (caller, sizeof (res), &res);
    }

  return (E_NOSPT);
}


/* ���ꤷ���ǥХ��� + �ѡ��ƥ������Υ֥�å����ɤ߼��
 *
 */
W
read_block_in_partition (W target, W lun, W partition, W start, W length, UB *buf)
{
  W startblock;

  start = BYTE_TO_BLOCK (target, start);
  length = BYTE_TO_BLOCK (target, length);

  if ((start + length)  > disk_table[target].partition_table[partition].size)
    {
      return (-1);
    }

  startblock = disk_table[target].partition_table[partition].start + start;
  read_block (target, lun, startblock, length, buf);
}



/************************************************************************
 * write_scsi
 *
 * ������	caller
 *		packet
 *
 * ���͡�	
 *
 * ������	�����ȥݥ��󥿤ΰ��֤�ʸ�����ɽ�����롣
 *		���ΤȤ������������ץ������󥹤ˤ�������Ԥ���		
 *
 */
W
write_scsi(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  W		target;
  W		lun;
  W		part;
  UB		buf[1024];
  
  if (packet->size == 1)
    {
      target = (packet->dd >> 7)& 0xff;		/* SCSI ID �μ��Ф� */
      lun = (packet->dd >> 4) & 0x07;		/* SCSI LUN �μ��Ф� */
      part = packet->dd & 0x0f;			/* �ѡ��ƥ�������ֹ� */
      write_block_in_partition (target, lun, part, packet->start, packet->size, buf);
      res.body.rea_res.dd = packet->dd;
      res.body.rea_res.errcd = E_OK;
      res.body.rea_res.errinfo = E_OK;
      bcopy (buf, res.body.rea_res.dt, disk_table[target].block_size);
      snd_mbf (caller, sizeof (res), &res);
    }
  else
    {
      res.body.rea_res.dd = packet->dd;
      res.body.rea_res.errcd = E_PAR;
      res.body.rea_res.errinfo = E_NOSPT;
      snd_mbf (caller, sizeof (res), &res);
    }

  return (E_NOSPT);
}

/* ���ꤷ���ǥХ��� + �ѡ��ƥ������Υ֥�å����ɤ߼��
 *
 */
W
write_block_in_partition (W target, W lun, W partition, W start, W length, UB *buf)
{
  W startblock;

  start = BYTE_TO_BLOCK (target, start);
  length = BYTE_TO_BLOCK (target, length);

  if ((start + length)  > disk_table[target].partition_table[partition].size)
    {
      return (-1);
    }

  startblock = disk_table[target].partition_table[partition].start + start;
  write_block (target, lun, startblock, length, buf);
}


/************************************************************************
 * control_scsi
 *
 * ������	caller
 *		packet
 *
 * ���͡�	E_NOSPT ���֤���
 *
 * ������
 *
 */
W
control_scsi (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;
  W		target, lun;

  switch (packet->cmd)
    {
    case SCSI_GET_INFO:
      res.body.ctl_res.dd = packet->dd;
      target = (packet->dd >> 7) & 0xff;	/* SCSI ID �μ��Ф� */
      lun = (packet->dd >> 4) & 0x07;		/* SCSI LUN �μ��Ф� */
      print_disk_param (target, lun);
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    case SCSI_GET_PARTITION:
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);
      
    default:
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_NOSPT;
      res.body.ctl_res.errinfo = E_NOSPT;
      snd_mbf (caller, sizeof (res), &res);
      return (E_NOSPT);
    }
}








