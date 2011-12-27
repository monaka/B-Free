/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/sleep.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/sleep.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: sleep.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:53:25  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/03/04  14:36:32  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include "../funcs.h"
#include "../global.h"

/*
 * ���ꤷ�����֤����ץ����� sleep ���롣
 * ���Υ����ƥॳ����ˤĤ��Ƥϡ��ޥ͡�����ϻȤ鷺��ITRON �˥����ƥ�
 * ������Τߤ���Ѥ��ƽ������롣
 *
 * Syntax:
 * unsigned int	sleep (int second);
 *
 */
unsigned int
psys_sleep (void *argp)
{
  struct a
    {
      int	second;	/* sleep ������� (��) */
    } *args = (struct a *)argp;

  /*
   * ITRON �����ƥॳ�����
   */
  /*
   * �ޤ�����ץ���Ȥ��Ƥ��ʤ���ENOSYS ���֤���
   */
  errno = ENOSYS;

  return (NULL);
}
