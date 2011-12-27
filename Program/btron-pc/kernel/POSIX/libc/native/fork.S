/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
    $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/libc/native/fork.S,v 1.1 2011/12/27 17:13:35 liu1 Exp $
*/

.text
.globl fork

fork:
	movl	%esp,%eax
	pushl	%ebp
	pushl	%eax
	call	_fork
	popl	%ebx
	popl	%ebx
	ret
