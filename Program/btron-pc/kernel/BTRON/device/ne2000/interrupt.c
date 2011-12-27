/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: interrupt.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-03-17 10:48:29  night
 * dbg_printf for debugging was made effective only when the DEBUG macro
 * was on.
 *
 * Revision 1.2  1999/03/07 12:45:04  night
 * set_interrupt_handler () ���ѹ� dev ���ɲá�
 * ne2000_interrupt_handler () ����Ȥ��ɲá�
 *
 * Revision 1.1  1999/03/02 15:12:09  night
 * first version.
 *
 *
 *
 */

/*
 *
 *
 */

#include "ne2000.h"
#include "ne2000_internal.h"


/* ===================================================================== *
 *                                                                       *
 *                �����ߥϥ�ɥ�ط��ν���                             *
 *                                                                       *
 * ===================================================================== */

static struct ne2000_device	*device;

/* �����ߥϥ�ɥ������ 
 */
ER
set_interrupt_handler (struct ne2000_device *dev, W intno, FP handler)
{
  T_DINT	pk_dint;
  ER		errno;

  pk_dint.intatr = ATR_INTR;
  pk_dint.inthdr = handler;
  errno = def_int (intno, &pk_dint);
  if (errno != E_OK)
    {
      return (E_DEV);
    }

  device = dev;
  return (E_OK);
}



/* �����ߥϥ�ɥ�
 */
void
ne2000_interrupt_handler (void)
{
  UB	reason;

  dbg_printf ("NE2000: catch interrupt\n");

  select_page0 (device);
  reason = inb (IO_ISR (device));
  
  if (reason == 0)
    {
      return;
    }

#ifdef DEBUG
  dbg_printf ("NE2000: interrupt status = 0x%x\n", reason);
#endif /* DEBUG */

  outb (IO_ISR (device), reason);

  if ((reason & EPISR_TX) || (reason & EPISR_TX_ERR))
    {
      device->collision = inb (IO_BASE (device) + EP0_NCR);

      if (reason & EPISR_TX)
	{
	  /* ���������� */
	  device->tx_count++;
	}
      else if (reason & EPISR_TX_ERR)
	{
	  /* �������顼������ */
	  device->tx_error++;
	}

      device->tx_busy = 0;
      device->tx_use--;
    }


  if ((reason & EPISR_RX) || (reason & EPISR_RX_ERR) || (reason & EPISR_OVER))
    {
      if (reason & EPISR_RX)
	{
	  /* �������� */
	}
      else if (reason & EPISR_RX_ERR)
	{
	}
      else if (reason & EPISR_OVER)
	{
	  select_page0 (device);
	  init_device (device);
	}
    }


  select_page0 (device);
  if (reason & EPISR_COUNTERS)
    {
      UB tmp;

      tmp = inb (IO_BASE (device) + EP0_COUNTER0);
      tmp = inb (IO_BASE (device) + EP0_COUNTER1);
      tmp = inb (IO_BASE (device) + EP0_COUNTER2);
    }

}

