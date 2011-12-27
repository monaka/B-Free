/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* posix.s - POSIX 環境マネージャのスタートアップルーチン
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

	