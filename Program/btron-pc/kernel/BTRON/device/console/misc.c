/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char misc_rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/console/misc.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  2000-07-09 16:44:48  kishida0
 * fix some warning (sample for other devices)
 *
 * Revision 1.6  2000/02/07 18:04:36  kishida0
 * minor fix
 *
 * Revision 1.5  1999/03/21 12:49:24  night
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
 * Revision 1.4  1997/08/31 14:00:36  night
 * console_internal.h �Υ��󥯥롼�ɤ��ɲá�
 *
 * Revision 1.3  1996/07/24  16:43:23  night
 * �ޥ��� CURSOL_POS �� CURSOR_POS ��̾�����ѹ���
 *
 * Revision 1.2  1996/07/24  16:42:25  night
 * �ƥ����� VRAM �˽񤭹���Ȥ��ν����� PC9801 �Τ�Τ��� IBM PC �Ѥ��ѹ�
 * ������
 *
 * Revision 1.1  1996/07/24  16:03:59  night
 * �ǽ����Ͽ
 *
 * Revision 1.4  1995/12/05 14:17:30  night
 * ���̤򥹥����뤹����˺ǲ��Ԥ�õ������Ǻǲ��� + 1 ��õ�Ƥ�����
 * ���Τ��ᡢ�ǲ��ԤϾõ�Ǥ���ʸ�����ĤäƤ��ޤä���
 * �������ǲ��Ԥ�õ��褦���ѹ�������
 *
 * Revision 1.3  1995/09/21  15:50:55  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/17  16:51:06  night
 * ʸ��������Ѥδؿ������������
 *
 * Revision 1.1  1995/09/14  04:29:20  night
 * �ǽ����Ͽ
 *
 *
 */


#include "console_internal.h"

#define C(x)	((x) & 0x1f)

static void	scroll_up (void);
static void	write_vram_xy (W x, W y, W ch, W attr);
static void	caridge_return (void);
static void	next_tab (void);
static void	backspace (void);

struct position
{
  W	x;
  W	y;
};

static struct position	curpos;


void
set_curpos (W x, W y)
{
  curpos.x = x;
  curpos.y = y;
  goto_cursol (x, y);
}

void
move_curpos (W x, W y)
{
  curpos.x += x;
  if (curpos.x < 0) curpos.x = 0;
  else if (curpos.x >= MAX_COLUMN) curpos.x = MAX_COLUMN-1;
  curpos.y += y;
  if (curpos.y < 0) curpos.y = 0;
  else if (curpos.y >= MAX_COLUMN) curpos.y = MAX_LINE-1;
  goto_cursol (curpos.x, curpos.y);
}

/*************************************************************************
 * goto_cursol --- ����������֤�����.
 *
 * ������	x, y	��ɸ
 *
 * ���͡�	�ʤ�
 *
 * ������	���ꤷ����ɸ�˥���������ư�����롣
 *
 */
void
goto_cursol (W x, W y)
{
  W	addr;
  
  dis_int ();
  addr = x + (y * 80);

  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  outb (GDC_DATA, addr & 0xff);
  ena_int ();
}

/*
 *
 */
ER
write_char (UB	ch)
{
  if (ch < 0x20)	/* ����ȥ��륳���� */
    {
      switch (ch)
	{
	case '\n':
	  caridge_return ();
	  break;

	case '\t':
	  next_tab ();
	  break;

	case '\b':
	  backspace ();
	  break;
	}
    }
  else if (ch <= 0x7f)
    {
      write_vram_xy (curpos.x, curpos.y, ch, 0);
      curpos.x++;
      if (curpos.x >= MAX_COLUMN)
	{
	  curpos.y++;
	  curpos.x = 0;
	  if (curpos.y >= MAX_LINE)
	    {
	      curpos.y = MAX_LINE - 1;
	      scroll_up ();
	    }
	}
    }
  goto_cursol (curpos.x, curpos.y);
  return (E_OK);
}

static void
caridge_return (void)
{
  curpos.x = 0;
  curpos.y++;
  if (curpos.y >= MAX_LINE)
    {
      curpos.x = 0;
      curpos.y = MAX_LINE - 1;
      scroll_up ();
    }
}


static void
next_tab ()
{
  if ((curpos.x % 8) == 0)
    {
      curpos.x += 8;
    }
  else
    {
      curpos.x += (8 - (curpos.x % 8));
    }

  if (curpos.x >= MAX_COLUMN)
    {
      curpos.y++;
      if (curpos.y >= MAX_LINE)
	{
	  curpos.x = 0;
	  curpos.y = MAX_LINE - 1;
	  scroll_up ();
	}
    }
}


static void
backspace ()
{
  if (curpos.x <= 0)
    {
      if (curpos.y > 0)
	{
	  curpos.x = MAX_COLUMN - 1;
	  curpos.y--;
	}
    }
  else
    {
      curpos.x--;
    }
  write_vram_xy (curpos.x, curpos.y, ' ', 0);
  goto_cursol (curpos.x, curpos.y);
}

/*************************************************************************
 * write_vram_xy --- VRAM ��ʸ����񤭹���
 *
 * ������	x, y	��ɸ
 *		ch	���Ϥ���ʸ��
 *		attr	ʸ��°��
 *
 * ���͡�	�ʤ�
 *
 * ������	���� x, y �ǻ��ꤷ����ɸ��ʸ����񤭹���.
 *		���δؿ��ϡ�ASCII ʸ���ѤˤʤäƤ��롣
 *
 * ��ա�	���ߡ�attr ��̵�뤷�Ƥ��롣
 *
 */
static void
write_vram_xy (W x, W y, W ch, W attr)
{
  short	*addr;
  short *attr_addr;
  
#define CURSOR_POS(x,y)		(x + y * 80)
  addr = (short *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
}

/*************************************************************************
 * scroll_up --- �������Τ򥹥����륢�åפ���
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	�������Τ��ԥ������뤹�롣
 *
 */
static void
scroll_up (void)  
{
  UH	*addr;
  W		i;
  
  addr = (unsigned short *)TEXT_VRAM_ADDR;
  for (i = 0; i < TEXT_VRAM_SIZE - 80; i++)
    {
      addr[i] = addr[i + 80];
    }
  for (i = 0; i <= MAX_COLUMN; i++)
    {
      write_vram_xy (i, MAX_LINE - 1, ' ', 0xe1);
    }
}


void
clear_console (void)
{
  W	x, y;
  
  for (y = 0; y < MAX_LINE; y++)
    for (x = 0; x < MAX_COLUMN; x++)
      {
	write_vram_xy (x, y, ' ', 0xE1);
      }
  set_curpos (0, 0);
}

void
clear_rest_line(void)
{
  W x, y;
  y = curpos.y;
  for(x = curpos.x+1; x < MAX_COLUMN; x++) {
    write_vram_xy (x, y, ' ', 0xE1);
  }
}

void
clear_rest_screen(void)
{
  W x, y;
  clear_rest_line();
  for (y = curpos.y+1; y < MAX_LINE; y++)
    for (x = 0; x < MAX_COLUMN; x++)
      {
	write_vram_xy (x, y, ' ', 0xE1);
      }
}
