!
!
! <1stboot.s> --- �ե������ȥ֡���.���ֺǽ�˥����ɤ����.
!
! $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/1st/1stboot_hd.S,v 1.1 2011/12/27 17:13:35 liu1 Exp $
!
! $Log: 1stboot_hd.S,v $
! Revision 1.1  2011/12/27 17:13:35  liu1
! Initial Version.
!
! Revision 1.2  1999-12-23 17:54:18  kishida0
! loader $B$N0LCV$r(B 1st loader $B$K$&$a$`MM$K%(%s%H%j$rDI2C$7$?(B
!
! Revision 1.1  1999/03/15 02:10:54  monaka
! renamed some asm files included cpp macros. *.s was obsolute.
!
! Revision 1.5  1998/06/11 14:37:24  night
! 10�ӥåȤ��륷�����ֹ�ξ�� 2 �ӥåȤ�Ȥ���褦���ѹ�������
! (���ޤǤ� 8 �ӥåȤ���ͭ���Ǥʤ��ä�)
! �����ѹ��η�̡����������� 256 �ʾ�� HD ���Ф��Ƥ�֡��ȤǤ�
! ��褦�ˤʤä���
!
! �ʤ��������ǽ񤤤Ƥ��륷�������� BIOS �ˤ��Ѵ����줿����͡�
!
! Revision 1.4  1998/02/23 15:34:23  night
! Dynabook �� HD �˹�碌��������ѹ�������
!
! Revision 1.3  1997/08/31 14:23:22  night
! �ƥ����ѴĶ����ѹ���ȼ����HD �����������ѹ�������
!
! Revision 1.2  1997/06/29 13:12:16  night
! HD �Υ�����ȥ���󤪤�� 2nd boot �����äƤ���ѡ��ƥ�����������ѹ���
!
# Revision 1.1  1996/08/11  15:09:47  night
# �ǽ����Ͽ
#
!
! Discription
!	�ܥץ������ϡ�BTRON/386 �Υ����ͥ�����ɥץ������Ǥ��롣
!
!	���Υץ������ϡ��ȣ� (IDE) ����֡��Ȥ��뤳�Ȥ�����Ȥ��Ƥ��롣
!	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
!
!	���Υץ�����ब�ɣУ̤�������ɤ��줿�����쥸������ξ��֤ϼ���
!	�褦�ˤʤäƤ��롣
!
!	AX,BX,CX,DX			����
!
!	�����ɥ��ɥ쥹	��������	0x07C0
!			���ե��å�	0x0000
!	�����ɥ�����			512 �Х���
!
! ��������
!
!	��ʬ���Ȥ򥳥ԡ� (0x07C0 -> 0x70000)
!	2nd BOOT ����������
!	2nd BOOT ���Τ������ (GDT, IDT, Page table ��ޤ�)
!	2nd BOOT ����Ƭ�˥�����
!

seg_2ndboot = 		0x0800
off_2ndboot = 		0x0000

seg_2ndboot_info =	0x7800
off_2ndboot_info =	0x0000
seg_size_info =		0x7800
off_size_info =		0x0000
seg_loadpoint =		0x0100		! loadpoint = 0x1000
off_loadpoint =		0x0000

.text
begtext:

.text

entry		first_boot

		.org	0x0000
first_boot:
		mov	ax,#0x07c0
		mov	ds,ax
		mov	ax,#0x7000
		mov	es,ax
		mov	cx,#512
		sub	si,si
		sub	di,di
		cld
		rep
		movsw
		jmpi	#restart, #0x7000

restart:
		mov	ax, #0x7000
		mov	es, ax
		mov	ss, ax
		mov	ds, ax
		mov	sp, #2048

		mov	bx, #message
		call	print

		call	read2ndboot			! 2nd boot ���ɤ߹���

		mov	bx, #done_message
		call	print

		mov	ax, #seg_2ndboot		! 2nd boot �θƤӽФ�
		push	ax
		mov	ax, #off_2ndboot
		push	ax
		reti

delay:
		mov	ax,#0x1000
		mov	bx,#0xffff
delay1:
		dec	bx
		jnz	delay1
		mov	bx,#1000
		dec	ax
		jnz	delay1
		ret
	
read2ndboot:
		! 2nd BOOT �ξ���������
		mov	bx, #boot2_message
		call	print

		mov	seg_addr, #seg_size_info
		mov	off_addr, #off_size_info

		movb	ah, #0x0d			! HD �Υꥻ�å�
		movb	dl, #0x80
		int	0x13

		movb	bl, s_sector			! �ѡ��ƥ������γ��ϰ��֤����
		movb	sector, bl			!

		mov	bx, s_cylinder			!
		mov	cylinder, bx			!

		movb	bl, s_head			!
		movb	head, bl			!
		inc	sector				! 1st boot ��ʬ (1 sector ʬ��
		call	readdisk			! ���󥯥����)
		inc	sector

		! �ɤ߼�ä������Ȥä�FD����2nd BOOT���Τ��ɤ߼��
		mov	seg_addr, #seg_loadpoint
		mov	off_addr, #off_loadpoint
		mov	ax, #seg_size_info
		mov	es, ax
		seg es
		mov	ax, off_size_info

load_loop:
		call	print_dot
		cmp	ax, #0
		jz	loop_done

		movb	bl, n_sector
		cmpb	sector, bl		! �������ֹ椬��¤�ۤ���
		jne	l2

		movb	sector, #1		! �������ֹ��1���᤹
		inc	head			! ���Υإåɤذ�ư

		movb	bl, n_head		! �إå��ֹ椬��¤�ۤ���
		cmp	head, bl		!
		jne	l3			
		inc	cylinder		! ���Υ������ذ�ư
		movb	head, #0		! �إåɤ� 0 ���ѹ�

l3:
l2:
		call	readdisk
		add	seg_addr, #0x0020	! 512 bytes/sector
		dec	ax
		inc	sector
		jmp	load_loop

loop_done:
!		mov	bx, #done_message
!		call	print
		ret
				

!
!	�������ؿ���
!
!
! print --- ʸ����򥳥󥽡����ɽ�����롣
!
! ������
!	bx	ɽ������ʸ����Υ��ɥ쥹
!		ʸ����ϡ�<Ĺ��>:1�Х���
!			  <ʸ����>:����255�Х���
!		�ȤʤäƤ��롣
print:
		push	ax
		push	bx
		push	cx
		push	dx

		push	bx
		mov	ah,#0x03		! get cursor pos
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
		ret

!
!
!
print_dot:
		push	ax
		mov	ax, #0xe2e		! '.'
		int	0x10
		pop	ax
		ret
		

!
!	fatal --- ����Ū�ʼ���! ̵�¥롼�פ�����
!
fatal:
		j	fatal

!
!	disk io routines
!
! 	�ǥ�����������ꤷ�����������ɤ߹��ࡣ
!	BIOS �롼���� 0x13 ����Ѥ��롣
!
readdisk:
		push	ax
		push	bx
		push	cx
		push	dx

		movb	dl, drive		! �ɥ饤���ֹ�

		mov	ax, cylinder
		mov	ch, al			! �������ֹ�Υ��å�(���� 8 �ӥå�)

		movb	al, head
		movb	dh, al			! �إå��ֹ�Υ��å�

		movb	cl, sector		! �������ֹ�Υ��å�
		shl	ah, 6			! �������ֹ�ξ�� 2 �ӥåȤ˥������ֹ��
		add	cl, ah			! ��� 2 �ӥåȤ��äƤ��롣

		mov	ax, seg_addr		! �ɤ߹�����Υ���������
		mov	es, ax
		mov	bx, off_addr		! �ɤ߹�����Υ��ե��å���
		movb	al, #0x01
		movb	ah, #0x02
		int	0x13			! BIOS �θƤӽФ�
		jc	fatal

		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret


! �ƥ�ݥ���ѿ���
seg_addr:	.word	0	! �Хåե��Υ�������
off_addr:	.word	0	! �Хåե��Υ��ե��å�
cylinder:	.word	0	! �������ֹ�
sector:		.byte	0	! �������ֹ�
head:		.byte	0	! �إå��ֹ�
fatal_message:	
		.byte	35
		.ascii	"can't read 2ndboot program from FD."


!
! 	�ǡ����� (���ʸ���󡣡���)
!
message: 	.byte	28
		.ascii	"1st boot for btron/386(PC)"
		.byte   13, 10

!
!	read2ndboot --- 2nd BOOT �ץ������Υ�����
!
read2ndboot:
		ret

boot2_message:
		.byte   21
		.ascii	"loading 2nd boot..."
		.byte   13, 10

done_message:
		.byte   25
		.ascii	"loading 2nd boot...done"
		.byte	13, 10



fatal_message:
		.byte	34
		.ascii	"can't read 2ndboot program from FD."
		.byte	0

.org    498
loader_offset:	.word   0xffff

.org    500

.org	501
drive:		.byte 0x80	!  0x0 = FD
				! 0x80 = HD

! �ѡ��ƥ������γ��ϰ���
.org	502
! HD �ξ�� (�ѡ��ƥ������ 0)
s_cylinder:	.word 1
s_head:		.byte 2
s_sector:	.byte 3

.org	506
! HD �� GEOMETORY ����
! �㡧Cylinder 255, Head 10, Sector 55
n_cylinder:	.word 4
n_head:		.byte 5		! old value is 16.
n_sector:	.byte 6		! �����������ϡ������ + 1 �ˤ��롣

.org	510
boot_mark:
		.word 0xAA55

.text
endtext:

!.data
!enddata:

!.bss
!endbss:

		end	
	