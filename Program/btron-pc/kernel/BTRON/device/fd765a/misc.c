/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/fd765a/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$header$";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-07-28 19:59:33  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 * Revision 1.3  1995/09/21  15:50:58  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/06  16:12:28  night
 * ��֥С������
 * start() ����ǵ�ư��å�������Ф��Ȥ���ޤǺ�����
 *
 * Revision 1.1  1995/03/18  14:09:03  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * ��¿�ʴؿ����������ե�����
 *
 */

void
busywait(int x)
{
  int i,j;
  int	tmp;

  for (i = 0; i < x; i++)
     for (j = 0; j < 100; j++)
       tmp = j;
}


