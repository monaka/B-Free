/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library misc function.
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/exit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* $Log: exit.c,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.1  1996-11-11 13:33:00  night
/* �ǽ����Ͽ
/*
 * Revision 1.2  1995/09/21  15:52:10  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/21  13:20:47  night
 * �ǽ����Ͽ
 *
 *
 */

#include <sys/types.h>
#include <native/syscall.h>


/*
 * �ץ����ν�����λ���롣
 */
exit (int exitcode)
{
  shutdown_stdio ();	/* ɸ�������ϥ饤�֥��θ���� */
                        /* �Хåե���ե�å��夹�� */

  _exit (exitcode);
}

