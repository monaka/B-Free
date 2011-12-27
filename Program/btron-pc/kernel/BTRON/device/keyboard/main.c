/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  2000-04-03 14:30:05  naniwa
 * to call timer handler in task
 *
 * Revision 1.6  2000/02/27 15:28:43  naniwa
 * to work as multi task OS
 *
 * Revision 1.5  2000/01/30 19:02:53  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.4  2000/01/24 23:28:52  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.3  2000/01/23 15:50:49  kishida0
 * 106JP��101US �����ܡ������ؤ��ѤΥ��ޥ�ɤ��ɲä�����
 * �ؿ��ˤ��Ƥ���Τ����ޤ����������Ȥꤢ�����ϻ��Ѳ�ǽ
 * note-pc�ѤˤϤ⤦���������Υơ��֥���ǧ����ɬ�פ�����
 * �ץ�ݡ���
 *
 * Revision 1.2  1999/03/15 05:58:27  monaka
 * modified some debug messages.
 *
 * Revision 1.1  1996/07/24 16:28:44  night
 * IBM PC �� B-Free OS �ؤκǽ����Ͽ
 *
 * Revision 1.4  1995/10/01  12:55:24  night
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
#include "keyboard_type.h"
#include "../device/wconsole/wconsole.h"

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
ID	wconsole = 0, local_recv = 0;
W	send_switch = 0;
ID	my_tskid;


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
  dbg_printf ("keyboard driver started. receive port is %d\n", recvport);

  main_loop ();
}

static void
main_loop ()
{
  DDEV_REQ	req;	/* ���������׵�ѥ��å� */
  ER		errno;
  INT		rsize;

  /*
   * �׵���� - �����Υ롼��
   */
  rsize = sizeof (req);
  for (;;) {

    /* �׵�μ��� */
#ifdef undef
    errno = trcv_mbf (&req, &rsize, recvport, 51);
#else
    errno = rcv_mbf (&req, &rsize, recvport);
#endif
    switch (errno) {
    case E_OK:
      /* ���辰���� */
      doit (&req);
      break;

    case E_TMOUT:
    case E_RLWAI:
      break;
      
    default:
      /* Unknown error */
      dbg_printf ("KEYBOARD: get_req() Unknown error(error = %d)\n", errno);
      break;
    }

    if (send_switch) {
      send_switch = 0;
      send_switch_msg();
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

  get_tid(&my_tskid);
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

    case DEV_PRD:
      posix_read_keyboard (packet->header.mbfid, packet->header.tskid,
			   &(packet->body.prd_req));
      break;

    case DEV_RLY:
      relay_keyboard (packet->header.mbfid, &(packet->body.rly_req));
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
 * ���͡�	E_OK ���֤���
 *
 * ������	��å��������������ɤ߹����ʸ������֤���
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

#if 0
  dbg_printf("KEYBOARD: send to caller\n");
#endif
  snd_mbf (caller, sizeof (res), &res);
  return (E_OK);
}

/*************************************************************************
 * posix_read_keyboard --- 
 *
 * ������	caller
 *		packet
 *
 * ���͡�	E_OK ���֤���
 *
 * ������	��å������������(POSIX lowlib)��ʸ������֤���
 *
 */
W
posix_read_keyboard (ID caller, ID tskid, DDEV_PRD_REQ *packet)
{
  struct posix_response res;
  W		i;

#ifdef notdef
    dbg_printf("\nKEYBOARD: posix_read_keyboard %d len = %d\n",
	       caller, packet->length);
#endif
  {
    B buf[packet->length+1];

    for (i = 0; i < packet->length; i++) {
      buf[i] = read_key (driver_mode);
    }
    buf[packet->length] = 0;
    vput_reg(tskid, packet->buf, packet->length, buf);

    res.receive_port = 0;
    res.msg_length = sizeof (res);
    res.operation = PSC_READ;
    res.errno = EP_OK;
    res.status = packet->length;
    res.ret1 = 0;
    res.ret2 = 0;

#ifdef notdef
    dbg_printf("KEYBOARD: reply to app %d\n", caller);
#endif
    snd_mbf (caller, sizeof (res), &res);
    return (E_OK);
  }
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

    case KEYBOARD_CHANGE_106JP:
      keyboard_select = K_106JP;
      res.body.ctl_res.dd = packet->dd;
      res.body.ctl_res.errcd = E_OK;
      res.body.ctl_res.errinfo = E_OK;
      snd_mbf (caller, sizeof (res), &res);
      return (E_OK);

    case KEYBOARD_CHANGE_101US:
      keyboard_select = K_101US;
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

W send_switch_msg()
{
  DDEV_REQ	req;
  DDEV_RES	res;
  W		rsize;
  W		error;

  if (wconsole == 0) {
    if (find_port (WCONSOLE_DRIVER, &wconsole) != E_PORT_OK)
      return(E_DEV);
  }
  if (local_recv == 0) {
    local_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
    if (local_recv <= 0) return(E_NOMEM);
  }

  req.header.mbfid = local_recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.cmd = WC_CHGFCS;
  req.body.ctl_req.len = 0;
#ifdef notdef
  dbg_printf("KEYBOARD: send to wconsole\n");
#endif
  error = snd_mbf (wconsole, sizeof (req), &req);
  if (error != E_OK) {
    dbg_printf("KEYBOARD: can't send to wconsole %d\n", error);
    return (error);
  }
  rsize = sizeof (res);
#ifdef notdef
  dbg_printf("KEYBOARD: wait from wconsole\n");
#endif
  error = rcv_mbf (&res, (INT *)&rsize, local_recv);
#ifdef notdef
  dbg_printf("KEYBOARD: recv from wconsole\n");
#endif
  if (res.body.ctl_res.errcd != E_OK) {
    return (res.body.ctl_res.errcd);
  }
}

/* relay_keyboard()
 */
W relay_keyboard (ID caller, DDEV_RLY_REQ *packet)
{
  DDEV_REQ	req;
  W		i;

  req.header.mbfid = local_recv;
  req.header.msgtyp = DEV_RLY;
  req.body.rly_req.dd = packet->dd;
#ifdef notdef
  dbg_printf("KEYBOARD: read %d\n", packet->size);
#endif
  for (i = 0; i < packet->size; i++)
    {
      req.body.rly_req.dt[i] = read_key (driver_mode);
    }
  req.body.rly_req.size = i;

#ifdef notdef
  dbg_printf("KEYBORAD: send relay to wconsole\n");
#endif
  snd_mbf (caller, sizeof (req), &req);
  return (E_OK);
}
