/****************************************************************************
 *
 *		I/O インタフェース用のルーチン
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/io.s,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: io.s,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.1  1993-10-11 21:29:20  btron
 * btron/386
 *
# Revision 1.1.1.1  93/01/14  12:30:22  btron
# BTRON SYSTEM 1.0
# 
# Revision 1.1.1.1  93/01/13  16:50:28  btron
# BTRON SYSTEM 1.0
# 
 */

#include "location.h"

.globl	_outb, _outw, _outl, _inb, _inw, _inl

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
_inl:
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movl	$0, %eax
		inl	%dx, %eax
		leave
		ret

