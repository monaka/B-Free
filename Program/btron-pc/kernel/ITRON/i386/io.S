/* io.s
 *
 *
 */


.globl	outb, outw, outl, inb, inw, inl

.text

/*
 *	output byte
 *
 *	引数
 *		ioaddr: short
 *		data:	byte
 *
 *	返値
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
outb:
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
 *	引数
 *		ioaddr: short
 *		data:	short
 *
 *	返値
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
outw:
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
 *	引数
 *		ioaddr: short
 *		data:	byte
 *
 *	返値
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
outl:
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
 *	引数
 *		ioaddr: short
 *
 *	返値
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
inb:
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
 *	引数
 *		ioaddr: short
 *
 *	返値
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
inw:
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
 *	引数
 *		ioaddr: short
 *
 *	返値
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
inl:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	$0, %eax
		inl	%dx, %eax
		leave
		ret

