/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* tss.c

 TSS (Task State Segment) �δ����򤹤롣
 
 
 Interface:
 	creat_context:	����ƥ����Ⱦ���򿷤����������롣
	delete_context:	����ƥ����Ⱦ���������롣
	
 */


#include "itron.h"
#include "errno.h"
#include "task.h"
#include "misc.h"
#include "func.h"

/* creat_context ����ƥ����Ⱦ���򿷤����������롣
 *
 */
W
create_context (T_TCB *task)
{
  T_I386_CONTEXT	*tss;

  tss = &(task->context);
  init_task_state_segment (task->tskid, tss);
}

/* init_task_state_segment -- TSS ��ؤ��ǥ�������ץ��ν����
 *
 * ������	index	��������� task_state_seg �ơ��֥�Υ���ǥå���
 *
 * ���͡�	�ʤ�
 *
 * ������	�����ǻ��ꤷ�Ƥ��륿������ TSS ��ؤ�����Υǥ�������ץ���
 *		��������롣
 *		TSS �ǥ����ץ꥿�η����򼡤˼�����
 *
 *	
 *
 */
void
init_task_state_segment (W index, T_I386_CONTEXT *tss)
{
  TASK_DESC *task_desc;

  task_desc = (TASK_DESC *)GDT_ADDR;
  task_desc = &task_desc[index + TSS_BASE];

#ifdef TSKSW_DEBUG
  printf ("init_task_state_segment: gdt[%d] = 0x%x\n", index + TSS_BASE);
#endif
  task_desc->zero0 = 0;
  task_desc->zero1 = 0;
  task_desc->dpl = 0;
  task_desc->present = 1;
  task_desc->type = 9;
  task_desc->g = 1;
  task_desc->d = 1;
  SET_TSS_ADDR (*task_desc, tss);
  SET_TSS_LIMIT (*task_desc, 104);
#ifdef TSKSW_DEBUG
  print_task_desc (task_desc);
#endif
}

void
print_task_desc (TASK_DESC *desc)
{
  printf ("** task descriptor **\n");
  printf ("sizeof TASK_DESC = %d\n", sizeof (*desc));
  printf ("tss addr = 0x%x\n", desc);
  printf ("   type  = %d\n", desc->type);
  printf ("   addr  = 0x%x\n", GET_TSS_ADDR(*desc));
  printf ("   limit = 0x%x\n", GET_TSS_LIMIT(*desc));
}

