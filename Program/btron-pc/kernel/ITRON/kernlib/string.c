/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/string.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/string.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: string.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-02-07 18:23:58  kishida0
 * add tolower toupper functions
 *
 * Revision 1.3  2000/01/15 15:21:54  naniwa
 * changeg to EUC code
 *
 * Revision 1.2  1999/12/18 15:53:08  kishida0
 * �ؿ��ɲ� strnlen
 *
 * Revision 1.1  1999/04/18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.1  1996/07/22 23:52:05  night
 * �ǽ����Ͽ
 *
 * Revision 1.7  1995/12/05 15:13:16  night
 * strncmp() ���ѹ���
 * �����ǻ��ꤷ��ʸ����κǸ�ޤ���Ӥ�Ԥä��顢̵���� 0 ���֤��褦��
 * ������
 * �����ϡ��Ǹ����Ӥ���ʸ���μ���ʸ������Ӥ�����̤��֤��Ƥ��������Τ�
 * �ᡢ�������ʤ��ͤˤʤ��礬���롣
 *
 * Revision 1.6  1995/10/01  13:07:10  night
 * strcmp() �ν������ѹ���
 * (�롼�פν�λ��郎�ְ�äƤ���)
 *
 * Revision 1.5  1995/09/21  15:51:42  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.4  1995/02/26  14:14:20  night
 * strlen() �ؿ���������ɲá�
 *
 * Revision 1.3  1995/01/14  14:20:57  night
 * strcpy(), strncpy() �����������
 *
 * Revision 1.2  1995/01/14  14:13:51  night
 * �إå��ե����� <types.h> �Υ��󥯥롼�ɤλ�����ɲá�
 *
 * Revision 1.1  1995/01/14  13:59:46  night
 * strcmp(), strncmp() �������
 *
 *
 */

/*
 * ʸ�������ؿ�
 * �����δؿ��ϡ����ճ� (������) �ǻ��Ѥ��롣
 * �̾�Υ��ץꥱ�������Ǥϡ�libc.a ����������ؿ���Ȥ���
 *
 * ���Υե�������������ؿ��ϰʲ��ΤȤ��ꡣ
 *
 *	strcat		ʸ������� (concat) ���롣
 *	strchr		ʸ�������ǻ��ꤷ��ʸ�������äƤ�����֤�ߤĤ��롣
 *	strcmp		ʸ�������Ӥ��롣
 *	strncmp		ʸ�������Ӥ���(����Ĺ�λ���Ĥ�)
 *	strcpy		ʸ����Υ��ԡ�
 *	strncpy		ʸ����Υ��ԡ�(����Ĺ�λ���Ĥ�)
 *	strlen		ʸ�����Ĺ����Х���ñ�̤��֤�
 *      strnlen         ʸ�����Ĺ����Х���ñ�̤��֤�(�������դ�)
 *
 */


/*
 * �إå��ե�������ɤ߹���
 * �إå��ե�����Υѥ��ϡ�Makefile �˻��ꤷ�Ƥ��롣
 */
#include <types.h>


/*
 * 2 �Ĥ�ʸ�������Ӥ��롣
 * ʸ�������������ˤϡ� 0 ���֤���
 * �������ʤ����ˤϡ��ʲ��ε�§�ˤȤ�����ͤ��֤���
 *
 * (s1) > (s2) : 0 �ʲ����ͤ��֤���
 * (s1) < (s2) : 0 �ʾ���ͤ��֤���
 *
 */
W
strcmp (B *s1, B *s2)
{
  while (*s1 && (*s1 == *s2))
    {
      s1++;
      s2++;
    }

  /*
   * �֤��ͤϡ��� 2 ���������� 1 �����κǸ��ʸ�����ͤ�������Ȥˤ��
   * �����롣�⤷��ξ���Ȥ�������ʸ���ʤ�С�0 ���֤롣
   */
  return (*s2 - *s1);
}


/*
 * 2 �Ĥ�ʸ�������Ӥ��롣
 * ʸ�������������ˤϡ� 0 ���֤���
 * �������ʤ����ˤϡ��ʲ��ε�§�ˤȤ�����ͤ��֤���
 *
 * (s1) > (s2) : 0 �ʲ����ͤ��֤���
 * (s1) < (s2) : 0 �ʾ���ͤ��֤���
 *
 * ���δؿ��Ǥ�ʸ����κ���Ĺ����ꤹ�롣
 */
W
strncmp (B *s1, B *s2, W length)
{
  W	counter;

  /*
   * ʸ������ʸ��������Ӥ��Ƥ�����
   * ʸ����κ���Ĺ�ޤ���Ӥ��뤫���������ʤ�ʸ�����Ф������ǥ롼�פ�
   * ��λ���롣 
   */
  for (counter = 0;
       (*s1 == *s2) && (counter < length) && (*s1 != '\0');
       counter++)
    {
      s1++;
      s2++;
    }

  if (counter >= length)
    return (0);

  /*
   * �֤��ͤϡ��� 2 ���������� 1 �����κǸ��ʸ�����ͤ�������Ȥˤ��
   * �����롣�⤷��ξ���Ȥ�������ʸ���ʤ�С�0 ���֤롣
   */
  return (*s2 - *s1);
}


/*
 * ʸ����Υ��ԡ���
 *
 * ��2����������1������ʸ����򥳥ԡ����롣
 * ʸ����ν���ϡ�ʸ�����ͤ� 0 ���ɤ�����Ƚ�̤��롣
 */
void
strcpy (B *s1, B *s2)
{
  while (*s2 != '\0')
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
}


/*
 * ʸ����Υ��ԡ���
 *
 * ��2����������1������ʸ����򥳥ԡ����롣
 * ʸ����ν���ϡ�ʸ�����ͤ� 0 ���ɤ�����Ƚ�̤��롣
 *
 * ���δؿ��ϡ����ԡ�����ʸ����κ���Ĺ����ꤹ�롣�⤷�����ꤷ������
 * Ĺ����Ĺ��ʸ����򥳥ԡ����褦�Ȥ������ˤϡ�����Ĺ - 1 �ǥ��ԡ�
 * ����ߤ��롣
 * ����Ĺ�ϡ��Х���ñ�̤ǻ��ꤹ�롣
 *
 */
void
strncpy (B *s1, B *s2, W size)
{
  W	counter;		/* ���ԡ�����Х��ȿ��򥫥���Ȥ��뤿 */
				/* ����ѿ�                           */

  for (counter = 0; (counter < (size - 1)) && (*s2); counter++)
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
}


/*
 * ʸ�����Ĺ����Х���ñ�̤��֤��ؿ���
 *
 */
int
strlen (char *s)
{
  int	count;

  for (count = 0; s[count]; count++)
    ;
  return (count);
}
/*
 * ʸ�����Ĺ����Х���ñ�̤��֤��ؿ�(�������դ�)
 *
 */
int strnlen(char * s, int count)
{
  char *sc;

  for (sc = s; count-- && *sc != '\0'; ++sc)
    ;
  return sc - s;
}

char
tolower (char c)
{
  if(c>='A'){
    if(c<='Z'){
      return c-('A'-'a');
    }
  }
  return c;
}
char
toupper(char c)
{
  if(c>='a'){
    if(c<='z'){
      return c-('a'-'A');
    } 
  }
  return c;
}


