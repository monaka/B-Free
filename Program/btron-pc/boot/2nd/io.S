/****************************************************************************
 *
 *		I/O インタフェース用のルーチン
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/io.S,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: io.S,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-15 02:59:18  monaka
 * *.s files renamed extension to *.S
 *
 * Revision 1.2  1997/06/29 13:14:48  night
 * 関数名の最初についている '_' を削除。
 * (C コンパイルを ELF オブジェクトに変更したことによる修正)
 *
# Revision 1.1  1996/05/11  10:45:04  night
# 2nd boot (IBM-PC 版 B-FREE OS) のソース。
#
 * Revision 1.1  1993/10/11 21:29:20  btron
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

