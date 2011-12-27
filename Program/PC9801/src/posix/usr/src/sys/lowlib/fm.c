/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/fm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/fm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: fm.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:56  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/02  09:33:30  night
 * �������ɲ�
 *
 * Revision 1.1  1995/03/04  14:23:11  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX �ե���������ޥ͡����� (FM) ���̿���Ԥ�����δؿ���
 *
 * ���Υե����������������ؿ��ϰʲ��ΤȤ��ꡧ
 *
 * fm_call
 * fm_send
 * fm_receive
 */

#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include <server/proc.h>
#include <server/file.h>
#include "../../../../../kernel/servers/port-manager.h"
#include "funcs.h"


/***********************************************************************/
/*			                                               */
/*			    static �ѿ������			       */
/*		    (���Υե��������ǤΤ߻��Ѥ���)                   */
/*			                                               */
/***********************************************************************/


/*
 * �ե����륵���Ф��׵�ѥ��åȤ��������뤿��Υ�å������Хåե��� ID 
 * �򵭲����Ƥ���������ѿ���
 *
 * �ե����륵���Ф��׵������Ȥ��������ѿ�������å����롣
 * �⤷�������ѿ����ͤ� 0 �ξ��ˤϡ��ե����륵���Ф��׵�����Ĥ��Ѥ�
 * ��å������Хåե���ߤĤ��뤿��ˡ��ݡ��ȥޥ͡�������䤤��碌�롣
 * (�ݡ��ȥޥ͡��������Ͽ����Ȥ���̾���ϡ��ޥ��� POSIX_FILE_SERVER ��
 * ������Ƥ���)
 * 
 */
static ID	file_server_port;



/***********************************************************************/
/*			                                               */
/*			    static �ؿ������			       */
/*		    (���Υե��������ǤΤ߻��Ѥ���)                   */
/*			                                               */
/***********************************************************************/


/*
 * �ե�����ޥ͡�������̿��˻Ȥ��ݡ��Ȥ����뤿��˻��Ѥ���ؿ���
 * ���δؿ��ϡ�static �ѿ� file_server_port ����Ѥ��롣
 *
 */
static ID	get_request_port (void);


 
/*
 * �ե�����ޥ͡�������׵�ѥ��åȤ�������������������Ȥ롣
 *
 */
int
fm_call (file_message_t *req, file_reply_t *reply)
     /*
      * req	�ե�����ޥ͡��������������ѥ��å�
      * reply	�ե�����ޥ͡����㤫����������Ϥ����ΰ�
      */
{
  ID receive_port;	/* ����������Ȥ뤿��Υ�å������ݡ��� */

  receive_port = get_port (sizeof (file_reply_t), sizeof (file_reply_t));
  if (receive_port == 0)
    {
      /* �ݡ��Ȥ������Ǥ��ʤ��ä� */
      reply->error = FILE_ENOMEM;
      return (-1);
    }
  req->caller = receive_port;	/* �����ѥݡ��Ȥ�������å�������	*/
				/* �Ȥ������ 				*/

  /*
   * �ե�����ޥ͡�����˥�å�����������
   */
  if (fm_send (req) == 0)
    {
      /* �����Ǥ��ʤ��ä� */
      /* �ݡ��Ȥ��˴����� */
      
      reply->error = FILE_ENOMEM;
      return (-1);
    }

  /*
   * �ե�����ޥ͡����㤫�������������Ȥ� (�ޤ�����)
   */


  return (0);
}


/*
 * POSIX �ե�����ޥ͡�����Τ���׵��å����������Ĥ��ѥ�å������ݡ�
 * �Ȥ��������Ȥ��˻��Ѥ���ؿ���
 *
 * �׵�����Ĥ��ѤΥ�å������Хåե� ID �ϡ�file_server_port ����Ͽ��
 * ��Ƥ��롣 ���δؿ��ϡ�file_server_port �� 0 (�Ĥޤꡢ��å������ݡ�
 * �Ȥ���Ͽ����Ƥ��ʤ�) ���˥ݡ��ȥޥ͡�������̿�������Ͽ���Ƥ���
 * �׵�����Ĥ��ѤΥ�å������Хåե������롣
 *
 */
static ID
get_request_port (void)
{
  PORT_MANAGER_ERROR	error;

  if (file_server_port == 0)
    {
      /* static �ѿ� file_server_port �� 0 �ξ��ˤϡ��ե�����ޥ͡�
       * ����Τ���׵��å������ݡ��Ȥ� ID ��ݡ��ȥޥ͡�������䤤
       * ��碌�롣                                                  
       */
      error = find_port (POSIX_FILE_SERVER, &file_server_port);
      if (error != E_PORT_OK)
	{
	  /* �ݡ��Ȥ������Ǥ��ʤ��ä���
	   * 0 ���֤���
	   */
	  return (0);
	}
    }

  return (file_server_port);
}
