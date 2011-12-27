/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/request.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/request.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * ���Υե�����ϡ�B-Free OS/POSIX �Ķ���ư���ե����륵���Фΰ����Ǥ���
 *
 * �ե����륵���ФؤΥꥯ�����Ȥ˴ؤ��������Ԥ��ޤ���
 */

/*
 * $Log: request.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1995-09-21 15:53:38  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.4  1995/08/12  16:27:28  night
 * ���ڥ졼�����ؿ��������ѹ���
 * (̾���򤹤٤��֤�������)
 *
 * Revision 1.3  1995/08/12  16:16:26  night
 * RCS �ޥ�����ɲá�
 *
 *
 */

#include <sys/types.h>
#include <server/file.h>

#include "FM.h"

/*
 * �ꥯ�����Ȥ�����Ȥäơ�������Ԥ���
 *
 */
void
doit (fm_msg_t *req)
{
  /* �ꥯ�����ȥ�å�������������ơ�Ŭ���ʽ�����Ԥ���
   */
  switch (req->request)
    {
    default:
      /* ��å�����������Ǥ��ʤ��ä���
       * ��ץ饤���֤�
       */
      send_error_reply (req, FILE_EUNKNOWN);
      break;

    case FILE_NULL:		/* ���⤷�ʤ� */
      send_null_reply (req);
      break;

#ifdef notdef
    case FILE_ACCESS:		/* �ե�����Υ���������������å� */
      do_access (req);
      break;

    case FILE_LOOKUP:		/* �ѥ�̾��é�� 		*/
      do_lookup (req);
      break;

    case FILE_CLOSE:		/* �ե�����򥯥�������	*/
      do_close (req);
      break;

    case FILE_GETATTR:		/* �ե�����°��������		*/
      do_getattr (req);
      break;

    case FILE_SETATTR:		/* �ե�����°��������		*/
      do_setattr (req);
      break;

    case FILE_READ:		/* �ե�������ɤ߼��   	*/
      do_read (req);
      break;

    case FILE_WRITE:		/* �ե�����ν񤭹���      	*/
      do_write (req);
      break;

    case FILE_TRANC:		/* �ե�����Υ��������ѹ�	*/
      do_tranc (req);
      break;

    case FILE_MKDIR:		/* �ǥ��쥯�ȥ�����       	*/
      do_mkdir (req);
      break;

    case FILE_RMDIR:		/* �ǥ��쥯�ȥ����       	*/
      do_rmdir (req);
      break;

    case FILE_MKNOD:		/* ���ڥ����ե�����κ��� 	*/
      do_mknod (req);
      break;

    case FILE_RMNOD:           /* ���ڥ����ե�����κ�� 	*/
      do_rmnod (req);
      break;

    case FILE_LINK:            /* �ե�����Υϡ��ɥ��   	*/
      do_link (req);
      break;

    case FILE_REMOVE:		/* �ե�����κ��		*/
      do_remove (req);
      break;

    case FILE_SELECT:		/* �ǥХ����� select       	*/
      do_select (req);
      break;

    case FILE_CONTROL:		/* �ե����������           	*/
      do_control (req);
      break;

    case FILE_MOVE:		/* �ե�����ΰ�ư(̾�����ѹ�)  	*/
      do_move (req);
      break;

    case FILE_MOUNTROOT:	/* root �ե����륷���ƥ�Υޥ���� */
      do_mountroot (req);
      break;

    case FILE_MOUNT:		/* �ե����륷���ƥ�Υޥ����	*/
      do_mount (req);
      break;

    case FILE_UNMOUNT:		/* �ե����륷���ƥ�Υ���ޥ���� */
      do_unmount (req);
      break;
#endif /* notdef */      
    }

}



/*
 * ������å�������ƤӽФ��������롣
 * �����ǻ��ꤷ�Ƥ���������å������� ITRON �� snd_mbf �����ƥॳ����
 * ����Ѥ��ơ��ƤӽФ���Ȥ��������롣
 */
ER
send_reply (fm_msg_t *req, struct file_reply *reply)
{
  return (snd_mbf (req->caller, sizeof (*reply), reply));
}

/*
 * ���顼�Ȥʤä����Ȥ��֤���
 */
ER
send_error_reply (fm_msg_t *req, enum file_error errno)
{
  struct file_reply	reply;

  reply.error = errno;
  return (send_reply (req, &reply));
}

/*
 * NULL ���֤���
 *
 */
ER
send_null_reply (fm_msg_t *req)
{
  struct file_reply	reply;

  bzero (&reply, sizeof (reply));
  return (snd_mbf (req->caller, sizeof (reply), &reply));
}
