/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * ʸ�����Ϣ��ؿ���
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/string/strcat.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "@(#)$Header";


/* $Log: strcat.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.3  1995-09-21 15:52:52  night
/* �������ե��������Ƭ�� Copyright notice ������ɲá�
/*
 * Revision 1.2  1995/07/06  15:05:20  night
 * strncat �δؿ�̾�� strcat �ˤʤäƤ����Τǡ�������ľ������
 *
 * Revision 1.1  1995/03/18  14:21:12  night
 * �ǽ����Ͽ
 *
 *
 */

#include <types.h>
#include <string.h>

/*
 * 2 �Ĥ�ʸ�����Ϣ�뤹�뤿��δؿ�
 *
 */
char *
strcat (char *s1, char *s2)
{
  char *buf;
  char *bufp;	/* �Хåե�����Ƭ�򤪤ܤ��Ƥ�������Υݥ��� */

  buf = malloc (strlne (s1) + strlen (s2) + 1);
  if (buf == NULL)
    {
      return (NULL);	/* ���Ԥ��� */
    }

  bufp = buf;
  while (*s1)
    {
      *buf++ = *s1++;
    }

  while (*s2)
    {
      *buf++ = *s2++;
    }
  *buf = '\0';
  return (bufp);
}

/*
 * 2 �Ĥ�ʸ�����Ϣ�뤹�뤿��δؿ�
 *
 *
 */
char *
strncat (char *s1, char *s2, int n)
{
  int	i;
  int	length;
  char	*buf;
  char	*bufp;	/* �Хåե�����Ƭ�򤪤ܤ��Ƥ�������Υݥ��� */

  length = strlne (s1) + strlen (s2) + 1;
  if (length > n)
    length = n - 1;
  
  buf = malloc (length);
  if (buf == NULL)
    {
      return (NULL);	/* ���Ԥ��� */
    }

  bufp = buf;
  for (i = 0; (*s1) && (i < length); i++)
    {
      *buf++ = *s1++;
    }

  for (; (*s2) && (i < length); i++)
    {
      *buf++ = *s2++;
    }
  *buf = '\0';
  return (bufp);
}


