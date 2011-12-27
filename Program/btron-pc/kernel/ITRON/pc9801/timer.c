/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
  W	i;
  
  set_idt (INT_TIMER, 0x08, (int)int32_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (0);
  
  outb (TIMER_CONTROL, 0x36);
#if 0	/* 10 ms ごとの割り込み */
  outb (TIMER0_WRITE, 29491 & 0xff);
  outb (TIMER0_WRITE, 29491 >> 8);
#endif

  outb (TIMER0_WRITE, 2949 & 0xff);
  outb (TIMER0_WRITE, 2949 >> 8);
}

/*************************************************************************
 * intr_interval --- 
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：
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
 * 引数：
 *
 * 返値：
 *
 * 処理：
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

#define TEXT_VRAM_ADDR	0xA0000		/* TEXT VRAM のアドレス		*/
#define CURSOR_POS(x,y)		(x + y * 80)

static void
write_vram (W x, W y, W ch, W attr)
{
  H	*addr;
  H *attr_addr;
  
  addr = (H *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = ch & 0x00ff;
}
