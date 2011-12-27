/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: doit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:43  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:25:18  night
 * ��å������� proc_message_t �� pm_msg_t ���ѹ���
 *
 * Revision 1.1  1995/03/18  14:02:38  night
 * �ǽ����Ͽ
 *
 *
 */


/*
 * ���Υե�����ϡ�B-Free OS/POSIX �Ķ���ư���ץ��������Фΰ����Ǥ���
 *
 *
 */

#include "pm.h"


/*
 * �ץ����ޥ͡����������줿�׵���������ؿ���
 *
 * �ºݤν������̤δؿ����Ԥ������δؿ��ϡ�ñ�˼ºݤν����򤹤�ؿ���
 * �ꥯ�����Ȥˤ�äƿ���ʬ���롣
 *
 */
doit (pm_msg_t *req)
{
  proc_error_t	error;

  switch (req->request)
    {
    case PROC_FORK:	/* fork �����ƥॳ���� */
      error = do_proc_fork (req);
      send_reply (req, error);
      break;

    default:
      /* 
       * ���������å������Υ����פ����Ĥ���ʤ��ä���
       * ���顼���֤���
       */
      break;
    }
}


/*
 * ������å�������ƤӽФ��������롣
 * �����ǻ��ꤷ�Ƥ���������å������� ITRON �� snd_mbf �����ƥॳ����
 * ����Ѥ��ơ��ƤӽФ���Ȥ��������롣
 */
ER
send_reply (pm_msg_t *req, struct proc_reply *reply)
{
  return (snd_mbf (req->caller, sizeof (*reply), reply));
}

/*
 * ���顼�Ȥʤä����Ȥ��֤���
 */
ER
send_error_reply (pm_msg_t *req, enum proc_error errno)
{
  struct proc_reply	reply;

  reply.error = errno;
  return (send_reply (req, &reply));
}

/*
 * NULL ���֤���
 *
 */
ER
send_null_reply (pm_msg_t *req)
{
  struct proc_reply	reply;

  bzero (&reply, sizeof (reply));
  return (snd_mbf (req->caller, sizeof (reply), &reply));
}
