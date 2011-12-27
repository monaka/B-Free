/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* fault.cj
 *
 *
 */

#include "itron.h"
#include "pc98.h"
#include "interrupt.h"
#include "errno.h"
#include "task.h"

/* T_INTR_HANDLER	�����ߥϥ�ɥ����
 *
 */
typedef struct intr_handler_t
{
  W		masklevel;
  void		(*handler)(VP sp);
} T_INTR_HANDLER;

/*
 *	�����߽���������ѿ�
 *
 */
BOOL	on_interrupt = FALSE;	/* �������椫�ɤ����򼨤��ե饰 */
BOOL	delayed_dispatch = FALSE;

/*
 * �����ߥϥ�ɥ�ơ��֥�
 * def_int �����ƥॳ�������Ͽ����Ȥ��˻��Ѥ��롣
 */
struct intr_entry
{
  ATR	attr;
  FP	func;
};

struct intr_entry	intr_table[128];



/**************************************************************************
 * init_interrupt --- �����ߵ�ǽ�ν������Ԥ���
 *
 * �������ʤ�
 *
 * ���͡����顼�ֹ�
 *
 */
W
init_interrupt (void)
{
  W	i;

  printf ("init_interrupt\n");
  /* 8259 �ν���� */
  /* init master 8259A */
  outb (MASTER_8259A_COM, 0x11);
  outb (MASTER_8259A_DATA, 0x20);
  outb (MASTER_8259A_DATA, 0x80);
  outb (MASTER_8259A_DATA, 0x1d);
  
  /* init slave 8259A */
  outb (SLAVE_8259A_COM, 0x11);
  outb (SLAVE_8259A_DATA, 0x28);
  outb (SLAVE_8259A_DATA, 0x07);
  outb (SLAVE_8259A_DATA, 0x09);

  /* set mask */
  outb (MASTER_8259A_DATA, 0x7f);
  outb (SLAVE_8259A_DATA,  0xff);

  set_idt (1, 			0x08, (int)int1_handler, INTERRUPT_DESC, 0);
  set_idt (2, 			0x08, (int)int2_handler, INTERRUPT_DESC, 0);
  set_idt (3, 			0x08, (int)int3_handler, INTERRUPT_DESC, 0);
  set_idt (4, 			0x08, (int)int4_handler, INTERRUPT_DESC, 0);
  set_idt (5, 			0x08, (int)int5_handler, INTERRUPT_DESC, 0);
  set_idt (INT_INVALID_OPCODE, 	0x08, (W)int6_handler, FAULT_DESC, 0);
  set_idt (INT_DOUBLE_FAULT,	0x08, (W)int8_handler, FAULT_DESC, 0);
  set_idt (INT_TSS_FAULT,   	0x08, (W)int10_handler, FAULT_DESC, 0);
  set_idt (INT_INVALID_SEG, 	0x08, (W)int11_handler, FAULT_DESC, 0);
  set_idt (INT_STACK_SEG,   	0x08, (W)int12_handler, FAULT_DESC, 0);
  set_idt (INT_PROTECTION,  	0x08, (W)int13_handler, FAULT_DESC, 0);
  set_idt (INT_PAGE_FAULT,  	0x08, (W)int14_handler, FAULT_DESC, 0);
  set_idt (15, 			0x08, (int)int15_handler, INTERRUPT_DESC, 0);
  set_idt (16, 			0x08, (int)int16_handler, INTERRUPT_DESC, 0);
/*
  set_idt (17, 			0x08, (int)int17_handler, INTERRUPT_DESC, 0);
  set_idt (18, 			0x08, (int)int18_handler, INTERRUPT_DESC, 0);
*/
  set_idt (INT_SYSCALL,  	0x08, (W)int64_handler, TRAP_DESC, 0);
/* 
  set_idt (INT_FD, 		0x08, (int)int43_handler, INTERRUPT_DESC, 0);
*/
  set_idt (37, 			0x08, (int)int37_handler, INTERRUPT_DESC, 0);
  set_idt (35, 			0x08, (int)int35_handler, INTERRUPT_DESC, 0);
  set_idt (INT_HD, 		0x08, (int)int41_handler, INTERRUPT_DESC, 0);
/*
  set_idt (INT_FD, 		0x08, (int)int42_handler, INTERRUPT_DESC, 0); 	/* FD 640K type. */

/*  reset_intr_mask (3); */
/*  reset_intr_mask (5); */
/*  reset_intr_mask (9); */
/*  reset_intr_mask (12); */

  for (i = 0; i < 128; i++)
    {
      intr_table[i].attr = 0;
      intr_table[i].func = 0;
    }
/*
  intr_table[INT_KEYBOARD].attr = 0;
  intr_table[INT_FD].attr = 0;
  intr_table[INT_HD].attr = 0;
*/

  return (E_OK);
}

/*************************************************************************
 * reset_intr_mask 
 *
 * ������	intn	�������ֹ�
 *
 * ���͡�	�ʤ�
 *
 * ������	�����ߥޥ�����ꥻ�åȤ��롣
 *
 */
void
reset_intr_mask (W intn)
{
  dis_int ();
  if (intn < 8)
    outb (MASTER_8259A_DATA, inb (MASTER_8259A_DATA) & ~(1 << intn));
  else
    outb (SLAVE_8259A_DATA, inb (SLAVE_8259A_DATA) & ~(1 << (intn - 8)));
  ena_int ();
}    

/*************************************************************************
 * interrupt --- ���������ߤν���
 *
 * ������	intn	�������ֹ�
 *
 * ���͡�	�ʤ�
 *
 * ������	���������ߤ�ȯ�������Ȥ��ν�����Ԥ���
 *
 */
void
interrupt (W intn)
{
  if (on_interrupt)
    {
      return;
    }
  on_interrupt = TRUE;
  delayed_dispatch = FALSE;

#ifdef notdef
  if ((intn != INT_TIMER) && (intn != INT_KEYBOARD))	/* */
    printf ("Interrupt!! %d\n", intn);	/* */
#endif

  switch (intn)
    {
    default:
      if (intr_table[intn].func)
	{
	  (intr_table[intn].func)();
	}
      else
	{
	  /* error!! */
	  printf ("unknown interrupt from %d\n", intn);
	}
      break;

    case INT_TIMER:
      intr_interval ();
      break;
    case INT_KEYBOARD:
      if (intr_table[INT_KEYBOARD].func != 0)
	{
	  (intr_table[INT_KEYBOARD].func) ();
	}
      else
	{
	  intr_kbd ();
	}
      break;
    case INT_FD:
      if (intr_table[INT_FD].func != 0)
	{
	  (intr_table[INT_FD].func) ();
	}
      break;

    case INT_HD:
      printf ("fault: int HD\n");	/* */
      if (intr_table[INT_HD].func != 0)
	{
	  (intr_table[INT_HD].func) ();
	}
      break;

    case INT_SCSI:
      printf ("fault: int SCSI\n");	/* */
      if (intr_table[INT_SCSI].func != 0)
	{
	  (intr_table[INT_SCSI].func) ();
	}
      break;
    }
  on_interrupt = FALSE;
  ena_int ();
  if (delayed_dispatch)
    {
      task_switch (TRUE);
    }
}

/*
 * def_int �����ƥॳ����ˤ�äơ������ߥϥ�ɥ����Ͽ���롣
 *
 * ��Ͽ����Ȥ��ˤϡ�ľ�� IDT ���ͤ��ѹ�������intr_table[] ����Ͽ���롣
 */
ER
set_interrupt_entry (W intno, FP func, ATR attr)
{
  if (intr_table[intno].attr == -1)
    {
      return (E_OBJ);
    }

  intr_table[intno].attr = attr;
  intr_table[intno].func = func;
  return (E_OK);
}

/*************************************************************************
 * trap
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
trap (W intn)
{
  on_interrupt = TRUE;
  on_interrupt = FALSE;
}

/*************************************************************************
 * page_fault
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
page_fault (W errcode, W eip, W cs, W eflags, W esp, W ss)
{
  on_interrupt = TRUE;
  printf ("KERNEL PAGE FAULT (memory fault).\n");
  printf ("current PID= %d (0x%x)\n", run_task->tskid, run_task->tskid);
  printf ("context addr = %d (0x%x)\n", run_task, run_task);
  printf ("error code = 0x%x\n", errcode);
  printf ("fault addr = 0x%x\n", get_cr2 ());
  printf ("       eip = 0x%x\n", eip);
  printf ("        cs = 0x%x\n", cs);
  printf ("    eflags = 0x%x\n", eflags);
  printf ("       esp = 0x%x\n", esp);
  printf ("        ss = 0x%x\n", ss);
  printf ("       cr3 = 0x%x\n", get_cr3 ());
  for (;;)
    ;
  on_interrupt = FALSE;
}

/*************************************************************************
 * fault ---
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
fault (W intn, void *argptr)
{
  dis_int ();
  printf ("\nFAULT: Interrupt Number is %d.\n", intn);
  printf ("PID = %d\n", run_task->tskid);
  printf ("run_task->state.state = %d\n", run_task->tskstat);
/*  printf ("tss selector      = %d\n", run_task->tss_selector); */
  printf ("run_task->context.backlink = 0x%\n", run_task->context.backlink);
  printf ("run_task->context.cs  = 0x%x\n", run_task->context.cs);
  printf ("run_task->context.ds  = 0x%x\n", run_task->context.ds);
  printf ("run_task->context.ss  = 0x%x\n", run_task->context.ss);
  printf ("run_task->context.eip = 0x%x\n", run_task->context.eip);
  printf ("run_task->context.esp = 0x%x\n", run_task->context.esp);
  ena_int ();
  for (;;)
    ;
}

/**************************************************************************
 *
 */
W
wait_int (W *flag)
{
  while (*flag == FALSE)
    ;
  dis_int ();
  *flag = FALSE;
  ena_int ();
  return (*flag);
}

/*************************************************************************
 * set_idt --- IDT �Υ���ȥ�򥻥åȤ��롣
 *
 * ������	entry
 *		selector
 *		offset
 *		type
 *		dpl
 *
 * ���͡�	�ʤ�
 *
 * ������	IDT �ơ��֥�Υ���ȥ�򥻥åȤ��롣
 *
 */
void
set_idt (UW entry, UW selector, UW offset, UW type, UW dpl)
{
  struct idt_t	*table;
  UW		*tmp;
  
  table = (struct idt_t *)IDT_ADDR;
  SET_OFFSET_IDT (table[entry], offset);
  table[entry].p = 1;
  table[entry].selector = selector;
  table[entry].dpl = dpl;
  table[entry].type = type;
  table[entry].dt0 = 0;
  table[entry].zero = 0;
#ifdef IDT_DEBUG
  printf ("set_idt: entry = %d, selector = %d, type = %d\n",
	  entry, selector, type);
  tmp = (unsigned int *)&table[entry];
  printf ("idt[%d] = 0x%x, 0x%x\n", entry, tmp[0], tmp[1]);
#endif IDT_DEBUG
}



/*************************************************************************
 * protect_fault --- 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
protect_fault (UW errno, UW eip, UW tmp)
{
  printf ("******* PROTECT FAULT: interrupt 13. ******\n");
  printf ("PID = %d\n", run_task->tskid);
  printf ("EIP = 0x%x\n", eip);
  printf ("ERROR CODE = 0x%x\n\n", errno);
  printf ("TMP = 0x%x\n", tmp);
  printf ("run_task->state.state = %d\n", run_task->tskstat);
/*  printf ("tss selector      = %d, 0x%x\n",
	  run_task->tss_selector, run_task->tss_selector); */
  printf ("STACK TOP: 0x%x\n", &(run_task->stackptr[run_task->stksz]));
  printf ("STACK BASE: 0x%x\n", run_task->stackptr);
  printf ("run_task->context.backlink  = 0x%x\n", run_task->context.backlink);
  printf ("run_task->context.ldtr  = 0x%x\n", run_task->context.ldtr);
  printf ("run_task->context.cr3  = 0x%x\n", run_task->context.cr3);
  printf ("run_task->context.eflags  = 0x%x\n", run_task->context.eflags);
  printf ("run_task->context.cs  = 0x%x\n", run_task->context.cs);
  printf ("run_task->context.ds  = 0x%x\n", run_task->context.ds);
  printf ("run_task->context.ss  = 0x%x\n", run_task->context.ss);
#ifdef notdef
  printf ("run_task->context.es  = 0x%x\n", run_task->context.es);
  printf ("run_task->context.fs  = 0x%x\n", run_task->context.fs);
  printf ("run_task->context.gs  = 0x%x\n", run_task->context.gs);
#endif /* notdef */
  printf ("run_task->context.eip = 0x%x\n", run_task->context.eip);
  printf ("run_task->context.esp = 0x%x\n", run_task->context.esp);
  for (;;)
    ;
}
