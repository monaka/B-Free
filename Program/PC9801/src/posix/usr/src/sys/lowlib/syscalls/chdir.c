/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chdir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chdir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: chdir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:08  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/03/18  14:29:44  night
 * �����ѹ���
 *
 * Revision 1.1  1995/02/26  14:55:38  night
 * �ǽ����Ͽ
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include "../funcs.h"
#include "../global.h"

/*
 * ��ǽ��
 * �ץ��������ܤ��Ƥ��륫���ȥǥ��쥯�ȥ� (CWD) ���ư���롣
 * �ǥ��쥯�ȥ�̾�λ�����ˡ�Ȥ��Ƥϡ����η����Υѥ�̾�����ѤǤ��롣
 *
 * 1) ���Хѥ��ˤ�������ˡ
 *    /a/b/c	
 *
 * 3) ���Хѥ��ˤ�������ˡ
 *    a/b/c
 *
 * �����ȥǥ��쥯�ȥ�ξ���ϡ��ե�����Υ����ץ���ʤɤΥѥ�̾���
 * �ꤹ������Τʤ��ǥѥ�̾��é��Ȥ��λ����Ȥ��ƻ��Ѥ��롣
 * (�����󡢤���ϥѥ�̾�����Хѥ�̾�Ȥ��ƻ��ꤷ���Ȥ�����)
 *
 * ������
 * �ץ����ޥ͡�������Ф��������ȥǥ��쥯�ȥ���ư����Ȥ����׵�
 * �����롣�ƥץ����Υ����ȥǥ��쥯�ȥ�ϡ��ץ����ޥ͡����㤬��
 * �����Ƥ��롣
 * 
 */
int
psys_chdir (void *argp)
{
  int	error;
  int	retval;		/* API �ؿ����֤��� */
  char	path[PATH_MAX];
  struct a
    {
      const char	*path;	/* ��ư��Υǥ��쥯�ȥ�Υѥ�̾ */
    } *args = (struct a *)argp;

  /*
   * ���ꤵ��Ƥ���ѥ�̾�����Хѥ��Ǥʤ����ˤϡ�
   * ���Хѥ�̾���Ѵ����롣
   */
  if (args->path[0] != '/')
    {
      if (resolv_fullpath (args->path, path) == NULL)
	{
	  /*
	   * ���Хѥ�̾���Ѵ��Ǥ��ʤ��ä���
	   * �������������� (EINVAL) �Υ��顼���֤���
	   */
	  errno = EINVAL;
	  return (-1);
	}

      args->path = path;
    }

  /*
   * POSIX �ץ����ޥ͡�������Ф��ơ�chdir ��Ԥ��Ȥ����׵�����롣
   * proc_chdir ���Ф��Ƥϡ����Хѥ�̾�����롣
   */
  retval = proc_chdir (args->path, &error);
  if (error)	/* ���顼�ȤʤäƤ��ޤä���*/
    {
      errno = error;
    }
  return (retval);
}
