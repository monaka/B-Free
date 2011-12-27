/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/hcm_pci/hcm_pci.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $*/
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/hcm_pci/hcm_pci.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: hcm_pci.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-03-21 15:06:50  monaka
 * Careless bug. Parameter for probe added.
 *
 * Revision 1.1  1999/03/15 05:55:38  monaka
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

#include "hcm_pci.h"
#include "hcm_pci_internal.h"


/*
 *	 �ɽ��ѿ��������
 *
 */
static W	mydevid;	/* ��ʬ���Ȥ�id */
static ID	recvport;	/* �׵�����Ĥ��ѥݡ��� */
static W	initialized;

struct device	dev_table[MAX_HCM_PCI];


static void	main_loop (void);
static void	init_driver (void);


/* start --- hcm_pci �ɥ饤�ФΥᥤ��ؿ�
 */
void
start ()
{
  probe (&dev_table[0]); /* �ǥХ�����¸�ߤ��Ƥ��뤫������å����� */
  init_driver ();

  printf ("registed hcm_pci driver.\n");
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
      dbg_printf ("hcm_pci: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (HCM_PCI_DRIVER, recvport);
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
      open_hcm_pci (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_hcm_pci (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_hcm_pci (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_hcm_pci (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_hcm_pci (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * �ǥХ����Υ����ץ�
 */
ER
open_hcm_pci (ID caller, DDEV_OPN_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 * �ǥХ����Υ�����
 */
ER
close_hcm_pci (ID caller, DDEV_CLS_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
read_hcm_pci (ID caller, DDEV_REA_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
write_hcm_pci (ID caller, DDEV_WRI_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}

/*
 *
 */
ER
control_hcm_pci (ID caller, DDEV_CTL_REQ *packet)
{
  /*** �����˥����ɤ�Ĥ������Ƥ������� ***/

  return (E_NOSPT);
}
