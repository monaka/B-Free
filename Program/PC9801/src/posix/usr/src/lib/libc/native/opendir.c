/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library native functions.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/opendir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/opendir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $Log: opendir.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.4  1996-11-24 14:38:48  night
/* �ؿ����ɲá�
/*
 * Revision 1.3  1995/09/21  15:52:32  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/02/19  15:20:20  night
 * ����ѥ���Ǥ���褦�ˡ����󥯥롼�ɤ���ե�����λ�����ѹ�������
 *
 * <�ѹ���>
 * #include <h/types.h>
 *
 * <�ѹ���>
 * #include <sys/types.h>
 *
 * Revision 1.1  1995/02/19  14:43:26  night
 * �ǽ����Ͽ��
 *
 *
 */

#include <sys/types.h>
#include <native/syscall.h>

/* opendir --- ���ꤷ���ǥ��쥯�ȥ�򥪡��ץ󤹤�
 *
 * �����ץ󤷤��ǥ��쥯�ȥ�ϡ��ʲ��Υ����ƥॳ����ˤ�ä�
 * ���Ƥ��ɤߤȤ뤳�Ȥ��Ǥ��롣
 * ���ΤȤ����ɤ߼��ñ�̤ϡ��ǥ��쥯�ȥ����Ͽ���Ƥ���
 * �ե�����/�ǥ��쥯�ȥ�ñ�̤Ȥʤ롣
 *
 */
DIR
opendir (const char *dirname)
{
  return (CALL_SYSCALL (PSYS_OPENDIR, dirname));
}

