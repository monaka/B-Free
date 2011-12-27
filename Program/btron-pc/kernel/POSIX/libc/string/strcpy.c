/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * ʸ����Υ��ԡ��ؿ���
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strcpy.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strcpy.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: strcpy.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-02-17 14:29:03  naniwa
 * minor fix on log comment
 *
 * Revision 1.2  2000/01/15 15:26:32  naniwa
 * minor fix
 *
 * Revision 1.1  1996/11/11 13:33:42  night
 * �ǽ����Ͽ
 *
 * Revision 1.2  1995/09/21  15:52:53  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/03/18  14:21:13  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * ʸ����Υ��ԡ�
 *
 */
char *
strcpy (char *s1, char *s2)
     /* s1	���ԡ���ΥХåե���ؤ��ݥ��� */
     /* s2	���ԡ����ΥХåե���ؤ��ݥ��� */
{
  char *s;
  s = s1;
  while (*s2 != '\0')
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
  return (s);
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
char *
strncpy (char *s1, char *s2, int size)
{
  int	counter;		/* ���ԡ�����Х��ȿ��򥫥���Ȥ��뤿 */
				/* ����ѿ�                           */
  char *s;

  s = s1;
  for (counter = 0; (counter < (size - 1)) && (*s2); counter++)
    {
      *s1++ = *s2++;
    }
  *s1 = '\0';
  return(s);
}
