/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/request.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


/* request.c - POSIX �ط��Υ�å�����������Ԥ�
 *
 *
 *
 * $Log: request.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  2000-02-19 03:02:15  naniwa
 * minor change on debug write
 *
 * Revision 1.6  1999/03/24 08:00:20  monaka
 * Modified include pathes (POSIX manager should be independ from ITRON.)
 *
 * Revision 1.5  1997/10/24 14:00:10  night
 * �ѿ��ΰ�����碌��̩�ˤ�����
 *
 * Revision 1.4  1997/04/28 15:29:10  night
 * �ǥХ����ɥ饤�Ф���Υ�å���������������ΰ�򥹥��å����
 * �ȤäƤ����Ȥ����static �ΰ�����ꤷ����
 *
 * Revision 1.3  1996/11/20  12:12:47  night
 * ���顼�����å����ɲá�
 *
 * Revision 1.2  1996/11/07  12:48:52  night
 * get_request() �� put_response() ���ɲá�
 *
 * Revision 1.1  1996/11/05  15:13:46  night
 * �ǽ����Ͽ
 *
 */

#include "posix.h"
#include <port-manager.h>


static ID	request_port;	/* �׵�����դ��ѤΥݡ���	*/
				/* ITRON �Υ�å������ݡ���	*/




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
  if ((port = get_port (sizeof (struct posix_request), sizeof (struct posix_request))) <= 0)
    {
      /* �ݡ��Ȥ������Ǥ��ʤ��ä� */
      return (FAIL);
    }

  request_port = port;

  /* �ݡ��ȥޥ͡��������Ͽ���� */
  if (regist_port ((PORT_NAME *)POSIX_MANAGER, port) != E_PORT_OK)
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
get_request (struct posix_request *req)
{
  extern ER	sys_errno;
  INT		size;

  ASSERT (req != NULL);

  size = sizeof (struct posix_request);
  sys_errno = rcv_mbf (req, &size, request_port);
  if (sys_errno != E_OK) {
    dbg_printf("[PM] get_request: rcv_mbf error %d\n", sys_errno);
    return (FAIL);
  }
  return (SUCCESS);
}


/* put_response -
 *
 */
W
put_response (struct posix_request *req, 
	      W errno,
	      W status,
	      W ret1,
	      W ret2)
{
  static struct posix_response res;
  ER	syserr;

  if (req == NULL)
    {
      return (EP_INVAL);
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
  return (EP_OK);
}


/* ���顼�ˤʤä����Ȥ�ꥯ�����Ȥ����긵���֤�
 *
 */
W
error_response (struct posix_request *req, W errno)
{
  /* �׵ḵ���������� */
  return (put_response (req, errno, 0, 0, 0));
}
