/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/server/file.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: file.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1996-01-06 16:12:58  night
 * �إå��ե�����Υ��󥯥롼�ɥѥ����ѹ���
 *
 * Revision 1.2  1995/09/21  15:51:58  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:47:57  night
 * �ǽ����Ͽ
 *
 *
 */


#ifndef __SERVER_FILE_H__
#define __SERVER_FILE_H__	1

#include "../../../../kernel/kernlib/message.h"
#include <sys/types.h>
#include <limits.h>

/*
 * �ե�����ޥ͡����㤬�ݡ��ȥޥ͡��������Ͽ����Ȥ���
 * ���Ѥ���̾����
 */
#define POSIX_FILE_SERVER	"posix.server.file"


/*
 * �ե�����ޥ͡����������ꥯ������
 */
enum file_request
{
  FILE_NULL,		/* ���⤷�ʤ� 				*/
  FILE_ACCESS,		/* �ե�����Υ���������������å�	*/
  FILE_LOOKUP,		/* �ѥ�̾��é�� 			*/
  FILE_CLOSE,		/* �ե�����򥯥�������		*/
  FILE_GETATTR,		/* �ե�����°��������			*/
  FILE_SETATTR,		/* �ե�����°��������			*/
  FILE_READ,		/* �ե�������ɤ߼��   	   	*/
  FILE_WRITE,		/* �ե�����ν񤭹���      		*/
  FILE_TRANC,		/* �ե�����Υ��������ѹ�		*/
  FILE_MKDIR,		/* �ǥ��쥯�ȥ�����       		*/
  FILE_RMDIR,		/* �ǥ��쥯�ȥ����       		*/
  FILE_MKNOD,		/* ���ڥ����ե�����κ��� 		*/
  FILE_RMNOD,           /* ���ڥ����ե�����κ�� 		*/
  FILE_LINK,            /* �ե�����Υϡ��ɥ��   		*/
  FILE_REMOVE,		/* �ե�����κ��			*/
  FILE_SELECT,		/* �ǥХ����� select       		*/
  FILE_CONTROL,		/* �ե����������           		*/
  FILE_MOVE,		/* �ե�����ΰ�ư(̾�����ѹ�)  		*/

  FILE_MOUNTROOT,	/* root �ե����륷���ƥ�Υޥ����	*/
  FILE_MOUNT,		/* �ե����륷���ƥ�Υޥ����		*/
  FILE_UNMOUNT,		/* �ե����륷���ƥ�Υ���ޥ����	*/
};

typedef enum file_request file_request_t;


/*
 * �ե����� ID
 */
struct fileid
{
  unsigned short	hostid;		/* �ۥ��� ID */
  unsigned short	fsid;		/* �ե����륷���ƥ� ID */
  unsigned long		fileno;		/* �ե������ֹ� */
};


/*
 * �ե�����ޥ͡����㤬�֤����顼�ֹ�
 */
enum file_error
{
  FILE_EOK,		/* ���ｪλ 				*/
  FILE_ENOMEM,		/* �����ƥ��ΰ�Υ���(��)��­��ʤ�	*/
  FILE_EUNKNOWN,	/* ��å�����������Ǥ��ʤ�             */
  FILE_ENOSPT,		/* ̤���ݡ��� */
};

typedef enum file_error	file_error_t;


/*
 * �����ǡ����׵�ѥ��åȤ������ѥ��åȤ�������롣
 */
struct file_access_req
{
  char	path[PATH_MAX];
  int	mode;
};

typedef struct file_access_req	file_access_req_t;

/*
 * �ե������Ǥ�դΥѡ��ߥå��������ꤹ�뤿����׵�ѥ��å�
 *
 */
struct file_setattr_req
{
  char	path[PATH_MAX];
  int	mode;
};

typedef struct file_setattr_req	file_setattr_req_t;


/*
 * �ե����륵���Ф������׵�ѥ��å�
 *
 */
struct fm_msg
{
  SVC_REQ	header;

  union
    {
      struct file_access_req	file_access_req;
      struct file_setattr_req	file_setattr_req;
    } body;
};

typedef struct fm_msg	fm_msg_t;


/*
 * �ե����륵���Ф����֤äƤ��������ѥ��å�
 */
struct file_reply
{
  SVC_REQ	header;

  file_error_t		error;

  /*
   * ���׵ᤴ�Ȥ˰ۤʤ��ΰ�
   */
};

typedef struct file_reply	file_reply_t;


#endif /* __SERVER_FILE_H__ */
