!
!
! <1stboot.s> --- ¥Õ¥¡¡¼¥¹¥È¥Ö¡¼¥È.°ìÈÖºÇ½é¤Ë¥í¡¼¥É¤µ¤ì¤ë.
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
! 10¥Ó¥Ã¥È¤¢¤ë¥·¥ê¥ó¥ÀÈÖ¹æ¤Î¾å°Ì 2 ¥Ó¥Ã¥È¤ò»È¤¨¤ë¤è¤¦¤ËÊÑ¹¹¤·¤¿¡£
! (º£¤Þ¤Ç¤Ï 8 ¥Ó¥Ã¥È¤·¤«Í­¸ú¤Ç¤Ê¤«¤Ã¤¿)
! ¤³¤ÎÊÑ¹¹¤Î·ë²Ì¡¢¥·¥ê¥ó¥À¿ô¤¬ 256 °Ê¾å¤Î HD ¤ËÂÐ¤·¤Æ¤â¥Ö¡¼¥È¤Ç¤­
! ¤ë¤è¤¦¤Ë¤Ê¤Ã¤¿¡£
!
! ¤Ê¤ª¡¢¤³¤³¤Ç½ñ¤¤¤Æ¤¤¤ë¥·¥ê¥ó¥À¿ô¤Ï BIOS ¤Ë¤ÆÊÑ´¹¤µ¤ì¤¿¸å¤ÎÃÍ¡£
!
! Revision 1.4  1998/02/23 15:34:23  night
! Dynabook ¤Î HD ¤Ë¹ç¤ï¤»¤ÆÀßÄê¤òÊÑ¹¹¤·¤¿¡£
!
! Revision 1.3  1997/08/31 14:23:22  night
! ¥Æ¥¹¥ÈÍÑ´Ä¶­¤ÎÊÑ¹¹¤ËÈ¼¤¤¡¢HD ¤ÎÀßÄê¾ðÊó¤òÊÑ¹¹¤·¤¿¡£
!
! Revision 1.2  1997/06/29 13:12:16  night
! HD ¤Î¥¸¥ª¥á¥È¥ê¾ðÊó¤ª¤è¤Ó 2nd boot ¤ÎÆþ¤Ã¤Æ¤¤¤ë¥Ñ¡¼¥Æ¥£¥·¥ç¥ó¾ðÊó¤ÎÊÑ¹¹¡£
!
# Revision 1.1  1996/08/11  15:09:47  night
# ºÇ½é¤ÎÅÐÏ¿
#
!
! Discription
!	ËÜ¥×¥í¥°¥é¥à¤Ï¡¢BTRON/386 ¤Î¥«¡¼¥Í¥ë¥í¡¼¥É¥×¥í¥°¥é¥à¤Ç¤¢¤ë¡£
!
!	¤³¤Î¥×¥í¥°¥é¥à¤Ï¡¢£È£Ä (IDE) ¤«¤é¥Ö¡¼¥È¤¹¤ë¤³¤È¤òÁ°Äó¤È¤·¤Æ¤¤¤ë¡£
!	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
!
!	¤³¤Î¥×¥í¥°¥é¥à¤¬£É£Ð£Ì¤«¤é¥í¡¼¥É¤µ¤ì¤¿»þ¡¢¥ì¥¸¥¹¥¿Îà¤Î¾õÂÖ¤Ï¼¡¤Î
!	¤è¤¦¤Ë¤Ê¤Ã¤Æ¤¤¤ë¡£
!
!	AX,BX,CX,DX			ÉÔÄê
!
!	¥í¡¼¥É¥¢¥É¥ì¥¹	¥»¥°¥á¥ó¥È	0x07C0
!			¥ª¥Õ¥»¥Ã¥È	0x0000
!	¥í¡¼¥É¥µ¥¤¥º			512 ¥Ð¥¤¥È
!
! ½èÍýÆâÍÆ
!
!	¼«Ê¬¼«¿È¤ò¥³¥Ô¡¼ (0x07C0 -> 0x70000)
!	2nd BOOT ¾ðÊó¤ò¥í¡¼¥É
!	2nd BOOT ËÜÂÎ¤ò¥í¡¼¥É (GDT, IDT, Page table ¤ò´Þ¤à)
!	2nd BOOT ¤ÎÀèÆ¬¤Ë¥¸¥ã¥ó¥×
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

		call	read2ndboot			! 2nd boot ¤ÎÆÉ¤ß¹þ¤ß

		mov	bx, #done_message
		call	print

		mov	ax, #seg_2ndboot		! 2nd boot ¤Î¸Æ¤Ó½Ð¤·
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
		! 2nd BOOT ¤Î¾ðÊó¤ò¥í¡¼¥É
		mov	bx, #boot2_message
		call	print

		mov	seg_addr, #seg_size_info
		mov	off_addr, #off_size_info

		movb	ah, #0x0d			! HD ¤Î¥ê¥»¥Ã¥È
		movb	dl, #0x80
		int	0x13

		movb	bl, s_sector			! ¥Ñ¡¼¥Æ¥£¥·¥ç¥ó¤Î³«»Ï°ÌÃÖ¤ò»ØÄê
		movb	sector, bl			!

		mov	bx, s_cylinder			!
		mov	cylinder, bx			!

		movb	bl, s_head			!
		movb	head, bl			!
		inc	sector				! 1st boot ¤ÎÊ¬ (1 sector Ê¬¤ò
		call	readdisk			! ¥¤¥ó¥¯¥ê¥á¥ó¥È)
		inc	sector

		! ÆÉ¤ß¼è¤Ã¤¿¾ðÊó¤ò»È¤Ã¤ÆFD¤«¤é2nd BOOTËÜÂÎ¤òÆÉ¤ß¼è¤ë
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
		cmpb	sector, bl		! ¥»¥¯¥¿ÈÖ¹æ¤¬¾å¸Â¤ò±Û¤¨¤¿
		jne	l2

		movb	sector, #1		! ¥»¥¯¥¿ÈÖ¹æ¤ò1¤ËÌá¤¹
		inc	head			! ¼¡¤Î¥Ø¥Ã¥É¤Ø°ÜÆ°

		movb	bl, n_head		! ¥Ø¥Ã¥ÉÈÖ¹æ¤¬¾å¸Â¤ò±Û¤¨¤¿
		cmp	head, bl		!
		jne	l3			
		inc	cylinder		! ¼¡¤Î¥·¥ê¥ó¥À¤Ø°ÜÆ°
		movb	head, #0		! ¥Ø¥Ã¥É¤ò 0 ¤ËÊÑ¹¹

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
!	²¼¼õ¤±´Ø¿ô·²
!
!
! print --- Ê¸»úÎó¤ò¥³¥ó¥½¡¼¥ë¤ËÉ½¼¨¤¹¤ë¡£
!
! °ú¿ô¡§
!	bx	É½¼¨¤¹¤ëÊ¸»úÎó¤Î¥¢¥É¥ì¥¹
!		Ê¸»úÎó¤Ï¡¢<Ä¹¤µ>:1¥Ð¥¤¥È
!			  <Ê¸»úÎó>:ºÇÂç255¥Ð¥¤¥È
!		¤È¤Ê¤Ã¤Æ¤¤¤ë¡£
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
!	fatal --- ¹±µ×Åª¤Ê¼ºÇÔ! Ìµ¸Â¥ë¡¼¥×¤ËÆþ¤ë
!
fatal:
		j	fatal

!
!	disk io routines
!
! 	¥Ç¥£¥¹¥¯¤«¤é»ØÄê¤·¤¿¥»¥¯¥¿¤òÆÉ¤ß¹þ¤à¡£
!	BIOS ¥ë¡¼¥Á¥ó 0x13 ¤ò»ÈÍÑ¤¹¤ë¡£
!
readdisk:
		push	ax
		push	bx
		push	cx
		push	dx

		movb	dl, drive		! ¥É¥é¥¤¥ÖÈÖ¹æ

		mov	ax, cylinder
		mov	ch, al			! ¥·¥ê¥ó¥ÀÈÖ¹æ¤Î¥»¥Ã¥È(²¼°Ì 8 ¥Ó¥Ã¥È)

		movb	al, head
		movb	dh, al			! ¥Ø¥Ã¥ÉÈÖ¹æ¤Î¥»¥Ã¥È

		movb	cl, sector		! ¥»¥¯¥¿ÈÖ¹æ¤Î¥»¥Ã¥È
		shl	ah, 6			! ¥»¥¯¥¿ÈÖ¹æ¤Î¾å°Ì 2 ¥Ó¥Ã¥È¤Ë¥·¥ê¥ó¥ÀÈÖ¹æ¤Î
		add	cl, ah			! ¾å°Ì 2 ¥Ó¥Ã¥È¤ò¤â¤Ã¤Æ¤¯¤ë¡£

		mov	ax, seg_addr		! ÆÉ¤ß¹þ¤à¾ì½ê¤Î¥»¥°¥á¥ó¥ÈÃÍ
		mov	es, ax
		mov	bx, off_addr		! ÆÉ¤ß¹þ¤à¾ì½ê¤Î¥ª¥Õ¥»¥Ã¥ÈÃÍ
		movb	al, #0x01
		movb	ah, #0x02
		int	0x13			! BIOS ¤Î¸Æ¤Ó½Ð¤·
		jc	fatal

		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret


! ¥Æ¥ó¥Ý¥é¥êÊÑ¿ô·²
seg_addr:	.word	0	! ¥Ð¥Ã¥Õ¥¡¤Î¥»¥°¥á¥ó¥È
off_addr:	.word	0	! ¥Ð¥Ã¥Õ¥¡¤Î¥ª¥Õ¥»¥Ã¥È
cylinder:	.word	0	! ¥·¥ê¥ó¥ÀÈÖ¹æ
sector:		.byte	0	! ¥»¥¯¥¿ÈÖ¹æ
head:		.byte	0	! ¥Ø¥Ã¥ÀÈÖ¹æ
fatal_message:	
		.byte	35
		.ascii	"can't read 2ndboot program from FD."


!
! 	¥Ç¡¼¥¿·² (¼ç¤ËÊ¸»úÎó¡£¡£¡£)
!
message: 	.byte	28
		.ascii	"1st boot for btron/386(PC)"
		.byte   13, 10

!
!	read2ndboot --- 2nd BOOT ¥×¥í¥°¥é¥à¤Î¥í¡¼¥É
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

! ¥Ñ¡¼¥Æ¥£¥·¥ç¥ó¤Î³«»Ï°ÌÃÖ
.org	502
! HD ¤Î¾ì¹ç (¥Ñ¡¼¥Æ¥£¥·¥ç¥ó 0)
s_cylinder:	.word 1
s_head:		.byte 2
s_sector:	.byte 3

.org	506
! HD ¤Î GEOMETORY ¾ðÊó
! Îã¡§Cylinder 255, Head 10, Sector 55
n_cylinder:	.word 4
n_head:		.byte 5		! old value is 16.
n_sector:	.byte 6		! ¥»¥¯¥¿¤À¤±¤Ï¡¢ºÇÂç¿ô + 1 ¤Ë¤¹¤ë¡£

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
	
