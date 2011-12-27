	.file	"_exit.c"
gcc2_compiled.:
___gnu_compiled_c:
.data
_rcsid:
	.ascii "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/native/_exit.s,v 1.1 2011/12/27 17:13:35 liu1 Exp $\0"
.text
	.align 4
.globl __exit
__exit:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	pushl %edx
	pushl $16
	call _CALL_SYSCALL
	addl $8,%esp
L1:
	movl %ebp,%esp
	popl %ebp
	ret
