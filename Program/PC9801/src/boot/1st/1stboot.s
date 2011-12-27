; <1stboot.s> --- �ե������ȥ֡���.���ֺǽ�˥��ɤ����.
;
; $Date: 2011/12/27 17:13:36 $
; $Author: liu1 $
; $Log: 1stboot.s,v $
; Revision 1.1  2011/12/27 17:13:36  liu1
; Initial Version.
;
; Revision 1.2  1994-11-19 07:20:48  night
; ���ιԤ���Ƭʸ���� # ���� ; ���ѹ�������
;
; Revision 1.1  1993/10/11  21:28:05  btron
; btron/386			
;
; Revision 1.1.1.1  93/01/14  12:30:10  btron
; BTRON SYSTEM 1.0
;
; Revision 1.1.1.1  93/01/13  16:50:17  btron
; BTRON SYSTEM 1.0
; 
;Revision 1.3  92/09/10  02:44:00  night
;reti ������ jmpi ���Ѥ���褦���ѹ�������
;
;Revision 1.2  92/09/08  02:01:37  night
;�����Ȥν���
;
; Revision 1.1  92/09/08  00:47:55  night
; Initial revision
;
; Discription
;	�ܥץ����ϡ�BTRON/386 �Υ����ͥ���ɥץ����Ǥ��롣
;
;	���Υץ����ϡ��ƣĤ���֡��Ȥ��뤳�Ȥ�����Ȥ��Ƥ��롣
;	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;	���Υץ���ब�ɣУ̤�����ɤ��줿�����쥸������ξ��֤ϼ���
;	�褦�ˤʤäƤ��롣
;
;	AX,BX,CX,DX			����
;
;	���ɥ��ɥ쥹	��������	0x1FC0
;			���ե��å�	0x0000
;	���ɥ�����			1024 �Х���
;
; ��������
;
;	��ʬ���Ȥ򥳥ԡ� (0x1FC00 -> 0x70000)
;	2nd BOOT ��������
;	2nd BOOT ���Τ���� (GDT, IDT, Page table ��ޤ�)
;	2nd BOOT ����Ƭ�˥�����
;

seg_2ndboot = 		0x0800
off_2ndboot = 		0x0000

seg_2ndboot_info =	0x7800
off_2ndboot_info =	0x0000
seg_size_info =		0x7800
off_size_info =		0x0000
seg_loadpoint =		0x0100		; loadpoint = 0x1000 
off_loadpoint =		0x0000
;info_sector	=	2		; 1K bytes/sector 
info_sector	=	3		; 512 bytes/sector
info_cylinder	=	0

.text
begtext:
.data
begdata:
.bss
begbss:

.text

entry	first_boot

first_boot:
		mov	ax, cs
		mov	ss, ax
		mov	sp, #0xffff
		; ��ʬ���Ȥ򥳥ԡ�

		mov	cx, #0x200
		mov	ax,cs
		mov	ds,ax
		mov	si, #0
		mov	ax, #0x7000
		mov	es,ax
		mov	di, #0
		cld
copy_1stboot:
		lodsw
		stosw
		loop	copy_1stboot

		jmpi	#restart, #0x7000
;		mov	ax, #0x7000
;		push	ax
;		mov	ax, #restart
;		push	ax
;		reti			; restart �˥�����

restart:	
		mov	ax, cs
		mov	ds, ax
		mov	es, ax
		mov	ss,ax
		mov	sp,#0xffff

		call	console_init
		mov	bx, #message
		call	print
		call	read2ndboot

		mov	ax, #seg_2ndboot		; 2nd boot �θƤӽФ�
		push	ax
		mov	ax, #off_2ndboot
		push	ax
		reti

;
;	console_init --- ���󥽡��������
;	
console_init:
		movb	ah,*0x03	; init keyboard.
		int	0x18
		mov	ax,#0x0a00	; init screen.	--- set screen mode.
		int	0x18
		movb	ah,*0x0e	; init screen.	--- set vram address.
		xor	dx,dx
		int	0x18
		xor	dx,dx		; init screen.	--- initialize vram.
		movb	dh,*0xe1
		movb	ah,*0x16
		int	0x18
		movb	ah,*0x0c	; init screen.	--- start display.
		int	0x18
		movb	ah,*0x11	; init screen.	--- start cursol.
		int	0x18
		ret

print:
		cld
		mov	si,bx
		mov	ax,#0xa000
		mov	es,ax
		mov	di,cline
		xor	ax,ax
l1:
		lodb
		cmpb	al,*0x0
		jz	print_done
		stow
		j	l1

print_done:
		mov	ax, #160
		add	ax, cline
		mov	cline,ax
		ret

message: 	.ascii	"1st boot for btron/386"
		.byte	0
message2:	.ascii	"loading 2nd boot..."
		.byte	0

cline:		.word	0
;
;	fatal --- ����Ū�ʼ���; ̵�¥롼�פ�����
;
fatal:
		j	fatal		

;
;	read2ndboot --- 2nd BOOT �ץ����Υ���
;
read2ndboot:
		; 2nd BOOT �ξ�������
		mov	bx, #boot2_message
		call	print
		mov	seg_addr, #seg_size_info
		mov	off_addr, #off_size_info
		movb	sector, *info_sector
		movb	cylinder, *info_cylinder
		call	readdisk

		; �ɤ߼�ä������Ȥä�FD����2nd BOOT���Τ��ɤ߼��
		movb	sector, *5
		movb	cylinder, *0
		mov	seg_addr, #seg_loadpoint
		mov	off_addr, #off_loadpoint
		mov	ax, #seg_size_info
		mov	es, ax
		seg es
		mov	ax, off_size_info

load_loop:
		cmp	ax, #0
		jz	loop_done
		push	ax

;		cmpb	sector, *9
		cmpb	sector, *16
		jne	l2

		mov	sector, *1		; �������ֹ��1���᤹
		incb	cylinder

l2:
		call	readdisk

;		add	seg_addr, #0x0040	; 1024 bytes/sector
		add	seg_addr, #0x0020	; 512 bytes/sector
		pop	ax
		dec	ax
		incb	sector
		jmp	load_loop

loop_done:
		mov	bx, #read_done_message
		call	print
		jmpi	#off_2ndboot, #seg_2ndboot
;		mov	ax, #seg_2ndboot
;		push	ax
;		mov	ax, #off_2ndboot
;		push	ax
;		reti		; 2nd BOOT �إ�����
				
loop_done:
		jmp	fatal			

boot2_message:	.ascii	"loading 2nd boot..."
		.byte	0
read_done_message:	.ascii	"loaded 2nd boot..."
		.byte	0
boot2_count:	.word	0

;
;	disk io routines
;
; 	�ǥ�����������ꤷ�����������ɤ߹��ࡣ
;	BIOS �롼���� 0x1B ����Ѥ��롣
;
readdisk:
		movb	al, cylinder
		shr	al, *1
		movb	cl, al			; �������ֹ�Υ��å�
		movb	al, cylinder
		and	al, *0x01
		movb	dh, al			; �إå��ֹ�Υ��å�
		movb	dl, sector		; �������ֹ�Υ��å�
		mov	ax, seg_addr
		mov	es, ax
		mov	bp, off_addr
;		mov	bx, #0x400		; 1K bytes / sector
		mov	bx, #0x200		; 512 bytes / sector
;		movb	ch, *0x03		; 1K bytes / sector
		movb	ch, *0x02		; 512 bytes / sector
		movb	al, *0x90
;		movb	ah, *0x56		; 1K bytes / sector
		movb	ah, *0x76		; 512 bytes/sector
		int	0x1B
		cmpb	ah, *0
		jz	read_ok
		mov	bx, #fatal_message
		call	print
		call	fatal
read_ok:
		ret

seg_addr:	.word	0	; �Хåե��Υ�������
off_addr:	.word	0	; �Хåե��Υ��ե��å�
cylinder:	.byte	0	; �������ֹ�
sector:		.byte	0	; �������ֹ�(������ˤϡ��إå��ֹ��ޤޤ��)
; read_count:	.word	0	; �ɤ߹���֥�å���(512 ñ��)
fatal_message:	.ascii	"can't read 2ndboot program from FD."
		.byte	0
.text
endtext:

.data
enddata:

.bss
endbss:

		end	
	
