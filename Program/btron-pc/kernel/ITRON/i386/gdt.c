/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
** gdt.c --- GDT ���������롼����
**
*/



#include "itron.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"



/* set_gdt --- ���ꤷ�� GDT �Υ���ȥ���ͤ򥻥åȤ��롣
 *
 * ������
 *	int	index	�������ͤ򥻥åȤ���GDT�Υ���ȥ��ؤ�����ȥ�
 *	VP	desc	�ǥ�������ץ��ؤΥݥ���
 *			(�ǥ�������ץ����Τϡ�8�Х��Ȥ��礭������)
 *
 * �֤��͡�
 *	���顼�ֹ�
 *		E_OK	�����	
 *		E_PAR	index���ͤ���������(GDT���ϰϤ򥪡��С����Ƥ���)
 *
 */
ER
set_gdt (int index, GEN_DESC *desc)
{
  GEN_DESC	*p;

  if ((index <= 0) || (index > MAX_GDT))
    {
      return (E_PAR);	/* �ѥ�᡼������������ */
    }
  p = (GEN_DESC *)GDT_ADDR;
  bcopy (desc, &p[index], sizeof (GEN_DESC));
  return (E_OK);
}


/* get_gdt --- ���ꤷ�� GDT �Υ���ȥ�Υǥ�������ץ�����Ф���
 *
 * ������
 *	int	index	�ͤ���Ф���GDT�Υ���ȥ��ؤ�����ȥ�
 *
 * �֤��͡�
 *	�����ǻ��ꤵ�줿GDT�Υ���ȥ�ؤΥݥ��󥿡�
 *
 */
GEN_DESC *
get_gdt (int index)
{
  GEN_DESC	*p;

#if 0
  printk ("get_gdt = %d\n", index);	/* */
#endif
  if ((index <= 0) || (index > MAX_GDT))
    {
      return (NULL);	/* �ѥ�᡼������������ */
    }

  p = (GEN_DESC *)GDT_ADDR;
  return (&p[index + TSS_BASE]);
}





