! <1stboot.s> --- ¥Õ¥¡¡¼¥¹¥È¥Ö¡¼¥È.°ìÈÖºÇ½é¤Ë¥í¡¼¥É¤µ¤ì¤ë.
!
! $Date: 2011/12/27 17:13:35 $
! $Log: 1stboot.S,v $
! Revision 1.1  2011/12/27 17:13:35  liu1
! Initial Version.
!
! Revision 1.2  1999-12-23 17:54:17  kishida0
! loader $B$N0LCV$r(B 1st loader $B$K$&$a$`MM$K%(%s%H%j$rDI2C$7$?(B
!
! Revision 1.1  1999/03/15 02:10:52  monaka
! renamed some asm files included cpp macros. *.s was obsolute.
!
! Revision 1.7  1998/12/09 00:01:27  monaka
! Minor changed. (This is not bug fix)
!
! Revision 1.6  1998/11/20 08:02:16  monaka
! *** empty log message ***
!
! Revision 1.5  1998/05/23 15:29:32  night
! Æ£±Ê¤µ¤ó¥Þ¥·¥óÍÑ¤ÎÊÑ¹¹¡£
! (¤¿¤À¤·¡¢¤³¤ÎÊÑ¹¹¤Ç¤Ï¡¢Æ£±Ê¤µ¤ó¥Þ¥·¥ó¤Ç¤Ï¤¦¤Þ¤¯Æ°¤«¤Ê¤«¤Ã¤¿)
!
! Revision 1.4  1996/07/06 13:10:58  night
! 2nd boot ¤Ë½èÍý¤ò°Ü¹Ô¤¹¤ëÁ°¤Ë 1 ÉÃ´Ö¤Î delay ¤òÃÖ¤¤¤¿¡£
!
# Revision 1.3  1996/05/11  14:36:41  night
# 1stboot ½ªÎ»»þ¤Î¥á¥Ã¥»¡¼¥¸¤òºï½ü
#
# Revision 1.2  1996/05/11  14:22:19  night
# 2nd boot ¤Î¾ðÊó¤¬Æþ¤Ã¤¿¥»¥¯¥¿¤òÊÑ¹¹(1 -> 2)
#
# Revision 1.1  1996/05/05  14:11:59  night
# 1st boot ¥×¥í¥°¥é¥à¡£
# BIOS ¤Ë¤è¤ê FD ¤«¤éÆÉ¤ß¹þ¤Þ¤ì¡¢2nd boot ¤òÆÉ¤ß¹þ¤à¡£
#
!
! Discription
!	ËÜ¥×¥í¥°¥é¥à¤Ï¡¢BTRON/386 ¤Î¥«¡¼¥Í¥ë¥í¡¼¥É¥×¥í¥°¥é¥à¤Ç¤¢¤ë¡£
!
!	¤³¤Î¥×¥í¥°¥é¥à¤Ï¡¢£Æ£Ä¤«¤é¥Ö¡¼¥È¤¹¤ë¤³¤È¤òÁ°Äó¤È¤·¤Æ¤¤¤ë¡£
!	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
info_sector	=	2		! 512 bytes/sector
info_cylinder	=	0

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
		call	delay
!		call	kill_motor
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

!
! stop motor.
!
kill_motor:
		push	dx
		mov	dx, #0x3f2
		xor	al, al
		outb
		pop	dx
		ret	
read2ndboot:	
		! BIOS ¤¬»ÈÍÑ¤¹¤ë FD ¥Ñ¥é¥á¡¼¥¿¤Î½é´ü²½
		mov	cx,#0
		sub	si,si
		mov	fs,cx
		mov	bx,#0x78		! fs:dl ¤¬¥Ñ¥é¥á¡¼¥¿¥Æ¡¼¥Ö¥ë¤Î¥¢¥É¥ì¥¹¤È¤Ê¤ë
		push	ds
		seg fs
		lds	si,(bx)			! ds:si ¤¬¥Æ¡¼¥Ö¥ë¤ËÆþ¤ì¤ë¾ðÊó¤Î¥³¥Ô¡¼¸µ

		mov	cl,#6			! copy 12 bytes
		cld
		push	di

		rep
		movsw

		pop	di
		pop	ds

		movb	4(di),*36		! patch sector count
	
		seg fs
		mov	(bx),di
		seg fs
		mov	2(bx),es
	
		! 2nd BOOT ¤Î¾ðÊó¤ò¥í¡¼¥É
		mov	bx, #boot2_message
		call	print
		mov	seg_addr, #seg_size_info
		mov	off_addr, #off_size_info
		movb	sector, *info_sector
		movb	cylinder, *info_cylinder
		call	readdisk

		! ÆÉ¤ß¼è¤Ã¤¿¾ðÊó¤ò»È¤Ã¤ÆFD¤«¤é2nd BOOTËÜÂÎ¤òÆÉ¤ß¼è¤ë
		movb	sector, *3
		movb	cylinder, *0
		mov	seg_addr, #seg_loadpoint
		mov	off_addr, #off_loadpoint
		mov	ax, #seg_size_info
		mov	es, ax
		seg es
		mov	ax, off_size_info

load_loop:
!		call	print_dot
		cmp	ax, #0
		jz	loop_done

		cmpb	sector, #19		! ¥»¥¯¥¿ÈÖ¹æ¤¬ 19 ¤Î¾ì¹ç
		jne	l2

		call	print_dot
		mov	sector, #1		! ¥»¥¯¥¿ÈÖ¹æ¤ò1¤ËÌá¤¹
		incb	cylinder

l2:
		call	readdisk
		add	seg_addr, #0x0020	! 512 bytes/sector
		dec	ax
		incb	sector
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

print_dot:
		pusha
		mov	ax, #0xe2e		! '.'
		int	0x10
		popa
		ret
		
!
!
!

!
!	fatal --- ¹±µ×Åª¤Ê¼ºÇÔ! Ìµ¸Â¥ë¡¼¥×¤ËÆþ¤ë
!
fatal:
		mov	bx, #fatal_message
		call	print
fatal_loop:
		jmp	fatal_loop

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

!		xor	ah, ah			! reset FDC
!		xor	dl, dl			! ¥É¥é¥¤¥ÖÈÖ¹æ
!		int	0x13
retry_reading:
		xor	dx, dx
		movb	al, cylinder
		shr	al, #1
		movb	ch, al			! ¥·¥ê¥ó¥ÀÈÖ¹æ¤Î¥»¥Ã¥È

		movb	al, cylinder
		and	al, #0x01
		movb	dh, al			! ¥Ø¥Ã¥ÉÈÖ¹æ¤Î¥»¥Ã¥È

		movb	cl, sector		! ¥»¥¯¥¿ÈÖ¹æ¤Î¥»¥Ã¥È

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
bad_rt:
		! reset FDC
		xor	ah, ah
		xor	dl, dl
		int	0x13

		! Print error message
		mov	bx, #retry_message
		call	print

		jmp	retry_reading


seg_addr:	.word	0	! ¥Ð¥Ã¥Õ¥¡¤Î¥»¥°¥á¥ó¥È
off_addr:	.word	0	! ¥Ð¥Ã¥Õ¥¡¤Î¥ª¥Õ¥»¥Ã¥È
cylinder:	.byte	0	! ¥·¥ê¥ó¥ÀÈÖ¹æ(¤³¤ÎÃæ¤Ë¤Ï¡¢¥Ø¥Ã¥ÉÈÖ¹æ¤â´Þ¤Þ¤ì¤ë)
sector:		.byte	0	! ¥»¥¯¥¿ÈÖ¹æ
fatal_message:	
		.byte	35
		.ascii	"can't read 2ndboot program from FD."


!
! 	¥Ç¡¼¥¿·² (¼ç¤ËÊ¸»úÎó¡£¡£¡£)
!
message: 	.byte	28
		.ascii	"1st boot for btron/386(PC)"
		.byte   13, 10

message_dot:	.byte	1
		.ascii	"."

retry_message:	.byte	1
		.ascii	"e"

boot2_message:
		.byte   21
		.ascii	"loading 2nd boot..."
		.byte   13, 10

done_message:
		.byte   20
		.ascii	"loaded 2nd boot..."
		.byte	13, 10


seg_addr:	.word	0	! ¥Ð¥Ã¥Õ¥¡¤Î¥»¥°¥á¥ó¥È
off_addr:	.word	0	! ¥Ð¥Ã¥Õ¥¡¤Î¥ª¥Õ¥»¥Ã¥È
cylinder:	.byte	0	! ¥·¥ê¥ó¥ÀÈÖ¹æ
sector:		.byte	0	! ¥»¥¯¥¿ÈÖ¹æ(¤³¤ÎÃæ¤Ë¤Ï¡¢¥Ø¥Ã¥ÉÈÖ¹æ¤â´Þ¤Þ¤ì¤ë)
! read_count:	.word	0	! ÆÉ¤ß¹þ¤à¥Ö¥í¥Ã¥¯¿ô(512 Ã±°Ì)
fatal_message:
		.byte	34
		.ascii	"can't read 2ndboot program from FD."
		.byte	0

.org    498
loader_offset:  .word 0xffff

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
	
