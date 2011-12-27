/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * Posix library routines on B-Free.
 *
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/port.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/* $log$
 *
 */

#include <sys/types.h>
#include <native/syscall.h>
#include <native/port.h>


/*
 * POSIX ������ (��) ���̿���Ԥ�����Υݡ��Ȥ򥢥����Ȥ���
 *
 */
int
LP_alloc_port (port_t *rport)
{
  ER error;

  error = posix_syscall (LP_ALLOC_PORT, rport);
  return (itron_to_posix_error (error));
}

/*
 * �ݡ��Ȥκ��
 */
int
LP_delete_port (port_t port)
{
  ER	error;

  error = posix_syscall (LP_DELETE_PORT, port);
  return (itron_to_posix_error (error));
}

/*
 * ������̾�����׵���������뤿��Υݡ��Ȥ򸫤Ĥ��롣
 * (�����Ф���äƤ����׵�����դ��Ѥμ����ݡ��Ȥ򸫤Ĥ���)
 */
int
LP_find_port (B *service, port_t *rport)
{
  ER	error;

  error = posix_syscall (LP_FIND_PORT, service, rport);
  return (itron_to_posix_error (error));
}


