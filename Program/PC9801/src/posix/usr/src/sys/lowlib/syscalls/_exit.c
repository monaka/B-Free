/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/_exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/_exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: _exit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:07  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/03/18  14:52:16  night
 * �����ѹ�
 *
 * Revision 1.1  1995/02/27  14:23:29  night
 * �ǽ����Ͽ
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include "../funcs.h"
#include "../global.h"

/*
 * �ץ�����λ���롣
 *
 * ��ǽ��
 * ���Υ����ƥॳ����Ǥϡ�ɸ�������ϥ饤�֥�� (stdio) �Ǵ������Ƥ���
 * �Хåե��ˤĤ��Ƥϴ��Τ��Ƥ��ʤ���ñ�˥ץ�������äƤ���񸻤�
 * �٤��������ƴط���������λ����������Ǥ��롣
 *
 * �������ơ�
 *   (1) �ץ����ޥ͡�����˥ץ����ν�λ�׵�����롣
 *   (2) �ץ����ޥ͡�����ϡ����Υץ����˴ؤ������������롣
 *   (3) lowlib �� posix_exit () ��¹ԡ�
 *
 * Syntax:
 * void	_exit (int exit_code);
 *
 */
void
psys__exit (void *argp)
{
  struct a
    {
      int	exit_code;
    } *args = (struct a *)argp;

  /*
   * �ޤ�����ץ���Ȥ��Ƥ��ʤ���ENOSYS ���֤���
   */
  errno = ENOSYS;
}
