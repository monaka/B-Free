/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *
 */

#ifndef __ITRON_TASK_H__
#define __ITRON_TASK_H__	1

#include "itron.h"
#include "i386.h"

#define KERNEL_TASK	(1)

struct t_task_wait
{
  W	time_wait:1;
  W	semaph_wait:1;
  W	event_wait:1;
  W	msg_wait:1;
};

typedef struct t_task_wait  T_TASK_WAIT;


/* task_spec_interrupt - ��������ͭ�����߽����ؿ�
 *
 *	���ι�¤�Τˤϡ���������ͭ�γ����߽����ؿ�����Ͽ���롣
 *	(LOWLIB �Υ��եȥ�������������Ͽ�˻��Ѥ���)
 */
typedef struct task_spec_interrupt
{
  W		intr_no;
  ER		(*intr_func)(W, VP);
} T_TSI;


/* t_tcb --- ��������¤��
 *
 *	���ι�¤�Τˤϡ��������θ�ͭ���󤬴ޤޤ�롣
 *	��������ITRON ��٥�ʤΤǡ��ե�����ξ���ʤɤϴޤޤ�Ƥ��ʤ���
 *
 */
typedef struct t_tcb
{
  struct t_tcb		*next;		/* ����TCB��			*/
  struct t_tcb		*before;	/* ����TCB��			*/

  W			tskid;		/* ������ ID			*/
  W			tsklevel;	/* ��������ͥ����		*/
  W			tsklevel0;	/* ��������ͥ����(�����������) */
  					/* ��λ���������������ٵ�ư����� */
					/* ���ˤϤ����ͤ��Ȥ��롣       */
  ATR			tskatr;		/* ������°��			*/
  H			tskstat;	/* �������ξ���			*/

  /* ���������Ԥ������Ѥ����� */
  T_TASK_WAIT		tskwait;	/* ���������Ԥ����֤򼨤��ե饰 */
  W			wakeup_count;	/* �������Ⱦ��֤Υ�����       */
  W			suspend_count;	/* �����ڥ�ɾ��֤Υ�����     */

  ER			slp_err;	/* �Ԥ����֤˴ؤ��ƤΥ��顼	*/
					/* ���٤Ƥ�Ʊ�������Ƕ��̤˻��� */
					/* ����				*/

/* ������Ʊ����ǽ */
  /* ���ޥե� */
  struct t_tcb	 	*sem_next;	/* ���Υ��ޥե��Ԥ���������	*/
  ID			sem_id;		/* ���������ԤäƤ��륻�ޥե��� ID */

  /* ���٥�ȥե饰 */
  struct t_tcb	 	*event_next;	/* ���Υ��٥�ȥե饰�Ԥ���������	*/
  UINT			flag_pattern;	/* ���٥�ȥե饰���Ԥ��ѥ�����	*/
  UINT			wfmode;		/* �Ԥ����λ���		*/
  ID			event_id;	/* ���������ԤäƤ��륤�٥�ȥե饰�� ID */
  UINT			rflgptn;	/* �ؿ����ͤȤ����֤��ե饰�ѥ�����q */

  /* ��å����� */
  struct t_tcb	 	*msg_next;	/* ���Υ�å������Ԥ���������	*/
  ID			msg_id;		/* ���������ԤäƤ����å������� ID */


/* �����å����� */
  W			stksz;		/* �������λ��ĥ����å��Υ�����	*/
  					/* ����ϡ������ͥ륹���å��Υ�	*/
					/* �����Ȥʤ�			*/

  B			*stackptr;	/* �����ͥ륹���å����ΰ�ؤΥ�	*/
					/* ����			*/


/* �����������׾��� */
  W			total;		/* ��¹Ի���			*/

#ifdef I386
  T_I386_CONTEXT	context;	/* ����ƥ����Ⱦ��� (CPU��¸)	*/
  UW			tss_selector;	/* �������Υ��쥯�� (CPU��¸)	*/
#endif

/* ���۵������� */
  T_REGION		regions[MAX_REGION];	/* �������˷�ӤĤ��� */
						/* ���� REGION �Υơ� */
						/* �֥� */ 


  T_TSI			interrupt[MAX_MODULE];	/* ��������ͭ�γ��� */
						/* �߽����ؿ��ơ��֥� */
  W			n_interrupt;

  W			(*page_fault_handler)(W, W);	/* �ڡ����ե���Ȼ��ν����Ѵؿ� */

  UW			initial_stack;
} T_TCB;


extern T_TCB		*run_task;		/* ���ߡ�������Υ����� */

#define GET_TSKWAIT(tcb)	(tcb.tskwait)


#endif /* __ITRON_TASK_H__ */
