/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/interrupt.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: interrupt.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.14  2000-04-03 14:30:03  naniwa
 * to call timer handler in task
 *
 * Revision 1.13  2000/02/27 15:28:41  naniwa
 * to work as multi task OS
 *
 * Revision 1.12  2000/02/17 14:22:34  naniwa
 * minor fix
 *
 * Revision 1.11  2000/01/30 19:02:49  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.10  2000/01/28 08:54:02  naniwa
 * minor fix on 101US key map
 *
 * Revision 1.9  2000/01/24 23:10:36  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.8  2000/01/24 23:05:06  kishida0
 * default 106JP -> 101US
 *
 * Revision 1.7  2000/01/23 15:50:50  kishida0
 * 106JP��101US �����ܡ������ؤ��ѤΥ��ޥ�ɤ��ɲä�����
 * �ؿ��ˤ��Ƥ���Τ����ޤ����������Ȥꤢ�����ϻ��Ѳ�ǽ
 * note-pc�ѤˤϤ⤦���������Υơ��֥���ǧ����ɬ�פ�����
 * �ץ�ݡ���
 *
 * Revision 1.6  1999/03/21 12:47:15  night
 * ϲ��(naniwa@mechgw.mech.yamaguchi-u.ac.jp) ����λ�Ŧ�ˤ���ѹ���
 * ------------------
 * > ���Ѥ�餺 b-free-pc.0.0.40 ��١����ˤ��Ƥ���ΤǤ�����'%' ������
 * > ����Ȥ��θ奷�եȥ������Ф���ȿ�������������ʤ�褦�Ǥ���101 ��
 * > 106 (¿ʬ) Keyboard �Ǥ��θ��ݤ��������ޤ�����
 * >
 * > SIFT ������ˤʤ�äѤʤ��ˤʤä��ꡤCPAS Lock ���ݤ��ä��ꡤ����
 * > ��ä��ꤷ�ޤ���
 *
 * ����Ǥ�����'%' �����Ϥ���Ȥ��˱��Υ��եȥ����򲡤��Ƥ����������
 * ���褦�Ǥ� (^^;)������ȡ�Cpas Lock ���ȥ���ư��Ƥ��륭���ܡ���
 * ��¿���Ȼפ��ޤ��Τǡ������ѤΥѥå����碌�ƺ�äƤߤޤ�����
 * # kernel/BTRON/device/keyboard �β��� patch -p1 �Ǥ��Ƥ��ΤǤ���
 * -------------------
 *
 * Revision 1.5  1998/12/07 01:07:52  monaka
 * Key bind table for 101 Keyboard fixed. You can type ! " # $ and so on.
 *
 * Revision 1.4  1998/11/26 19:14:57  monaka
 *
 * 106 keymap added.
 *
 * Revision 1.3  1996/07/27 14:25:07  night
 * �����ܡ��ɥޥåפΰ����ѹ���
 * SHIFT + '-' �����Ϥ��줿�Ȥ��� '_' ���֤��褦�ˤ�����
 *
 * Revision 1.2  1996/07/25  16:02:59  night
 * ������ȯ�����������ܡ��ɤ��Ф��� outb (0x16) ��¹Ԥ��Ƥ������ᡢ
 * ���ޤ�ʸ�������ɤ��ꤳ��ʤ��ä���
 * outb �ιԤ����ä���
 *
 * Revision 1.1  1996/07/24  16:28:44  night
 * IBM PC �� B-Free OS �ؤκǽ����Ͽ
 *
 * Revision 1.3  1995/09/21  15:50:59  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/09/19  18:01:01  night
 * �����ܡ��ɤ������ϤǤ���Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1995/09/18  11:42:03  night
 * �ǽ����Ͽ
 *
 *
 *
 */

#include "keyboard.h"
#include "keyboard_def.h"
#include "keyboard_type.h"

#define NORMAL_CODE	0
#define SHIFT_CODE	1
#define CONTROL_CODE	2
#define ALT_CODE	3

/*******************************************************************************
 *	���եȥ����ξ��֤�ɽ���ޥ������
 *
 */
#define NORMAL		0x0000
#define SHIFT_DOWN	0x0001
#define CONTROL_DOWN	0x0002
#define ALT_DOWN	0x0004

#define CAPS_DOWN	1

#define ISBREAK(ch)	(ch & 0x80)



W keyboard_select = K_101US;

UW	shiftkey_code;
UW	capskey;

static void	keyboard_interrupt ();


/*
 * �����ܡ��ɳ����ߥϥ�ɥ����Ͽ
 *
 */
init_keyboard_interrupt ()
{
  T_DINT	pk_dint;
  ER		errno;

  pk_dint.intatr = ATR_INTR;
  pk_dint.inthdr = (void (*)()) keyboard_interrupt;
  errno = def_int (KEYBOARD_INT, &pk_dint);
  if (errno != E_OK)
    {
      dbg_printf ("keyboard: %d\n", errno);
    }
  shiftkey_code = NORMAL;
}

static W
get_keycode(int cont,int num)
{
  if(keyboard_select == K_101US){
    return key_table_101[cont][num];
  }else{
    return key_table_106[cont][num];
  }
}
/*
 * �����ܡ��ɳ����ߤΥϥ�ɥ�
 *
 */
static void
keyboard_interrupt ()
{
  W	key_code;
  W	ch;

  key_code = inb (KEY_DATA);

  if (driver_mode & RAWMODE)
    {
      put_entry (key_code);
      set_flg (waitflag, ONKEYS);
      return;
    }

  switch (key_code)
    {
    case 0xf1:
      capskey = NORMAL;
      return;

    case 0x2a:
    case 0x36:
      shiftkey_code |= SHIFT_DOWN;
      return;

    case 0x3a:
      if (capskey == CAPS_DOWN) capskey = NORMAL;
      else capskey = CAPS_DOWN;
      return;

    case 0x38:
      shiftkey_code |= ALT_DOWN;
      return;

    case 0x1d:
      shiftkey_code |= CONTROL_DOWN;
      return;

    case 0xb8:
      shiftkey_code &= ~ALT_DOWN;
      return;

    case 0x9d:
      shiftkey_code &= ~CONTROL_DOWN;
      return;

    case 0xaa:
    case 0xb6:
      shiftkey_code &= ~SHIFT_DOWN;
      return;
    }
  
  /* �⤷��������Υ�����Ȥ���ʤ�С�̵�뤹�� */
  if (ISBREAK (key_code))
    return;
  
  /* �ޥȥ�å����ơ��֥뤫�顢���������ɤ���Ф���
   */
  if (shiftkey_code & CONTROL_DOWN)
    ch = get_keycode(CONTROL_CODE,key_code);
  else if (capskey)
    {
      if (shiftkey_code & SHIFT_DOWN)
	ch = get_keycode(NORMAL_CODE,key_code);
      else
	ch = get_keycode(SHIFT_CODE,key_code);
    }
  else if (shiftkey_code & SHIFT_DOWN)
    ch = get_keycode(SHIFT_DOWN,key_code);
  else
    ch = get_keycode(NORMAL_CODE,key_code);

#if 0
  /* DEBUG HERE! */
  dbg_printf ("<%x>", key_code);
#endif

  if (key_code == 0x3B) { /* FN01 */
    send_switch = 1;
#if 1
    rel_wai(my_tskid);
#endif
  }

  if (ch == NULL)
    return;
  
  /* ���٥�Ȥ�Хåե���ί���
   */
  put_entry (ch);
  set_flg (waitflag, ONKEYS);

#ifdef notdef
  /*
   * �ǥХå���
   */
  dbg_printf ("<%x>", key_code);
#endif
}

