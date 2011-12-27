/**************************************************************************
 *
 * interrupt.s
 *
 */

.globl	_intr_keyboard, _open_interrupt, _ignore_handler
.globl	_lock, _unlock, _int33_handler, _int43_handler


.text


intr_msg:	.ascii	"ignore int.\n"
_ignore_handler:
		cli
/*		pusha */
/*
		pushl	intr_msg
		call	_printf
		popl	%eax
*/
		movb	0x20, %al
		movw	0x00, %dx
		outw	%al, %dx
/*		popa */
		iret

_int0_handler:
		pusha
		pushl	$0
		call	_fault
		popl	%eax
		popa
		iret
	
_int1_handler:
		pusha
		pushl	$1
		call	_fault
		popl	%eax
		popa
		iret
	
_int2_handler:
		pusha
		pushl	$2
		call	_fault
		popl	%eax
		popa
		iret
	
_int3_handler:
		pusha
		pushl	$3
		call	_fault
		popl	%eax
		popa
		iret
	
_int4_handler:
		pusha
		pushl	$4
		call	_fault
		popl	%eax
		popa
		iret
	
_int5_handler:
		pusha
		pushl	$5
		call	_fault
		popl	%eax
		popa
		iret

_int6_handler:
		pusha
		pushl	$6
		call	_fault
		popl	%eax
		popa
		iret

_int7_handler:
		pusha
		pushl	$7
		call	_fault
		popl	%eax
		popa
		iret


_int8_handler:
		pusha
		pushl	$8
		call	_fault
		popl	%eax
		popa
		iret


_int9_handler:
		pusha
		pushl	$9
		call	_fault
		popl	%eax
		popa
		iret


_int10_handler:
		pusha
		pushl	$10
		call	_fault
		popl	%eax
		popa
		iret


_int11_handler:
		pusha
		pushl	$11
		call	_fault
		popl	%eax
		popa
		iret


_int12_handler:
		pusha
		pushl	$12
		call	_fault
		popl	%eax
		popa
		iret


_int13_handler:
		pusha
		pushl	$13
		call	_fault
		popl	%eax
		popa
		iret


_int14_handler:
		pusha
		pushl	$14
		call	_fault
		popl	%eax
		popa
		iret


_int15_handler:
		pusha
		pushl	$15
		call	_fault
		popl	%eax
		popa
		iret

_int16_handler:
		pusha
		pushl	$16
		call	_fault
		popl	%eax
		popa
		iret

/**************************************************************************
 * _int32_handler --- timer 
 *
 */
_int32_handler:
		cli
		pusha
		pushl	$33
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
 * _int35_handler --- INT0 (SCSI)
 *
 *	no work.
 */
_int35_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

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
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

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
 * _int41_handler --- INT3(SASI HD)
 *
 *	no work.
 */
_int41_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	$0x00, %dx
		outw	%al, %dx
		popa
		iret

/**************************************************************************
 * _int42_handler --- INT41 (FD 640K)
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
 * _int43_handler --- INT42 (FD 1M)
 *
 *	no work.
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

_lock:
		cli
		ret

_unlock:
		sti
		ret

_open_interrupt:
		sti
		ret

		