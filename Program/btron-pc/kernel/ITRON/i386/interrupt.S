/* $Source: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/i386/interrupt.S,v $
 *
 * $Revision: 1.1 $
 * $Date: 2011/12/27 17:13:35 $
 * $Author: liu1 $
 *
 * Discription
 *
 */

#define MASTER_8259A_COM	$0x20
#define MASTER_8259A_DATA	$0x21

#define SLAVE_8259A_COM		$0xa0
#define SLAVE_8259A_DATA	$0xa1


.globl	open_interrupt, ignore_handler
.globl  int1_handler	
.globl  int2_handler	
.globl  int3_handler	
.globl  int4_handler	
.globl  int5_handler	
.globl	int6_handler
.globl  int8_handler
.globl  int9_handler
.globl	int10_handler
.globl  int11_handler
.globl  int12_handler
.globl  int13_handler
.globl	int14_handler
.globl  int15_handler
.globl  int16_handler
#ifdef notdef
.globl  int17_handler
.globl  int18_handler
.globl  int19_handler
.globl  int20_handler
.globl  int21_handler
.globl  int22_handler
.globl  int23_handler
.globl  int24_handler
.globl  int25_handler
.globl  int26_handler
.globl  int27_handler
.globl  int28_handler
.globl  int29_handler
.globl  int30_handler
.globl  int31_handler
#endif
.globl  int32_handler
.globl  int33_handler
.globl  int34_handler
.globl	int35_handler
.globl	int36_handler
.globl  int37_handler
.globl  int38_handler
.globl	int40_handler
.globl	int41_handler
.globl	int43_handler
.globl	int46_handler
.globl	syscall_handler, int64_handler, posix_handler
.globl  btron_handler, int66_handler	
/* .globl	int32_handler, int33_handler, int41_handler, int43_handler */
.globl	dis_int, ena_int

.text


intr_msg:	.ascii	"ignore int.\n"
ignore_handler:
		cli
		movb	0x20, %al
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
		iret

/****************************************************************************
 * int0_handler --- �������顼
 *
 */
int0_handler:
		pusha
		pushl	$0
		call	fault
		popl	%eax
		popa
		iret
	
/****************************************************************************
 * int1_handler --- �ǥХå�������
 *
 */
int1_handler:
		pusha
		pushl	$1
		call	fault
		popl	%eax
		popa
		iret
	
/****************************************************************************
 * int2_handler --- MNI
 */
int2_handler:
		pusha
		pushl	$2
		call	fault
		popl	%eax
		popa
		iret

/*************************************************************************
 * int3_handler --- break point ������
 *
 */
int3_handler:
		pusha
		pushl	$3
		call	fault
		popl	%eax
		popa
		iret

/**************************************************************************
 * int4_handler --- overflow ������
 *
 */
int4_handler:
		pusha
		pushl	$4
		call	fault
		popl	%eax
		popa
		iret
	
/**************************************************************************
 * int5_handler --- SCSI
 * 
 */
int5_handler:
		cli
		pusha
		pushl	$5
		call	interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
		popa
		iret

/*	
		pusha
		pushl	$5
		call	fault
		popl	%eax
		popa
		iret
*/
/**************************************************************************
 *
 */
int6_handler:
/*		pusha
 */
		pushl	$6
		call	fault
		popl	%eax
		popa
		iret

/**************************************************************************
 *
 */
int7_handler:
		pusha
		pushl	$7
		call	fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
int8_handler:
		pusha
		pushl	$8
		call	fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
int9_handler:
		pusha
		pushl	$9
		call	fault
		popl	%eax
		popa
		iret


/**************************************************************************
 * int10_handler --- ��ˡ�ԣӣ��㳰
 *
 */
int10_handler:
		pusha
		pushl	$10
		call	fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
int11_handler:
		pusha
		pushl	$11
		call	fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
int12_handler:
		pusha
		pushl	$12
		call	fault
		popl	%eax
		popa
		iret


/**************************************************************************
 *
 */
int13_handler:
		call	protect_fault
		popl	%eax
		iret


/**************************************************************************
 * int14_handler --- page fault.
 */
int14_handler:
		call page_fault
		iret


/**************************************************************************
 *
 */
int15_handler:
		pusha
		pushl	$15
		call	fault
		popl	%eax
		popa
		iret

/**************************************************************************
 *
 */
int16_handler:
		pusha
		pushl	$16
		call	fault
		popl	%eax
		popa
		iret

/**************************************************************************
 *
 * int32_handler --- timer 
 *
 *
 */
int32_handler:
/*
		cli
*/
		pusha
#if 1
		inb	MASTER_8259A_DATA, %al
		orb	$1, %al
		outb	%al, MASTER_8259A_DATA
		movb	$0x20, %al
		outb	%al, MASTER_8259A_COM
		sti
#endif
		pushl	$32
		call	interrupt
		popl	%eax
/*
		movb	$0x21, %al	# set EOI
		outw	%al, MASTER_8259A_COM
*/
#if 0
#if 1
		movb	$0x20, %al	# set EOI
		cli
		outb	%al, MASTER_8259A_COM
#else
		cli
		inb	MASTER_8259A_DATA, %al
		andb	$0xFE, %al
		outb	%al, MASTER_8259A_DATA
#endif
#endif
		popa
		iret

/**************************************************************************
 * int33_handler --- keyboard
 *
 */
int33_handler:
/*
		cli
*/
		pusha
#if 1
		inb	MASTER_8259A_DATA, %al
		orb	$2, %al
		outb	%al, MASTER_8259A_DATA
		movb	$0x20, %al
		outb	%al, MASTER_8259A_COM
		sti
#endif
		pushl	$33
		call	interrupt
		popl	%eax
/*
		movb	$0x21, %al	# set EOI
		outw	%al, MASTER_8259A_COM
*/
#if 0
#if 1
		movb	$0x20, %al	# set EOI
		cli
		outb	%al, MASTER_8259A_COM
#else
		cli
		inb	MASTER_8259A_DATA, %al
		andb	$0xFD, %al
		outb	%al, MASTER_8259A_DATA
#endif
#endif
		popa
		iret

/**************************************************************************
 * int34_handler --- CRTV
 *
 *	no work.
 */
int34_handler:
		pusha
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
		popa
		iret

/**************************************************************************
 * int35_handler --- INT0
 *
 *	no work.
 */
int35_handler:
		cli
		pusha
		pushl	$35
		call	interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
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
 * int36_handler --- RS232C
 *
 *	no work.
 */
int36_handler:
		pusha
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
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
		pushl	$37
		call	interrupt
		popl	%eax
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
		popa
		iret
/*	
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		outw	%al, %dx
		popa
		iret
*/
	
/**************************************************************************
 * int38_handler --- floppy disk
 *
 *	no work.
 */
int38_handler:
/*
		cli
*/
		pusha
#if 1
		inb	MASTER_8259A_DATA, %al
		orb	$0x40, %al
		outb	%al, MASTER_8259A_DATA
		movb	$0x20, %al
		outb	%al, MASTER_8259A_COM
		sti
#endif
		pushl	$38
		call	interrupt
		popl	%eax
#if 0
		movb	$0x20, %al	/* set EOI */
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
#endif
#ifdef notdef	
		movb	$0x20, %al	# set EOI
		outw	%al, $0x20
#endif
	
		popa
		iret

/**************************************************************************
 * int39_handler --- connect to slave 
 *
 *	no work.
 */
int39_handler:
		pusha
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
		popa
		iret

/**************************************************************************
 * int40_handler --- Interval timer.
 *
 *	no work.
 */
int40_handler:
		cli
		pusha
		pushl	$40
		call	interrupt
		popl	%eax
		movb	$0x20, %al
		movw	SLAVE_8259A_COM, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_40_l1
		movb	$0x20, %al
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
int_40_l1:
		sti
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
 * int41_handler --- INT3(SASI/SCSI HD)
 *
 *	
 */
int41_handler:
		cli
		pusha
		pushl	$41
		call	interrupt
		popl	%eax
		movb	$0x20, %al
		movw	SLAVE_8259A_COM, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_41_l1
		movb	$0x20, %al
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
int_41_l1:
		sti
		popa
		iret

/**************************************************************************
 * int42_handler --- INT42 (FD 640K)
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
		movw	SLAVE_8259A_COM, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_42_l1
		movb	$0x20, %al
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
int_42_l1:
		popa
		iret

/**************************************************************************
 * int43_handler --- INT43 (FD 1M)
 *
 *
 */
int43_handler:
		cli
		pusha
		pushl	$43
		call	interrupt
		popl	%eax
		movb	$0x20, %al
		movw	SLAVE_8259A_COM, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_43_l1
		movb	$0x20, %al
		movw	MASTER_8259A_COM, %dx
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
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
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
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
		popa
		iret

/**************************************************************************
 * int46_handler --- IDE 0
 *
 *	no work.
 */
int46_handler:
		cli
		pusha
#if 1
		inb	SLAVE_8259A_DATA, %al
		orb	$0x40, %al
		outb	%al, SLAVE_8259A_DATA
		movb	$0x20, %al
		outb	%al, MASTER_8259A_COM
		jmp	int46_out	# delay
int46_out:	outb	%al, SLAVE_8259A_COM
		sti
#endif
		pushl	$46
		call	interrupt
		popl	%eax
#ifdef notdef
		movb	$0x20, %al
		movw	SLAVE_8259A_COM, %dx
		cli
		outb	%al, %dx
		movb	$0x0b, %al
		outb	%al, %dx
		inb	%dx, %al
		test	%al, %al
		jnz	int_46_l1
		movb	$0x20, %al
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
int_46_l1:
		sti
#endif
		popa
		iret

#ifdef notdef	
		cli
		pusha
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		outb	%al, %dx
		popa
		iret
#endif

/**************************************************************************
 * int47_handler --- IDE 1
 *
 *	no work.
 */
int47_handler:
		pusha
		movb	$0x20, %al	# set EOI
		movw	MASTER_8259A_COM, %dx
		cli
		outb	%al, %dx
		popa
		iret

/**************************************************************************
 * int64_handler --- ITRON system call.
 *
 *	�����ƥॳ��������ߤν�����
 *	���Τ��ᡢ¾�γ����ߤȰۤʤꡢEOI �ʤɤν�����Ԥ�ʤ���
 *
 *
 *	�����ƥॳ��������Ǥϼ��Τ��Ȥ�ֲ���פ��Ƥ��롣
 *	1) �����ƥॳ�����ֹ�ϡ��쥸���� eax �˼�����Ƥ��롣
 *	2) Ʊ���������ƥॳ����ؤΰ�������Ƭ���Ϥϡ��쥸���� ebx ��
 *	   ������Ƥ��롣
 *	3) �����ƥॳ���뤫����֤��ͤϡ��쥸���� eax �˼�����롣
 *	   (�桼���ץ������Ϥ��Τ��Ȥ���Τäơפ��뤳�Ȥ��ꤷ�Ƥ���)
 *
 */
syscall_handler:
int64_handler:
		pushfl
/*		pushal */
		nop
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* ��������Ƭ���� */
		pushl	%eax	/* �����ƥॳ�����ֹ� */
		movw	$0x10, %ax
		movw	%ax,%ds 
		movw	%ax,%es 

		call 	syscall
#if 0	
		popl	%ecx	/* �����ƥॳ����ΰ����ν��� */
		popl	%ecx
#endif
		movw	$0x23, %cx 
		movw	%cx,%ds    
		movw	%cx,%es    
		leave
/*		popal		 */
		nop
		popfl
		iret

/**************************************************************************
 * int65_handler --- POSIX system call.
 *
 *	�����ƥॳ��������ߤν�����
 *	���Τ��ᡢ¾�γ����ߤȰۤʤꡢEOI �ʤɤν�����Ԥ�ʤ���
 *
 *
 *	�����ƥॳ��������Ǥϼ��Τ��Ȥ�ֲ���פ��Ƥ��롣
 *	1) �����ƥॳ�����ֹ�ϡ��쥸���� eax �˼�����Ƥ��롣
 *	2) Ʊ���������ƥॳ����ؤΰ�������Ƭ���Ϥϡ��쥸���� ebx ��
 *	   ������Ƥ��롣
 *	3) �����ƥॳ���뤫����֤��ͤϡ��쥸���� eax �˼�����롣
 *	   (�桼���ץ������Ϥ��Τ��Ȥ���Τäơפ��뤳�Ȥ��ꤷ�Ƥ���)
 *
 */
posix_handler:
int65_handler:
		pushfl
/*		pushal */
		nop
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* ��������Ƭ���� */
		pushl	%eax	/* �����ƥॳ�����ֹ� */
		pushl	$65	/* trap �ֹ� */
		movw	$0x10, %ax
		movw	%ax,%ds 
		movw	%ax,%es 

		call 	trap
#if 0
		popl	%ecx	/* trap �ֹ�ν��� */
		popl	%ecx	/* �����ƥॳ����ΰ����ν��� */
		popl	%ecx
#endif
		movw	$0x23, %cx 
		movw	%cx,%ds    
		movw	%cx,%es    
		leave
/*		popal		 */
		nop
		popfl
		iret

#ifdef notdef	
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* ��������Ƭ���� */
		pushl	%eax	/* �����ƥॳ�����ֹ� */
		movw	$0x10, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		call 	trap
		movw	$0x23, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		iret
#endif	

/**************************************************************************
 * int66_handler --- BTRON system call.
 *
 *	�����ƥॳ��������ߤν�����
 *	���Τ��ᡢ¾�γ����ߤȰۤʤꡢEOI �ʤɤν�����Ԥ�ʤ���
 *
 *	�����ƥॳ��������Ǥϼ��Τ��Ȥ�ֲ���פ��Ƥ��롣
 *	1) �����ƥॳ�����ֹ�ϡ��쥸���� eax �˼�����Ƥ��롣
 *	2) Ʊ���������ƥॳ����ؤΰ�������Ƭ���Ϥϡ��쥸���� ebx ��
 *	   ������Ƥ��롣
 *	3) �����ƥॳ���뤫����֤��ͤϡ��쥸���� eax �˼�����롣
 *	   (�桼���ץ������Ϥ��Τ��Ȥ���Τäơפ��뤳�Ȥ��ꤷ�Ƥ���)
 *
 */
_btron_handler:
int66_handler:
		pushl 	%ebp
		movl	%esp,%ebp
		pushl	%ebx	/* ��������Ƭ���� */
		pushl	%eax	/* �����ƥॳ�����ֹ� */
		movw	$0x10, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		call 	syscall
		movw	$0x23, %ax
		movw	%ax,%ds
		movw	%ax,%es
		movw	%ax,%fs
		movw	%ax,%gs		# and ds to gs
		iret
		

lock_count:	.word	0
/***************************************************************************
 *
 */
dis_int:
		cli
		ret

/*************************************************************************
 *
 */
ena_int:
		sti
		ret

/*****************************************************************************
 *
 */
open_interrupt:
		sti
		ret

	
	
.globl	call_syscall

/* call_syscall: ���ѷ������ƥॳ����롼����
 *
 * . �����å�����:
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
call_syscall:
		pushl 	%ebp
		movl	%esp,%ebp
		movl	8(%ebp), %eax
		leal	12(%ebp), %ebx
		int	$64
		leave	
		ret