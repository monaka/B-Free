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
#include "itron_module.h"
#include "errno.h"
#include "misc.h"
#ifdef PORTMANAGER
#include "../servers/port-manager.h"
#endif
#include "func.h"
#ifdef KERNLIB
#include "../kernlib/device.h"
#endif

/*	���ȥ�ӥ塼�Ȥ�����
 */
#define NORM_ATTR	0x1e		/* �̾�ʸ�� 			*/
#define MAX_WIDTH	79
#define MAX_HEIGHT	24

#ifdef IBMPC
#define TEXT_VRAM_ADDR	0x800B8000
#define TEXT_VRAM_SIZE	2000

#define GDC_ADDR	0x03d4
#define GDC_DATA	0x03d5
#else
#define TEXT_VRAM_ADDR	0x800A0000	/* TEXT VRAM �Υ��ɥ쥹		*/
#define TEXT_VRAM_SIZE	2000

#define GDC_STAT	0x0060
#define GDC_COMMAND	0x0062
#endif


#define CURSOR_POS(x,y)		(x + y * 80)

#if 0
static W	mydevid;
#endif
static ER	call_console (ID driver, TC ch);

ID		console_driver = 0;
ID		console_recv;



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
  return (TRUE);
}

/************************************************************************
 * intr_crt
 */
W
intr_crt ()
{
  return 0;  /* dummy value */
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
putchar (TC ch)
{
#if 0
  TC	c;
  static B before_ch;
#endif

  if (console_driver != NULL)
    {
      call_console (console_driver, ch);
#ifdef notdef
      return;
#endif
    }
#ifdef ntodef
  dis_int ();
#endif

  switch (ch)
    {
    default:
      write_vram (cursor.x, cursor.y, ch, NORM_ATTR);
      inc_cursor (1);
      break;

    case '\n':
      write_cr ();
      break;

    case '\t':
      write_tab ();
      break;
    }
#ifdef ntodef
  ena_int ();
#endif
}


/* call_console
 *
 */
static ER
call_console (ID driver, TC ch)
{
  DDEV_REQ		req;		/* �׵�ѥ��å� */
  DDEV_RES		res;		/* �����ѥ��å� */
  W			rsize;
  ER			error;
#if 0
  W			i;
#endif
  
  ena_int ();
  req.header.mbfid = console_recv;
  req.header.msgtyp = DEV_WRI;
  req.body.wri_req.dd = 0xAA;
  req.body.wri_req.size = 1;
  req.body.wri_req.dt[0] = (B) (ch & 0xff);
  error = snd_mbf (console_driver, sizeof (req), &req);
  if (error != E_OK)
    {
      return (E_SYS);
    }
  rsize = sizeof (res);
  error = rcv_mbf (&res, (INT *)&rsize, console_recv);
  if (res.body.wri_res.errcd != E_OK)
    {
      return (E_SYS);
    }
  return (E_OK);
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
putchar_tron (TC ch)
{
  TC c;

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
#ifdef PC9801
  set_cursor_pos (cursor.x, cursor.y);
#endif
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
#if 0
  short *attr_addr;
#endif
  
  addr = (short *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
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
#if 0
  H	*attr_addr;
#endif
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
  W	addr;
#ifdef notdef  
  dis_int ();
#endif
  addr = CURSOR_POS (x, y);
  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  outb (GDC_DATA, addr & 0xff);
#ifdef notdef
  ena_int ();
#endif
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


/*
 * ���ߤΥ���������֤��֤�
 */
ER
get_cursor_position (W *x, W *y)
{
  *x = cursor.x;
  *y = cursor.y;
  return (E_OK);  
}
