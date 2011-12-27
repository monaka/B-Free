/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/startup.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static unsigned char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/startup.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * POSIX �Ķ��Τ���� startup �ؿ��������
 */

#include <stdio.h>


/*
 * �ץ����Υ���ȥ�ؿ���
 *
 * �ץ����� main() �ǤϤʤ������� __entry() �ؿ��ˤ�äƼ¹Ԥ򳫻Ϥ��롣
 *
 * ���δؿ��Ǥϼ��ν�����Ԥ���
 *
 * 1) standard I/O �ؿ��Τ���ν����
 * 2) main() �ؿ��θƤӽФ�
 * 3) �ץ����ν�λ�Τ���θ����
 *
 *
 * POSIX �ץ����μ¹Ԥ򳫻Ϥ���Ȥ��ˤϡ������å��ϼ��Τ褦�ˤʤäƤ��롣
 *
 *  	+----------------------+ <-- return address.
 *	|    Argument Count    | 
 *	+----------------------+
 *	|    Argument Vector   |
 *	+----------------------+
 *	|    Environment       |
 *	+----------------------+
 *	|                      |
 *      //   ������ʸ����     //
 *	|    �ơ��֥�          |
 *	+----------------------+
 *	|                      |
 *	//   �Ķ��ѿ�          //
 *	|      �μ�Ǽ�ΰ�      | <- user_stack_top (����)
 *	+----------------------+
 *
 * entry �ؿ��ϡ�Argument Count, Argument Vector, Environment ��3�Ĥξ����
 * main �ؿ��ˤ��Τޤް����Ϥ��� 
 * 
 */
__entry (int ac, char **av, char *ae)
{
  int	errcode;

  setup_standard_io ();		/* stdin, stdout, stderr �ν������ */
				/* setup_standard_io �ϡ�libc.a �ˤ��� */
				/* �ؿ���*/

  errcode = main (ac, av, ae);	/* �桼���ؿ� main �μ¹ԡ����ΤȤ��� */
				/* �֤äƤ����ͤ�ץ����� exit �� */
				/* ���롣 */

  exit (errcode);		/* ���顼�����ɤ�ƤӽФ������֤� */
}
