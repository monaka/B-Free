/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/console.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: console.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.16  2000-02-27 15:25:16  naniwa
 * minor fix
 *
 * Revision 1.15  2000/02/17 14:21:07  naniwa
 * fixed scroll_vga
 *
 * Revision 1.14  2000/02/07 18:04:34  kishida0
 * minor fix
 *
 * Revision 1.13  2000/01/30 19:02:43  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.12  2000/01/29 16:15:11  naniwa
 * to work with POSIX fcntl
 *
 * Revision 1.11  2000/01/24 23:34:31  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.10  2000/01/23 15:46:29  kishida0
 * ����ե��å����̤� scroll ��ǽ���ɲä��� (�ޤ��Х����ʤΤ����)
 *
 * Revision 1.9  1999/05/15 05:01:18  naniwa
 * add ellipse_vga(), fillellipse_vga()
 *
 * Revision 1.8  1999/04/28 13:12:45  naniwa
 * Addition of splbitmap
 *
 * Revision 1.7  1999/04/18 18:43:19  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.6  1999/03/21 12:49:22  night
 * ϲ��(naniwa@mechgw.mech.yamaguchi-u.ac.jp) ����λ�Ŧ�ˤ���ѹ���
 * ------------------
 * ���������ס��������󥹤�������Ƥ�������˵����դ�����Ǥ�����
 * console �ϴ��˥Хå����ڡ����ˤ��б�����Ƥ�����Ǥ��� (^^;)��frtm
 * �����Ϥˤ� '\b' ���Ȥ���н�ʬ�ʤΤǡ����������ץ������󥹤���ɬ
 * �פ�̵���ʤäƤ��ޤä��ΤǤ������޳ѤʤΤǺǸ�ޤǼ������ޤ�����
 *
 * �����������������ס��������󥹤λ��ͤǤ������ mail �Υߥ���������
 * �ޤ�����
 *
 * ESC[Pl;PcH	��������� (Pc,Pl) �ذ�ư���ǥե���ȤϤ��줾�� 1��
 * ESC[PnA		��������� Pn �Ծ�ذ�ư���ǥե���Ȥ� 1��
 * 		���̾�ü����ؤΰ�ư��̵�롥
 * ESC[PnB		��������� Pn �Ծ�ذ�ư���ǥե���Ȥ� 1��
 * 		���̲�ü��겼�ؤΰ�ư��̵�롥
 * ESC[PnC		��������� Pn �屦�ذ�ư���ǥե���Ȥ� 1��
 * 		���̱�ü��걦�ؤΰ�ư��̵�롥
 * ESC[PnD		��������� Pn �庸�ذ�ư���ǥե���Ȥ� 1��
 * 		���̱�ü��꺸�ؤΰ�ư��̵�롥
 * ESC[2J		���̤�õ����������� (1,1) �ء�
 * ESC[J		ɽ���ν���ޤǤ�õ
 * ESC[K		����������֤���Ԥν���ޤǤ�õ
 *
 * ��������κ�ɸ�Ϻ������ (1,1)�������� (MAX_COLUMN, MAX_LINE)��
 *
 * kernel/BTRON/device/console �β��� patch -p1 �Ǥ��Ƥ�褦�˥ѥå�
 * ����ޤ�����
 * ------------------
 *
 * Revision 1.5  1997/09/10 13:07:22  night
 * �ե�������ǻ��Ȥ��Ƥ���ޥ����������Ƥ��� device.h �򥤥󥯥롼�ɤ�
 * ��褦�ˤ�����
 *
 * Revision 1.4  1997/08/31 13:59:11  night
 * �ɥ饤�������ǻ��Ѥ�������� console_internal.h ��ʬΥ������
 *
 * Revision 1.3  1997/03/25 13:34:38  night
 * ELF �����μ¹ԥե�����ؤ��б�
 *
 * Revision 1.2  1996/11/11  13:27:52  night
 * ���󥯥롼�ɤ���إå��ե�����Υѥ�̾���ѹ�������
 *
 * Revision 1.1  1996/07/24  16:03:59  night
 * �ǽ����Ͽ
 *
 * Revision 1.5  1995/12/05 14:16:13  night
 * device.h �� #include ʸ���ѹ�������
 *
 * Revision 1.4  1995/09/21  15:50:55  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.3  1995/09/17  16:49:26  night
 * MAX_COLUMN �� MAX_LINE �������
 * ���󥽡��������ѤΥ��ޥ�� (CONSOLE_CLEAR��CONSOLE_MOVE) �������
 * �����δؿ��������
 * ��Ԥä���
 *
 * Revision 1.2  1995/09/14  04:27:47  night
 * init_console() �����ؿ����ѹ���
 * send_reply() ���ɲá�
 *
 * 
 *
 *
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__	1


#include "../../../ITRON/kernlib/device.h"

/*  ���������
 */

#define CONSOLE_DRIVER	"driver.console"

#define MAX_COLUMN	80
#define MAX_LINE	25

#define TEXT_MODE	0
#define GRAPHIC_MODE	1


/* Console control */
#define CONSOLE_CLEAR	0x00000001
#define CONSOLE_MOVE	0x00000002
#define CONSOLE_GRAPHIC	0x00000003
#define CONSOLE_TEXT	0x00000004

/* ����ե��å��⡼�ɻ��Υ��ޥ�� 
 *
 * ����ط� (���Ȥ�)
 */
#define GR_NULL		0x00008000
#define GR_POINT	0x00008001
#define GR_LINE		0x00008002
#define GR_BOX		0x00008003
#define GR_FILLBOX	0x00008004
#define GR_CIRCLE	0x00008005
#define GR_SETBG	0x00008006
#define GR_SETBITMAP	0x00008007
#define GR_SPLBITMAP	0x00008008
#define GR_ELLIPSE	0x00008009
#define GR_FILLELLIPSE	0x0000800A
#define GR_SCROLL       0x0000800B

#define NORM_ATTR	0x1e		/* �̾�ʸ�� 			*/

#define SCROLL_UP       0
#define SCROLL_DOWN     1

#define TEXT_VRAM_ADDR	0x800B8000	/* TEXT VRAM �Υ��ɥ쥹		*/
#define TEXT_VRAM_SIZE	2000

#define GDC_ADDR	0x03d4
#define GDC_DATA	0x03d5

#define GDC_STAT	0x03d4
#define GDC_COMMAND	0x03d5


struct gr_point_t
{
  W	x, y;
  W	color;
  W	mode;
};


struct gr_line_t
{
  W	x1, y1;
  W	x2, y2;
  W	color;
  W	mode;
};

struct gr_box_t
{
  W	x, y;
  W	width, height;
  W	color;
};

struct gr_fillbox_t
{
  W	x, y;
  W	width, height;
  W	color;
};

struct gr_circle_t
{
  W	x, y;
  W	r;
  W	color;
};


struct gr_setbg_t
{
  ID	caller;
  W	width, height;
  W	size;
  VP	bitmap;
  W	frcolor;
  W	bgcolor;
};


struct gr_setbitmap_t
{
  ID	caller;
  W	x, y;
  W	width, height;
  W	size;
  VP	bitmap;
  W	frcolor;
  W	bgcolor;
};


struct gr_splbitmap_t
{
  ID	caller;
  W	x, y;
  W	width, height;
  W	size;
  VP	bitmap;
  W	frcolor;
};


struct gr_ellipse_t
{
  W	x, y;
  W	width, height;
  W	color;
};

struct gr_scroll_t
{
  W     direc;
  W     x, y;
  W     width, height;
  W     step;
  W     color;
};

union gr_cmd_t
{
  struct gr_point_t	point;
  struct gr_line_t	line;
  struct gr_box_t	box;
  struct gr_fillbox_t	fillbox;
  struct gr_circle_t	circle;
  struct gr_setbg_t	setbg;
  struct gr_setbitmap_t	setbitmap;
  struct gr_splbitmap_t	splbitmap;
  struct gr_ellipse_t	ellipse;
  struct gr_scroll_t    scroll;
};

#endif __CONSOLE_H__


