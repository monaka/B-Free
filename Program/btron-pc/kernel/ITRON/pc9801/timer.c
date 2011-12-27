/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* timer.cj
 *
 *
 */

#include "itron.h"
#include "errno.h"
#include "config.h"
#include "pc98.h"
#include "interrupt.h"
#include "task.h"
 
#define TIMER0_WRITE	0x71
#define TIMER0_READ	0x71
#define TIMER_CONTROL	0x77


static struct timer_list
{
  struct timer_list	*next;
  W			time;
  W			(*func)(VP);
  void			*argp;
} timer[MAX_TIMER];

static struct timer_list *free_timer;
static struct timer_list *time_list;

#ifdef TIMER_TEST
static W	counter = 0;
#endif
static void	check_timer (void);
static void	write_vram (W, W, W, W);


/*************************************************************************
 * start_interval --- ���󥿡��Х륿���ޤν����
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	���󥿡��Х륿���ޤ��������롣
 *		1 ms ���Ȥ˳����ߤ�ȯ��������褦�ˤ��롣
 *
 */
void
start_interval (void)
{
  W	i;
  
  set_idt (INT_TIMER, 0x08, (int)int32_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (0);
  
  outb (TIMER_CONTROL, 0x36);
#if 0	/* 10 ms ���Ȥγ����� */
  outb (TIMER0_WRITE, 29491 & 0xff);
  outb (TIMER0_WRITE, 29491 >> 8);
#endif

  outb (TIMER0_WRITE, 2949 & 0xff);
  outb (TIMER0_WRITE, 2949 >> 8);
}

/*************************************************************************
 * intr_interval --- 
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������
 *
 */
void
intr_interval (void)
{
#ifdef TIMER_TEST
  static W	flag = 0;
  
  if (counter-- < 0)
    {
      if (flag)
	write_vram (78, 0, 'o', 0);
      else
	write_vram (78, 0, 'x', 0);
      flag = flag ^ 1;
      counter = 500;
    }
#endif
  check_timer ();
  run_task->total++;
}

/*************************************************************************
 * init_timer
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
init_timer (void)
{
  W	i;
  static struct timer_list banhei = { NULL, 0, 0, NULL };

  printk ("** init_timer **\n");
  dis_int ();
  for (i = 0; i < MAX_TIMER - 2; i++)
    {
      timer[i].next = &timer[i + 1];
    }
  timer[MAX_TIMER - 1].next = NULL;
  free_timer = timer;
  time_list = NULL;
  ena_int ();
}

/*************************************************************************
 * set_timer 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
set_timer (W time, FP func, VP argp)  
{
  struct timer_list	*p, *q, *r;
  W			total;

  if ((func == NULL) || (time <= 0))
    {
      return;
    }
  dis_int ();
  p = free_timer;
  free_timer = free_timer->next;
  if (p == NULL)
    {
      printk ("timer entry empty.\n");
      ena_int ();
      return;
    }
  p->time = time;
  p->func = (W (*)())func;
  p->argp = argp;
  p->next = NULL;

  if (time_list == NULL)
    {
      time_list = p;
      return;
    }

  total = time_list->time;
  if (total > p->time)
    {
      time_list->time -= p->time;
      p->next = time_list;
      time_list = p;
      return;
    }
  
  for (q = time_list, r = q->next; r !=  NULL; q = q->next, r = r->next)
    {
      if ((total + r->time) > p->time)
	break;
      total += q->next->time;
    }
  
  p->time = p->time - total;
  p->next = r;
  q->next = p;
  r->time -= p->time;
  ena_int ();
}  

/*************************************************************************
 * unset_timer --- �����ޡ��Ԥ����󤫤顢�����ǻ��ꤷ�����˹礦����ȥ�
 *		   �������롣
 *
 * ������
 *	func	
 *	arg
 *
 * ���͡�
 *     ���顼�ֹ�
 *
 * ������
 *	�����ޡ��ꥹ�Ȥ�����å������Ԥ����֤Τ���������ȥ꤬����С�
 *	����ȥ�˥��åȤ��줿�ؿ���ƤӽФ���
 *
 *
 */
ER
unset_timer (FP func, VP arg)
{
  struct timer_list	*point, *before;

  before = NULL;
  for (point = time_list; point != NULL; point = point->next)
    {
      if ((point->func == (W (*)())func) && (point->argp == arg))
	{
	  break;
	}
      before = point;
    }
  if (point == NULL)
    {
      return (E_PAR);
    }

  if (point->next)
    {
      point->next->time += point->time;
    }
  before->next = point->next;
  point->next = free_timer;
  free_timer = point;
  return (E_OK);
}

/*************************************************************************
 * check_timer
 *
 * ������
 *	�ʤ�
 *
 * ���͡�
 *	�ʤ�
 *
 * ������
 *	�����ޡ��ꥹ�Ȥ�����å������Ԥ����֤Τ���������ȥ꤬����С�
 *	����ȥ�˥��åȤ��줿�ؿ���ƤӽФ���
 *
 */
static void
check_timer (void)
{
  struct timer_list *p, *q;
  
  dis_int ();
  (time_list->time)--;
  if (time_list == (struct timer_list *)NULL)
    {
      ena_int ();
      return;
    }

  for (p = time_list; (p != NULL) && (p->time <= 0L); p = q)
    {
      (p->func)(p->argp);
      q = p->next;
      p->next = free_timer;
      free_timer = p;
    }
/*  p->time += time_list->time; */
  time_list = p;
  ena_int ();
}

#define TEXT_VRAM_ADDR	0xA0000		/* TEXT VRAM �Υ��ɥ쥹		*/
#define CURSOR_POS(x,y)		(x + y * 80)

static void
write_vram (W x, W y, W ch, W attr)
{
  H	*addr;
  H *attr_addr;
  
  addr = (H *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = ch & 0x00ff;
}
