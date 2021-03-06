/*	ITRON $B$r%H%i%C%W$r$+$1$F8F$S=P$9(B($B%7%9%F%`%3!<%k(B)
 *
 */

.globl	call_syscall
#if 0
		, call_posix
#endif
	
/* call_syscall: ITRON $B%7%9%F%`%3!<%k%k!<%A%s(B
 *
 * . $B%9%?%C%/7A<0(B:
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
call_syscall:
		pushl 	%ebp
		movl	%esp,%ebp
		movl	8(%ebp), %eax
		leal	12(%ebp), %ebx
		int	$64
		leave	
		ret


#if 0
/* call_posix: POSIX $B%7%9%F%`%3!<%k%k!<%A%s(B
 *
 * . $B%9%?%C%/7A<0(B:
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
call_posix:
		pushl 	%ebp
		movl	%esp,%ebp
		movl	8(%ebp), %eax
		leal	12(%ebp), %ebx
		int	$65
		leave	
		ret

#endif
