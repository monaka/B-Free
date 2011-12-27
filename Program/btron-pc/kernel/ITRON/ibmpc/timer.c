/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* timer.c
 *
 *
 */

#include "itron.h"
#include "errno.h"
#include "config.h"
#include "ibmpc.h"
#include "interrupt.h"
#include "task.h"
#include "func.h"
 
#define TIMER0_WRITE	0x40
#define TIMER0_READ	0x40
#define TIMER_CONTROL	0x43

/* 大域変数の宣言 */
W do_timer = 0;

static struct timer_list
{
  struct timer_list	*next;
  W			time;
  void			(*func)(VP);
  VP			argp;
} timer[MAX_TIMER];

static struct timer_list *free_timer;
static struct timer_list *time_list;
static unsigned long free_run;  /* 今の所初期化する予定なし */


#ifdef TIMER_TEST
static W	counter = 0;
#endif
static void	write_vram (W, W, W, W);


/*************************************************************************
 * start_interval --- インターバルタイマの初期化
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	インターバルタイマを初期化する。
 *		1 ms ごとに割り込みを発生させるようにする。
 *
 */
void
start_interval (void)
{
#if 0
  W	i;
#endif
  
  printk ("start interval\n");
  /* timer
   */
  set_idt (32, 0x08, (int)int32_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (0);

  dis_int ();
#ifdef notdef
  outb (TIMER_CONTROL, 0x34);
  outb (TIMER0_WRITE, 11930 & 0xff);
  outb (TIMER0_WRITE, (11930 >> 8) & 0xff);
#else
  outb (TIMER_CONTROL, 0x36); /* MODE 3 */
  outb (TIMER0_WRITE, 11932 & 0xff);
  outb (TIMER0_WRITE, (11932 >> 8) & 0xff); /* 約 10.0002 ms 周期 */
#endif
  ena_int ();

  write_vram (78, 0, 'x', 0);
}

#ifdef notdef
outb (W addr, unsigned char data)
{
  if ((addr == 0x40) || (addr == 0x43))
    {
      printk ("Warning!! outb (0x%x) output!! (data is %d)\n", addr, data);
      return;
    }
  outb2 (addr, data);
}
#endif
     

/*************************************************************************
 * intr_interval --- 
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	インターバルタイマの割り込み処理を行う。
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
#if 1
      counter = 100;
#else
      counter = 30;
#endif
    }
#endif
#ifdef CALL_HANDLER_IN_TASK
  if (time_list != NULL) {
    (time_list->time)--;
    if ((time_list->time <= 0) && (do_timer == 0)) {
      /* KERNEL_TASK で timer に設定されている関数を実行 */
      do_timer = 1;
#if 1
      chg_pri(KERNEL_TASK, MIN_PRIORITY);
#else
      chg_pri(KERNEL_TASK, KERNEL_LEVEL);
#endif
      task_switch(TRUE);
    }
  }
#else
  check_timer ();
#endif
  run_task->total++;
  free_run++;
}

/*************************************************************************
 * init_timer
 *
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
init_timer (void)
{
  W	i;
#if 0
  static struct timer_list banhei = { NULL, 0, 0, NULL };
#endif

  printk ("** init_timer **\n");
  dis_int ();
  for (i = 0; i <= MAX_TIMER - 2; i++)
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
 * 引数：
 *
 * 返値：
 *
 * 処理：
 *
 */
void
set_timer (W time, void  (*func)(VP), VP argp)  
{
  struct timer_list	*p, *q, *r;
  W			total;

  if ((func == NULL) || (time <= 0))
    {
      return;
    }
  dis_int ();
  p = free_timer;
  if (p == NULL)
    {
      printk ("timer entry empty.\n");
      ena_int ();
      return;
    }
  free_timer = free_timer->next;
  ena_int ();
  p->time = time;
  p->func = (void (*)(VP))func;
  p->argp = argp;
  p->next = NULL;

  dis_int ();
  if (time_list == NULL)
    {
      time_list = p;
      ena_int ();
      return;
    }

  total = time_list->time;
  if (total > p->time)
    {
      time_list->time -= p->time;
      p->next = time_list;
      time_list = p;
      ena_int ();
      return;
    }
  
  for (q = time_list, r = q->next; r !=  NULL; q = q->next, r = r->next)
    {
      if ((total + r->time) > p->time)
	break;
      total += r->time;
    }

  p->time = p->time - total;
  p->next = r;
  q->next = p;
  if (r != NULL) r->time -= p->time;
  ena_int ();
}  

/*************************************************************************
 * unset_timer --- タイマー待ち行列から、引数で指定した条件に合うエントリ
 *		   を削除する。
 *
 * 引数：
 *	func	
 *	arg
 *
 * 返値：
 *     エラー番号
 *
 * 処理：
 *	タイマーリストをチェックし、待ち時間のすぎたエントリがあれば、
 *	エントリにセットされた関数を呼び出す。
 *
 *
 */
ER
unset_timer (void (*func)(VP), VP arg)
{
  struct timer_list	*point, *before;

  dis_int ();
  before = NULL;
  for (point = time_list; point != NULL; point = point->next)
    {
      if ((point->func == func) && (point->argp == arg))
	{
	  break;
	}
      before = point;
    }
  if (point == NULL)
    {
      ena_int ();
      return (E_PAR);
    }

  if (point->next)
    {
      point->next->time += point->time;
    }
  if (before == NULL) {
    time_list = point->next;
  }
  else {
    before->next = point->next;
  }
  point->next = free_timer;
  free_timer = point;
  ena_int ();
  return (E_OK);
}

/*************************************************************************
 * check_timer
 *
 * 引数：
 *	なし
 *
 * 返値：
 *	なし
 *
 * 処理：
 *	タイマーリストをチェックし、待ち時間のすぎたエントリがあれば、
 *	エントリにセットされた関数を呼び出す。
 *
 */
void check_timer (void)
{
  struct timer_list *p, *q;

#ifdef notdef
  dis_int ();
#endif
  if (time_list == NULL)
    {
#ifdef notdef
      ena_int ();
#endif
      return;
    }

#ifndef CALL_HANDLER_IN_TASK
  (time_list->time)--;
#endif

  for (p = time_list; (p != NULL) && (p->time <= 0L); p = q)
    {
      (p->func)(p->argp);
      q = p->next;
      if (q != NULL) q->time += time_list->time;
      p->next = free_timer;
      dis_int();
      free_timer = p;
      time_list = q;
      ena_int();
    }
}

#define TEXT_VRAM_ADDR	0x800B8000		/* TEXT VRAM のアドレス		*/
#define CURSOR_POS(x,y)		(x + y * 80)

static void
write_vram (W x, W y, W ch, W attr)
{
  H	*addr;
#if 0
  H *attr_addr;
#endif
  
  addr = (H *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
}

/* left_time()
 */

W left_time (void (*func)(VP), VP arg)
{
  struct timer_list	*point, *before;
  W time;

  time = 0;
  for (point = time_list; point != NULL; point = point->next) {
    time += point->time;
    if ((point->func == func) && (point->argp == arg)) {
      break;
    }
  }
  return (time);
}

UW get_free_run ( void )
{
  return free_run;
}
