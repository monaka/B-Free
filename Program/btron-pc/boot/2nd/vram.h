/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT VRAM routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/vram.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: vram.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1996-05-11 15:52:44  night
 * VRAM のアドレスを変更。
 * ビデオコントローラのアドレスを定義。
 *
 * Revision 1.1  1996/05/11  10:45:08  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * -------------------------------------------------------------------
 *
 * Revision 1.2  1995/09/21  15:50:44  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:30:04  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:29  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:27  btron
 * BTRON SYSTEM 1.0
 * 
 */

#define TEXT_VRAM_ADDR	0x0B8000
#define TEXT_VRAM_SIZE	2000

#define GDC_STAT	0x0060
#define GDC_COMMAND	0x0062

#define GDC_ADDR	0x03d4
#define GDC_DATA	0x03d5

#define CURSOR_POS(x,y)		(x + y * 80)

void	write_vram (int x, int y, int ch, int attr);
void	write_kanji_vram (int x, int y, unsigned int ch, int attr);
void	scroll_up ();     
void	set_cursor_pos (int x, int y);
