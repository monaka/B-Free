/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * ʸ�����Ĺ�����֤��ؿ���
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strlen.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/strlen.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";
/* 
 * $Log: strlen.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-02-17 14:29:04  naniwa
 * minor fix on log comment
 *
 * Revision 1.1  2000/01/15 15:26:10  naniwa
 * first version
 *
 */

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
