	/*
	 *	A part of BTRON/386
	 *
	 *	2nd BOOT.(16bit mode)
	 */

	.code16

	GDT		=	0x1000
	IDT		=	0x2000

	;; 
	; .text	
	;begtext:
	;.data
	;begdata:
	;.bss
	;begbss:

	.section(text)
		.global second_boot

		.org	0x0000
	second_boot:
		mov	%cs, %ax
		mov	%ax, %ds
		mov	%ax, %ss
		mov	%ax, %es
		mov	$0xffff, %sp
		mov	$start_message, %bx
		call	print
		call	sleep_1s

		cli
		/* IDTR をセット */
		lldt	idt_ptr
		/* GDTR をセット */
		lgdt	gdt_ptr

		/* IDTR テーブルをセット */

		/* 2ndboot 32 ビットモードに遷移 */
		mov	$0x0001, %ax
		lmsw	%ax
		jmp	start32
	start32:
		jmpl	$0x9000, $0x8
	#ifdef notdef
		.byte	0xea		/* jmpi #0x9000, 0x8 */
		.word	0x9000
		.word	0x0008
	#endif
	
	/*
	 * speep_1s
	 * 1 秒間何もせずに待つ
	 */
	sleep_1s:
		push	%ax
		push	%cx
		push	%dx
		mov	$0x86, %ax
		mov	$0x000f, %cx
		mov	$0x4240, %dx
		int	$0x15
		pop	%dx
		pop	%cx
		pop	%ax
		ret
	
	/*
	 * print --- 文字列をコンソールに表示する。
	 *
	 * 引数：
	 *	bx	表示する文字列のアドレス
	 *		文字列は、<長さ>:1バイト
	 *			  <文字列>:最大255バイト
	 *		となっている。
	 */
	print:
		push	%si
		push	%di
		push	%ax
		push	%bx
		push	%cx
		push	%dx

		push	%bx
		mov	$0x03, %ah 		/* get cursor pos */
		xor	%bh, %bh
		int	$0x10
		pop	%bx

		xor	%cx, %cx
		movb	%cl, (%bx)
		inc	%bx
		mov	%bp, %bx
		mov	$0x0007, %bx
		mov	$0x1301, %ax
		int	$0x10

		pop	%dx
		pop	%cx
		pop	%bx
		pop	%ax
		pop	%di
		pop	%si
		ret
		
	fatal:
		jmp	fatal

	.section(data)
	start_message:	.byte	19
		.byte	13, 10
		.ascii	"2nd BOOT start."
		.byte	13, 10


	.align 4
	idt_ptr:
		.word	256*8-1		/* 256 entries * 8 --- size. */
		.word	IDT,0		/* offset */

	.align 4
	gdt_ptr:
		.word	256*8-1		/* 256 entries * 8 --- size. */
		.word	GDT 		/* offset. */
		.word	0
;	.text			
;	endtext:

;	.data
;	enddata:

;	.bss
;	endbss:


	
