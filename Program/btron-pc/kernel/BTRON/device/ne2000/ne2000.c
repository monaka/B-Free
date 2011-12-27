/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/ne2000.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/ne2000.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: ne2000.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  1999-03-07 12:45:58  night
 * write_ne2000() �� read_ne2000() ����Ȥ��ɲá�
 *
 * Revision 1.7  1999/03/02 15:15:15  night
 * �����������㴳�ѹ�
 *
 * Revision 1.6  1998/02/23 14:40:09  night
 * get_req �ΰ������ְ�äƤ������ὤ����
 * ���ޤǤΥ����ɤ��ȡ����۶��֤���Ƭ��ʬ���˲����Ƥ�����
 *
 * Revision 1.5  1997/10/29 22:39:27  night
 * NE2000 �����ץ��� SA_PROM ���ɤ߼��������ɲá�
 *
 * Revision 1.4  1997/10/28 14:44:25  night
 * control_ne2000 () �� NE2000_PROBE �ν������ɲá�
 *
 * Revision 1.3  1997/10/28 12:46:00  night
 * ne2000_internal.h �Υ��󥯥롼��ʸ���ɲá�
 * dev_table ��������ɲá�
 * control_ne2000 () �� NE2000_RESET �ν������ɲá�
 *
 * Revision 1.2  1997/05/17 10:41:15  night
 * �ե������ʸ�������ɤ� SJIS ���� EUC ���ѹ���
 *
 * Revision 1.1  1997/05/06 12:43:31  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 *
 */

#include "ne2000.h"
#include "ne2000_internal.h"



struct ne2000_device	dev_table[MAX_NE2000];
int			max_adaptor;
UB			read_buffer[MAX_MTU_LENGTH];
W			read_buffer_length;	



/*
 *	 �ɽ��ѿ��������
 *
 */
static W	mydevid;	/* ��ʬ���Ȥ�id */
static ID	recvport;	/* �׵�����Ĥ��ѥݡ��� */
static W	initialized;



static void	main_loop (void);
static void	init_driver (struct ne2000_device *);


/* start --- ne2000 �ɥ饤�ФΥᥤ��ؿ�
 */
void
start ()
{
  /* NE2000 �����ץ���¸�ߤ��Ƥ��뤫������å����� */
  if (probe (&dev_table[0]) != E_OK)
    {
      dbg_printf ("Can not probe: ne2000\n");
      slp_tsk ();
    }


  max_adaptor = MAX_NE2000;	/* ��� 1 */
  
  init_driver (&dev_table[0]);
  printf ("registed ne2000 driver.\n");

  main_loop ();
}


/*
 * �����
 *
 * o �ե�����ơ��֥� (file_table) �ν����
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 */
static void
init_driver (struct ne2000_device *dev)
{
  int		i;
  ER		error;

  init_log ();

  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recvport <= 0)
    {
      dbg_printf ("ne2000: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (NE2000_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("ne2000: cannot make receive porrt.\n");
      slp_tsk ();
      /* port manager ����Ͽ���� */
    }

  for (i = 0; i < max_adaptor; i++)
    {
      if (init_adaptor (&dev_table[i]) != E_OK)
	{
	  dbg_printf ("ne2000: cannot initialize NE*000.\n");
	  slp_tsk ();
	}
	  
      if (init_device (&dev_table[i]) != E_OK)
	{
	  dbg_printf ("ne2000: cannot initialize NE*000.\n");
	  slp_tsk ();
	}
	  
    }

  return;
}



/*
 *
 */
static void
main_loop (void)
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
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* ���辰���� */
	  process_request (&req);
	  break;

	default:
	  /* Unknown error */
	  slp_tsk ();
	  break;
	}
    }

  /* �����ιԤˤϡ���ʤ� */
}


/*
 *
 */
ER
process_request (DDEV_REQ *req)
{
  switch (req->header.msgtyp)
    {
    case DEV_OPN:
      /* �ǥХ����Υ����ץ� */
      open_ne2000 (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_ne2000 (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_ne2000 (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_ne2000 (req->header.mbfid, req->header.tskid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_ne2000 (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 *
 */
ER
open_ne2000 (ID caller, DDEV_OPN_REQ *packet)
{
}

/*
 *
 */
ER
close_ne2000 (ID caller, DDEV_CLS_REQ *packet)
{
}

/*
 *
 */
ER
read_ne2000 (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_REA_RES	res;
  ER		err;

  res.dd = packet->dd;
  if (read_buffer_length > 0)
    {
      err = vput_reg (caller,
		      packet->start, 
		      (read_buffer_length > packet->size) ? packet->size : read_buffer_length,
		      read_buffer);
    }

  res.errcd = err;
  res.errinfo = 0;
  res.a_size = read_buffer_length;
  snd_mbf (caller, sizeof (res), &res);
  return (err);
}

/*
 *
 */
ER
write_ne2000 (ID caller, ID tskid, DDEV_WRI_REQ *packet)
{
  DDEV_REA_RES	res;
  ER		err;
  UB		buf[MAX_MTU_LENGTH];

  res.dd = packet->dd;

printf ("write_ne2000: write\n");
printf ("%s: %d\n", __FILE__, __LINE__);
  if (packet->size > MAX_MTU_LENGTH)
    {
      err = E_PAR;
    }
  else
    {
      int	i;

      vget_reg (tskid, packet->start, packet->size, buf);

      for (i = 0; i < 64; i++)
	{
	  printf ("buf[%d]: 0x%x ", i, buf[i]);
	  if ((i % 4) == 0)
	    {
	      printf ("\n");
	    }
	}

      err = write_frame (&dev_table[packet->dd & 0x0f], buf, packet->size);
    }

  res.errcd = err;
  res.errinfo = 0;
  snd_mbf (caller, sizeof (res), &res);
  return (err);
}

/*
 *
 */
ER
control_ne2000 (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;
  ER		errno;
  UW		devno;
  W		i;

  devno = packet->dd;
  if ((devno < 0) || (devno >= MAX_NE2000))
    {
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_PAR;
      res.body.ctl_res.errinfo = 0;
      snd_mbf (caller, sizeof (res), &res);
    }

  switch (packet->cmd)
    {
    case NE2000_RESET:
      errno = reset_adaptor (&dev_table[devno]);
      if (errno)
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_DEV;
	  res.body.ctl_res.errinfo = errno;
	  snd_mbf (caller, sizeof (res), &res);
	}
      else
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_OK;
	  res.body.ctl_res.errinfo = E_OK;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_OK);
	}
      break;

    case NE2000_PROBE:
      if (dev_table[devno].io_base != 0)
	{
	  errno = probe_1 (&dev_table[devno]);
	}
      else
	{
	  errno = E_DEV;
	}
      if (errno)
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_DEV;
	  res.body.ctl_res.errinfo = errno;
	  snd_mbf (caller, sizeof (res), &res);
	}
      else
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_OK;
	  res.body.ctl_res.errinfo = E_OK;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_OK);
	}
      break;

    case NE2000_GETMAC:
      for (i = 0; i < MACADDRLEN; i++)
	{
	  res.body.ctl_res.res[i] = dev_table[devno].sa_prom[i];
	}
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);
    }

  res.body.ctl_res.dd = packet->dd;
  res.body.ctl_res.errcd = E_PAR;
  res.body.ctl_res.errinfo = errno;
  snd_mbf (caller, sizeof (res), &res);
  return (E_PAR);
}

