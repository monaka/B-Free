/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/vga.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: vga.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.11  2000-07-28 19:16:20  kishida0
 * fix vga XOR function
 *
 * Revision 1.10  2000/07/09 16:44:50  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.9  2000/02/17 14:21:06  naniwa
 * fixed scroll_vga
 *
 * Revision 1.8  2000/02/07 18:04:39  kishida0
 * minor fix
 *
 * Revision 1.7  2000/01/30 19:02:45  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.6  2000/01/24 23:34:30  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.5  2000/01/23 15:46:30  kishida0
 * グラフィック画面の scroll 機能を追加した (まだバギーなので注意)
 *
 * Revision 1.4  1999/05/15 05:01:20  naniwa
 * add ellipse_vga(), fillellipse_vga()
 *
 * Revision 1.3  1999/04/28 13:13:31  naniwa
 * Addition of stipple_bitmap().
 *
 * Revision 1.2  1999/02/15 09:14:31  monaka
 * Circle draw code support (but not yet tested enough...).
 *
 * Revision 1.1  1997/08/31 14:01:35  night
 * 最初の登録
 *
 *
 * 
 */


#ifndef __VGA_H__
#define __VGA_H__


/*
 * 各種レジスタのアドレス値
 */
#define    ATTRREG    0x3C0
#define    MISCREG    0x3C2
#define    SEQUREG    0x3C4
#define    GRAPHREG   0x3CE
#define    CRTCREG    0x3D4
#define    STATREG    0x3DA


#define VGA_GR_INDEX_PORT		0x3CE
#define VGA_SR_INDEX_PORT		0x3C4

#define VGA_VRAM_ADDRESS		0x800A0000U


/* Sequencer register のパラメータ設定 */
#define SR_MAP_MASK			0x02

/* Graphics controller register のパラメータ定義 */
#define	VGA_SET_RESET			0x00
#define	VGA_ENABLE_SET_RESET		0x01
#define	VGA_COLOR_COMP			0x02
#define	VGA_FUNCTION			0x03
#define	VGA_READ_MAP_SELECT		0x04
#define	VGA_MODE			0x05
#define	VGA_MISC			0x06
#define	VGA_CDONT_CARE			0x07
#define	VGA_BIT_MASK			0x08

/*
	VGA_SET_RESET,	VGA_ENABLE_SET_RESET register のパラメータ定義 
*/
#define	VGA_PLANE1			0x01
#define	VGA_PLANE2			0x02
#define	VGA_PLANE3			0x04
#define	VGA_PLANE4			0x08
#define ALL_PLANE	(VGA_PLANE1 | VGA_PLANE2 | VGA_PLANE3 | VGA_PLANE4)

/* VGA_FUNCTION register のパラメータ */
#define	VGA_OVERWRITE			0x00
#define	VGA_AND				0x08
#define	VGA_OR				0x10
#define	VGA_XOR				0x18

/* VGA_MODE register のパラメータ */
#define	VGA_WM0				0x00
#define	VGA_WM1				0x01
#define	VGA_WM2				0x02
#define	VGA_WM3				0x03
#define	VGA_RD0				0x00
#define	VGA_RD1				0x08

/* ビットオペレーション */
#define	MakeMask(index)		(1 << (index))
#define	BitTest(data,index)	((data) & MakeMask (index))

/* アドレス計算 */
#define	P2G(x,y)	((y) * 80 + (x))

extern W graphic_command (ID caller, DDEV_CTL_REQ *packet);

extern void	vga_graph (void);
extern W        vga_text(void);
extern void	write_vga_port (unsigned short *data, int length, unsigned int port);
extern void	change_mode ();
extern void	clear_vga (int color);
extern void	point_vga (W x, W y, W color, W mode);
extern void	line_vga (W x0, W x1, W y0, W y1 ,W color, W mode);
extern void	save_register ();
extern void	stick_bitmap (int x, int y, int x_length, int y_length, unsigned char *bitmap, int color, int bkcolor);
extern void	stipple_bitmap (int x, int y, int x_length, int y_length, unsigned char *bitmap, int color);
extern void	set_palette ();
extern void	get_palette ();
extern void	box_vga ();
extern void	fillbox_vga (W x, W y, W width, W height, W color, W restop);
extern void	circle_vga (W x, W y, W width, W height, W color, W restop);
extern void	setbg_vga (W width, W height, W size, VP bitmap, W color, W bgcolor);
extern void ellipse_vga(W x, W y, W width, W height, W color, W restop);
extern void fillellipse_vga(W x, W y, W width, W height, W color, W restop);
extern void scroll_vga (W direc, W x, W y, W width, W height, W step, W color);

#endif /* __VGA_H__ */

