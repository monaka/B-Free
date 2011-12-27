/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* console.c --- ���󥽡���ؤ������ϥ⥸�塼��
 *
 *
 * ���󥽡���ؤ����Ϥ���ӽ��Ϥ�Ԥ���
 *
 * �ʤ���������������Ƥ��륳�󥽡��������ϴؿ����ϡ�ITRON ��٥�Τ��ᡢ
 * ����ؤǤϡ��̤ΥǥХ����ɥ饤�Ф������Ϥ�Ԥ����ޤ���ITRON ����ν��Ϥ⡢
 * ����ؤ��������ǥХ����ɥ饤�Ф�𤷤ƹԤ��������δؿ�������Ѥ���Τϡ�
 * Ω���夲ľ������Ǥ��롣
 *
 *
 */

#include "itron.h"
#include "errno.h"
#include "func.h"
#include "misc.h"

/*	���ȥ�ӥ塼�Ȥ�����
 */
#define NORM_ATTR	0x1e		/* �̾�ʸ�� 			*/
#define MAX_WIDTH	79
#define MAX_HEIGHT	24

#define TEXT_VRAM_ADDR	0x800A0000	/* TEXT VRAM �Υ��ɥ쥹		*/
#define TEXT_VRAM_SIZE	2000

#define GDC_STAT	0x0060
#define GDC_COMMAND	0x0062

#define CURSOR_POS(x,y)		(x + y * 80)

static W	mydevid;

/***********************************************************************
 * position --- ����������־���
 *
 */
struct position
{
  W	x;
  W	y;
};

static	struct position	cursor;


static void	set_cursor_pos (W x, W y);
static void	inc_cursor (W count);
static void	write_vram (W x, W y, W ch, W attr);
static void	write_kanji_vram (W x, W y, UW kanji, W attr);
static void	scroll_up (void);

/*************************************************************************
 * simple_init_console 
 *
 * ������
 *
 * ���͡�
 *
 * ������
 *
 */
void
simple_init_console (void)
{
  console_clear ();
}

/*************************************************************************
 * init_crt --- CRT �ν����
 *
 * ������	�ʤ�
 *
 * ���͡�	���󥽡���ǥХ����ΥǥХ�������
 *
 * ������	���󥽡���ǥХ�������������(���̤򥯥ꥢ����)��
 *		��������ݥ��� (cursor) ���������롣
 *		�ɥ饤�о�������������ƤӽФ��ؿ����֤���
 *
 * ��ա�	�ɥ饤�о���ϡ����餫���� GLOBAL �Ȥ���������Ƥ��뤬��
 *		�ɥ饤��̾�����ϡ���ȯ�Ķ��δ��������ɤ� TRON �����ɤ�
 *		�ʤ����ᡢ���δؿ��� TRON �����ɤ��Ѵ����롣
 *
 */
BOOL
init_crt (W id)
{
/*
  console_clear ();
  cursor.x = 0;
  cursor.y = 0;
  set_cursor_pos (cursor.x, cursor.y);
*/
/*  string_shift_to_tron (device_table[id].name); */
  mydevid = id;
  return (TRUE);
}

/************************************************************************
 * intr_crt
 */
W
intr_crt ()
{
}

/*************************************************************************
 * putchar --- ��ʸ������; �����ͥ���Υ����ӥ��롼����
 *
 *
 * ������	ch	���Ϥ���ʸ�������� (TRON ������)
 *
 * ���͡�	�ʤ�
 *
 * ������	write_vram ��ƤӽФ���VRAM �ˣ�ʸ�����Ϥ��롣
 *		���δؿ��ϡ������ͥ뤫���å���������Ϥ�����ˤ���Ѥ��롣
 *		���Τ��ᡢ���δؿ��� GLOBAL �Ȥ���������Ƥ��롣
 *
 *
 * ��ա�	���δؿ��ϡ����ϤǤ���ʸ�����ɤ���������å����Ƥ��ʤ���
 *		���ϤǤ��ʤ�ʸ�������ꤵ�줿���Ǥ⡢VRAM �˽񤭹��⤦�Ȥ��롣
 *
 */
void
putchar (TCODE ch)
{
  TCODE	c;
  static B before_ch;

  dis_int ();
#ifdef notdef
  if ((ch >= 0x80) && (ch <= 0xff) && (before_ch == 0))
    {
      before_ch = ch;
      return;
    }
  else if (before_ch)
    {
      ch = (before_ch << 8) | ch;
      write_kanji_vram (cursor.x, cursor.y, c, NORM_ATTR);
      inc_cursor (2);
      before_ch = 0;
      return;
    }
#endif /* notdef */

  switch (ch)
    {
    default:
#ifdef notdef
      if (ch & 0xFF00)
	{
	  c = halfword_swap (ch);
	  write_kanji_vram (cursor.x, cursor.y, c, NORM_ATTR);
	  inc_cursor (2);
	}
      else
	{
	  write_vram (cursor.x, cursor.y, ch, NORM_ATTR);
	  inc_cursor (1);
	}
#else
      write_vram (cursor.x, cursor.y, ch, NORM_ATTR);
      inc_cursor (1);
#endif /* notdef */
      break;

    case '\n':
      write_cr ();
      break;

    case '\t':
      write_tab ();
      break;
    }
  ena_int ();
}

/*************************************************************************
 * putchar_tron --- ��ʸ������; �����ͥ���Υ����ӥ��롼����
 *
 *
 * ������	ch	���Ϥ���ʸ�������� (TRON ������)
 *
 * ���͡�	�ʤ�
 *
 * ������	write_vram ��ƤӽФ���VRAM �ˣ�ʸ�����Ϥ��롣
 *		���δؿ��ϡ������ͥ뤫���å���������Ϥ�����ˤ���Ѥ��롣
 *		���Τ��ᡢ���δؿ��� GLOBAL �Ȥ���������Ƥ��롣
 *
 *
 * ��ա�	���δؿ��ϡ����ϤǤ���ʸ�����ɤ���������å����Ƥ��ʤ���
 *		���ϤǤ��ʤ�ʸ�������ꤵ�줿���Ǥ⡢VRAM �˽񤭹��⤦�Ȥ��롣
 *
 */
void
putchar_tron (TCODE ch)
{
  TCODE	c;

  switch (ch)
    {
    default:
      if (ch & 0xFF00)
	{
	  c = halfword_swap (ch);
	  write_kanji_vram (cursor.x, cursor.y, c, NORM_ATTR);
	  inc_cursor (2);
	}
      else
	{
	  write_vram (cursor.x, cursor.y, ch, NORM_ATTR);
	  inc_cursor (1);
	}
      break;

    case '\n':
      write_cr ();
      break;

    case '\t':
      write_tab ();
      break;
    }
}

/*************************************************************************
 * inc_cursor --- ���������ʤ�롣
 *
 * ������	count	��������ΰ�ư��
 *
 * ���͡�	�ʤ�
 *
 * ������	���ꤷ���̤��������������ʤ��(count �ϡ���������2�Ǥ���)��
 *		�⤷���������뤬���̤�Ϥߤ��������ϡ���������򼡤ιԤ�
 *		�ʤ�롣
 *		����˰��ֲ��ιԤˤ�����ˤϡ�����ʾ岼�ιԤˤ����ʤ��Τǡ�
 *		�������륢�åפ��롣
 *
 */
static void
inc_cursor (W count)
{
  cursor.x += count;
  if (cursor.x >= MAX_WIDTH)
    {
      cursor.y++;
      if (cursor.y >= MAX_HEIGHT)
	{
	  scroll_up ();
	  cursor.y = MAX_HEIGHT;
	}
      cursor.x = 0;
    }
  set_cursor_pos (cursor.x, cursor.y);
}

/*************************************************************************
 * write_cr --- ���Ԥ���
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	��������򼡤ιԤ���Ƭ�˰�ư�����롣
 *		�⤷�����Ǥ˹Ԥ����ֺǸ�ʤ�С��������Τ򥹥����뤹�롣
 *
 */
void
write_cr (void)
{
  cursor.x = 0;
  if (cursor.y >= MAX_HEIGHT)
    {
      scroll_up ();
    }
  else
    {
      cursor.y++;
    }
  set_cursor_pos (cursor.x, cursor.y);
}

/*************************************************************************
 * write_tab --- ���֤���Ϥ��롣
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	���֤���Ϥ��롣
 *
 */
void
write_tab ()
{
  W	tmp;

  if (cursor.x < MAX_WIDTH)
    {
      tmp = ROUNDUP (cursor.x + 1, 8) - 1;
      while (cursor.x < tmp)
	{
	  write_vram (cursor.x, cursor.y, ' ', 0xe1);
	  cursor.x++;
	}
    }
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
  for (i = 0; i <= MAX_WIDTH; i++)
    {
      write_vram (i, MAX_HEIGHT, ' ', 0xe1);
    }
}

/*************************************************************************
 * console_clear --- ���󥽡���򥯥ꥢ���롣
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	���󥽡���ΤϤ�����Ϥ��ޤǤ����ʸ�������롣
 *
 */
void
console_clear (void)
{
  W	x, y;
  
  for (y = 0; y <= MAX_HEIGHT; y++)
    for (x = 0; x <= MAX_WIDTH; x++)
      {
	write_vram (x, y, ' ', 0xE1);
      }
  cursor.x = cursor.y = 0;
  set_cursor_pos (cursor.x, cursor.y);
}

/*************************************************************************
 * write_vram --- VRAM ��ʸ����񤭹���
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
 *
 * ��ա�	���ߡ�attr ��̵�뤷�Ƥ��롣
 *
 */
static void
write_vram (W x, W y, W ch, W attr)
{
  short	*addr;
  short *attr_addr;
  
  addr = (short *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = ch & 0x00ff;
}

/*************************************************************************
 * write_kanji_vram --- �֣ң��ͤ˴�����ʸ���񤭹���.
 *
 * ������	x, y	��ɸ
 *		kanji	���Ϥ���ʸ��
 *		attr	ʸ��°��
 *
 * ���͡�	�ʤ�
 *
 * ������	���� x, y �ǻ��ꤷ����ɸ��ʸ����񤭹���.
 *		���δؿ��ϡ�TRON ʸ���ѤˤʤäƤ��롣
 *
 *
 * ��ա�	���ߡ�attr ��̵�뤷�Ƥ��롣
 *
 */
static void
write_kanji_vram (W x, W y, UW kanji, W attr)
{
  UH	*addr;
  H	*attr_addr;
  UH	 first, second;

  first = ((kanji >> 8) & 0xff) - 0x20;
  second = (kanji << 8) & 0xff00;

  addr = (unsigned short *)TEXT_VRAM_ADDR;
  addr [CURSOR_POS (x, y)] = second | first;
  addr [CURSOR_POS (x + 1, y)] = second | (first | 0x80);
}

/*************************************************************************
 * set_cursor_pos --- ����������֤�����.
 *
 * ������	x, y	��ɸ
 *
 * ���͡�	�ʤ�
 *
 * ������	���ꤷ����ɸ�˥���������ư�����롣
 *
 */
static void
set_cursor_pos (W x, W y)
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

/*************************************************************************
 * delete_key --- ʸ�����ʸ���õ�롣
 *
 * ������	�ʤ�
 *
 * ���͡�	�ʤ�
 *
 * ������	���ߥ������뤬����ԤκǸ��ʸ����õ�롣
 *
 */
void
delete_key (void)
{
  if (cursor.x > 0)
    {
      cursor.x--;
      write_vram (cursor.x, cursor.y, ' ', 0x1f);
      set_cursor_pos (cursor.x, cursor.y);
    }
}


/*************************************************************************
 * move_cursol --- �����������ꤷ�����֤˰�ư
 *
 * ������	x
 *		y
 *
 * ���͡�	TRUE
 *		FALSE
 *
 * ������	����ѿ� cursor �ˤϡ�����������֤����äƤ��롣
 *		move_cursol() �ϡ�cursor ���ͤ��ѹ����롣
 *		����ˡ����������ɽ�����֤򿷤��� cursor �ΰ��֤��ѹ����롣
 *
 */
W
move_cursol (int x, int y)
{
  if ((x >= 0) && (x < 80) &&
      (y >= 0) && (y < 25))
    {
      cursor.x = x;
      cursor.y = y;
      set_cursor_pos (cursor.x, cursor.y);
      return (TRUE);
    }
  else
    {
      return (FALSE);
    }
}
