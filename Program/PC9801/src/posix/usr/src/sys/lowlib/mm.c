/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/mm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/mm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: mm.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:52:57  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/21  13:19:21  night
 * �������ɲ�
 *
 * Revision 1.1  1995/03/04  14:23:12  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX ��������ޥ͡����� (MM) ���̿���Ԥ�����δؿ���
 *
 * ����Ū�˥�������ޥ͡����㤬��������ѥ��åȤμ���ʬ�����ؿ���¸
 * �ߤ��Ƥ��롣
 *
 * ���Υե����������������ؿ��ϰʲ��ΤȤ��ꡧ
 *
 * vm_create	���ꤷ���������β��۶��֤��������롣
 * vm_remove	���ꤷ���������β��۶��֤������롣
 * vm_write	���۶��֤˾����񤭹���
 * vm_read	���۶��֤����Ƥ��ɤߤ���
 * vm_stat	���۶��֤ˤĤ��Ƥξ�����������
 *
 */


#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include "funcs.h"

/*
 * ���ۥ�����֤��ΰ���������뤿��δؿ�
 *
 *
 */
int
vm_create (ID task, void *start, unsigned int size, int attr)
     /* task	���۶��֥����� ID */
     /* start	���۶��֤Υ������ȥ��ɥ쥹 */
     /* size	���۶��֤��礭�� */
     /* attr	���۶��֤�°�� */
{
  struct mm_arg		arg;
  struct mm_res		res;
  ID			sendport;
  ID			recvport;
  ER			error;

  /*
   * ������Υ�å������ݡ��Ȥμ���������ֿ���å������μ����ѤΥ��
   * �������ݡ��Ȥκ�����
   */
  recvport = get_port (sizeof (struct mm_res), 
		       sizeof (struct mm_res));
  if (recvport == 0)
    {
      return (E_NOMEM);
    }
  if (find_port (MEMORY_MANAGER_NAME, &sendport) != E_OK)
    {
      return (E_NOMEM);
    }

  /*
   * ����ޥ͡����������ѥ��åȤ�������롣
   */
  arg.a.mm_create_arg.taskid = task;
  arg.a.mm_create_arg.start = start;
  arg.a.mm_create_arg.size = size;
  arg.a.mm_create_arg.attr = attr;
  arg.recvport = recvport;

  /*
   * ����ޥ͡�������Ф��ơ����۶��֤����������׵�����롣
   */
  error = snd_mbf (sendport, sizeof (arg), &arg);
  if (error != E_OK)
    {
      /* ���顼�ˤʤäƤ��ޤä��� */
      return (error);
    }

  /*
   * ����ޥ͡����㤫����ֿ���å�������������롣
   */
  error = rcv_mbf (recvport, sizeof (res));
  if (error != E_OK)
    {
      /* ���顼�ˤʤäƤ��ޤä��� */
      return (error);
    }

  return (E_OK);
}

/*
 * ���۶��֤��������׵�����ޥ͡���������롣
 *
 */
int
vm_delete (ID task, void *start)
     /* task
      * start
      */
{

}

       
