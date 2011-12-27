/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/wd33c93/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1995-12-05 15:18:03  night
 * �ǽ����Ͽ��
 * ���ΥС������Ǥϡ��ǥХ��������褿���顼�Υ����å���ԤäƤ��ʤ��Τǡ�
 * ��դ��뤳�ȡ�
 *
 *
 */

#include "scsi.h"


print_buf (UB *buf, W length)
{
  W	i, j;

  for (i = 0; i < length; i += 16)
    {
      dbg_printf ("0x%x: ", i);
      for (j = 0; j < 16; j++)
	{
	  dbg_printf ("%x ", buf[i * 16 + j]);
	}
      for (j = 0; j < 16; j++)
	{
	  if ((buf[i * 16 + j] >= 'a') && (buf[i * 16 + j] >= 'Z'))
	    putchar (buf[i * 16 + j]);
	}
      putchar ('\n');
    }
}
