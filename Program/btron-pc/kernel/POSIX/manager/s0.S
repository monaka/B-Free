/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* posix.s - POSIX �Ķ��ޥ͡�����Υ������ȥ��åץ롼����
 *
 *
 *
 */

.globl	startup

startup:
	pushl %ebp
	movl %esp,%ebp
	pushl $0
	pushl $0
	call	posix_start
	addl $8,%esp
L2:
	movl %ebp,%esp
	popl %ebp
	ret			;

	