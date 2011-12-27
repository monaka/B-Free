/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* main.c --- ITRON �Υᥤ��ؿ��������
 *
 *
 */

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.19  2000-07-31 18:55:45  kishida0
 * add debug I/F
 *
 * Revision 1.18  2000/04/03 14:34:40  naniwa
 * to call timer handler in task
 *
 * Revision 1.17  2000/02/06 09:10:54  naniwa
 * minor fix
 *
 * Revision 1.16  1999/11/14 14:53:35  naniwa
 * add time management function
 *
 * Revision 1.15  1999/04/13 04:49:02  monaka
 * Some code in 2nd/config.h is separated to ITRON/h/config_boot.h
 *
 * Revision 1.14  1999/04/13 04:15:04  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.13  1999/04/12 14:49:06  monaka
 * Commented out a unused variable define.
 *
 * Revision 1.12  1999/04/12 13:29:18  monaka
 * printf() is renamed to printk().
 *
 * Revision 1.11  1999/03/16 13:02:49  monaka
 * Modifies for source cleaning. Most of these are for avoid gcc's -Wall message.
 *
 * Revision 1.10  1998/02/25 12:38:42  night
 * vmap() �ؿ��λ����ѹ��ˤ�뽤����
 * (���ۥڡ����Υ�������������ꤹ��������ҤȤ�������)
 *
 * Revision 1.9  1997/10/11 16:21:40  night
 * ���ޤ��ޤ�������
 *
 * Revision 1.8  1997/09/21 13:32:01  night
 * log �ޥ�����ɲá�
 *
 *
 */

#include "itron.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"
#include "misc.h"
#include "memory.h"
#include "../io/io.h"
#ifdef I386
#include "interrupt.h"
#endif
#include "config_boot.h"

static ER	init_itron (void);
static void	init_device (void);
#ifdef notdef
static void 	memory_test (void);
#endif
void run_init_program (void);
void banner(void);

/* �����ѿ������ */
extern W do_timer;

/*******************************************************************
 * itron --- �ᥤ��ؿ�
 *
 */
ER
itron (void)
{
#define DEBUGTASK 1
#if DEBUGTASK
  T_CTSK	par_debug_task;
#endif

  if (init_itron () != E_OK)
    {
      falldown ("main: cannot initialize.\n");
    }

  init_device ();

#ifdef DEBUGTASK
  par_debug_task.exinf = 0;
  par_debug_task.startaddr = debugger;
  par_debug_task.itskpri = 10;
  par_debug_task.stksz = PAGE_SIZE * 2;
  par_debug_task.addrmap = NULL;
  if (cre_tsk (ITRON_DEBUG, &par_debug_task) != E_OK)
    {
      printk ("cannot create task for debugger.\n");
    }

  printk ("DEBUG TASK START\n");

  if (sta_tsk (ITRON_DEBUG, NULL) != E_OK)
    {
      printk ("cannot start task for debugger.\n");
    }
#endif

#ifdef AUTO_START
  run_init_program ();
#endif

  for (;;)		/* Idle �������Ȥʤ롣 */
    {
#ifdef CALL_HANDLER_IN_TASK
      if (do_timer) {
	/* timer ���������Ƥ���ؿ��μ¹� */
	check_timer();
	do_timer = 0;
	chg_pri(KERNEL_TASK, MAX_PRIORITY);
      }
#if 0
      else {
	salvage_task();
      }
#endif
#endif
      task_switch (TRUE);
#ifdef notdef
      ena_int ();	/* Idle ��������¹Ԥ��Ƥ�����ϡ������ߤϥ��͡��֥�
			 * ���Ƥ��ʤ���Ф����ʤ� */
#endif /* notdef */
    }
  falldown ("falldown.");
/* not return */
}


void
run (W entry)
{
  W	i;
  struct boot_header	*info;
  struct module_info	*modulep;
  ID			rid;
  T_CTSK		pktsk;
  T_TCB			*new_taskp;

  info = (struct boot_header *)MODULE_TABLE;
  if ((entry < 1) || (entry >= info->count))
    {
      printk ("module is overflow. (info->count = %d, entry = %d)\n", info->count, entry);
      return;
    }
  modulep = info->modules;
  pktsk.tskatr = TA_HLNG;
  pktsk.itskpri = KERNEL_LEVEL;
  pktsk.stksz = PAGE_SIZE * 10;
  pktsk.addrmap = NULL;
  pktsk.startaddr = (FP)modulep[entry].entry;
  if (new_task (&pktsk, &rid, FALSE) != E_OK)
    {
      printk ("Can not make new task.\n");
      return;
    }
  printk ("Task id = %d, eip = 0x%x\n", rid, modulep[entry].entry);
  new_taskp = get_tskp (rid);
  if (new_taskp == NULL)
    {
      printk ("new task is NULL.\n");
      return;
    }

  /* ���������������β��ۥ���˥⥸�塼���ޥåԥ� */
  /* �������ɥ饤�Фξ��ˤϡ��ޥåԥ󥰤��ʤ� */
  if ((modulep[entry].type == driver) || (modulep[entry].type == lowlib))
    {
      printk ("This module is driver or lowlib. not mapped\n");
    }
  else
    {
      for (i = 0; i < ROUNDUP (modulep[entry].mem_length, PAGE_SIZE) / PAGE_SIZE; i++)
	{
	  if (vmap (new_taskp,
		    modulep[entry].vaddr + i * PAGE_SIZE, 
		    modulep[entry].paddr + i * PAGE_SIZE,
		    ACC_KERNEL) == FALSE)
	    {
	      printk ("Cannot memory map: virtual addr: 0x%x, phisical addr = 0x%x\n",
		      modulep[entry].vaddr + i * PAGE_SIZE, 
		      modulep[entry].paddr + i * PAGE_SIZE);
	    }
	}
    }
  sta_tsk (rid, 0);
  task_switch (TRUE);
}

void
run_init_program (void)
{
  struct boot_header	*info;
  W			i;
#if 0
  struct module_info	*modulep;
  ID			rid;
  T_CTSK		pktsk;
  T_TCB			*new_taskp;
  W n;
#endif

  info = (struct boot_header *)MODULE_TABLE;
  for (i = 1; i < info->count; i++)
    {
      if (info->modules[i].type == lowlib)
	{
	  ER	errno;

	  errno = init_lowlib (&info->modules[i]);
	  if (errno)
	    {
	      printk ("cannot initialize LOWLIB.\n");
	    }
	}
      else
	{
	  run (i);
	}
    }
}



/* init_itron --- ITRON �ν������Ԥ���
 *
 */
static ER
init_itron (void)
{
  init_interrupt ();
  simple_init_console ();	/* ���󥽡����ʸ������ϤǤ���褦�ˤ��� */
  printk ("init_itron: start\n");

  pmem_init ();			/* ʪ�����������ǽ�ν����		*/
  banner ();			/* Ω���夲��å���������		*/

  init_kalloc ();		/* �Х���ñ�̤Υ��������ǽ�ν����	*/
  init_semaphore ();		/* ���ޥե��δ�����ǽ�ν����		*/
  init_msgbuf ();		/* ��å�����������ǽ�ν����		*/
  init_eventflag ();		/* ���٥�ȥե饰������ǽ�ν����	*/
  init_mpl ();			/* ����ס��������ǽ�ν����		*/
#ifdef nodef
  simple_init_console ();	/* ���󥽡����ʸ������ϤǤ���褦�ˤ��� */
#endif
  init_task ();			/* ������������ǽ�ν���� */

  /* 1���ܤΥ��������������롣�����Ƥ��Υ�������ʸ�ν�
   * ���ǻ��Ѥ��롣
   */
  init_task1 ();

  printk ("call init_timer\n"); 
  init_timer ();	/* ���󥿡��Х륿���޵�ǽ�ν���� */
  start_interval ();	/* ���󥿡��Х륿���ޤε�ư	  */
  init_time();		/* ���ִ�����ǽ�ν���� */

/*  init_io (); */
  return (E_OK);
}

static void
init_device (void)
{
  W	i;

  printk ("init_device: start.\n");
  for (i = 0; devices[i] != NULL; i++)
    {
      printk ("Init device: 0x%x call.\n", (*devices[i]));
      (*devices[i]) ();
    }
  printk ("init_device: end.\n");
}


/*
 *
 */
void
banner (void)
{
  printk ("** startup ITRON for BTRON/386 **\n");
  printk ("version %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
  printk ("target CPU ID: %d\n", CPU);
  printk ("base memory = %d Kbytes\n", base_mem / 1024);
  printk ("extend memory = %d Kbytes\n", ext_mem / 1024);
}


#if 0
static void
memory_test (void)
{
  VP	p;

  p = (VP)kalloc (1024);
  printk ("p = 0x%x\n", (UW)p);
  print_memory_list ();
  kfree (p, 1024);
  print_memory_list ();

  p = (VP)kalloc (1024);
  printk ("p = 0x%x\n", (UW)p);
  kfree (p, 1024);
  print_memory_list ();
}
#endif  
