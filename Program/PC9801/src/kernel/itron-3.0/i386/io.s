/* io.s
 *
 *
 */


.globl	_outb, _outw, _outl, _inb, _inw, _inl

.text

/*
 *	output byte
 *
 *	����
 *		ioaddr: short
 *		data:	byte
 *
 *	����
 *		none
 *
 *	+-----------------------+  0 = sp
 *	|	bp		|
 *	+-----------------------+ +4
 *	|    return addr	|
 *	+-----------------------+ +8
 *	|	arg1		|	ioaddr
 *	+-----------------------+ +12
 *	|	arg2		| 	data
 *	+-----------------------+
 */
_outb:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		andl	$0xffff, %edx
		movl	12(%ebp), %eax
/*		andl	$0xff, %eax */
		outb	%al, %dx
		leave
		ret

/*
 *	output word
 *
 *	����
 *		ioaddr: short
 *		data:	short
 *
 *	����
 *		none
 *
 *	+-----------------------+  0 = sp
 *	|	bp		|
 *	+-----------------------+ +4
 *	|    return addr	|
 *	+-----------------------+ +8
 *	|	arg1		|	ioaddr
 *	+-----------------------+ +12
 *	|	arg2		| 	data
 *	+-----------------------+
 */
_outw:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	12(%ebp), %eax
		outw	%ax, %dx
		leave
		ret

/*
 *	output long
 *
 *	����
 *		ioaddr: short
 *		data:	byte
 *
 *	����
 *		none
 *
 *	+-----------------------+  0 = sp
 *	|	bp		|
 *	+-----------------------+ +4
 *	|    return addr	|
 *	+-----------------------+ +8
 *	|	arg1		|	ioaddr
 *	+-----------------------+ +12
 *	|	arg2		| 	data
 *	+-----------------------+
 */
_outl:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	12(%ebp), %eax
		outl	%eax, %dx
		leave
		ret

/*
 *	input byte
 *
 *	����
 *		ioaddr: short
 *
 *	����
 *		data:	byte
 *
 *	+-----------------------+  0 = sp
 *	|	bp		|
 *	+-----------------------+ +4
 *	|    return addr	|
 *	+-----------------------+ +8
 *	|	arg1		|	ioaddr
 *	+-----------------------+
 */
_inb:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	$0, %eax
		inb	%dx, %al
		leave
		ret


/*
 *	input word
 *
 *	����
 *		ioaddr: short
 *
 *	����
 *		data:	short
 *
 *	+-----------------------+  0 = sp
 *	|	bp		|
 *	+-----------------------+ +4
 *	|    return addr	|
 *	+-----------------------+ +8
 *	|	arg1		|	ioaddr
 *	+-----------------------+
 */
_inw:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	$0, %eax
		inw	%dx, %ax
		leave
		ret


/*
 *	input long
 *
 *	����
 *		ioaddr: short
 *
 *	����
 *		data:	long
 *
 *	+-----------------------+  0 = sp
 *	|	bp		|
 *	+-----------------------+ +4
 *	|    return addr	|
 *	+-----------------------+ +8
 *	|	arg1		|	ioaddr
 *	+-----------------------+
 */
_inl:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	$0, %eax
		inl	%dx, %eax
		leave
		ret

