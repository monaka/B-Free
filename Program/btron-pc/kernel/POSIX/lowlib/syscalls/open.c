/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/open.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/open.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: open.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  1999-03-30 13:26:41  monaka
 * Minor fixes to a comment.
 *
 * Revision 1.7  1999/03/21 00:57:52  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.6  1999/02/19 10:04:42  monaka
 * Addition or fix for parameter structure.
 *
 * Revision 1.5  1998/02/25 12:51:51  night
 * �ץ�����θ�ͭ�ǡ�������򡢤��줾������ѿ��Ǹ���Ū�ˤ�ĤΤǤϤʤ���
 * lowlib_data �Ȥ����ݥ��󥿤Ǵ�������褦�ˤ�����
 * �����ѹ���ȼ�����ץ�����˰�ä����������/���Ȥ�������ϡ��ݥ���
 * lowlib_data ��𤷤ƹԤ��褦���ѹ�������
 *
 * lowlib_data ���ͤϡ��ޥ��� LOWLIB_DATA �ǻ��ꤷ�����ɥ쥹 (0x7fff0000)
 * �����ꤵ��롣���Υ��ɥ쥹�ϡ��ץ�����˰�ä�ʪ���ڡ����˥ޥåԥ�
 * ����롣���ۥ��ɥ쥹�ϸ���������ºݤ��ΰ�ϥץ�����˰�ä�����¸
 * �ߤ��Ƥ��롣LOWLIB ����������ץ�����ξ���Ϥ����ΰ�������褦��
 * ���롣
 *
 * Revision 1.4  1997/10/11 16:24:55  night
 * ���ޤ��ޤ���������
 *
 * Revision 1.3  1997/09/21 13:33:35  night
 * �ѥ�̾��Ĺ����¬�ꤹ��ؿ� strlen_with_length() �� static �ؿ��Ȥ�����
 * ��������
 *
 * Revision 1.2  1997/09/09 13:52:30  night
 * �ɽ����������ν��֤��ѹ���
 *
 * Revision 1.1  1997/08/31 13:10:46  night
 * �ǽ����Ͽ
 *
 *
 *
 */


#include "../lowlib.h"


#ifdef notdef
static int
strlen_with_length (char *s, int maxlen)
{
  int	i;

  for (i = 0; i < maxlen; i++)
    {
      if (*s == '\0')
	{
	  break;
	}
      s++;
    }

  return (i);
}
#endif

int
psys_open (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_open *args = (struct psc_open *)argp;

  req.param.par_open.pathlen = args->pathlen;
  req.param.par_open.path = args->path;
  req.param.par_open.oflag = args->oflag;	/* o_rdonly | o_wronly | o_rdwr | o_creat */
  req.param.par_open.mode = args->mode;		/* no use except oflag includes o_creat */

  error = _make_connection(PSC_OPEN, &req, &res);

  if (error != E_OK)
    {
      /* What should I do? */
    }
  else if (res.errno)
    {
      ERRNO = res.errno;
      return (-1);
    }

  return (res.status);
}



