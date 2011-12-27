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

#include "itron.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"
#include "misc.h"
#include "memory.h"
#include "../io/io.h"
#include "../../../boot/2nd/boot.h"
#include "../../../boot/2nd/config.h"
#include "../../kernlib/device.h"

static ER	init_itron ();
static void	banner ();
static void	init_device ();
static void 	memory_test ();

/*******************************************************************
 * itron --- �ᥤ��ؿ�
 *
 */
ER
itron (void)
{
  T_CTSK	par_debug_task;

  if (init_itron () != E_OK)
    {
      falldown ("main: cannot initialize.\n");
    }

  init_device ();

  par_debug_task.exinf = 0;
  par_debug_task.startaddr = debugger;
  par_debug_task.itskpri = 10;
  par_debug_task.stksz = PAGE_SIZE * 2;
  par_debug_task.addrmap = NULL;
  if (cre_tsk (ITRON_DEBUG, &par_debug_task) != E_OK)
    {
      printf ("cannot create task for debugger.\n");
    }
  if (sta_tsk (ITRON_DEBUG, NULL) != E_OK)
    {
      printf ("cannot start task for debugger.\n");
    }

#ifdef AUTO_START
  run_init_program ();
#endif

  for (;;)		/* Idle �������Ȥʤ롣 */
    {
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
      printf ("module is overflow.\n");
      return;
    }
  modulep = info->modules;
  pktsk.tskatr = TA_HLNG;
  pktsk.itskpri = 2;
  pktsk.stksz = PAGE_SIZE * 2;
  pktsk.addrmap = NULL;
  pktsk.startaddr = (FP)modulep[entry].entry;
  if (new_task (&pktsk, &rid, FALSE) != E_OK)
    {
      printf ("Can not make new task.\n");
      return;
    }
  printf ("Task id = %d, eip = 0x%x\n", rid, modulep[entry].entry);
  new_taskp = get_tskp (rid);
  if (new_taskp == NULL)
    {
      printf ("new task is NULL.\n");
      return;
    }

  /* ���������������β��ۥ���˥⥸�塼���ޥåԥ� */
  /* �������ɥ饤�Фξ��ˤϡ��ޥåԥ󥰤��ʤ� */
  if (modulep[entry].type == driver)
    {
      printf ("This module is driver. not mapped\n");
    }
  else
    {
      for (i = 0; i < ROUNDUP (modulep[entry].length, PAGE_SIZE) / PAGE_SIZE; i++)
	{
	  if (vmap (new_taskp,
		    modulep[entry].vaddr + i * PAGE_SIZE, 
		    modulep[entry].paddr + i * PAGE_SIZE) == FALSE)
	    {
	      printf ("Cannot memory map: virtual addr: 0x%x, phisical addr = 0x%x\n",
		      modulep[entry].vaddr + i * PAGE_SIZE, 
		      modulep[entry].paddr + i * PAGE_SIZE);
	    }
	}
    }
  sta_tsk (rid, 0);
  task_switch (TRUE);
}

run_init_program ()
{
  struct boot_header	*info;
  struct module_info	*modulep;
  ID			rid;
  T_CTSK		pktsk;
  T_TCB			*new_taskp;
  W			i, n;

  info = (struct boot_header *)MODULE_TABLE;
  for (i = 0; i < info->count; i++)
    {
      run (i);
    }
}


scsi ()
{
  int	i;
  unsigned char	*flag;
  
  flag = (unsigned char*)0x80000482;
  printf ("SCSI ID BITS: 0x%x\n", *flag);
}


/* init_itron --- ITRON �ν������Ԥ���
 *
 */
static ER
init_itron ()
{
  init_interrupt ();
  simple_init_console ();	/* ���󥽡����ʸ������ϤǤ���褦�ˤ��� */
  pmem_init ();			/* ʪ�����������ǽ�ν����		*/
  banner ();			/* Ω���夲��å���������		*/
  printf ("init_itron: start\n");
  init_kalloc ();		/* �Х���ñ�̤Υ��������ǽ�ν����	*/
  init_semaphore ();		/* ���ޥե��δ�����ǽ�ν����		*/
  init_msgbuf ();		/* ��å�����������ǽ�ν����		*/
  init_eventflag ();		/* ���٥�ȥե饰������ǽ�ν����	*/
#ifdef notdef
  init_mpl ();			/* ����ס��������ǽ�ν����		*/
  simple_init_console ();	/* ���󥽡����ʸ������ϤǤ���褦�ˤ��� */
#endif
  init_task ();			/* ������������ǽ�ν���� */

  /* 1���ܤΥ��������������롣�����Ƥ��Υ�������ʸ�ν�
   * ���ǻ��Ѥ��롣
   */
  init_task1 ();

  printf ("call init_timer\n"); 
  init_timer ();	/* ���󥿡��Х륿���޵�ǽ�ν���� */
  start_interval ();	/* ���󥿡��Х륿���ޤε�ư	  */
  init_io ();
  return (E_OK);
}

static void
init_device ()
{
  W	i;

  for (i = 0; devices[i] != NULL; i++)
    {
      (*devices[i]) ();
    }
  printf ("initdevice: end.\n");
}


/*
 *
 */
static void
banner (void)
{
  printf ("** startup ITRON for BTRON/386 **\n");
  printf ("version %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
  printf ("target CPU ID: %d\n", CPU);
  printf ("base memory = %d Kbytes\n", base_mem / 1024);
  printf ("extend memory = %d Kbytes\n", ext_mem / 1024);
}


static void
memory_test (void)
{
  VP	p;

  p = (VP)kalloc (1024);
  printf ("p = 0x%x\n", (UW)p);
  print_memory_list ();
  kfree (p, 1024);
  print_memory_list ();

  p = (VP)kalloc (1024);
  printf ("p = 0x%x\n", (UW)p);
  kfree (p, 1024);
  print_memory_list ();
}
  
