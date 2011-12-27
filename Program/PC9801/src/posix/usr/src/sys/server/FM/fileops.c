/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/fileops.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/fileops.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * �ե����륪�ڥ졼�����ؿ�����
 *
 */
/*
 * $Log: fileops.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:53:38  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/08/12  16:27:24  night
 * ���ڥ졼�����ؿ��������ѹ���
 * (̾���򤹤٤��֤�������)
 *
 * Revision 1.2  1995/08/12  16:11:41  night
 * ��¤�� file_message_t �� fm_msg_t ���ѹ���
 *
 * Revision 1.1  1995/08/12  16:05:37  night
 * �ǽ����Ͽ
 *
 *
 */


#include <sys/types.h>
#include <sys/errno.h>
#include <server/file.h>

#include "FM.h"
#include "dosfs/dosfs.h"


/* �ե�����Υ���������������å�	*/
void
do_access (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ѥ�̾��é�� 			*/
void
do_lookup (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����򥯥�������		*/
void
do_close (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����°��������			*/
void
do_getattr (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����°��������			*/
void
do_setattr (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�������ɤ߼��   	   	*/
void
do_read (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����ν񤭹���      		*/
void
do_write (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����Υ��������ѹ�		*/
void
do_tranc (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ǥ��쥯�ȥ�����       		*/
void
do_mkdir (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ǥ��쥯�ȥ����       		*/
void
do_rmdir (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ���ڥ����ե�����κ��� 		*/
void
do_mknod (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ���ڥ����ե�����κ�� 		*/
void
do_rmnod (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����Υϡ��ɥ��   		*/
void
do_link (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����κ��			*/
void
do_remove (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ǥХ����� select       		*/
void
do_select (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե����������           		*/
void
do_control (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե�����ΰ�ư(̾�����ѹ�)  		*/
void
do_move (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* root �ե����륷���ƥ�Υޥ����	*/
void
do_mountroot (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե����륷���ƥ�Υޥ����		*/
void
do_mount (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* �ե����륷���ƥ�Υ���ޥ����	*/
void
do_unmount (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

