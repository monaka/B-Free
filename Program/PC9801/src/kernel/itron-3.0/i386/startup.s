/* startup.s
 *
 *	startup system.
 *
 */

#define GDT_ADDR	0x80001000
#define IDT_ADDR	0x80002000

.globl		startup

startup:
		cli
		/* idtr をセット */
		lidt	idt_ptr
		/* gdtr をセット */
		lgdt	gdt_ptr

		movl	$0x10, %eax
		mov	%ax, %ds
		mov	%ax, %es
		mov	%ax, %fs
		mov	%ax, %gs
		lss	_stack_ptr, %esp

		pushl	$0
		pushl	$0
		pushl	$0
		cld

		call	_itron			/* Go C world. */

.align 4
_stack_ptr:
		.long	0x00070000	/* offset */
		.word	0x10		/*8 selector */
			
.align 4
idt_ptr:
		.word	256*8-1		/* 256 entries * 8 --- size.	*/
		.long	IDT_ADDR	/* offset			*/

/*
	Global Descpritor Table.


	Entry:
	0x0000			0	Unused
	0x0008			1	kernel code
	0x0010			2	kernel data
	0x0018			3	local descpritor table
	0x0020			4	unused
	0x0028			5	idt
	0x0030...0x03F8   6...127	unused

	0x0400		      128	unused
	0x0408		129...256	TSS
*/

.align 4
gdt_ptr:
		.word	256*8-1		/* 256 entries * 8 --- size. */
		.long	GDT_ADDR	/* offset. */

		