/* $Source: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/i386/interrupt.s,v $
 *
 * $Revision: 1.1 $
 * $Date: 2011/12/27 17:13:36 $
 * $Author: liu1 $
 *
 * Discription
 *
 */

.globl	_open_interrupt, _ignore_handler
.globl  _int1_handler	
.globl  _int2_handler	
.globl  _int3_handler	
.globl  _int4_handler	
.globl  _int5_handler	
.globl	_int6_handler
.globl  _int8_handler
.globl  _int9_handler
.globl	_int10_handler
.globl  _int11_handler
.globl  _int12_handler
.globl  _int13_handler
.globl	_int14_handler
.globl  _int15_handler
.globl  _int16_handler
#ifdef notdef
.globl  _int17_handler
.globl  _int18_handler
.globl  _int19_handler
.globl  _int20_handler
.globl  _int21_handler
.globl  _int22_handler
.globl  _int23_handler
.globl  _int24_handler
.globl  _int25_handler
.globl  _int26_handler
.globl  _int27_handler
.globl  _int28_handler
.globl  _int29_handler
.globl  _int30_handler
.globl  _int31_handler
#endif
.globl  _int32_handler
.globl  _int33_handler
.globl  _int34_handler
.globl	_int35_handler
.globl	_int36_handler
.globl  _int37_handler
.globl	_int41_handler
.globl	_int43_handler
.globl	_syscall_handler, _int64_handler, _posix_handler
.globl  _btron_handler, _int66_handler	
/* .globl	_int32_handler, _int33_handler, _int41_handler, _int43_handler */
.globl	_dis_int, _ena_int


.text


intr_msg:	.ascii	"ignore int.\n"
_ignore_handler:
		cli
		movb	0x20, %al
		movw	0x00, %dx
		outw	%al, %dx
		iret

/****************************************************************************
 * int0_handler --- 除算エラー
 *
 */
_int0_handler:
		pusha
		pushl	$0
		call	_fault
		popl	%eax
		popa
		iret
	
/****************************************************************************
 * int1_handler --- デバッガ割り込み
 *
 */
_int1_handler:
		pusha
		pushl	$1
		call	_fault
		popl	%eax
		popa
		iret
	
/****************************************************************************
 * int2_handler --- MNI
 */
_int2_handler:
		pusha
		pushl	$2
		call	_fault
		popl	%eax
		popa
		iret

/*************************************************************************
 * int3_handler --- break point 割り込み
 *
 */
_int3_handler:
		pusha
		pushl	$3
		call	_fault
		popl	%eax
		popa
		iret

/**************************************************************************
 * int4_handler --- overflow 割り込み
 *
 */
_int4_handler:
		pusha
		pushl	$4
		call	_fault
		popl	%eax
		popa
		iret
	
/**************************************************************************
 * int5_handler --- SCSI
 * 
 */
_int5_handler:
		cli
		pusha
		pushl	$5
		call	_interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/*	
		pusha
		pushl	$5
		call	_fault
		popl	%eax
		popa
		iret
*/
/**************************************************************************
 *
 */
_int6_handler:
		pusha
		pushl	$6
		call	_fault
		popl	%eax
		popa
		iret

/**************************************************************************
 *
 */
_int7_handler:
		pusha
		pushl	$7
		call	_fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
_int8_handler:
		pusha
		pushl	$8
		call	_fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
_int9_handler:
		pusha
		pushl	$9
		call	_fault
		popl	%eax
		popa
		iret


/**************************************************************************
 * int10_handler --- 不法ＴＳＳ例外
 *
 */
_int10_handler:
		pusha
		pushl	$10
		call	_fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
_int11_handler:
		pusha
		pushl	$11
		call	_fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
_int12_handler:
		pusha
		pushl	$12
		call	_fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
_int13_handler:
		call	_protect_fault
		popl	%eax
		iret


/**************************************************************************
 * int14_handler --- page fault.
 */
_int14_handler:
		call _page_fault
		iret


/**************************************************************************
 *
 */
_int15_handler:
		pusha
		pushl	$15
		call	_fault
		popl	%eax
		popa
		iret

/**************************************************************************
 *
 */
_int16_handler:
		pusha
		pushl	$16
		call	_fault
		popl	%eax
		popa
		iret

/**************************************************************************
 *
 * _int32_handler --- timer 
 *
 *
 */
_int32_handler:
		cli
		pusha
		pushl	$32
		call	_interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int33_handler --- keyboard
 *
 */
_int33_handler:
		cli
		pusha
		pushl	$33
		call	_interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		sti
		iret

/**************************************************************************
 * _int34_handler --- CRTV
 *
 *	no work.
 */
_int34_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int35_handler --- INT0
 *
 *	no work.
 */
_int35_handler:
		cli
		pusha
		pushl	$35
		call	_interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/*
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret
*/

/**************************************************************************
 * _int36_handler --- RS232C
 *
 *	no work.
 */
_int36_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int37_handler --- INT1
 *
 *	no work.
 */
_int37_handler:
		cli
		pusha
		pushl	$37
		call	_interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret
/*	
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret
*/

/**************************************************************************
 * _int38_handler --- INT2
 *
 *	no work.
 */
_int38_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int39_handler --- connect to slave 
 *
 *	no work.
 */
_int39_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int40_handler --- PRINTER
 *
 *	no work.
 */
_int40_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int41_handler --- INT3(SASI/SCSI HD)
 *
 *	
 */
_int41_handler:
		cli
		pusha
		pushl	$41
		call	_interrupt
		popl	%eax
		movb	$0x20, %al
		movw	$0x08, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_41_l1
		movb	$0x20, %al
		movw	$0x00, %dx
		outb	%al, %dx
int_41_l1:
		sti
		popa
		iret

/**************************************************************************
 * _int42_handler --- INT42 (FD 640K)
 *
 *	no work.
 */
_int42_handler:
		cli
		pusha
		/*
		 *
		 */
		movb	$0x20, %al
		movw	$0x08, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_42_l1
		movb	$0x20, %al
		movw	$0x00, %dx
		outb	%al, %dx
int_42_l1:
		popa
		iret

/**************************************************************************
 * _int43_handler --- INT43 (FD 1M)
 *
 *
 */
_int43_handler:
		cli
		pusha
		pushl	$43
		call	_interrupt
		popl	%eax
		movb	$0x20, %al
		movw	$0x08, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_43_l1
		movb	$0x20, %al
		movw	$0x00, %dx
		outb	%al, %dx
int_43_l1:
		sti
		popa
		iret

/**************************************************************************
 * _int44_handler --- INT 5
 *
 *	no work.
 */
_int44_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int45_handler --- INT 6(MOUSE)
 *
 *	no work.
 */
_int45_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int46_handler --- NDP
 *
 *	no work.
 */
_int46_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int47_handler --- Noise (Hardware error)
 *
 *	no work.
 */
_int47_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int64_handler --- ITRON system call.
 *
 *	システムコール割り込みの処理。
 *	そのため、他の割り込みと異なり、EOI などの処理を行わない。
 *
 *
 *	システムコール処理では次のことを「仮定」している。
 *	1) システムコール番号は、レジスタ eax に収められている。
 *	2) 同じくシステムコールへの引数の先頭番地は、レジスタ ebx に
 *	   収められている。
 *	3) システムコールからの返り値は、レジスタ eax に収められる。
 *	   (ユーザプロセスはこのことを「知って」いることを仮定している)
 *
 */
_syscall_handler:
_int64_handler:
		pushfl
/*		pushal */
		nop
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* 引数の先頭番地 */
		pushl	%eax	/* システムコール番号 */
		movw	$0x10, %ax
		movw	%ax,%ds 
		movw	%ax,%es 

		call 	_syscall

		movw	$0x23, %cx 
		movw	%cx,%ds    
		movw	%cx,%es    
		leave
/*		popal		 */
		nop
		popfl
		iret

/**************************************************************************
 * _int65_handler --- POSIX system call.
 *
 *	システムコール割り込みの処理。
 *	そのため、他の割り込みと異なり、EOI などの処理を行わない。
 *
 *
 *	システムコール処理では次のことを「仮定」している。
 *	1) システムコール番号は、レジスタ eax に収められている。
 *	2) 同じくシステムコールへの引数の先頭番地は、レジスタ ebx に
 *	   収められている。
 *	3) システムコールからの返り値は、レジスタ eax に収められる。
 *	   (ユーザプロセスはこのことを「知って」いることを仮定している)
 *
 */
_posix_handler:
_int65_handler:
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* 引数の先頭番地 */
		pushl	%eax	/* システムコール番号 */
		movw	$0x10, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		call 	_syscall
		movw	$0x23, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		iret
/**************************************************************************
 * _int66_handler --- BTRON system call.
 *
 *	システムコール割り込みの処理。
 *	そのため、他の割り込みと異なり、EOI などの処理を行わない。
 *
 *	システムコール処理では次のことを「仮定」している。
 *	1) システムコール番号は、レジスタ eax に収められている。
 *	2) 同じくシステムコールへの引数の先頭番地は、レジスタ ebx に
 *	   収められている。
 *	3) システムコールからの返り値は、レジスタ eax に収められる。
 *	   (ユーザプロセスはこのことを「知って」いることを仮定している)
 *
 */
_btron_handler:
_int66_handler:
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* 引数の先頭番地 */
		pushl	%eax	/* システムコール番号 */
		movw	$0x10, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		call 	_syscall
		movw	$0x23, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		iret
		

_lock_count:	.word	0
/***************************************************************************
 *
 */
_dis_int:
		cli
		ret

/*************************************************************************
 *
 */
_ena_int:
		sti
		ret

/*****************************************************************************
 *
 */
_open_interrupt:
		sti
		ret

	
	
.globl	_call_syscall

/* call_syscall: 汎用型システムコールルーチン
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
_call_syscall:
		pushl 	%ebp
		movl	%esp,%ebp
		movl	8(%ebp), %eax
		leal	12(%ebp), %ebx
		int	$64
		leave	
		ret

