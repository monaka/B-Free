;	A part of BTRON/386
;
;	2nd BOOT.(16bit mode)
;

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

second_boot:
		mov	ax, cs
		mov	ds, ax
		mov	ss, ax
		mov	es, ax
		mov	sp, #0xffff
		mov	bx, #start_message
		call	print

		cli
		; IDTR をセット
		lidt	[idt_ptr]
		; GDTR をセット
		lgdt	[gdt_ptr]

		; IDTR テーブルをセット


		; 2ndboot 32 ビットモードに遷移
		mov	ax, #0001
		lmsw	ax
		jmp	start32
start32:
		jmpi	#0x9000, #0x8
;		.byte	0xea		; jmpi #0x9000, 0x8
;		.word	0x9000
;		.word	0x0008

		

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; print --- 画面にメッセージ表示
;
; 引数
;	BX:	出力するメッセージを指すオフセット
;	
print:
		push	si
		push	di
		push	ax
		push	bx
		push	cx
		push	dx

		cld
		mov	si,bx
		mov	ax,#0xA000
		mov	es,ax
		mov	di,[loc_y]
		xor	ax,ax
print_loop:
		lodb
		cmpb	al,*0x0
		jz	print_done
		stow
		j	print_loop

print_done:
		mov	ax, #160
		add	ax, [loc_y]
		mov	[loc_y],ax

		pop	dx
		pop	cx
		pop	bx
		pop	ax
		pop	di
		pop	si
		ret
		
loc_y:		.word	3 * 160

fatal:
		j	fatal

start_message:	.ascii	"2nd BOOT start."
		.byte	0

;gdt:
;		.word	0,0,0,0		; dummy entry (no use)
;
;		; Entry 1 (Code)
;		.word	0xC000		; limit = 3192 * 4096 = 12MB
;		.word	0x0000		; Base = 0
;		.word	0x9A00		; CODE/READ
;		.word	0x00C0
;
;		; Entry 2 (Data)
;		.word	0xC000		; limit = 3192 * 406 = 12MB
;		.word	0x0000		; Base = 0
;		.word	0x9200		; READ/WRITE
;		.word	0x00C0

.align 4
idt_ptr:
;		.word	0		; 256 entries * 8 --- size.
;		.word	0,0		; offset
		.word	256*8-1		; 256 entries * 8 --- size.
		.word	IDT,0		; offset

.align 4
gdt_ptr:
		.word	256*8-1		; 256 entries * 8 --- size.
		.word	GDT 		; offset.
		.word	0
.text
endtext:

.data
enddata:

.bss
endbss:

		end	
	
