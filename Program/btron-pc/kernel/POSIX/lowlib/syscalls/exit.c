/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/syscalls/exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: exit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-07-23 14:37:36  naniwa
 * implemented
 *
 * Revision 1.2  1999/03/21 00:57:26  monaka
 * Major fix. Many system calls runs correctly (...perhaps ;-). But signal and process functions are not yet.
 *
 * Revision 1.1  1997/08/31 13:10:43  night
 * �ǽ����Ͽ
 *
 *
 *
 */


#include "../lowlib.h"



int
psys_exit (void *argp)
{
  ER			error;
  struct posix_request	req;
  struct posix_response	res;
  struct psc_exit *args = (struct psc_exit *)argp;

  req.param.par_exit.evalue = args->evalue;
  error = _make_connection(PSC_EXIT, &req, &res);

  /* ��å������ݡ��Ȥ��� */
  del_mbf(lowlib_data->recv_port);

  /* ����������λ */
  exd_tsk ();

  /* ����ʹߤϼ¹Ԥ���ʤ� */
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