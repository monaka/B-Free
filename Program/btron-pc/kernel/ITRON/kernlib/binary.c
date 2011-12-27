/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/binary.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/binary.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: binary.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.1  1996/07/22 23:52:04  night
 * �ǽ����Ͽ
 *
 * Revision 1.2  1995/09/21  15:51:39  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/01/14  14:26:18  night
 * �ǽ����Ͽ��
 * bzero(), bcopy() �����������
 *
 *
 */

/*
 * �Х��ʥ�ط������ؿ���
 *
 * �ʲ��δؿ����󶡤��롣
 *
 *	bzero	�ΰ�� 0 ���ꥢ���롣
 *	bcopy	�ΰ�Υ��ԡ�
 *	
 *
 */


/*
 * �إå��ե�������ɤ߹���
 * �إå��ե�����Υѥ��ϡ�Makefile �˻��ꤷ�Ƥ��롣
 */
#include <types.h>


/*
 * ���ꤷ���ΰ�� 0 ���ꥢ���롣
 *
 * ��ա��ΰ�Υ��ɥ쥹�ϥ����å����Ƥ��ʤ��Τǡ�
 *	 ����ե�����Ȥ��������ǽ�������롣
 *
 * ���δؿ����ä��ͤ��֤��ʤ���
 */
void
bzero (B *bufp, W size)
     /*
      * bufp	0 ���ꥢ�����ΰ����Ƭ���ɥ쥹��
      * size	�ΰ���礭����
      */
{
  while (size-- > 0)
    {
      *bufp++ = 0;
    }
}


/*
 * �ΰ�Υ��ԡ�
 *
 */
void
bcopy (UB *buf1, UB *buf2, W size)
{
  W	counter;	/* ���ԡ����Ƥ�����֤򵭲����뤿��Υ����� */

  /*
   * 1�Х��Ȥ��ĥ��ԡ����Ƥ�����
   */
  for (counter = 0; counter < size; counter++)
    {
      buf2[counter] = buf1[counter];
    }
}


