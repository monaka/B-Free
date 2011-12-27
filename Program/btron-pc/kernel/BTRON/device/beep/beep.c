/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/beep/beep.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $*/
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/beep/beep.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: beep.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-03-15 05:58:22  monaka
 * modified some debug messages.
 *
 * Revision 1.1  1999/02/17 04:23:56  monaka
 * First version.
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/*
 *
 *
 */

#include "beep.h"
#include "beep_internal.h"


/*
 *	 �ɽ��ѿ��������
 *
 */
static W	mydevid;	/* ��ʬ���Ȥ�id */
static ID	recvport;	/* �׵�����Ĥ��ѥݡ��� */
static W	initialized;

/*struct device	dev_table[MAX_BEEP];*/


static void	main_loop (void);
static void	init_driver (void);


/* start --- beep �ɥ饤�ФΥᥤ��ؿ�
 */
void
start ()
{
/*  probe (&dev_table[0]); /* �ǥХ�����¸�ߤ��Ƥ��뤫������å����� */
  init_driver ();

  printf ("beep driver started.\n");
  main_loop ();
}


/*
 * �����
 *
 * o �ե�����ơ��֥� (file_table) �ν����
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 */
static void
init_driver (void)
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
      dbg_printf ("beep: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (BEEP_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

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
      open_beep (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_beep (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_beep (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_beep (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_beep (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * �ǥХ����Υ����ץ�
 */
ER
open_beep (ID caller, DDEV_OPN_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/
  B bPortData;
  bPortData = inb(BEEP_PORT);
  bPortData |= 3;
  outb(BEEP_PORT, bPortData);

  return (E_OK);
}

/*
 * �ǥХ����Υ�����
 */
ER
close_beep (ID caller, DDEV_CLS_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/
  B bPortData;
  bPortData = inb(BEEP_PORT);
  bPortData &= 0xFC;
  outb(BEEP_PORT, bPortData);

  return (E_OK);
}

/*
 *
 */
ER
read_beep (ID caller, DDEV_REA_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
write_beep (ID caller, DDEV_WRI_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
control_beep (ID caller, DDEV_CTL_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/
  
  return (E_NOSPT);
}
