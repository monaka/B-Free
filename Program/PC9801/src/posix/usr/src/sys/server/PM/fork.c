/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: fork.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:53:44  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/05  12:29:54  night
 * �����ɲá�
 *
 *
 */

#include "pm.h"

do_proc_fork (pm_msg_t *req)
{
  struct proc_reply reply;

  reply.error = PROC_EOK;
  send_reply (req, &reply);
}

	      
