/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: port.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-06-29 15:44:34  night
 * �����Ȥ��ѹ�
 *
 * �����������ɤ� SJIS ���� EUC ���ѹ�������
 * ��RCS �� Log �ޥ�������Υ����Ȥ��ɲä���(�ޤ�����Ƥ��ʤ��ä��ե�����Τ�)��
 *
 *
 */

#include "memory_manager.h"


/* init_port - �׵�����դ��ѤΥݡ��Ȥ���������
 *
 * ����
 *	�⤷�����Ǥ˥�å������ݡ��Ȥ����������Ƥ������ˤ�
 * 	���⤷�ʤ������ξ��ˤϡ�SUCCESS ���֤롣
 *
 * �֤���
 *	SUCCESS	����������
 *	FAIL	����������
 *
 */
W
init_port (void)
{
  ID	port;

  /* ���Ǥ˥ݡ��Ȥ����������Ƥ���
   */
  if (request_port != 0)
    {
      return (SUCCESS);
    }

  /* �ݡ��Ȥ�������� */
  if ((port = get_port (sizeof (struct mm_request_t), sizeof (struct mm_request_t))) <= 0)
    {
      /* �ݡ��Ȥ������Ǥ��ʤ��ä� */
      return (FAIL);
    }

  request_port = port;

  /* �ݡ��ȥޥ͡��������Ͽ���� */
  if (regist_port (BTRON_MM_MANAGER, port) != E_PORT_OK)
    {
      /* �ݡ��Ȥ���������������������� */
      return (FAIL);
    }

  return (SUCCESS);	/* �ݡ��Ȥ������Ǥ��� */
}


/* get_request - �ꥯ�����Ȥ��������
 *
 */
W
get_request (struct mm_request_t *req)
{
  extern ER	sys_errno;

  ASSERT (req != NULL);

  sys_errno = rcv_mbf (req, sizeof (struct mm_request_t), request_port);
  if (sys_errno != E_OK)
    return (0);
  return (SUCCESS);
}


/* put_response -
 *
 */
W
put_response (struct mm_request_t *req, 
	      W errno,
	      W status,
	      W ret1,
	      W ret2)
{
  static struct mm_response_t	res;
  ER				syserr;

  if (req == NULL)
    {
      return (EMM_INVAL);
    }

  res.receive_port = request_port;
  res.msg_length = sizeof (res);
  res.operation = req->operation;
  res.errno = errno;
  res.status = status;
  res.ret1 = ret1;
  res.ret2 = ret2;

  /* �׵ḵ���������� */
  syserr = snd_mbf (req->receive_port, sizeof (res), &res);
  return (EMM_OK);
}


/* ���顼�ˤʤä����Ȥ�ꥯ�����Ȥ����긵���֤�
 *
 */
W
error_response (struct mm_request_t *req, W errno)
{
  /* �׵ḵ���������� */
  return (put_response (req, errno, 0, 0, 0));
}
