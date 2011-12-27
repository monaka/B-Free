/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/eventflag.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/eventflag.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: eventflag.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-10 10:45:02  naniwa
 * minor fix
 *
 * Revision 1.2  1999/12/18 15:55:02  kishida0
 * �ޥ��ʡ��ʽ���
 *
 * Revision 1.1  1999/04/18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.1  1996/07/22 23:52:05  night
 * �ǽ����Ͽ
 *
 * Revision 1.2  1995/09/21  15:51:41  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/09/19  18:07:38  night
 * �ǽ����Ͽ
 *
 *
 */

#include <types.h>
#include <itron.h>
#include <config.h>
#include <errno.h>
#include "port-manager.h"


/*
 * Ŭ���� ID ���ĥ�å������Хåե���������롣
 */
ID
get_flag (ATR atr, UINT pattern)
{
  ID		flag;
  W		errno;
  T_CFLG	t_cflg;

  /*
   */
  t_cflg.flgatr = atr;
  t_cflg.iflgptn = pattern;
  for (flag = MIN_EVENTFLAG;
       flag <= MAX_EVENTFLAG;
       flag++)
    {
      errno = cre_flg (flag, &t_cflg);
      if (errno == E_OK)
	{
	  return (flag);
	}
    }
  return (0);
}
