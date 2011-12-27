/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 */

/*
 *
 *
 *
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-10-29 15:48:06  night
 * �ǽ����Ͽ
 *
 *
 */

#include "manager.h"


/* �ץ�������ǻ��Ѥ�������ѿ�
 */
ID	recvport;


/* ���Υե��������ǤΤ߻��Ѥ���ؿ�������ѿ�
 */
static	void	doit (MANAGER_MSG *);


/*
 * BTRON �ޥ͡������ main �ؿ�
 *
 */
void 
main ()
{
  ER	error;
  extern ER	sys_errno;
  MANAGER_MSG	req;


  /* ��å������ݡ��Ȥ���Ͽ
   */
  recvport = get_port (sizeof (MANAGER_MSG), sizeof (MANAGER_MSG));
  if (recvport <= 0)
    {
      dbg_printf ("manager: cannot make receive port.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (MANAGER, recvport);
  if (error != E_OK)
    {
      dbg_printf ("manager: cannot regist port (error = %d)\n", error);
    }

  /* �ޥ͡�����ν����
   * �ե����������ʬ
   * ���������ʬ
   * �ץ���������ʬ
   *
   */
  /* fm_init () */
  /* pm_init () */
  /* mm_init () */


  /* ��å��������Ф� - �����롼��
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
	  dbg_printf ("CONSOLE: get_req() Unknown error(error = %d)\n", sys_errno);
	  break;
	}
    }

  /* NOT REACHED */
}


/* doit --- �Ƶ�ǽ�̤Υ⥸�塼��ؽ�����ʬ������
 *
 */
static void
doit (MANAGER_MSG *request)
{
  switch ((request->body.request.command) & 0xff00)
    {
    case PM_MASK:		/* PM_MASK == 0x0100 */
      pm_receive (request);	/* �ץ��������إ�å��������Ϥ� */
      break;

    case FM_MASK:		/* FM_MASK == 0x0200 */
      fm_receive (request);	/* �ե���������إ�å��������Ϥ� */
      break;

    case MM_MASK:		/* MM_MASK == 0x0300 */
      mm_receive (request);	/* ��������إ�å��������Ϥ� */
      break;

      /* ... �ǥХ����ɥ饤�����ν��������� */

    default:		/* ����Ǥ��ʤ���å�����������Ȥä� */
      unknown_error (request);
      break;
    }
}
