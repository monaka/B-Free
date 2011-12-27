/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chown.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chown.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: chown.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:08  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/02/28  14:10:28  night
 * ����ѥ���Ǥ���褦�ˡ���¤�Τ�̾���ʤɤ�������줿��Τ�ľ������
 *
 * Revision 1.1  1995/02/27  14:23:30  night
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
 * ���ꤷ���ե�����ν�ͭ�Ԥȥ��롼�פ��ѹ����롣
 * ��ͭ�Ԥȥ��롼�פϡ����줾�� uid_t �� gid_t �ǻ��ꤹ�롣
 *
 * Syntax:
 *	int chown (const char *FILENAME, uid_t OWNER, gid_t GROUP)
 *
 * return valud:
 *	0	���ｪλ
 *	-1	�۾ｪλ
 *
 * ���Υ����ƥॳ����η�̡�errno �ˤϼ����ͤ����롣
 *
 *	EROFS	���ꤷ���ե�����Τ���ե����륷���ƥ�ϡ��񤭹��ߤ���
 *		���ʤ���
 *	EPERM	���ꤷ���ե�������ѹ������ʤ���
 *	
 */
int
psys_chown (void *argp)
{
  int	retval;
  struct a
    {
      const char	*filename;	/* ��ͭ�Ԥ��ѹ��������ե����� */
      uid_t		owner;		/* �ѹ���������ͭ�� ID */
      gid_t		group;		/* �ѹ����������롼�� ID */
    } *args = (struct a *)argp;

  retval = fm_chown (my_pid, args->filename, args->owner, args->group, &errno);
  return (retval);
}
