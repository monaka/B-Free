/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* fault.cj
 *
 *
 */

#include "itron.h"
#include "ibmpc.h"
#include "interrupt.h"
#include "errno.h"
#include "task.h"
#include "func.h"

/* T_INTR_HANDLER	割り込みハンドラ定義
 *
 */
typedef struct intr_handler_t
{
  W		masklevel;
  void		(*handler)(VP sp);
} T_INTR_HANDLER;

/*
 *	割り込み処理の大域変数
 *
 */
#if 0
BOOL	on_interrupt = FALSE;	/* 割り込み中かどうかを示すフラグ */
#else
W	on_interrupt = 0;
#endif
BOOL	delayed_dispatch = FALSE;

/*
 * 割り込みハンドラテーブル
 * def_int システムコールで登録するときに使用する。
 */
struct intr_entry
{
  ATR	attr;
  FP	func;
};

struct intr_entry	intr_table[128];



/**************************************************************************
 * init_interrupt --- 割り込み機能の初期化を行う。
 *
 * 引数：なし
 *
 * 返値：エラー番号
 *
 */
W
init_interrupt (void)
{
  W	i;

  printk ("init_interrupt\n");
  /* 8259 の初期化 */
  /* init master 8259A */
  outb (MASTER_8259A_COM, 0x11);	/* ICW1 */
  outb (MASTER_8259A_DATA, 0x20);	/* ICW2 */
  outb (MASTER_8259A_DATA, 0x04);	/* ICW3 */
#ifdef notdef
  outb (MASTER_8259A_DATA, 0x1d);	/* ICW4 */
#else
  outb (MASTER_8259A_DATA, 0x01);	/* ICW4 */
#endif

  /* init slave 8259A */
  outb (SLAVE_8259A_COM, 0x11);		/* ICW1 */
  outb (SLAVE_8259A_DATA, 0x28);	/* ICW2 */
#ifdef notdef
  outb (SLAVE_8259A_DATA, 0x07);	/* ICW3 */
  outb (SLAVE_8259A_DATA, 0x09);	/* ICW4 */
#else
  outb (SLAVE_8259A_DATA, 0x02);	/* ICW3 */
  outb (SLAVE_8259A_DATA, 0x01);	/* ICW4 */
#endif

/* set mask */
  outb (MASTER_8259A_DATA, 0xfb);	/* 1111 1011 */
  outb (SLAVE_8259A_DATA,  0xff);	/* 1111 1111 */

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
  set_idt (65,	  		0x08, (W)posix_handler, TRAP_DESC, 0);
  set_idt (35, 			0x08, (int)int35_handler, INTERRUPT_DESC, 0);
  set_idt (37, 			0x08, (int)int37_handler, INTERRUPT_DESC, 0);
  set_idt (INT_FD, 		0x08, (int)int38_handler, INTERRUPT_DESC, 0);

  set_idt (46, 			0x08, (int)int46_handler, INTERRUPT_DESC, 0);	/* IDE 0 */

#ifdef notdef
  set_idt (47, 			0x08, (int)int47_handler, INTERRUPT_DESC, 0);
#endif


  reset_intr_mask (3);
  reset_intr_mask (5);
  reset_intr_mask (9);
  reset_intr_mask (12);

  reset_intr_mask (6); 
  for (i = 0; i < 128; i++)
    {
      intr_table[i].attr = 0;
      intr_table[i].func = 0;
    }

  intr_table[INT_KEYBOARD].attr = 0;
  intr_table[INT_FD].attr = 0;
#ifdef notdef
  intr_table[INT_HD].attr = 0;
#endif
  return (E_OK);
}

/*************************************************************************
 * reset_intr_mask 
 *
 * 引数：	intn	割り込み番号
 *
 * 返値：	なし
 *
 * 処理：	割り込みマスクをリセットする。
 *
 */
void
reset_intr_mask (W intn)
{
  dis_int ();
  if (intn < 8)
    {
      outb (MASTER_8259A_DATA, inb (MASTER_8259A_DATA) & ~(1 << intn));
    }
  else
    {
      outb (SLAVE_8259A_DATA, inb (SLAVE_8259A_DATA) & ~(1 << (intn - 8)));
    }
  ena_int ();
}    

/*************************************************************************
 * interrupt --- 外部割り込みの処理
 *
 * 引数：	intn	割り込み番号
 *
 * 返値：	なし
 *
 * 処理：	外部割り込みが発生したときの処理を行う。
 *
 */
static int mask;
void
interrupt (W intn)
{
#if 0
  if (on_interrupt)
    {
      return;
    }
  on_interrupt = TRUE;
#else
  on_interrupt++;
#endif
  delayed_dispatch = FALSE;

#ifdef notdef
  if ((intn != INT_TIMER) && (intn != INT_KEYBOARD))	/* */
    printk ("Interrupt!! %d\n", intn);	/* */
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
	  printk ("unknown interrupt from %d\n", intn);
	}
      if (intn > 40) { /* slave */
	mask = ~(1 << (intn - 40));
	asm("cli");
	asm("inb $0xA1, %al");
	asm("andb mask, %al");
	asm("outb %al, $0xA1");
      }
      else { /* master */
	mask = ~(1 << (intn - 32));
	asm("cli");
	asm("inb $0x21, %al");
	asm("andb mask, %al");
	asm("outb %al, $0x21");
      }
      break;

    case INT_TIMER:
      intr_interval ();
#if 0
      asm("movb	$0x20, %al");
      asm("cli");
      asm("outb	%al, $0x20");
#else
      asm("cli");
      asm("inb	$0x21, %al");
      asm("andb	$0xFE, %al");
      asm("outb	%al, $0x21");
#endif
      break;

    case INT_KEYBOARD:
      if (intr_table[INT_KEYBOARD].func != 0)
	{
#if 0
	  printk("%d[%d]", intn, run_task->tskid);
#endif
	  (intr_table[INT_KEYBOARD].func) ();
	}
      else
	{
	  intr_kbd ();
	}
#if 0
      asm("movb	$0x20, %al");
      asm("cli");
      asm("outb	%al, $0x20");
#else
      asm("cli");
      asm("inb	$0x21, %al");
      asm("andb	$0xFD, %al");
      asm("outb	%al, $0x21");
#endif
      break;

    case INT_FD:
      if (intr_table[INT_FD].func != 0)
	{
	  (intr_table[INT_FD].func) ();
	}
#if 1
      asm("cli");
      asm("inb	$0x21, %al");
      asm("andb	$0xBF, %al");
      asm("outb	%al, $0x21");
#endif
      break;

#ifdef notdef
    case INT_HD:
      printk ("interrupt: int HD\n");	/* */
      if (intr_table[INT_HD].func != 0)
	{
	  (intr_table[INT_HD].func) ();
	}
      break;
#endif

#ifdef notdef
    case INT_SCSI:
      printk ("fault: int SCSI\n");	/* */
      if (intr_table[INT_SCSI].func != 0)
	{
	  (intr_table[INT_SCSI].func) ();
	}
      break;
#endif
    }
#if 0
  on_interrupt = FALSE;
#else
  dis_int();
  --on_interrupt;
  ena_int();
#endif
#if 1
  if (delayed_dispatch)
    {
      task_switch (TRUE);
    }
#endif
}

/*
 * def_int システムコールによって、割り込みハンドラを登録する。
 *
 * 登録するときには、直接 IDT の値は変更せず、intr_table[] に登録する。
 */
ER
set_interrupt_entry (W intno, FP func, ATR attr)
{
  if (intr_table[intno].attr == -1)
    {
      return (E_OBJ);
    }

  printk ("set_interrupt_entry = %d, func = 0x%x\n", intno, func);
  intr_table[intno].attr = attr;
  intr_table[intno].func = func;
  return (E_OK);
}

/*************************************************************************
 * trap
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
trap (W intn, W syscallno, VP arg)
{
  W	i;

#ifdef DEBUG
  printk ("trap: %d, run_task->n_interrupt = %d\n", intn, run_task->n_interrupt);
#endif /* DEBUG */
  if (run_task->n_interrupt > 0)
    {
      for (i = 0; i < run_task->n_interrupt; i++)
	{
#ifdef DEBUG
	  printk ("interrupt handler searching...: %d, (trap no. = %d)\n", i, run_task->interrupt[i].intr_no);
#endif /* DEBUG */
	  if (run_task->interrupt[i].intr_no == intn)
	    {
#ifdef DEBUG
	      printk ("found: %d(0x%x)\n", run_task->interrupt[i].intr_func, run_task->interrupt[i].intr_func);
#endif /* DEBUG */
	      (run_task->interrupt[i].intr_func) (syscallno, arg);
	      return;
	    }
	}
    }
  
}

/*************************************************************************
 * page_fault
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
page_fault (W errcode, W eip, W cs, W eflags, W esp, W ss)
{
  W	result;

  if (run_task->page_fault_handler)
    {
      /* ページフォルト時の処理ハンドラが指定してあった */
      /* ページフォルトハンドラの引数は以下のとおり
       *
       * ページフォルトが発生したアドレス
       * 実行している EIP
       * ページフォルト処理の result code
       */
      result = (run_task->page_fault_handler)(get_cr2 (), eip);
      if (result == E_OK)
	{
	  /* ページフォルト処理に成功した */
	  return;
	}
    }


  on_interrupt = TRUE;
  printk ("KERNEL PAGE FAULT (memory fault).\n");
  printk ("current PID= %d (0x%x)\n", run_task->tskid, run_task->tskid);
  printk ("context addr = %d (0x%x)\n", run_task, run_task);
  printk ("ss0 = 0x%x/ss1 = 0x%x/ss2 = 0x%x\n",
	  run_task->context.ss0,
	  run_task->context.ss1,
	  run_task->context.ss2);
  printk ("esp0 = 0x%x/esp1 = 0x%x/esp2 = 0x%x\n",
	  run_task->context.esp0,
	  run_task->context.esp1,
	  run_task->context.esp2);

  printk ("current stack segment = 0x%x\n",
	  run_task->context.ss);	  
  printk ("stack pointer = 0x%x\n", run_task->context.esp);
  printk ("initial stack pointer = 0x%x\n", run_task->initial_stack);
  printk ("error code = 0x%x\n", errcode);
  printk ("fault addr = 0x%x\n", get_cr2 ());
  printk ("       eip = 0x%x\n", eip);
  printk ("        cs = 0x%x\n", cs);
  printk ("    eflags = 0x%x\n", eflags);
  printk ("       esp = 0x%x (0x%x)\n", esp, run_task->context.esp);
  printk ("        ss = 0x%x\n", ss);
  printk ("       cr3 = 0x%x\n", get_cr3 ());

#ifdef notdef
  {
    int			i;
    unsigned char	*p;

    p = (unsigned char *)eip;
    for (i = 0; i < 100; i++)
      {
	printk ("0x%x ", p[i]);
      }
  }
#endif
  
  for (;;)
    ;

  on_interrupt = FALSE;
}

/*************************************************************************
 * fault ---
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
fault (W intn, W errcode, W eip, W cs, W eflags, W esp, W ss)
{
  if (intn == 1)
    {
      ena_int ();
      printk ("\nFAULT: Interrupt Number is %d.\n", intn);
      return;
    }

  dis_int ();
  printk ("\nFAULT: Interrupt Number is %d.\n", intn);
  printk ("PID = %d\n", run_task->tskid);
  printk ("run_task->state.state = %d\n", run_task->tskstat);
/*  printk ("tss selector      = %d\n", run_task->tss_selector); */
  printk ("run_task->context.backlink = 0%x\n", run_task->context.backlink);
  printk ("run_task->context.cs  = 0x%x\n", run_task->context.cs);
  printk ("run_task->context.ds  = 0x%x\n", run_task->context.ds);
  printk ("run_task->context.ss  = 0x%x\n", run_task->context.ss);
  printk ("run_task->context.eip = 0x%x\n", run_task->context.eip);
  printk ("run_task->context.esp = 0x%x\n", run_task->context.esp);
  printk ("context addr = %d (0x%x)\n", run_task, run_task);
  printk ("ss0 = 0x%x/ss1 = 0x%x/ss2 = 0x%x\n",
	  run_task->context.ss0,
	  run_task->context.ss1,
	  run_task->context.ss2);
  printk ("esp0 = 0x%x/esp1 = 0x%x/esp2 = 0x%x\n",
	  run_task->context.esp0,
	  run_task->context.esp1,
	  run_task->context.esp2);

  printk ("current stack segment = 0x%x\n",
	  run_task->context.ss);	  
  printk ("stack pointer = 0x%x\n", run_task->context.esp);
  printk ("initial stack pointer = 0x%x\n", run_task->initial_stack);
  printk ("error code = 0x%x\n", errcode);
  printk ("fault addr = 0x%x\n", get_cr2 ());
  printk ("       eip = 0x%x\n", eip);
  printk ("        cs = 0x%x\n", cs);
  printk ("    eflags = 0x%x\n", eflags);
  printk ("       esp = 0x%x\n", esp);
  printk ("        ss = 0x%x\n", ss);
  printk ("       cr3 = 0x%x\n", get_cr3 ());

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
 * set_idt --- IDT のエントリをセットする。
 *
 * 引数：	entry
 *		selector
 *		offset
 *		type
 *		dpl
 *
 * 返値：	なし
 *
 * 処理：	IDT テーブルのエントリをセットする。
 *
 */
void
set_idt (UW entry, UW selector, UW offset, UW type, UW dpl)
{
  struct idt_t	*table;
#if 0
  UW		*tmp;
#endif
  
  table = (struct idt_t *)IDT_ADDR;
  SET_OFFSET_IDT (table[entry], offset);
  table[entry].p = 1;
  table[entry].selector = selector;
  table[entry].dpl = dpl;
  table[entry].type = type;
  table[entry].dt0 = 0;
  table[entry].zero = 0;
#ifdef IDT_DEBUG
  printk ("set_idt: entry = %d, selector = %d, type = %d\n",
	  entry, selector, type);
  tmp = (unsigned int *)&table[entry];
  printk ("idt[%d] = 0x%x, 0x%x\n", entry, tmp[0], tmp[1]);
#endif IDT_DEBUG
}



/*************************************************************************
 * protect_fault --- 
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
protect_fault (UW errno, UW eip, UW tmp)
{
  printk ("******* PROTECT FAULT: interrupt 13. ******\n");
  printk ("PID = %d\n", run_task->tskid);
  printk ("EIP = 0x%x\n", eip);
  printk ("ERROR CODE = 0x%x\n\n", errno);
  printk ("TMP = 0x%x\n", tmp);
  printk ("run_task->state.state = %d\n", run_task->tskstat);
/*  printk ("tss selector      = %d, 0x%x\n",
	  run_task->tss_selector, run_task->tss_selector); */
  printk ("STACK TOP: 0x%x\n", &(run_task->stackptr[run_task->stksz]));
  printk ("STACK BASE: 0x%x\n", run_task->stackptr);
  printk ("run_task->context.backlink  = 0x%x\n", run_task->context.backlink);
  printk ("run_task->context.ldtr  = 0x%x\n", run_task->context.ldtr);
  printk ("run_task->context.cr3  = 0x%x\n", run_task->context.cr3);
  printk ("run_task->context.eflags  = 0x%x\n", run_task->context.eflags);
  printk ("run_task->context.cs  = 0x%x\n", run_task->context.cs);
  printk ("run_task->context.ds  = 0x%x\n", run_task->context.ds);
  printk ("run_task->context.ss  = 0x%x\n", run_task->context.ss);
#ifdef notdef
  printk ("run_task->context.es  = 0x%x\n", run_task->context.es);
  printk ("run_task->context.fs  = 0x%x\n", run_task->context.fs);
  printk ("run_task->context.gs  = 0x%x\n", run_task->context.gs);
#endif /* notdef */
  printk ("run_task->context.eip = 0x%x\n", run_task->context.eip);
  printk ("run_task->context.esp = 0x%x\n", run_task->context.esp);
  for (;;)
    ;
}
