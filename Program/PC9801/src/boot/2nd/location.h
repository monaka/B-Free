/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/***********************************************************************
  $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/location.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $

  $Log: location.h,v $
  Revision 1.1  2011/12/27 17:13:36  liu1
  Initial Version.

  Revision 1.3  1995-09-21 15:50:40  night
  ソースファイルの先頭に Copyright notice 情報を追加。

 * Revision 1.2  1994/11/13  15:04:44  night
 * カーネルアドレス(物理)を 0x00010000 に変更
 *
 * Revision 1.1  1993/10/11  21:29:28  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:23  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:24  btron
 * BTRON SYSTEM 1.0
 * 


  			L O C A T I O N 


 ************************************************************************/

#define KERNEL_ADDR	0x80010000
#define KERNEL_PADDR	0x00010000

#define GDT		0x1000
#define IDT		0x2000
#define PAGE_DIR_ADDR	0x3000
#define PAGE_ENTRY_ADDR	0x4000

#define START32		0x9000

/***********************************************************************
 *
 */
#define TEXT_VRAM_ADDR	0x0a0000
#define TEXT_VRAM_SIZE	2000
#define TEXT_ATTR_ADDR	0x0a2000

#define CLOCK_COM	0x20
#define CLOCK_DATA	0x33

#define KEY_COM		0x43
#define KEY_DATA	0x41
#define KEY_STAT	0x43

#define PAGE_ENTRY_SIZE	(16 * 1024) / 4

#define PAGE_FLAG	0x80000000
