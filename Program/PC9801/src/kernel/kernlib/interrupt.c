/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* �����ߴط��δؿ�
 */

static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/interrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include <h/types.h>
#include <h/errno.h>
#include <pc9801/pc98.h>
#include "dma.h"


/*
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

