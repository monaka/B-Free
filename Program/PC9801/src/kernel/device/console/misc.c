/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/misc.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: misc.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-12-05 14:17:30  night
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


#include "console.h"

/*  ���������
 */
#define NORM_ATTR	0x1e		/* �̾�ʸ�� 			*/

#define TEXT_VRAM_ADDR	0x800A0000	/* TEXT VRAM �Υ��ɥ쥹		*/
#define TEXT_VRAM_SIZE	2000

#define GDC_STAT	0x0060
#define GDC_COMMAND	0x0062

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
  W	adr;
  
  dis_int ();
  adr = x + (y * 80);
  while ((inb (GDC_STAT) & 0x04) == 0)
    ;
  outb (GDC_COMMAND, 0x49);
  outb (GDC_STAT, adr & 0xff);
  outb (GDC_STAT, (adr >> 8) & 0xff);
  ena_int ();
}

/*
 *
 */
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
	  if (curpos.y >= MAX_LINE)
	    {
	      curpos.x = 0;
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
  
#define CURSOL_POS(x,y)		(x + y * 80)
  addr = (short *)TEXT_VRAM_ADDR;
  addr [CURSOL_POS (x, y)] = ch & 0x00ff;
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
