/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.5  1996-01-02 16:20:30  night
 * Ω���夲���˲��̤򥯥ꥢ����褦���ѹ�������
 *
 * Revision 1.4  1995/09/21  15:50:55  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/09/17  16:50:38  night
 * �������ѥ��åȤ������ src/kernel/kernlib/device.h �˹�碌����
 * open/close/read/write/control �ؿ������������
 *
 * Revision 1.2  1995/09/14  04:28:43  night
 * init_driver() �� init_console() ���ѹ���
 * ��å����������������ɲá�
 *
 * Revision 1.1  1995/09/12  18:18:11  night
 * �ǽ����Ͽ
 *
 *
 *
 */

/**************************************************************************
 Discription

**********************************************************************/

#include "console.h"

/*********************************************************************
 *	 �ɽ��ѿ��������
 *
 */
static void	main_loop ();
static void	doit (DDEV_REQ *packet);


/*********************************************************************
 *	 ����ѿ��������
 *
 */
ID	recvport;
W	initialized = 0;


/*
 * console �ǥХ����ɥ饤�Ф� main �ؿ�
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
  init_console ();

  /*
   * Ω���夲��å�����
   */
  dbg_printf ("console driver start\n");
  dbg_printf ("  receive port is %d\n", recvport);

  main_loop ();
}

static void
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
/*	  dbg_printf ("console: receive packet type = %d\n", req.header.msgtyp);	/* */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("CONSOLE: get_req() Unknown error(error = %d)\n", err);
	  break;
	}
    }

  /* �����ιԤˤϡ���ʤ� */
}

/*
 * �����
 *
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 */
W
init_console (void)
{
  int		i;
  ER		error;

  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_REQ), sizeof (DDEV_REQ));
  if (recvport <= 0)
    {
      dbg_printf ("CONSOLE: cannot make receive port.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (CONSOLE_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("console: cannot regist port (error = %d)\n", error);
    }
  initialized = 1;
  clear_console ();
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
	  init_console ();
	}
      open_console (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_console (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_console (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_console (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_console (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/************************************************************************
 * open_console --- console �Υ����ץ�
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
open_console (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_console --- �ɥ饤�ФΥ�����
 *
 * ������	dd	console �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	���󥽡���ϥ������ν����ǤϤʤˤ⤷�ʤ���
 *
 */
W
close_console (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_console --- 
 *
 * ������	caller
 *		packet
 *
 * ���͡�	E_NOSPT ���֤���
 *
 * ������	��å������������� E_NOSPT ���֤���
 *		���󥽡���ɥ饤�Ф�ɽ����������ʤΤ����Ϥϥ��ݡ��Ȥ���
 *		���ʤ���
 *
 */
W
read_console (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * write_console
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
write_console(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;
  int		i;
  ER		error;

  error = E_OK;
  for (i = 0; i < (packet->size); i++)
    {
      error = write_char (packet->dt[i]);
      if (error != E_OK)
	break;
    }  
  res.body.wri_res.dd = packet->dd;
  res.body.wri_res.errcd = error;
  res.body.wri_res.errinfo = error;
  res.body.wri_res.a_size = i;
  snd_mbf (caller, sizeof (res), &res);
  return (error);
}

/************************************************************************
 * control_console
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
control_console (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;

  switch (packet->cmd)
    {
    case CONSOLE_CLEAR:
      clear_console ();
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    case CONSOLE_MOVE:
      if (packet->len != 2)
	{
	  res.body.ctl_res.dd = packet->dd;
	  res.body.ctl_res.errcd = E_PAR;
	  res.body.ctl_res.errinfo = E_PAR;
	  snd_mbf (caller, sizeof (res), &res);
	  return (E_PAR);
	}
      set_curpos (packet->param[0], packet->param[1]);
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

