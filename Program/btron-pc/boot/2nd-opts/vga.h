/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/vga.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: vga.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.3  1999/03/15 01:35:35  monaka
 * Minor modify. Function printf was renamed to boot_printf. Some cast was added. and so on.
 *
 * Revision 1.2  1998/11/20 08:02:41  monaka
 * *** empty log message ***
 *
 * Revision 1.1  1997/08/31 14:22:45  night
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

#define VGA_VRAM_ADDRESS		0xA0000


/* Sequencer register のパラメータ設定 */
#define SR_MAP_MASK			0x02

/* Graphics controller register のパラメータ定義 */
#define	VGA_SET_RESET			0x00
#define	VGA_ENABLE_SET_RESET		0x01
#define	VGA_COLOR_COMP			0x02
#define	VGA_FUNCTION			0x03
#define	VGA_READ_MAP_SELECT		0x04
#define	VGA_MODE				0x05
#define	VGA_MISC				0x06
#define	VGA_CDONT_CARE			0x07
#define	VGA_BIT_MASK			0x08

/*
	VGA_SET_RESET,	VGA_ENABLE_SET_RESET register のパラメータ定義 
*/
#define	VGA_PLANE1			0x01
#define	VGA_PLANE2			0x02
#define	VGA_PLANE3			0x04
#define	VGA_PLANE4			0x08

/* VGA_FUNCTION register のパラメータ */
#define	VGA_OVERWRITE			0x00
#define	VGA_AND				0x10
#define	VGA_OR				0x20
#define	VGA_XOR				0x30

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

extern int	vga_text (void);
extern void	vga_graph (void);
extern void	write_vga_port (UWORD16 *data, int length, ULONG port);
extern void	clear_vga (int color);
extern void	point_vga (ULONG x, ULONG y, int color, int rastop );
extern void	line_vga (int x0, int x1, int y0, int y1 ,int color, int mode);
extern void	stick_bitmap (int x, int y, int x_length, int y_length, unsigned char *bitmap, int color, int bkcolor);


#endif /* __VGA_H__ */

