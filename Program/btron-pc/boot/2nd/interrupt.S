/**************************************************************************
 *
 * interrupt.s
 *
 */

.globl	intr_keyboard, open_interrupt, ignore_handler
.globl	lock, unlock, int33_handler, int43_handler
.globl	int38_handler, int32_handler, int46_handler, int47_handler


.text


intr_msg:	.ascii	"ignore int.\n"
ignore_handler:
		cli
/*		pusha */
		pushl	intr_msg
		call	boot_printf
		popl	%eax
		movb	0x20, %al
		movw	0x00, %dx
		outb	%al, %dx
/*		popa */
		iret

int0_handler:
		pusha
		pushl	$0
		call	fault
		popl	%eax
		popa
		iret
	
int1_handler:
		pusha
		pushl	$1
		call	fault
		popl	%eax
		popa
		iret
	
int2_handler:
		pusha
		pushl	$2
		call	fault
		popl	%eax
		popa
		iret
	
int3_handler:
		pusha
		pushl	$3
		call	fault
		popl	%eax
		popa
		iret
	
int4_handler:
		pusha
		pushl	$4
		call	fault
		popl	%eax
		popa
		iret
	
int5_handler:
		pusha
		pushl	$5
		call	fault
		popl	%eax
		popa
		iret

int6_handler:
		pusha
		pushl	$6
		call	fault
		popl	%eax
		popa
		iret

int7_handler:
		pusha
		pushl	$7
		call	fault
		popl	%eax
		popa
		iret


int8_handler:
		pusha
		pushl	$8
		call	fault
		popl	%eax
		popa
		iret


int9_handler:
		pusha
		pushl	$9
		call	fault
		popl	%eax
		popa
		iret


int10_handler:
		pusha
		pushl	$10
		call	fault
		popl	%eax
		popa
		iret


int11_handler:
		pusha
		pushl	$11
		call	fault
		popl	%eax
		popa
		iret


int12_handler:
		pusha
		pushl	$12
		call	fault
		popl	%eax
		popa
		iret


int13_handler:
		pusha
		pushl	$13
		call	fault
		popl	%eax
		popa
		iret


int14_handler:
		pusha
		pushl	$14
		call	fault
		popl	%eax
		popa
		iret


int15_handler:
		pusha
		pushl	$15
		call	fault
		popl	%eax
		popa
		iret

int16_handler:
		pusha
		pushl	$16
		call	fault
		popl	%eax
		popa
		iret

/**************************************************************************
 * int32_handler --- timer 
 *
 */
int32_handler:
		cli
		pusha
		pushl	$32
		call	interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * _int33_handler --- keyboard
 *
 */
int33_handler:
		cli
		pusha
		pushl	$33
		call	interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		sti
		iret

/**************************************************************************
 * int34_handler --- CRTV
 *
 *	no work.
 */
int34_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int35_handler --- INT0 (SCSI)
 *
 *	no work.
 */
int35_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int36_handler --- RS232C
 *
 *	no work.
 */
int36_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int37_handler --- INT1
 *
 *	no work.
 */
int37_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int38_handler --- floppy disk
 *
 *	no work.
 */
int38_handler:
		cli
		pusha
		pushl	$38
		call	interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int39_handler --- connect to slave 
 *
 *	no work.
 */
int39_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int40_handler --- PRINTER
 *
 *	no work.
 */
int40_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int41_handler --- INT3(SASI HD)
 *
 *	no work.
 */
int41_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int42_handler --- INT41 (FD 640K)
 *
 *	no work.
 */
int42_handler:
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
 * int43_handler --- INT42 (FD 1M)
 *
 *	no work.
 */
int43_handler:
		cli
		pusha
#ifdef notdef
		pushl	$43
		call	interrupt
		popl	%eax
#endif
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
 * int44_handler --- INT 5
 *
 *	no work.
 */
int44_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int45_handler --- INT 6(MOUSE)
 *
 *	no work.
 */
int45_handler:
		cli
		pusha
		movb	$0x20, %al	# set EOI
		outb	%al, $0x20
		popa
		iret

/**************************************************************************
 * int46_handler --- IDE HD
 *
 */
int46_handler:
		cli
		pusha
		pushl	$46
		call	interrupt
		popl	%eax
		movb	$0x20, %al
		movw	$0xa0, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_46_l1
		movb	$0x20, %al
		movw	$0x20, %dx
		outb	%al, %dx
int_46_l1:
		sti
		popa
		iret
	

/**************************************************************************
 * int47_handler --- IDE HD/slave
 *
 *	no work.
 */
int47_handler:
		cli
		pusha
		pushl	$47
		call	interrupt
		popl	%eax
		movb	$0x20, %al
		movw	$0xa0, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_47_l1
		movb	$0x20, %al
		movw	$0x20, %dx
		outb	%al, %dx
int_47_l1:
		sti
		popa
		iret
	

lock:
		cli
		ret

unlock:
		sti
		ret

open_interrupt:
		sti
		ret

		
