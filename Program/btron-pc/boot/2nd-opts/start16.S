/*
 *	A part of BTRON/386
 *
 *	2nd BOOT.(16bit mode)
 */

GDT		=	0x1000
IDT		=	0x2000

.text
begtext:
.data
begdata:
.bss
begbss:

.text
		entry second_boot

		.org	0x0000
second_boot:
		mov	ax, cs
		mov	ds, ax
		mov	ss, ax
		mov	es, ax
		mov	sp, #0xffff
		mov	bx, #start_message
		call	print
		call	sleep_1s

		cli
		/* IDTR をセット */
		lidt	[idt_ptr]
		/* GDTR をセット */
		lgdt	[gdt_ptr]

		/* IDTR テーブルをセット */

		/* 2ndboot 32 ビットモードに遷移 */
		mov	ax, #0001
		lmsw	ax
		jmp	start32
start32:
		jmpi	#0x9000, #0x8
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
		push	ax
		push	cx
		push	dx
		mov	ax, #0x86
		mov	cx, #0x000f
		mov	dx, #0x4240
		int	0x15
		pop	dx
		pop	cx
		pop	ax
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
		push	si
		push	di
		push	ax
		push	bx
		push	cx
		push	dx

		push	bx
		mov	ah,#0x03		/* get cursor pos */
		xor	bh,bh
		int	0x10
		pop	bx

		xor	cx, cx
		movb	cl, (bx)
		inc	bx
		mov	bp, bx
		mov	bx, #0x0007
		mov	ax, #0x1301
		int	0x10

		pop	dx
		pop	cx
		pop	bx
		pop	ax
		pop	di
		pop	si
		ret
		
fatal:
		j	fatal

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
.text
endtext:

.data
enddata:

.bss
endbss:

		end	
	
