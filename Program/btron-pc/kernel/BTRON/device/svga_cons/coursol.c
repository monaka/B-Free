/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/svga_cons/coursol.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
/*
 * $Log: coursol.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1999-06-20 14:47:22  kishida0
 * *** empty log message ***
 *
 * Revision 1.1  1999/05/05 17:53:57  kishida0
 * for debug
 *
 *
 */

static void     scroll_up (void);
static void     write_vram_xy (W x, W y, W ch, W attr);
static void     caridge_return (void);
static void     next_tab (void);
static void     backspace (void);

struct position
{
  W     x;
  W     y;
};

/* ʣ��Window�򳫤�ͽ��������Ȥꤢ����console���롼�������ä���ƶ�̣ */
static struct position  svga_curpos[256];
static struct position  curpos;


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
 * ������       x, y    ��ɸ
 *
 * ���͡�       �ʤ�
 *
 * ������       ���ꤷ����ɸ�˥���������ư�����롣
 *
 */
void
goto_cursol (W x, W y)
{
  W     addr;
  
  dis_int ();
  addr = x + (y * 80);

  outb (GDC_ADDR, 0x0e);
  outb (GDC_DATA, (addr >> 8) & 0xff);
  outb (GDC_ADDR, 0x0f);
  ena_int ();
}

/*
 *
 */

write_char (UB  ch)
{
  if (ch < 0x20)        /* ����ȥ��륳���� */
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
 * ������       x, y    ��ɸ
 *              ch      ���Ϥ���ʸ��
 *              attr    ʸ��°��
 *
 * ���͡�       �ʤ�
 *
 * ������       ���� x, y �ǻ��ꤷ����ɸ��ʸ����񤭹���.
 *              ���δؿ��ϡ�ASCII ʸ���ѤˤʤäƤ��롣
 *
 * ��ա�       ���ߡ�attr ��̵�뤷�Ƥ��롣
 *
 */
static void
write_vram_xy (W x, W y, W ch, W attr)
{
  short *addr;
  short *attr_addr;
  
#define CURSOR_POS(x,y)         (x + y * 80)
  addr = (short *)TEXT_VRAM_ADDR;
  ch = ch | (addr [CURSOR_POS (x, y)] & 0xff00);
  addr [CURSOR_POS (x, y)] = ch;
}


/*************************************************************************
 * scroll_up --- �������Τ򥹥����륢�åפ���
 *
 * ������       �ʤ�
 *
 * ���͡�       �ʤ�
 *
 * ������       �������Τ��ԥ������뤹�롣
 *
 */
static void
scroll_up (void)  
{
  UH    *addr;
  W             i;
  
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
  W     x, y;
  
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

