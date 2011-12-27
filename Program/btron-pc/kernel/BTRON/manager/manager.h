/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/manager.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 */

/*
 *
 *
 *
 * $Log: manager.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-10-29 15:48:08  night
 * �ǽ����Ͽ
 *
 *
 */


#ifndef __MANAGER_H__
#define __MANAGER_H__	1

#include <h/types.h>
#include <h/errno.h>
#include <ibmpc/ibmpc.h>

#define MANAGER		"btron.manager"

/* ��å�����������Ƶ�ǽ(PM �� MM �ʤ�)��
 * ��ͳ�˻Ȥ���ǡ����Υ�����
 * ��å������κǸ�������ΰ衣
 */
#define MAX_PRIVATE_SIZE	1024


#define PM_MASK			0x0100
#define FM_MASK			0x0200
#define MM_MASK			0x0300


/*
 * �ޥ͡����㤬�����Ȥ��å���������
 */
struct manager_request
{
  UW	command;		/* ���ޥ�� */
  B	data[MAX_PRIVATE_SIZE];
};


/*
 * �ޥ͡����㤬�֤���å������η���
 */
struct manager_response
{
  UW	result;			/* ��� */
  B	data[MAX_PRIVATE_SIZE];
};


struct manager_message
{
  ID	caller_port;	/* ��å������������ѤΥݡ��� */
  ID	caller_task;	/* ��å������������� task ID */
  W	length;		/* ��å������κ��祵���� */

  union
    {
      struct manager_request	request;
      struct manager_response	response;
    } body;
};


typedef struct manager_message	MANAGER_MSG;



#endif /* __MANAGER_H__ */
