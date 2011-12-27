/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/dc390/dc390.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $*/
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/dc390/dc390.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: dc390.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-06-20 14:46:41  kishida0
 * write defines for debug.
 *
 * Revision 1.1  1999/04/26 18:33:32  kishida0
 * add for debug
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/*
 *
 *
 */

#include "dc390.h"
#include "dc390_internal.h"


/*
 *	 �ɽ��ѿ��������
 *
 */
static W	mydevid;	/* ��ʬ���Ȥ�id */
static ID	recvport;	/* �׵�����Ĥ��ѥݡ��� */
static W	initialized;

struct device	dev_table[MAX_DC390];


static void	main_loop (void);
static void	init_driver (void);


/* start --- dc390 �ɥ饤�ФΥᥤ��ؿ�
 */
void
start ( void )
{
  probe (&dev_table[0]); /* �ǥХ�����¸�ߤ��Ƥ��뤫������å����� */
  init_driver ();

  printf ("registed dc390 driver.\n");
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
      dbg_printf ("dc390: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (DC390_DRIVER, recvport);
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
      open_dc390 (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_dc390 (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_dc390 (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_dc390 (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_dc390 (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * �ǥХ����Υ����ץ�
 */
ER
open_dc390 (ID caller, DDEV_OPN_REQ *packet)
{
UH VenderID,DeviceID,Command,Status,
  // PCIC OPEN 
  outb(0xF1, 0xCF8);
  outb(0x00, 0xCFA);

  GetPCI-ConigurationSpaceHeader(&pci-inf);
  // Get Vendor ID
  VenderID = inw(0x00);
  



  return (E_NOSPT);
}

/*
 * �ǥХ����Υ�����
 */
ER
close_dc390 (ID caller, DDEV_CLS_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
read_dc390 (ID caller, DDEV_REA_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
write_dc390 (ID caller, DDEV_WRI_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
control_dc390 (ID caller, DDEV_CTL_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}
