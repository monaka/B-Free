/* locore.s --- アセンブラで書かなければならない関数および
 *              変数群の定義.
 *
 *
 */


.text

.globl		_get_cr0, _get_cr2, _get_cr3, _load_task_register
.globl		_flush_tlb
.globl		_resume, _resume0
.globl		_set_task_registers

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
_get_cr0:
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
_get_cr2:
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
_get_cr3:
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
_set_cr3:
		movl	4(%esp),%eax		/* 新しい CR3 の内容をスタック */
						/* から取り出す */
		movl	%eax,%cr3		/* 設定する。*/
		ret


/*************************************************************************
 * load_task_register --- タスクセグメントレジスタのロード
 *
 * 引数：	ローディングするタスクセグメントレジスタへのセレクタ
 *
 * 返値：	なし
 *
 * 処理：	ltr を実行する。
 *
 */
string1:
		.ascii "load_task_register: arg = 0x%x\n"
		.align 2
_load_task_register:
		pushl	%ebp
		movl	%esp,%ebp
/*
		pushl	8(%ebp)
		pushl	$string1
		call	_printf
*/
		movl	8(%ebp),%eax
		ltr	%eax
		leave
		ret


/*************************************************************************
 * resume --- 引数で指定したプロセスに切り換える。
 *
 * 引数：	切り換えるプロセスの TSS をさすセレクタ
 *
 * 返値：	なし
 *
 * 処理：	引数で渡されたセレクタに TSS jump する。
 *		その結果プロセスを切り換える。
 *
 */
LC0:
		.ascii "resume: arg = 0x%x\n"
		.align 2
offset:		.long	0x0000
selector:	.word	0x0000

_resume:
#if 0
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		pushl	%edx
		pushl	$LC0
		call	_printf
		leave
		ret
#endif
		cli
		pushl	%ebp
		movl	%esp, %ebp
		movl	8(%ebp), %edx
		movw	%dx, (selector)
		lea	offset, %eax		/* */
		movl	%cr3, %ebx		/* TLB キャッシュをフラッシュする */
		movl	%ebx, %cr3
/*
 *		プロセス切り換え
 *		ljmp	far (%ax)
 */
_resume0:
		.byte	0xff
		.byte	0x28
		leave
		sti
		ret

/*************************************************************************
 * flush_tlb	--- ページ変換キャッシュをフラッシュする。
 *
 * 引数：	なし
 *
 * 返値：	なし
 *
 * 処理：	cr3 の内容を get し、同時に put する。
 *
 */
_flush_tlb:
		pushl	%eax
		movl	%cr3, %eax
		movl	%eax, %cr3
		popl	%eax
		ret


_set_task_registers:
		ret