/*	ITRON にトラップをかけて呼び出す(システムコール)
 *
 */

/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/call_posix.s,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
	


.globl	call_lowlib
	


/* call_posix: POSIX システムコールルーチン
 *
 * . スタック形式:
 *
 *		+--------------------+
 *		|    return addr     |	+4
 *		+--------------------+
 *		|    syscall no.     |  +8
 *		+--------------------+
 *		|    argument #1     |  +12
 *		+--------------------+
 *		|    argument #2     |  +16
 *		+--------------------+
 *		|    argument #3     |
 *
 */
call_lowlib:	
		pushl 	%ebp
		movl	%esp,%ebp
		movl	8(%ebp), %eax
		leal	12(%ebp), %ebx
		int	$65
		leave	
		ret


