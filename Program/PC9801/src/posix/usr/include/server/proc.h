/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/server/proc.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: proc.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:59  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:47:56  night
 * �ǽ����Ͽ
 *
 *
 */

#ifndef __SERVER_PM_H__
#define __SERVER_PM_H__	1

#define PROCESS_MANAGER_NAME	"posix.process_manager"

/*
 * �ץ����ξ��֤�ɽ�� enum ���
 *
 */
enum proc_status { NONE, RUN, READY, SLEEP, WAIT };


/*
 * �ץ����ޥ͡�����ꥯ������
 */
enum proc_request
{
  PROC_NULL,	/* ���⤷�ʤ� 			*/
  PROC_GETSTAT,	/* �ץ����ξ��֤�����		*/
  PROC_FORK,	/* �������ץ������������	*/
};

typedef enum proc_request	proc_request_t;


/*
 * �ץ����ޥ͡����㤫�������֤���륨�顼�μ���
 */
enum proc_error
{
  PROC_EOK,		/* ���ｪλ */
  PROC_ENOSPT,		/* ���ݡ��Ȥ��Ƥ��ʤ��ꥯ������ */
};

typedef enum proc_error		proc_error_t;


/*
 * �ץ����ξ��ּ����Τ���˻��Ѥ����å�����
 *
 */
struct proc_getstat_req
{
  enum proc_status	status;	/* �ץ����ξ��֤򼨤� */
  pid_t			pid;	/* �ץ��� ID */
};


/*
 * PROC_GETSTAT ��������å�����
 */
struct proc_getstat_rep
{
  pid_t		pid;		/* �ץ����� ID */
  uid_t		uid;		/* �ץ����ν�ͭ�� ID */
  gid_t		gid;		/* �ץ����Υ��롼�� ID */

  ID		main_task;	/* �桼���ץ����Υ����ɤ�¹Ԥ��륿���� */
  ID		signal_task;	/* �����ʥ�μ���������Ԥ������� */
  ID		fifo_task;	/* �ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿���� */
  ID		alarm_task;	/* alarm �����ƥॳ�����ѤΥ����� */
};


/*
 * fork ������Ԥ�����˻��Ѥ����å�����
 *
 */
struct proc_fork_req
{
};


/*
 * �ץ����ޥ͡���������뤿��Υ�å�����
 *
 *
 */
struct pm_msg
{
  /* ����2�Ĥϡ����٤ƤΥ�å������˶��� */
  proc_request_t	request;	/* �ꥯ�������ֹ�	*/
  ID			caller;		/* �ƤӸ��ؤΥݡ���	*/

  union
    {
      struct proc_getstat_req	proc_getstat_req;
    } body;
};

typedef struct pm_msg	pm_msg_t;


/*
 * �ץ����ޥ͡����㤫���������å�����
 */
struct proc_reply
{
  proc_error_t	error;	/* �����η�̤Υ��顼�ֹ� */

  union
    {
      struct proc_getstat_rep	proc_getstat_rep;
    } body;
};


#endif /* __SERVER_PM_H__ */
