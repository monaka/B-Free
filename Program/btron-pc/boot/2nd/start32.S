/************************************************************************


  			S T A R T 3 2 


 ************************************************************************/

/* #include "location.h" */

IDT		= 0x2000
PAGE_DIR_ADDR	= 0x3000
PAGE_ENTRY_ADDR	= 0x4000
PAGE_FLAG	= 0x80000000

.text

.globl		start32, setup_idt, enable_page
.globl		get_cr0, get_cr2, get_cr3 

start32:
		cli
/*
		lgdt gdt_descr
		lidt idt_descr
		ljmp  $8, $long_jump
long_jump:
*/
		movl	$0x10, %eax
		mov	%ax, %ds
		mov	%ax, %es
		mov	%ax, %fs
		mov	%ax, %gs
		cli
		lss	stack_ptr, %esp
		pushl	$0
		pushl	$0
		pushl	$0
		cli
		cld
		call	_main			# Go C world.

fatal:
		jmp	fatal

setup_idt:
		lea	ignore_handler,%edx
		movl	$0x00080000,%eax
		movw	%dx,%ax		/* selector = 0x0008 = cs */
		movw	$0x8E00,%dx	/* interrupt gate - dpl=0, present */

		movl	$IDT,%edi
		mov	$256,%ecx
rp_sidt:
		movl	%eax,(%edi)
		movl	%edx,4(%edi)
		addl	$8,%edi
		dec	%ecx
		jne	rp_sidt
		ret

enable_page:
		mov	$PAGE_DIR_ADDR,	%eax
		mov	%eax, %cr3
		mov	%cr0, %eax
		or	$PAGE_FLAG, %eax
		mov	%eax, %cr0
		jmp	pflush
pflush:
		ret

/*************************************************************************
 * get_cr0 --- コントロールレジスタ 0 の内容を返す。
 *
 * 引数：	なし
 *
 * 返値：	コントロールレジスタ 0 の内容を返す。
 *
 * 処理：	コントロールレジスタ 0 の内容を返す。
 *
 */
get_cr0:
		mov	%cr0, %eax
		ret

/*************************************************************************
 * get_cr2 --- コントロールレジスタ 2 の内容を返す。
 *
 * 引数：	なし
 *
 * 返値：	コントロールレジスタ 2 の内容。
 *
 * 処理：	コントロールレジスタ 2 の内容を返す。
 *
 */
get_cr2:
		mov	%cr2, %eax
		ret

/*************************************************************************
 * get_cr3 --- コントロールレジスタ 3 の内容を返す。
 *
 * 引数：	なし
 *
 * 返値：	コントロールレジスタ 3 の内容。
 *
 * 処理：	コントロールレジスタ 3 の内容を返す。
 *
 */
get_cr3:
		mov	%cr3, %eax
		ret


/*************************************************************************
 * set_cr3 --- コントロールレジスタ 3 の内容を設定する。
 *
 * 引数：	コントロールレジスタ 3 の内容。
 *
 * 返値：	なし
 *
 * 処理：	コントロールレジスタ 3 の内容を設定する。
 *
 */
set_cr3:
		movl	4(%esp),%eax		/* 新しい CR3 の内容をスタック */
						/* から取り出す */
		movl	%eax,%cr3		/* 設定する。*/
		ret

.align 4
		.word	4
idt_descr:
		.word 256*8-1		# idt contains 256 entries
		.long IDT
.align 4
gdt_descr:
		.word	256*8-1
		.word	0x1000
		.word	0

.data	
.align 4
stack_ptr:
		.long	0x00070000	# offset
		.word	0x10		# selector
.align 2
start_message:	.ascii	"START 32 bit mode!"
		.word	0
.align 2



