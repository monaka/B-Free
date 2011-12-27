/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/h/lowlib.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

#ifndef __LOWLIB_H__
#define __LOWLIB_H__	1


#include "itron_module.h"

/* LOWLIB ��Ϣ�Υ����ƥॳ����
 */
#define LOD_LOW		(3)
#define ULD_LOW		(4)
#define STS_LOW		(5)


struct lowlib_info
{
  struct module_info	*modp;
  B			name[MAX_MODULE_NAME];
  W			intr;

  ER			(*start)(VP stack_top);
  ER			(*quit)(void);
  ER			(*intr_func)(W signo, VP arg);	/* �����߽����ؿ� */
};


struct lowlib_data
{
  ID	recv_port;	/* POSIX �ޥ͡�����ؤ��׵�����������դ��ѥݡ��� */

  ID	my_pid;		/* POSIX �ץ��� ID */

  ID	main_task;	/* �桼���ץ����Υ����ɤ�¹Ԥ��륿���� */
  ID	signal_task;	/* �����ʥ�μ���������Ԥ������� */
  ID	fifo_task;	/* �ѥ��פ�Ȥ��Ȥ��˻��Ѥ��륿���� */
  ID	alarm_task;	/* alarm �����ƥॳ�����ѤΥ�����(POSIX �Ķ���) */
  
  FP	start_func;	/* �ץ����Υ����������� */

  ID	efile;		/* �¹ԥե������ؤ��Ƥ����å������ݡ��� 
			 * ���������Υڡ�������ΤȤ��˻��Ѥ��롣
			 */

  W	errno;		/* ���顼�ֹ浭Ͽ�� */

};

#define LOWLIB_DATA		((struct lowlib_data *)(0x7fff0000))


#ifdef __ITRON__

extern struct lowlib_info	lowlib_table[MAX_MODULE];

extern ER			init_lowlib (struct module_info *modp);
extern ER			load_lowlib (VP *argp);
extern ER			unload_lowlib (VP *argp);


#endif /* __ITRON__ */


#endif /* __LOWLIB_H__ */
