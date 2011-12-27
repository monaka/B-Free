/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chmod.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/chmod.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: chmod.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:08  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/02/28  14:09:48  night
 * ��ʬ�ǥե����륵���Ф������å��������Ȥ�Ω�Ƥ�褦���ѹ�������
 *
 * Revision 1.1  1995/02/26  14:55:39  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX �����ƥॳ���� chmod �� LOWLIB �ؿ���
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <server/file.h>
#include "../funcs.h"
#include "../global.h"

/*
 * ���ꤷ���ե�����Υѡ��ߥå������ѹ����롣
 *
 * Syntax:
 *	int chmod (const char *FILENAME, mode_t MODE)
 *
 * 
 */
int
psys_chmod (void *argp)
{
  struct a
    {
      char	*path;	/* �ѡ��ߥå������ѹ��������ե�����Υѥ�̾ */
      mode_t	mode;	/* ���ꤷ�����ѡ��ߥå������ */
    } *args = (struct a *)argp;
  char	path[PATH_MAX];
  int	retval;			/* API �ؿ�������֤��� */
  int	err_val;		/* ���顼�ֹ� */
  file_message_t	req;	/* �ե����륵���Ф������å����� */
  file_reply_t		reply;	/* �ե����륵���Ф����������å����� */
  
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
   * �ե�����ޥ͡�����˻��ꤷ���ե�����Υѡ��ߥå������ѹ�������
   * �׵�ѥ��åȤ������
   */
  req.request = FILE_SETATTR;
  req.pid     = my_pid;
  strcpy (req.body.file_setattr_req.path, args->path);
  req.body.file_setattr_req.mode = args->mode;

  retval = fm_call (&req, &reply);
  return (retval);
}
