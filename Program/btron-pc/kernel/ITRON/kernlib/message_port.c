/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/message_port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/message_port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: message_port.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1999/04/13 04:14:55  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.1  1996/07/22 23:52:05  night
 * �ǽ����Ͽ
 *
 * Revision 1.4  1995/09/21  15:51:41  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/09/17  17:06:23  night
 * get_req() �ؿ������ rcv_mbf �����ƥॳ�����ƤӽФ������ΤȤ��Υ��顼
 * �� kernlib ������ѿ� sys_errno �˵�������褦���ѹ�������
 * �����ϡ����顼�Ϥ��ƤƤ�����
 *
 * Revision 1.2  1995/08/05  12:09:48  night
 * get_req() �ؿ����ɲá�
 *
 * Revision 1.1  1995/02/26  14:17:51  night
 * �ǽ����Ͽ
 *
 *
 */
/*
 * ��å������Хåե����ñ�˰�������δؿ�����
 *
 */
#include <h/types.h>
#include <h/itron.h>
#include <h/config.h>
#include <h/errno.h>
#include "../servers/port-manager.h"


/*
 * Ŭ���� ID ���ĥ�å������Хåե���������롣
 */
ID
get_port (W minsize, W maxsize)
{
  ID		msg_port;
  T_CMBF	create_argument;
  W		errno;

  /*
   * �׵�����Ĥ��Τ���Υ�å������Хåե���������롣
   * ��å������Хåե��� ID ���ä˷�ޤäƤ��ʤ��������Ƥ����å���
   * ���Хåե���Ŭ�������֡�
   */
  create_argument.bufsz  = minsize;
  create_argument.maxmsz = maxsize;
  create_argument.mbfatr = TA_TFIFO;
  for (msg_port = MIN_USERMBFID;
       msg_port <= MAX_USERMBFID;
       msg_port++)
    {
      errno = cre_mbf (msg_port, &create_argument);
      if (errno == E_OK)
	{
	  return (msg_port);
	}
    }
  return (0);
}

/*
 * �����ǻ��ꤷ���ݡ��Ȥ����å�������������롣
 * �ޥ͡�����ǻ��Ѥ��뤿��δؿ���
 */
W
get_req (ID port, VP request, W *size)
{
  extern ER	sys_errno;

  sys_errno = rcv_mbf (request, (INT *)size, port);
  if (sys_errno != E_OK)
    return (0);
  return (*size);
}

