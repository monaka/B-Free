/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * �ɥ饤�С����Ǥ���Ѥ����硢�ƥ�ݥ���ѿ���Ȥä��ޥ���Ǻ��������
 * ����ˤʤ�Τǡ��ؿ��ˤ��������
 *
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/string/ctype.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: ctype.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  2000-02-17 14:29:00  naniwa
 * minor fix on log comment
 *
 * Revision 1.1  2000/02/07 18:14:20  kishida0
 * �ޥ���Ǥ�����ˤʤꤽ���� tolower toupper �ʤɤ��������
 *
 *
 *
 */

/*
 *
 */
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

