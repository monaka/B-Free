/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/access.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $  */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/syscalls/access.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: access.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1995-09-21 15:53:08  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.4  1995/03/18  14:29:28  night
 * �����ѹ���
 *
 * Revision 1.3  1995/02/28  14:09:12  night
 * ����ѥ���Ǥ���褦�ˡ���¤�Τ�̾���ʤɤ�������줿��Τ�ľ������
 *
 * Revision 1.2  1995/02/27  14:18:25  night
 * ��å��������Ȥ�Ω�Ƥ�ʬ�Ǥ��褦���ѹ���
 *
 * Revision 1.1  1995/02/26  14:55:15  night
 * �ǽ����Ͽ
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <server/file.h>
#include "../funcs.h"
#include "../global.h"



/*
 * �ե�����Υ����������Υ����å� (access �����ƥॳ����)
 *
 * ��ǽ��
 * �����Υѥ�̾�ǻ��ꤷ���ե�����/�ǥ��쥯�ȥ�Υ���������������å���
 * �롣
 *
 * ������
 * ����������������å����뤿��ˡ��ե�����ޥ͡������ FILE_ACCESS �׵�
 * ���������롣
 *
 */
int
psys_access (void *argp)
{
  struct a
    {
      const char	*path;	/* ����������������å�����ե����� */
      int		mode;	/* �����å����륢�������� */
    } *args = (struct a *)argp;
  char			path[PATH_MAX];
  int			retval;	/* API �ؿ�������֤��� */
  int			err_val;
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
   * �ե�����ޥ͡���������������å��������Ȥ�Ω�Ƥ� 
   */
  req.request = FILE_ACCESS;
  strcpy (req.body.file_access_req.path, args->path);
  req.body.file_access_req.mode = args->mode;

  /*
   * �ե�����ޥ͡�����إ�å��������������롣
   * fm_call �ϡ��ե�����ޥ͡�����˥�å���������������
   * ����������Ȥ�ؿ���
   */
  retval = fm_call (&req, &reply);
  errno = reply.error;

  return (retval);
}
