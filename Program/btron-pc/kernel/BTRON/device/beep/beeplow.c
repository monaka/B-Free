/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/beep/beeplow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/beep/beeplow.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 *
 *
 */

#include "beep.h"
#include "beep_internal.h"

UW	initilized = 0;


/* BEEP �����ץ���¸�ߤ��Ƥ��뤫�ɤ���������å�����
 */
ER
probe (struct device *dev)
{
  /*** ������ ***/

  return (E_OK);
}


/* 
 * $Log: beeplow.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-02-17 04:23:56  monaka
 * First version.
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */
