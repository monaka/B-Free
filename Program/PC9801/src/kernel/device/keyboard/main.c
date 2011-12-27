/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/main.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-10-01 12:55:24  night
 * KEYBOARD_CHANGEMODE �ˤĤ��Ƥν������ɲá�
 *
 * Revision 1.3  1995/09/21  15:51:00  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/19  18:01:02  night
 * �����ܡ��ɤ������ϤǤ���Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1995/09/18  11:42:02  night
 * �ǽ����Ͽ
 *
 *
 *
 */

/**************************************************************************
 Discription

**********************************************************************/

#include "keyboard.h"

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
ID	waitflag;		/* �����ܡ��ɤ��饭�����Ϥ��ԤĻ��� */
				/* ���Ѥ��륤�٥�ȥե饰�� ID */
W	driver_mode;


/*
 * keyboard �ǥХ����ɥ饤�Ф� main �ؿ�
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
  init_keyboard ();

  /*
   * Ω���夲��å�����
   */
  dbg_printf ("keyboard driver start\n");
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
      rsize = sizeof (req);
      get_req (recvport, &req, &rsize);
      switch (sys_errno)
	{
	case E_OK:
	  /* ���辰���� */
	  doit (&req);
	  break;

	default:
	  /* Unknown error */
	  dbg_printf ("KEYBOARD: get_req() Unknown error(error = %d)\n", err);
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
init_keyboard (void)
{
  int		i;
  ER		error;

  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_REQ), sizeof (DDEV_REQ));
  if (recvport <= 0)
    {
      dbg_printf ("KEYBOARD: cannot make receive port.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (KEYBOARD_DRIVER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("keyboard: cannot regist port (error = %d)\n", error);
    }

  init_keyboard_interrupt ();	/* �����ߥϥ�ɥ����Ͽ */
  init_keybuffer ();		/* �����ܡ��ɥХåե��ν���� */

  /* �������Ϥ��ԤĻ��˻��Ѥ��륤�٥�ȥե饰�ν���� */
  waitflag = get_flag (TA_WSGL, 0);
  dbg_printf ("keyboard: eventflag = %d\n", waitflag);	/* */

  driver_mode = WAITMODE;

  initialized = 1;
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
	  init_keyboard ();
	}
      open_keyboard (packet->header.mbfid, &(packet->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_keyboard (packet->header.mbfid, &(packet->body.cls_req));
      break;

    case DEV_REA:
      read_keyboard (packet->header.mbfid, &(packet->body.rea_req));
      break;

    case DEV_WRI:
      write_keyboard (packet->header.mbfid, &(packet->body.wri_req));
      break;

    case DEV_CTL:
      control_keyboard (packet->header.mbfid, &(packet->body.ctl_req));
      break;
    }
}

/************************************************************************
 * open_keyboard --- keyboard �Υ����ץ�
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
open_keyboard (ID caller, DDEV_OPN_REQ *packet)
{
  DDEV_RES	res;

  res.body.opn_res.dd = packet->dd;
  res.body.opn_res.errcd = E_OK;
  res.body.opn_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/************************************************************************
 * close_keyboard --- �ɥ饤�ФΥ�����
 *
 * ������	dd	keyboard �ɥ饤���ֹ�
 *		o_mode	�����ץ�⡼��
 *		error	���顼�ֹ�
 *
 * ���͡�	��� E_OK ���֤���
 *
 * ������	�����ܡ��ɤϥ������ν����ǤϤʤˤ⤷�ʤ���
 *
 */
W
close_keyboard (ID caller, DDEV_CLS_REQ *packet)
{
  DDEV_RES	res;

  res.body.cls_res.dd = packet->dd;
  res.body.cls_res.errcd = E_OK;
  res.body.cls_res.errinfo = E_OK;
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * read_keyboard --- 
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
read_keyboard (ID caller, DDEV_REA_REQ *packet)
{
  DDEV_RES	res;
  W		i;

  res.body.rea_res.dd = packet->dd;

  for (i = 0; i < packet->size; i++)
    {
      res.body.rea_res.dt[i] = read_key (driver_mode);
    }
  res.body.rea_res.a_size = i;
  res.body.rea_res.errcd = E_OK;
  res.body.rea_res.errinfo = E_OK;

  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * write_keyboard
 *
 * ������	caller
 *		packet
 *
 * ���͡�	��� E_NOSPT ���֤���
 *
 * ������	write �ϡ������ܡ��ɤǤϹԤ�ʤ���
 *
 */
W
write_keyboard(ID caller, DDEV_WRI_REQ *packet)
{
  DDEV_RES	res;

  res.body.rea_res.dd = packet->dd;
  res.body.rea_res.errcd = E_NOSPT;
  res.body.rea_res.errinfo = E_NOSPT;
  snd_mbf (caller, sizeof (res), &res);
  return (E_NOSPT);
}

/************************************************************************
 * control_keyboard
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
control_keyboard (ID caller, DDEV_CTL_REQ *packet)
{
  DDEV_RES	res;

  switch (packet->cmd)
    {
    case KEYBOARD_CLEAR:
      clear_keybuffer ();
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    case KEYBOARD_CHANGEMODE:
      driver_mode = packet->param[0];
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

