/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * ʸ�������Ӵؿ���
 * (��ʸ����1�Х��ȤȲ��ꤷ�Ƥ�����)
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/string/strcmp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Header";


/* $Log: strcmp.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:52  night
/* �������ե��������Ƭ�� Copyright notice ������ɲá�
/*
 * Revision 1.1  1995/03/18  14:21:12  night
 * �ǽ����Ͽ
 *
 *
 */

#include <types.h>
#include <string.h>


/*
 * ʸ�������� 
 *
 * ���� s1 �� s2 ����Ӥ��롣
 * �ɤ��餫��ʸ��������Ƥ��ۤʤ�ޤ���Ӥ��롣
 *
 * �֤��͡�
 *	0	ʸ��������Ƥ�������
 *	>0	s1 ��ʸ����������礭��
 *	<0	s2 ��ʸ����������礭��
 *
 */
int
strcmp (char *s1, char *s2)
{
  while (*s1 == *s2)
    {
      s1++;
      s2++;
    }

  return (*s1 - *s2);
}

/*
 * ʸ�������� 
 *
 * ���� s1 �� s2 ����Ӥ��롣
 * �ɤ��餫��ʸ��������Ƥ��ۤʤ뤢�뤤�ϰ��� n �ǻ��ꤷ��
 * Ĺ���ޤ�����������Ӥ��롣
 *
 * �֤��͡�
 *	0	ʸ��������Ƥ�������
 *		���뤤�ϡ����ꤷ��Ĺ���ޤ���Ӥ�����
 *	>0	s1 ��ʸ����������礭��
 *	<0	s2 ��ʸ����������礭��
 *
 */
int
strncmp (char *s1, char *s2, int n)
{
  int	i;

  for (i = 0; (*s1 == *s2) && (i < n); i++)
    {
      s1++;
      s2++;
    }

  if (i == n)
    return (0);

  return (*s1 - *s2);
}



