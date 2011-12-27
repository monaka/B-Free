/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* task.c $Revision: 1.1 $
 *
 * ������������Ԥ���
 *
 *
 * �����ؿ��ꥹ��
 *
 *	init_task ()		--- �����������ǡ����ν����
 *	init_task1 ()		--- �����ƥ�����������������ȵ�ư
 *	cre_tsk ()		--- ������������
 *	new_task ()		--- Ǥ�դΥ����� ID �ǤΥ���������
 *
 * ����ѿ�
 *	run_task		--- ����������Υ�����
 *
 * �����ؿ� (static)
 *
 *	make_task_context ()	--- �����������������롣
 *
 * �����ѿ� (static)
 *
 *	task
 *	task_buffer
 *	ready_task
 *	dispatch_flag		�ǥ����ѥå����뤫�ɤ����Υե饰
 */

/*
 * $Log: task.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.13  1995-10-01 12:58:37  night
 * �ؿ� wup_tsk() ����� wakeup ���륿�����ξ���Υ��ɥ쥹�Υ����å�����
 * �á�
 *
 * Revision 1.12  1995/09/21  15:51:12  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.11  1995/09/17  16:57:07  night
 * task_switch() �ƤӽФ����ν������ѹ���
 * �����ȥ��������ͥ���̤��⤤�����������ʤ���祿���������å���Ԥ�
 * �ʤ��ä�����ͥ���̤��㤤�������ؤ��ڤ꤫����褦���ѹ�������
 *
 * Revision 1.10  1995/09/14  04:32:05  night
 * ���������֤�ɽ������ؿ� (print_list()) ���ѹ���
 * ɽ�����Ƥ˥�������٥���ɲá�
 *
 * Revision 1.9  1995/08/26  02:15:23  night
 * RCS �� Log �ޥ�����ɲá�
 *
 *
 */

#include "itron.h"
#include "errno.h"
#include "task.h"
#include "misc.h"
#include "func.h"
#include "interrupt.h"

/***************************************************************************
 *	�����������Ѥ��ѿ�
 *
 *
 */
T_TCB		*run_task;			/* ���ߡ�������Υ����� */

static T_TCB	*task;
static T_TCB	task_buffer[MAX_TSKID - MIN_TSKID + 1];
static T_TCB	*ready_task[MAX_PRIORITY - MIN_PRIORITY + 1];
static BOOL	dispatch_flag = TRUE;

static ER 	make_task_stack (T_TCB *task, W size, VP *sp);
static void	print_list (void);


void
print_task_list (void)
{
  dis_int ();
  print_list ();
  ena_int ();
}

static void
print_list (void)
{
  int	i;

  for (i = 1; i < MAX_TSKID; i++)
    {
      switch (task[i].tskstat)
	{
	case TTS_RUN:
	  printf ("%d (%d)  <RUN>           0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_RDY:
	  printf ("%d (%d)  <RDY>           0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_WAI:
	  printf ("%d (%d)  <WAIT>          0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_SUS:
	  printf ("%d (%d)  <SUSPEND>       0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	case TTS_WAS:
	  printf ("%d (%d)  <WAIT-SUSPEND>  0x%x\n", i, task[i].tsklevel, task[i].context.eip);
	  break;
	}
    }
}

W
list_counter (T_TCB *list)
{
  W		i;
  T_TCB	*p, *q;

  q = NULL;
  for (i = 0, p = list;
       (q != list) && (list != NULL);
       p = q, i++)
    {	
      q = p->next;
    }
  return (i);
}


#ifdef TASK_DEBUG_PRORAM
main ()
{
  T_TCB a, b, c;
  T_TCB *list = NULL;

  a.tsklevel = 0;
  b.tsklevel = 1;
  c.tsklevel = 2;
  print_list (list);
  list = ins_tcb_list (list, &a);
  print_list (list);
  list = ins_tcb_list (list, &b);
  print_list (list);
  list = ins_tcb_list (list, &c);
  print_list (list);
}  
#endif /* TASK_DEBUG_PROGRAM */



/* init_task --- �����������ν����
 *
 * TCB �ơ��֥�����Ƥ��������롣�����ơ������ȥ������򥿥����ֹ� -1 
 * �Υ������Ȥ��롣
 *
 */
void
init_task (void)
{
  W	i;

  /* TCB �ơ��֥�κ����� GDT �ؤ���Ͽ��
   */
  for (i = 0; i < NTASK; i++)
    {
      task_buffer[i].tskstat = TTS_NON;
    }
/*  bzero (kernel_tss, sizeof (TASK_STATE_SEG_T) * MAX_TASK); */
  task = &task_buffer[-1];
}


/* init_task1 --- ������1 �ξ�����������롣
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	������ 1 �ξ�����������롣
 *		������ 1 �ξ���ϡ�proc_table[0] �˼�����롣
 *		�ʲ��ξ�����������롣
 *		  cr3		���ߤ� cr3 �����Ƥ�����롣
 *		  state		�ץ������֤� TTS_RUN ������(���Υ������ϼ¹���)
 *				ͥ���٤� 0 ������
 *				�ƥ������Υ����� id �� 0 �����ꡣ
 *		�������쥸�������ͤ� task[1] �� context �Υ��ɥ쥹
 *		�����ꡣ
 *		�������쥸�������ͤϡ�������ˤϡ��ʤˤ�ƶ����ʤ�����
 *		���������ڤ괹�����Ȥ������ߤΥ������쥸�����λؤ�TSS
 *		�ΰ�˺����ߤΥ�������������򤹤롣
 *		���Τ��ᡢ�����˥������쥸�������ͤ����ꤷ�Ƥ���ɬ�פ����롣
 */
void
init_task1 (void)
{
  /* ������1�ξ�����������롣*/
  task[KERNEL_TASK].tskstat = TTS_RUN;		/* ���������֡����Ծ��֤˥��å� */
  task[KERNEL_TASK].tsklevel = MAX_PRIORITY;	/* ��������٥�ϡ�31(�����)�˥��å� */
  task[KERNEL_TASK].tskid = KERNEL_TASK;	/* ������ ID �ϡ�KERNEL_TASK(1)�˥��å� */

#ifdef I386
  /* ������ 1 �Υ���ƥ����Ⱦ������������			*/
  /* �����ξ���ϡ����˥�����1�������ȥ������ˤʤä�����	*/
  /* ���Ѥ��� 							*/
  task[KERNEL_TASK].context.cr3 = (UW)PAGE_DIR_ADDR;
  task[KERNEL_TASK].context.cs  = KERNEL_ESEG;
  task[KERNEL_TASK].context.ds  = KERNEL_DSEG;
  task[KERNEL_TASK].context.es  = KERNEL_ESEG;
  task[KERNEL_TASK].context.fs  = KERNEL_ESEG;
  task[KERNEL_TASK].context.gs  = KERNEL_ESEG;
  task[KERNEL_TASK].context.ss  = KERNEL_DSEG;
  task[KERNEL_TASK].context.ss0  = KERNEL_DSEG;
  task[KERNEL_TASK].context.zero = 0;
  task[KERNEL_TASK].context.ldtr = 0;
  task[KERNEL_TASK].context.backlink = 0;
  task[KERNEL_TASK].context.iobitmap = 0;
  
#endif /* I386 */

  /* ���������ϥ�����1�Ǥ��롣*/
  run_task = &(task[KERNEL_TASK]);
  
  /* ���쥯���򥻥å� */
  task[KERNEL_TASK].tss_selector = ((KERNEL_TASK + TSS_BASE) << 3) & 0xfff8;
  create_context (&task[KERNEL_TASK]);

  /* �������쥸�������ͤ����ꤹ��.*/
#ifdef I386
  load_task_register ((KERNEL_TASK + TSS_BASE) << 3);
#endif I386
/*  run_task = &(task[KERNEL_TASK]); */
}


/* �������������������:
 *
 *	������
 *		task		��������TCB�ΰ�ؤΥݥ���
 *		stack_size	�������Υ����å�������
 *
 */
static ER
make_task_context (T_TCB *task, T_CTSK *pk_ctsk)
{
  VP	sp;
  ER	err;

  err = make_task_stack (task, pk_ctsk->stksz, &sp);
  if (err != E_OK)
    {
      return (err);
    }

#ifdef I386
  /* �쥸������򤹤٤ƽ��������:
   * reset_registers()  �ϡ��ʲ��ΰ�����ɬ�פȤ��롧
   *   1) TCB �ΰ�ؤΥݥ���
   *   2) �������Υ������ȥ��ɥ쥹
   *   3) �����ͥ륹���å��Υ��ɥ쥹
   */
/*  task->context.cr3 = (UW)PAGE_DIR_ADDR; */
  task->context.cs  = KERNEL_ESEG;
  task->context.ds  = KERNEL_DSEG;
  task->context.es  = KERNEL_ESEG;
  task->context.fs  = KERNEL_ESEG;
  task->context.gs  = KERNEL_ESEG;
  task->context.ss  = KERNEL_DSEG;
  task->context.ss0  = KERNEL_DSEG;
  task->context.esp = (UW)((sp + pk_ctsk->stksz) - sizeof (W));
  task->context.ebp = (UW)((sp + pk_ctsk->stksz) - sizeof (W));
  task->context.ldtr = 0;
  task->context.iobitmap = 0;
  task->context.eip = (UW)pk_ctsk->startaddr;
#ifdef TSKSW_DEBUG
  printf ("(UW)pk_ctsk->startaddr = 0x%x\n", (UW)pk_ctsk->startaddr);
#endif
  task->context.eflags = 0;
  task->context.eax = 0;
  task->context.ebx = 0;
  task->context.ecx = 0;
  task->context.esi = 0;
  task->context.edi = 0;
  task->context.zero = 0;
  task->context.iobitmap = 0;
  task->tss_selector = ((TSS_BASE + task->tskid) << 3) & 0xfff8;
#endif /* I386 */
  create_context (task);	/* ����ƥ������ΰ�(TSS)�Υ��ɥ쥹��GDT�˥��å� */
  return (E_OK); /* set_task_registers (task, pk_ctsk->startaddr, sp)); */
}

/* make_task_stack --- �����������å����������롣
 *
 * �����ͥ�⡼�ɤǻ��Ѥ��륿�����ѥ����å����������롣
 *
 */
static ER 
make_task_stack (T_TCB *task, W size, VP *sp)
{
  INT	err;

/*  err = pget_blk (&sp, TMPL_SYS, ROUNDUP (size, PAGE_SIZE)); */
  /* �����å��ݥ��󥿤� 0x80000000 �β��ۥ��ɥ쥹�ǥ�����������ɬ�פ����롣*/
  (*sp) = palloc (ROUNDUP (size, PAGE_SIZE) / PAGE_SIZE);
  (UW)(*sp) |= 0x80000000;
#ifdef TSKSW_DEBUG
  printf ("sp = 0x%x\n", *sp);
#endif
  if (*sp == (VP)NULL)
    {
#ifdef TSKSW_DEBUG
      printf ("make_task_stack: palloc fail.\n");
#endif
      return (E_NOMEM);
    }
  
  return (E_OK);
}

/*****************************************************************************
 * �������ꥹ�Ȥ����뤿��δؿ���
 *
 * 
******************************************************************************/
 
/* add_tcb_list --- ���� list �ǻ��ꤵ�줿�ꥹ�Ȥΰ��ֺǸ�˥��������ɲä��롣
 *
 * ����:
 *	list
 *	new
 *
 * �֤���:
 *	�������ꥹ�ȤؤΥݥ���
 *
 * ���.
 *	���δؿ���¹Ԥ��Ƥ⡢�ꥹ�Ȥ���Ƭ�ݥ��󥿤� *�ѹ�����ʤ�*��
 *	���������ꥹ�Ȥ����Ǥ��ʤ��ä����ˤ��㳰�ǡ����ξ��ˤ�
 *	���� new �����Ǥ������ĥꥹ�Ȥ��֤���
 *
 */
T_TCB *
add_tcb_list (T_TCB *list, T_TCB *new)
{
  dis_int ();
  if (list == NULL)
    {
      new->before = new;
      new->next = new;
      list = new;
      ena_int();
      return list;
    }

  new->before = list;
  new->next = list->next;
  list->next = new;
  new->next->before = new;
  ena_int();
  return (list);
}

/* ins_tcb_list --- ���� list �ǻ��ꤵ�줿�ꥹ�Ȥΰ��ֺǽ�˥��������������롣
 *
 * ����:
 *	list
 *	new
 *
 * �֤���:
 *	�������ꥹ�ȤؤΥݥ���
 *
 * ���.
 *	���δؿ���¹Ԥ�����̡��ꥹ�Ȥ���Ƭ�ݥ��󥿤��ѹ�����롣
 *	��äơ����δؿ����֤��������ꥹ�ȤؤΥݥ��󥿤�ꥹ�ȥݥ��󥿤�
 *	����������ɬ�פ����롣
 */
T_TCB *
ins_tcb_list (T_TCB *list, T_TCB *new)
{
  dis_int ();
  if (list == NULL)
    {
      new->before = new;
      new->next = new;
      list = new;
      ena_int ();
      return (list);
    }

  new->next = list;
  new->before = list->before;
  list->before = new;
  new->before->next = new;
  list = new;
  ena_int ();
  return (list);
}


/* del_tcb_list --- ���� list �ǻ��ꤵ�줿�ꥹ�Ȥ��顢���� del �������롣
 *
 * ����:
 *	list
 *	del
 *
 * �֤���:
 *	�������ꥹ�ȤؤΥݥ���
 *
 * �㳰:
 *	1) �⤷������ del ���ꥹ�Ȥ���Ƭ���Ǥξ�硢�ꥹ�Ȥ���Ƭ�ݥ��󥿤ϡ�
 *	   del �μ������Ǥˤʤ롣
 *	2) �⤷������ del ���ꥹ�Ȥ�ͣ������Ǥξ�硢�֤��ͤȤ��� NULL ��
 *	   �֤���
 *
 * ���.
 *	���δؿ���¹Ԥ�����̡��ꥹ�Ȥ���Ƭ�ݥ��󥿤��ѹ�����롣
 *	��äơ����δؿ����֤��������ꥹ�ȤؤΥݥ��󥿤�ꥹ�ȥݥ��󥿤�
 *	����������ɬ�פ����롣
 *
 */
T_TCB *
del_tcb_list (T_TCB *list, T_TCB *del) 
{
  dis_int ();
  if (del->next == del)	/* �ꥹ�Ȥ����Ǥ���Ĥ����ʤ��ä���� */
    {
      ena_int ();
      return (NULL);
    }

  if (list == del)	/* �ꥹ�Ȥ���Ƭ���Ǥ��ä���� */
    {
      list = del->next;
      if (list->next == del)	/* ���Ǥ�2�Ĥ����ʤ���硢delete �������Ǥμ������Ǥϡ�
				   delete �������Ǥ򤵤��Ƥ��롣��ʬ���Ȥ�ؤ��褦��
				   �ѹ����롣*/
	{
	  list->next = list;
	}
    }

  del->next->before = del->before;
  del->before->next = del->next;
  ena_int ();
  return (list);
}

/* task_switch --- �������ڤ괹��
 *
 * ������	save_nowtask	TRUE �ΤȤ������������� ready ���������塼����¸����
 *				FALSE �ΤȤ������������� ready ���������塼����¸���ʤ�
 *
 * ���͡�	���顼�ֹ�
 *
 * ������	ready_task ����ǡ�����ͥ���̤ι⤤�������򥫥���
 *		�������ˤ��롣
 *		�ºݤΥ������ڤ괹���ϡ�resume () �ˤ�äƤ����ʤ�
 *		���Τ��ᡢ���δؿ�����Ǥν����ϡ�run_tsk �ѿ���
 *		ready_task[] �ι�����Ԥ��Τ���Ȥʤ롣
 *		
 */
ER
task_switch (BOOL save_nowtask)
{
  T_TCB	*tcb;
  ID	tskid;
  T_TCB *old;		/* */

#ifdef TSKSW_DEBUG
  printf ("task_switch(): start\n");
#endif

  if (on_interrupt)
    {
      delayed_dispatch = TRUE;
      return (E_OK);
    }
  dis_int ();
  if (dispatch_flag == FALSE)
    {
      ena_int ();
      return (E_CTX);
    }

  for (tskid = MIN_PRIORITY; tskid <= MAX_PRIORITY; tskid++)
    {
      if (ready_task[tskid] != NULL)
	break;
    }
  if (tskid > MAX_PRIORITY)
    {
#if defined (TSKSW_DEBUG)
      printf ("task_switch(): error = E_NOEXS\n");	/* */
#endif
      ena_int ();
      return (E_NOEXS);
    }

  if (save_nowtask)	/* ���������� ready ���������塼����¸���� */
    {
      run_task->tskstat = TTS_RDY;
      ready_task[run_task->tsklevel] = add_tcb_list (ready_task[run_task->tsklevel],
						     run_task);
    }

#ifdef notdef
  /* ���򤷤��������������������ʤ�С����⤷�ʤ������ */
  if (run_task == ready_task[tskid])
    {
      ready_task[tskid] = del_tcb_list (ready_task[tskid], run_task);
      ena_int ();
      return (E_OK);
    }
#endif /* notdef */

  /* ���򤷤���������ready_task ���塼����Ϥ��� */
  tcb = ready_task[tskid];
  if (tcb->tskstat != TTS_RDY)
    {
      ena_int ();
      return (E_SYS);
    }
  old = run_task;	/* */
  run_task = tcb;
  run_task->tskstat = TTS_RUN;
  ready_task[run_task->tsklevel] = del_tcb_list (ready_task[run_task->tsklevel], run_task);

#ifdef TSKSW_DEBUG
  printf ("task_switch(): new task (ID = %d)\n", tcb->tskid);
#endif
  if (run_task->context.eip == 0)
    {
      printf ("ERROR!!!! context data is invalid.\n");
      printf ("OLD TASK ID = %d\n", old->tskid);
      printf ("NEW TASK ID = %d\n", run_task->tskid);
      falldown ("SYSTEM DOWN.\n");
    }
/* resume ��ƤӽФ���resume �ΰ����ϡ�TSS �ؤΥ��쥯�� */
#ifdef TSKSW_DEBUG
  printf ("resume (0x%x)\n", ((tcb->tskid + TSS_BASE) << 3) & 0xfff8);
#endif
  delayed_dispatch = FALSE;
  resume ((UW)(tcb->tskid + TSS_BASE) << 3);
/*  print_context (((tcb->tskid + TSS_BASE) << 3) & 0xfff8); */
  ena_int ();
  return (E_OK);	/* ����˽�λ���������Υ����������å��λ��ˤ�������� */
}


/* task_switch2 --- �������ڤ괹��
 *
 * ������	save_nowtask	TRUE �ΤȤ������������� ready ���������塼����¸����
 *				FALSE �ΤȤ������������� ready ���������塼����¸���ʤ�
 *
 * ���͡�	���顼�ֹ�
 *
 * ������	ready_task ����ǡ�����ͥ���̤ι⤤�������򥫥���
 *		�������ˤ��롣
 *		�ºݤΥ������ڤ괹���ϡ�resume () �ˤ�äƤ����ʤ�
 *		���Τ��ᡢ���δؿ�����Ǥν����ϡ�run_tsk �ѿ���
 *		ready_task[] �ι�����Ԥ��Τ���Ȥʤ롣
 *		
 */
ER
task_switch2 (BOOL save_nowtask)
{
  T_TCB	*tcb;
  ID	tskid;

#ifdef TSKSW_DEBUG
  printf ("task_switch(): start\n");
#endif

  if (on_interrupt)
    {
      delayed_dispatch = TRUE;
      return (E_OK);
    }
  dis_int ();
  if (dispatch_flag == FALSE)
    {
      ena_int ();
      return (E_CTX);
    }
  ena_int ();

  if (save_nowtask)	/* ���������� ready ���������塼����¸���� */
    {
      run_task->tskstat = TTS_RDY;
      ready_task[run_task->tsklevel] = add_tcb_list (ready_task[run_task->tsklevel],
						     run_task);
    }

  for (tskid = MIN_PRIORITY; tskid <= MAX_PRIORITY; tskid++)
    {
      if (ready_task[tskid] != NULL)
	break;
    }
  if (tskid > MAX_PRIORITY)
    {
#if defined (TSKSW_DEBUG)
      printf ("task_switch(): error = E_NOEXS\n");	/* */
#endif
      ena_int ();
      return (E_NOEXS);
    }

  /* ���򤷤��������������������ʤ�С����⤷�ʤ������ */
  if (run_task == ready_task[tskid])
    {
/*      printf ("task is non new.\n");	/* */
      ready_task[tskid] = del_tcb_list (ready_task[tskid], run_task);
      ena_int ();
      return (E_OK);
    }

  printf ("new task is %d\n", tskid);	/* */

  /* ���򤷤���������ready_task ���塼����Ϥ��� */
  tcb = ready_task[tskid];
  if (tcb->tskstat != TTS_RDY)
    {
      ena_int ();
      return (E_SYS);
    }
  run_task = tcb;
  run_task->tskstat = TTS_RUN;
  ready_task[run_task->tsklevel] = del_tcb_list (ready_task[run_task->tsklevel], run_task);

/* #ifdef TSKSW_DEBUG */
  printf ("task_switch(): new task (ID = %d)\n", tcb->tskid);
/* #endif */
/* resume ��ƤӽФ���resume �ΰ����ϡ�TSS �ؤΥ��쥯�� */
#ifdef TSKSW_DEBUG
  printf ("resume (0x%x)\n", ((tcb->tskid + TSS_BASE) << 3) & 0xfff8);
#endif
  delayed_dispatch = FALSE;
  ena_int ();
  resume ((UW)(tcb->tskid + TSS_BASE) << 3);
/*  print_context (((tcb->tskid + TSS_BASE) << 3) & 0xfff8); */
  return (E_OK);	/* ����˽�λ���������Υ����������å��λ��ˤ�������� */
}


print_context (UW selector)
{
  TASK_DESC		*desc;
  T_I386_CONTEXT	*tcb;

  desc = (TASK_DESC *)get_gdt ((selector >> 3) - TSS_BASE & 0xffff);
  tcb = (T_I386_CONTEXT *)GET_TSS_ADDR (*desc);
#ifdef TSKSW_DEBUG
  printf ("task ID = %d\n", (selector >> 3) - TSS_BASE);
  printf ("TSS addr = 0x%x\n", tcb);
  printf ("EIP = 0x%x\n", tcb->eip);
#endif
}


/*****************************************************************************
 *
 *		S Y S T E M   C A L L  
 *				  for  T A S K.
 *
 *
 */

/* cre_tsk --- create task.
 *
 * ��������1���������롣
 * ���������������ϡ�TTS_DMT ���֤Ȥʤꡢsta_tsk()��¹Ԥ���ޤǤ�
 * �ºݤ�ư�����ȤϤʤ���
 *
 *
 * ����: tskid 		�������륿������ID
 *	 pk_ctsk	�������륿������°������
 *			tskatr		������°��
 *			startaddr	��������ư���ɥ쥹
 *			itskpri		��������ư��ͥ����
 *			stksz		�����å�������
 *			addrmap		���ɥ쥹�ޥå�
 *
 * �֤��͡�	���顼�ֹ�
 *		E_OK	���ｪλ
 *		E_ID	�������� ID �ֹ椬����
 *		E_OBJ	Ʊ��Υ�������¸�ߤ��Ƥ���
 *
 */
ER
cre_tsk (ID tskid, T_CTSK *pk_ctsk)
{
  T_TCB	*newtask;
  W	i;

/* ������ ID ���ϰϥ����å� */
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
/* Ʊ�� ID �Υ�������¸�ߤ��Ƥ��뤫�ɤ����Υ����å� */
  if (task[tskid].tskstat != TTS_NON)
    {
      return (E_OBJ);
    }

  newtask = &task[tskid];
  bzero (newtask, sizeof (*newtask));
/* ���������� */
  
  newtask->tskid     = tskid;
  newtask->tskstat   = TTS_DMT;
  newtask->tsklevel  = pk_ctsk->itskpri;
  newtask->tsklevel0 = pk_ctsk->itskpri;
  if (make_task_context (newtask, pk_ctsk) != E_OK)
    {
      return (E_NOMEM);
    }

  /* ���ۥ���Υޥåԥ󥰥ơ��֥����� pk_ctsk �λ��ꤷ���ޥåפ�
   * �ѹ����롣
   * ���꤬�ʤ��Ȥ��ˤϡ������ȥץ�����Ʊ���ޥåפȤʤ롣
   */
  if (pk_ctsk->addrmap != NULL)
    {
      newtask->context.cr3 = (UW)(pk_ctsk->addrmap);
    }
  else
    {
      newtask->context.cr3 = (UW)dup_vmap_table (run_task->context.cr3) & 0x7fffffff;
/*
      newtask->context.cr3 = run_task->context.cr3;
*/
    }

  /* �������Υ꡼�����ơ��֥������
   */
  for (i = 0; i < MAX_REGION; i++)
    {
      newtask->regions[i].permission = 0;
    }
  newtask->regions[0].start_addr = (VP)0x80000000;
  newtask->regions[0].min_size   = 0x7fffffff;
  newtask->regions[0].max_size   = 0x7fffffff;
  newtask->regions[0].permission = VPROT_READ | VPROT_WRITE | VPROT_KERNEL;

  return (E_OK);
}

ER
del_tsk (ID tskid)
{
}

/* sta_tsk --- �������ε�ư
 * 
 * ����tskid�ǻ��ꤷ����������ư���롣
 * ���ꤷ���������ϡ�cre_tsk ����������Ƥ���ɬ�פ����롣
 *
 * ������
 *	tskid	��ư���륿������ ID
 *	stacd	��������ư������
 *
 * ����͡�
 *	E_OK	���ｪλ
 *
 */
ER
sta_tsk (ID tskid, INT stacd)
{
  register int	index;

#ifdef TSKSW_DEBUG
  printf ("sta_tsk: start\n");
#endif
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
  if (task[tskid].tskstat != TTS_DMT)
    {
      return (E_NOEXS);
    }

  index = task[tskid].tsklevel;
  task[tskid].tskstat = TTS_RDY;
  task[tskid].wakeup_count = 0;
  task[tskid].suspend_count = 0;
  task[tskid].total = 0;
  ready_task[index] = add_tcb_list (ready_task[index], &task[tskid]);
#ifdef TSKSW_DEBUG
  printf ("sta_tsk: task level = %d\n", index);
#endif
  return (E_OK);
}

/******************************************************************************
 * ext_tsk --- ����������λ
 *
 * run_task �ˤĤʤ���Ƥ��륿������ TTS_DMT ���֤ذ�ư���롣
 * ����񸻤ʤɤ��ֵѤ��ʤ���
 * 
 * ������	�ʤ�
 *
 * �֤��͡�	�ʤ�
 *
 * ��		���Υ����ƥॳ�����¹Ԥ������Ȥϡ���ȤΥ���ƥ�����
 *		�ˤ����ʤ���
 *
 * �������ơ�
 *	���δؿ��Ǥϡ�ready ���������塼�ˤĤʤ���Ƥ��륿�����Τ�����
 *	�Ǥ�ͥ���٤ι⤤��Τ����򤷡����˼¹Ԥ��륿�����Ȥ��롣
 *	(���򤹤륿�������ʤ��Ȥ������ȤϤʤ���--- ɬ�����Ԥ��Ƥ��� 
 *	idle �����������뤿��)
 *	switch_task() �Ǥϡ������Ԥ��Ƥ륿������ready���������塼����
 *	��롣��������ext_tsk() �Ǥϡ����Υ������Ͻ�λ���뤿�ᡢready
 *	���������塼�ˤ����줺�����֤�TTS_DMT�ˤ��롣
 */
void
ext_tsk (void)
{
  T_TCB	*tcb;
  ID	tskid;

  /* ���ߤΥ������� TTS_DMT ���֤ˤ������򤷤��������򼡤����餻��褦 */
  /* �ˤ��롣							       */
  run_task->tskstat = TTS_DMT;
  task_switch (FALSE);
}

/******************************************************************************
 * exd_tsk --- ����������λ�Ⱥ��
 *
 * run_task �ˤĤʤ���Ƥ��륿������ TTS_NON ���֤ذ�ư���롣
 * ����񸻤ʤɤ��ֵѤ��ʤ������ޥåԥ󥰤��줿����ˤĤ��Ƥϡ���
 * �����롣
 * 
 * ������	�ʤ�
 *
 * �֤��͡�	�ʤ�
 *
 * ��		���Υ����ƥॳ�����¹Ԥ������Ȥϡ���ȤΥ���ƥ�����
 *		�ˤ����ʤ���
 *
 * �������ơ�
 *	���δؿ��Ǥϡ�ready ���������塼�ˤĤʤ���Ƥ��륿�����Τ�����
 *	�Ǥ�ͥ���٤ι⤤��Τ����򤷡����˼¹Ԥ��륿�����Ȥ��롣
 *	(���򤹤륿�������ʤ��Ȥ������ȤϤʤ���--- ɬ�����Ԥ��Ƥ��� 
 *	idle �����������뤿��)
 *	switch_task() �Ǥϡ������Ԥ��Ƥ륿������ready���������塼����
 *	��롣��������ext_tsk() �Ǥϡ����Υ������Ͻ�λ���뤿�ᡢready
 *	���������塼�ˤ����줺�����֤�TTS_DMT�ˤ��롣
 */
void
exd_tsk (void)
{
  T_TCB	*tcb;
  ID	tskid;

  /* ���ߤΥ������� TTS_NON ���֤ˤ������򤷤��������򼡤����餻��褦�ˤ��롣*/
  run_task->tskstat = TTS_NON;
  release_vmap ((ADDR_MAP)run_task->context.cr3);	/* �ޥåԥ󥰥ơ��֥��������� */
  task_switch (FALSE);
}

/*************************************************************************
 * ter_tsk --- ¾������������λ
 *
 * ��ǽ��
 *	�����ǻ��ꤷ������������Ū�˽�λ�����롣
 *	��λ���륿�����Τ�äƤ���񸻤ϲ������ʤ���
 *	�����������Υ����ƥॳ����ˤ�äƽ�λ�����������ϡ�TTS_DMT ��
 *	�֤ˤʤä������ʤΤǡ�sta_tsk �����ƥॳ����ˤ�äƺƳ�����
 *	���Ȥ��Ǥ��롣
 */
ER
ter_tsk (ID tskid)
{
  switch (task[tskid].tskstat)
    {
    default:
    case TTS_RUN:	/* ���������ξ�� */
      return (E_OBJ);
      /* NOT REACHED */
   
      /* ready ���֤ˤ��륿�����ξ�硧������λ������ */
    case TTS_RDY:
      task[tskid].tskstat = TTS_DMT;
      break;

      /* �Ԥ����֤ˤ��륿�����ξ�硧�Ԥ����֤���������Ƥ��鶯����λ�����롣*/
    case TTS_WAI:
      /* relase_wait();		/* �ޤ�����ץ���Ȥ��Ƥ��ʤ���*/
      task[tskid].tskstat = TTS_DMT;
      break;
    }
  return (E_OK);
}

ER
dis_dsp ()
{
  dispatch_flag = FALSE;
  return (E_OK);
}

ER
ena_dsp ()
{
  dispatch_flag = TRUE;
  return (E_OK);
}

/* chg_pri --- �ץ饤����ƥ����ѹ�
 *
 */
ER
chg_pri (ID tskid, PRI tskpri)
{
  T_TCB	*p;

  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
  switch (task[tskid].tskstat)
    {
    default:
      return (E_OBJ);

    case TTS_NON:
      return (E_NOEXS);

    case TTS_RDY:
      ready_task[task[tskid].tsklevel] = del_tcb_list (ready_task[task[tskid].tsklevel], &task[tskid]);
      task[tskid].tsklevel = tskpri;
      ready_task[task[tskid].tsklevel] = add_tcb_list (ready_task[task[tskid].tsklevel], &task[tskid]);
      break;

    case TTS_RUN:
      task[tskid].tsklevel = tskpri;
      break;
    }
  return (E_OK);
}

/* rot_rdq --- Ʊ��ץ饤����ƥ��ǤΥ������ν�����ѹ�����
 *
 */
ER
rot_rdq (PRI tskpri)
{
  T_TCB	*first;

  if ((tskpri < MIN_PRIORITY) || (tskpri > MAX_PRIORITY))
    {
      return (E_PAR);
    }
  first = ready_task[tskpri];
  ready_task[tskpri] = del_tcb_list (ready_task[tskpri], first);
  ready_task[tskpri] = add_tcb_list (ready_task[tskpri], first);
  return (E_OK);
}

ER
rel_wai (ID tskid)
{
}

/***********************************************************************************
 * get_tid --- ���������Υ����� ID ����
 *
 *
 */
ER
get_tid (ID *p_tskid)
{
  *p_tskid = run_task->tskid;
  return (E_OK);
}

/***********************************************************************************
 * ref_tsk --- ���������֤λ���
 *
 * ���������֤��֤���
 *
 */
ER
ref_tsk (T_RTSK *pk_rtsk, ID tskid)
{
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return (E_ID);
    }
  switch (task[tskid].tskstat)
    {
    default:
      pk_rtsk->tskpri = task[tskid].tsklevel;
      pk_rtsk->tskstat = task[tskid].tskstat;
      pk_rtsk->total_time = task[tskid].total;
      break;

    case TTS_NON:
      return (E_NOEXS);
    }
  return (E_OK);
}

/*********************************************************************************
 * slp_tsk --- �����������Ԥ����֤ˤ���
 *
 *	��ʬ���Ȥ��Ԥ����֤ˤ��ơ�¾�Υ�������������Ϥ���
 *	�Ԥ��װ��ϡ����δؿ��Ǥϥ��åȤ��ʤ���
 *
 */
ER
slp_tsk (VOID)
{
/*  printf ("slp_tsk: start\n");	/* */
  dis_int ();
  if (run_task->wakeup_count > 0)
    {
      run_task->wakeup_count--;
/*      printf ("sleep task: wakeup count = %d\n", run_task->wakeup_count--); */
      ena_int ();
      return (E_OK);
    }
  
  run_task->slp_err = E_OK;
  run_task->tskstat = TTS_WAI;
  ena_int ();
  task_switch (FALSE);	/* run_task �� ready_task ���塼����¸���ʤ� */
/*  task_switch2 (FALSE);	/* run_task �� ready_task ���塼����¸���ʤ� */
  return (run_task->slp_err);
}

/*********************************************************************************
 * wup_tsk --- ���ꤵ�줿�������򵯾����롣
 *
 * ��ǽ��
 * 	�Ԥ����֥ե饰(tskwai)�ϡ����δؿ���ƤӽФ����˥ꥻ�åȤ��Ƥ��ʤ����
 *	�����ʤ���
 * 	�⤷�Ԥ����֥ե饰�����åȤ���Ƥ����ʤ�С�E_OBJ �Υ��顼�Ȥʤ롣
 * 
 */
ER
wup_tsk (ID taskid)
{
  T_TCB	*p;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  p = &(task[taskid]);
  if ((p == run_task) || (p->tskstat == TTS_DMT))
    {
      ena_int ();
      return (E_OBJ);
    }

  /* ���٤Ƥ��Ԥ����֤��������Ƥ��ʤ���С���ˤϿʤޤʤ� */
  if ((p->tskwait.time_wait) || (p->tskwait.semaph_wait) || (p->tskwait.event_wait) || (p->tskwait.msg_wait))
    {
      printf ("task %d is waiting. abort wakeup.\n", p->tskid);
      printf ("(p->tskwait.time_wait) = %d\n", (p->tskwait.time_wait));
      printf ("(p->tskwait.semaph_wait) = %d\n", (p->tskwait.semaph_wait));
      printf ("(p->tskwait.event_wait) = %d\n", (p->tskwait.event_wait));
      printf ("(p->tskwait.msg_wait) = %d\n", (p->tskwait.msg_wait));
      ena_int ();
      return (E_OBJ);
    }

  if (p->tskstat == TTS_WAS)
    {
      p->tskstat = TTS_SUS;
    }
  else if (p->tskstat == TTS_WAI)
    {
      p->tskstat = TTS_RDY;
      if ((p < 0x7fffffff) || (UW)(p >= 0x81000000))
	{
	  printf ("wup_tsk: error on tasklist\n");
	  print_task_list ();
	  falldown ("kernel: task.\n");
	}
      else
	{
	  ready_task[p->tsklevel] = ins_tcb_list (ready_task[p->tsklevel], p);
	}
    }
  else if (p->tskstat == TTS_SUS)
    {
      p->wakeup_count++;
    }

  ena_int ();
  return (E_OK);
}

/*****************************************************************************
 * sus_tsk --- ���ꤷ�������������Ԥ����֤˰ܹ�
 *
 * ������
 *	taskid --- suspend ���륿������ ID
 *
 * �֤��͡�
 *
 *
 * ��ǽ��
 *
 */
ER
sus_tsk (ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  if (&task[taskid] == run_task)
    {
      return (E_OBJ);
    }

  dis_int ();
  taskp = &task[taskid];
  taskp->suspend_count++;
  switch (taskp->tskstat)
    {
    case TTS_RDY:
      ready_task[taskp->tsklevel] = del_tcb_list (ready_task[taskp->tsklevel], taskp);
      taskp->tskstat = TTS_SUS;
      break;

    case TTS_SUS:
      if (taskp->suspend_count > MAX_SUSPEND_NEST)
	{
	  taskp->suspend_count = MAX_SUSPEND_NEST;
	  ena_int ();
	  return (E_QOVR);
	}
      break;

    case TTS_WAI:
      taskp->tskstat = TTS_WAS;
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);
      /* DO NOT REACHED */

    default:
      ena_int ();
      return (E_OBJ);
      /* DO NOT REACHED */
    }
  ena_int ();
  return (E_OK);
}

/******************************************************************************************
 * rsm_tsk --- �����Ԥ����֤Υ����������Ԥ����֤���
 *
 * ������
 *	taskid --- suspend ���Ƥ��륿������ ID
 *
 *
 * �֤��͡�
 *	���Υ��顼�ֹ椬�֤�
 *	
 * E_OK     �����ƥॳ���������˽�λ����
 * E_ID     ������ ID ������
 * E_NOEXS  ��������¸�ߤ��ʤ�(TTS_NON ����)
 * E_OBJ    �������ξ��֤�����(TTS_SUS, TTS_WAS, TTS_NON �ʳ�)
 *
 *
 * ��ǽ��
 *	�Ԥ����֤ˤ��륿�������Ԥ����֤��鶯��Ū�˲�����롣
 *
 *	�Ԥ����֤�¿�Ťˤʤ뤳�Ȥ����뤬�����Υ����ƥॳ����ϡ��ҤȤĤ����Ԥ�
 *	�������롣
 */
ER
rsm_tsk (ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  taskp = &task[taskid];
  switch (taskp->tskstat)
    {
    case TTS_SUS:
      taskp->suspend_count--;
      if (taskp->suspend_count <= 0)
	{
	  taskp->tskstat = TTS_RDY;
	  ready_task[taskp->tsklevel] = ins_tcb_list (ready_task[taskp->tsklevel], taskp);
	}
      break;

    case TTS_WAS:
      taskp->suspend_count--;
      if (taskp->suspend_count <= 0)
	{
	  taskp->tskstat = TTS_WAI;
	}
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);

    default:
      ena_int ();
      return (E_OBJ);
    }
  ena_int ();
  return (E_OK);
}

/******************************************************************************
 * frsm_tsk --- �����Ԥ����֤Υ����������Ԥ����֤���(¿�Ť��Ԥ�������)
 *
 * ������
 *	taskid --- suspend ���륿������ ID
 *
 * �֤��͡�
 *	���顼�ֹ�
 *
 * ��ǽ��
 *
 */
ER
frsm_tsk (ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  taskp = &task[taskid];
  switch (taskp->tskstat)
    {
    case TTS_SUS:
      taskp->tskstat = TTS_RDY;
      ready_task[taskp->tsklevel] = ins_tcb_list (ready_task[taskp->tsklevel], taskp);
      break;

    case TTS_WAS:
      taskp->tskstat = TTS_WAI;
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);

    default:
      ena_int ();
      return (E_OBJ);
    }
  ena_int ();
  return (E_OK);
}

/******************************************************************************************
 * can_wup --- �������ε����׵��̵����
 *
 * ������
 *	taskid --- �������� ID
 *
 * �֤��͡�
 *	���顼�ֹ�
 *
 * ��ǽ��
 *
 */
ER
can_wup (INT *p_wupcnt, ID taskid)
{
  T_TCB	*taskp;

  if ((taskid < MIN_TSKID) || (taskid > MAX_TSKID))
    {
      return (E_ID);
    }

  dis_int ();
  taskp = &task[taskid];
  switch (taskp->tskstat)
    {
    case TTS_WAS:
    case TTS_WAI:
      *p_wupcnt = taskp->wakeup_count;
      taskp->wakeup_count = 0;
      break;

    case TTS_NON:
      ena_int ();
      return (E_NOEXS);
      /* DO NOT REACHED */

    default:
      ena_int ();
      return (E_OBJ);
      }
  ena_int ();
  return (E_OK);
}


/***********************************************************************
 * new_task --- Ǥ�դΥ����� ID �ǤΥ���������
 *
 * ������
 *	pk_ctsk	�������륿������°������
 *		tskatr		������°��
 *		startaddr	��������ư���ɥ쥹 (run_flag == TRUE �ΤȤ�)
 *		itskpri		��������ư��ͥ����
 *		stksz		�����å�������
 *		addrmap		���ɥ쥹�ޥå�
 *	rid	���������������� ID (�֤���)
 *	run_flag  ����������������¹Ԥ���
 *
 * �֤��͡�
 *	���顼�ֹ�
 *	E_OK	���ｪλ
 *
 * ��ǽ��
 *	new_task �ϡ����������������������Ȥ��� cre_tsk �ȤۤȤ��
 *	Ʊ����ǽ���ġ���������cre_tsk �������� ID ��ɬ�פȤ���Τ���
 *	����new_task �ϡ������� ID ��ưŪ�˳�ꤢ�Ƥ롣
 *
 */
ER
new_task (T_CTSK *pk_ctsk, ID *rid, BOOL run_flag)
{
  ID	i;
  ER	err;

  for (i = MIN_USERTASKID; i <= MAX_USERTASKID; i++)
    {
      err = cre_tsk (i, pk_ctsk);
      if (err == E_OK)
	{
	  *rid = i;
	  if (run_flag == TRUE)
	    sta_tsk (i, 0);
	  return (E_OK);
	}
    }
  return (E_NOMEM);
}


T_TCB *
get_tskp (ID tskid)
{
  if ((tskid < MIN_TSKID) || (tskid > MAX_TSKID))
    {
      return ((T_TCB *)NULL);
    }
  return (&task[tskid]);
}

