/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/rs232c/rs232c_low.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: rs232c_low.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1998-12-07 13:54:22  night
 * �ե��������ʸ�������ɤ� SJIS ���� EUC-JP ���ѹ���
 *
 * Revision 1.1  1998/12/07 13:07:30  night
 * first version.
 *
 *
 *
 *
 */

#include "rs232c.h"
#include "rs232c_internal.h"


/* ======================================================================== *
 *
 *	�ե������� static �ѿ������
 *
 * ======================================================================== */
static UW i8250_base_address[4] = 
{ 
  COM0_BASE,
  COM1_BASE,
  COM2_BASE,
  COM3_BASE
};



static void
i8250_outw (W port, W offset, UH value)
{
  outw (i8250_base_address[port] + offset, value);
};

static void
i8250_outb (W port, W offset, UB value)
{
  outb (i8250_base_address[port] + offset, value);
};

static void
i8250_inb (W port, W offset, UB *value)
{
  *value = inb (i8250_base_address[port] + offset);
};


/* ======================================================================== *
 *
 *	�Ƽ�쥸����������/����
 *
 * ======================================================================== */

/* DLAB ������
 *
 */
ER
i8250_set_dlab (W port, W flag)
{
  UB	old_value;

  i8250_inb (port, UART_LCR, &old_value);
  if (flag == 1)
    {
      old_value |= 0x80;
    }
  else
    {
      old_value &= 0x7f;
    }

  i8250_outb (port, UART_LCR, old_value);

  return (E_OK);
}


/* ʬ���쥸����������
 *
 */
ER
i8250_set_frequency_register (W port, W baud)
{
  UB	line_status;
  static W baud_table[] = {
    0x0900,
    0x0600,
    0x0300,
    0x0180,
    0x0150,
    0x00c0,
    0x0030,
    0x0018,
    0x000c,
    0x0006,
    0x0003,
    0x0002,
    0x0001,
  };
  
  /* DLAB �� 1 ������ */
  i8250_set_dlab (port, 1);

  /* �̿�®�٤����� */
  i8250_outb (port, UART_FRE,     baud_table[baud] & 0xff);
  i8250_outb (port, UART_FRE + 1, (baud_table[baud] >> 8) & 0xff);

  /* DLAB �� 0 ������ */
  i8250_set_dlab (port, 0);
  
  return (E_OK);
}



/* �����ߵ��ĥ쥸����������
 *
 */
ER
i8250_set_interrupt_register (W port, W flag)
{
  i8250_outb (port, UART_IER, 0x0f);
  return (E_OK);
}


/* ������ǧ���쥸�������������
 *
 * �����߻��˻��Ѥ����ɤΤ褦�ʸ����ǳ����ߤ�ȯ����������
 * ������
 */
ER
i8250_get_interrupt_status (W port, UB *status)
{
  i8250_inb (port, UART_IIR, status);
  return (E_OK);
}


/* FIFO ����쥸����������
 *
 *
 */
ER
i8250_set_fifo (W port, W byte)
{
  return (E_NOSPT);
}


/* �饤������쥸����������
 *
 */
ER
i8250_set_line_control (W port, W value)
{
  i8250_outb (port, UART_LCR, value);
  return (E_OK);
}


/* ��ǥ�����쥸����������
 *
 */
ER
i8250_set_modem_control (W port, W value)
{
  i8250_outb (port, UART_MCR, value);

  return (E_OK);
}


/* �饤�󥹥ơ������쥸��������ξ������
 *
 */
ER
i8250_get_line_status (W port, UB *value)
{
  i8250_inb (port, UART_LSR, value);

  return (E_OK);
}


/* ��ǥॹ�ơ������쥸��������ξ������
 *
 */
ER
i8250_get_modem_status (W port, UB *value)
{
  i8250_inb (port, UART_MSR, value);

  return (E_OK);
}


