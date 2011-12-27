/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* task2.c
 *
 */

#include "itron.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"
#include "misc.h"

void tsk2 ();


void
task2_timer (VP arg)
{
  INT err;

  err = clr_flg (1, 0);
/*  err = wup_tsk (2); */
  if (err != E_OK)
    {
      printf ("cannot wakeup task 2. error = %d\n", err);
    }
  set_timer (10000, task2_timer, (VP)2);
}

/*******************************************************
 *
 */
void
tsk2_func (void)
{
  INT		i, j;
  ID		id, sem;
  T_CTSK	par_tsk2;
  ER		err;
  T_CFLG	pk_flg;

  par_tsk2.exinf = 0;
  par_tsk2.startaddr = tsk2;
  par_tsk2.itskpri = 3;
  par_tsk2.stksz = PAGE_SIZE;
  par_tsk2.addrmap = NULL;
  err = cre_tsk (2, &par_tsk2);
  printf ("call cre_tsk: error = %d\n", err);
  err = sta_tsk (2, NULL);		/* テスト用タスクの起動 */
  printf ("call sta_tsk: error = %d\n", err);
  pk_flg.iflgptn = 1;
  pk_flg.flgatr = TA_WMUL;
  err = cre_flg (1, &pk_flg);
  printf ("call cre_flg: error = %d\n", err);
  set_timer (10000, task2_timer, (VP)2);
}

void
tsk2 (void)
{
  INT	id, i;
  INT	size;
  ER	err;
  UINT	rflg;

  printf ("task2: start.\n");
  for (;;)
    {
      err = wai_flg (&rflg, 1, 0, TWF_ANDW);
      printf ("task2: wakeup\n");
      set_flg (1, 1); 
    }
}

