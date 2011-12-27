/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/sigfillset.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/sigfillset.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: sigfillset.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-03-21 00:58:11  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.2  1999/02/19 10:04:55  monaka
 * Addition or fix for parameter structure.
 *
 * Revision 1.1  1997/08/31 13:10:49  night
 * �ǽ����Ͽ
 *
 *
 *
 */


#include "../lowlib.h"




int
psys_sigfillset (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_sigfillset *args = (struct psc_sigfillset *)argp;

  /*
   * �ޤ�����ץ���Ȥ��Ƥ��ʤ���ENOSYS ���֤���
   */
  errno = ENOSYS;

  return (NULL);
}
