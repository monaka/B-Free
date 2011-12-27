/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library native functions.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/_exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/_exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: _exit.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:21  night
/* �������ե��������Ƭ�� Copyright notice ������ɲá�
/*
 * Revision 1.1  1995/02/19  14:43:11  night
 * �ǽ����Ͽ��
 *
 *
 */

#include <sys/types.h>
#include <native/syscall.h>

/*
 * �ץ�����λ���롣
 * ¾�Υ����ƥॳ����Ȱۤʤꡢ���Υ����ƥॳ����ϸƤӽФ��������ʤ���
 */
_exit (int ecode)
{
  CALL_SYSCALL (PSYS__EXIT, ecode);
  /* NOT REACHED */
}
