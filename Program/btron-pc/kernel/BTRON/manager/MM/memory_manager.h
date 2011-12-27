/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/memory_manager.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: memory_manager.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1997-09-10 13:12:35  night
 * #include �Ԥν�����
 *
 * Revision 1.3  1997/07/06 11:54:44  night
 * VERSION_ID �ޥ�����ѹ���
 *
 * Revision 1.2  1997/06/29 15:44:32  night
 * �����Ȥ��ѹ�
 *
 * �����������ɤ� SJIS ���� EUC ���ѹ�������
 * ��RCS �� Log �ޥ�������Υ����Ȥ��ɲä���(�ޤ�����Ƥ��ʤ��ä��ե�����Τ�)��
 *
 *
 */


#ifndef __MM_MEMORY_MANAGER_H__
#define __MM_MEMORY_MANAGER_H__		1


#include "../../../ITRON/h/itron.h"
#include "../../../ITRON/h/errno.h"
#include "../../kernlib/device.h"
#include "../../device/console/console.h"
#include "../../servers/port-manager.h"

#include "assert.h"
#include "errno.h"
#include "pageinfo.h"
#include "funcs.h"


#ifdef GLOBAL
#define EXTERN
#else
#define EXTERN	extern
#endif /* ! GLOBAL */

#define VERSION_ID_HIGH		1
#define VERSION_ID_LOW		0


#define BTRON_MM_MANAGER	"manager.btron.mm"

#define	SUCCESS			1
#define FAIL			0


#define ROUNDUP(x,align)	((((int)x) + (align) - 1) & ~((align) - 1))



struct mm_req_pagefault_t
{
  UW	fault_addr;
};


struct mm_request_t
{
  ID	receive_port;		/* �׵�����ѤΥݡ���	*/
  ID	caller;			/* �ƤӽФ����Υ����� ID */
  ID	procid;			/* �ƤӽФ����Υץ��� ID */
  W	msg_length;		/* ��å�������Ĺ��	*/
  W	operation;		/* �׵��ֹ�(�����ƥॳ������б�)	*/

  union
    {
      struct mm_req_pagefault_t	mm_req_pagefault;
    } req;
};


struct mm_response_t
{
  ID	receive_port;		/* �׵�����ѤΥݡ���	*/
  W	msg_length;		/* ��å�������Ĺ��	*/
  W	operation;		/* �׵��ֹ�(�����ƥॳ������б�)	*/
  W	errno;			/* ���顼�ֹ� */
  W	status;			/* ���ơ�����(�꥿������) */

  W	ret1;			/* ���֤˻��Ѥ���꥿������ */
  W	ret2;			/* ���֤˻��Ѥ���꥿������ */
};




/* ===============================================================
 *
 * =============================================================== */

EXTERN ID	request_port;


#endif /* __MM_MEMORY_MANAGER_H__ */


