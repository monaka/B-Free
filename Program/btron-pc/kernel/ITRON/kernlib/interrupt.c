/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* 割り込み関係の関数
 */

static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include <h/types.h>
#include <h/errno.h>
#include <ibmpc/ibmpc.h>
/* #include "dma.h" */

/*
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
    outb (MASTER_8259A_DATA, inb (MASTER_8259A_DATA) & ~(1 << intn));
  else
    outb (SLAVE_8259A_DATA, inb (SLAVE_8259A_DATA) & ~(1 << (intn - 8)));
  ena_int ();
}

