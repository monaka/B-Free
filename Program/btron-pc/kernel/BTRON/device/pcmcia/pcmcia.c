/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/pcmcia/pcmcia.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $*/
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/pcmcia/pcmcia.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#define __PCMCIA_C__ 1

/* 
 * $Log: pcmcia.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  2000-07-31 18:36:04  kishida0
 * test version
 *
 * Revision 1.5  2000/02/21 22:34:22  kishida0
 * minor fix
 *
 * Revision 1.4  2000/02/13 13:24:04  kishida0
 * add $(SRCS) Makefile
 *
 * Revision 1.3  1999/07/06 13:40:11  kishida0
 * for debug
 *
 * Revision 1.2  1999/05/13 16:23:58  kishida0
 * �ǥХå��Ѥ˥������ɲá��ä��礭���ѹ��ǤϤʤ�
 *
 * Revision 1.1  1999/04/21 17:56:28  kishida0
 * for debug
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/* See URL and modify.
 * http://www.cirrus.com/products/overviews/pd6729.html
 *
 */

#include "pcmcia.h"
#include "pcmcia_internal.h"


/*
 *	 �ɽ��ѿ��������
 *
 */
static W	mydevid;	/* ��ʬ���Ȥ�id */
static ID	recvport;	/* �׵�����Ĥ��ѥݡ��� */
static W	initialized;

struct device	dev_table[MAX_PCMCIA];


static void	main_loop (void);
static void	init_driver (void);


/* start --- pcmcia �ɥ饤�ФΥᥤ��ؿ�
 */
void
start (void)
{
  probe (&dev_table[0]); /* �ǥХ�����¸�ߤ��Ƥ��뤫������å����� */
  init_driver ();

  printf ("registed pcmcia driver.\n");
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

  dbg_printf("pcmcia: init_log\n");
  init_log ();
  dbg_printf("pcmcia: get_port\n");
  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (recvport <= 0)
    {
      dbg_printf ("pcmcia: cannot make receive port.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (PCMCIA_DRIVER, recvport);
  if (error != E_OK)
    {
      /* error */
      dbg_printf ("pcmcia: cannot regist receive port.\n");
      slp_tsk();
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
  dbg_printf("main loop\n");
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
          dbg_printf ("pcmcia: main loop Unknown error.\n");
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
      open_pcmcia (req->header.mbfid, &(req->body.opn_req));
      break;

    case DEV_CLS:
      /* �ǥХ����Υ����� */
      close_pcmcia (req->header.mbfid, &(req->body.cls_req));
      break;

    case DEV_REA:
      read_pcmcia (req->header.mbfid, &(req->body.rea_req));
      break;

    case DEV_WRI:
      write_pcmcia (req->header.mbfid, &(req->body.wri_req));
      break;

    case DEV_CTL:
      control_pcmcia (req->header.mbfid, &(req->body.ctl_req));
      break;
    }
}

/*
 * �ǥХ����Υ����ץ�
 */
ER
open_pcmcia (ID caller, DDEV_OPN_REQ *packet)
{
  wreg(0x02,0x00);
  wreg(0x2f,0x00);
  wreg(0x06,0x20);
  wreg(0x03,0x40);
  wreg(0x05,0x00);
  wreg(0x04,0xff);

  wreg(0x02,0x90);  // open 
  //busywait(1000);
  
  return (E_OK);
}

/*
 * �ǥХ����Υ�����
 */
ER
close_pcmcia (ID caller, DDEV_CLS_REQ *packet)
{

  return (E_NOSPT);
}

/*
 *
 */
ER
read_pcmcia (ID caller, DDEV_REA_REQ *packet)
{

  return (E_NOSPT);
}

/*
 *
 */
ER
write_pcmcia (ID caller, DDEV_WRI_REQ *packet)
{

  return (E_NOSPT);
}

/*
 *
 */
ER
control_pcmcia (ID caller, DDEV_CTL_REQ *packet)
{
int i;
  switch (packet->cmd)
    {
    case PCMCIA_READ_ATRIB_MEM:
      dbg_printf("pcmcia: read memory\n");
      dump_cis();
      break;
    default:
      break;
    }
  
  return (E_OK);
}


