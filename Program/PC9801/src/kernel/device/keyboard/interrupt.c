/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/interrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/interrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: interrupt.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:50:59  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/19  18:01:01  night
 * キーボードから入力できるところまでできた。
 *
 * Revision 1.1  1995/09/18  11:42:03  night
 * 最初の登録
 *
 *
 *
 */

#include "keyboard.h"

#define NORMAL_CODE	0
#define SHIFT_CODE	1
#define CONTROL_CODE	2
#define GRAPH_CODE	3

/*******************************************************************************
 *	シフトキーの状態を表すマクロ定数
 *
 */
#define NORMAL		0x0000
#define SHIFT_DOWN	0x0001
#define CONTROL_DOWN	0x0002
#define GRAPH_DOWN	0x0004

#define CAPS_DOWN	1

static B	key_table[4][128] =
{
  {	/* normal shift */
      0,   '1',  '2',  '3',  '4',  '5',  '6',  '7', 
     '8',  '9',  '0',  '-',  '^',  '\\', 0x8,  '\t',
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
     'o',  'p',  '@',  '[',  '\n', 'a',  's',  'd',
     'f',  'g',  'h',  'j',  'k',  'l',  ';',  ':',
     ']',  'z',  'x',  'c',  'v',  'b',  'n',  'm',
     ',',  '.',  '/',  '_',  ' ',   0,    0,    0,
      0,   0xff,  0,    0,    0,    0,    0,    0,
     '-',  '/',  '7',  '8',  '9',  '*',  '4',  '5',
     '6',  '+',  '1',  '2',  '3',  '=',  '0',  ',',
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
  },
  {	/* shift */
      0,   '!',  '"',  '#',  '$',  '%',  '&',  '\'', 
     '(',  ')',   0,   '=',  '`',  '|',  0x8,  '\t',
     'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
     'O',  'P',  '~',  '{',  '\n', 'A',  'S',  'D',
     'F',  'G',  'H',  'J',  'K',  'L',  '+',  '*',
     ']',  'Z',  'X',  'C',  'V',  'B',  'N',  'M',
     '<',  '>',  '?',  '_',  ' ',   0,    0,    0,
      0,   0xff,  0,    0,    0,    0,    0,    0,
     '-',  '/',  '7',  '8',  '9',  '*',  '4',  '5',
     '6',  '+',  '1',  '2',  '3',  '=',  '0',  ',',
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
  },
  {	/* cntrl */
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,     0x8,    '\t',
     C('q'), C('w'), C('e'), C('r'), C('t'), C('y'), C('u'), C('i'),
     C('o'), C('p'), C('@'), C('['),   '\n', C('a'), C('s'), C('d'),
     C('f'), C('g'), C('h'), C('j'), C('k'), C('l'), C(';'), C(':'),
     C(']'), C('z'), C('x'), C('c'), C('v'), C('b'), C('n'), C('m'),
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
       '-',    '/',    '7',    '8',    '9',    '*',    '4',    '5',
       '6',    '+',    '1',    '2',    '3',    '=',    '0',    ',',
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
  },
  {	/* graph */
      0,   '1',  '2',  '3',  '4',  '5',  '6',  '7', 
     '8',  '9',  '0',  '-',  '^',  '\\', 0x8,  '\t',
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
     'o',  'p',  '@',  '[',  '\n', 'a',  's',  'd',
     'f',  'g',  'h',  'j',  'k',  'l',  ';',  ':',
     ']',  'z',  'x',  'c',  'v',  'b',  'n',  'm',
     ',',  '.',  '/',  '_',  ' ',   0,    0,    0,
      0,   0xff,  0,    0,    0,    0,    0,    0,
     '-',  '/',  '7',  '8',  '9',  '*',  '4',  '5',
     '6',  '+',  '1',  '2',  '3',  '=',  '0',  ',',
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
  }
};  
    


UW	shiftkey_code;
UW	capskey;

static void	keyboard_interrupt ();


/*
 * キーボード割り込みハンドラの登録
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


/*
 * キーボード割り込みのハンドラ
 *
 */
static void
keyboard_interrupt ()
{
  W	key_code;
  W	ch;

  /* キーボードデバイスからイベントを取得 
   */
  while ((inb (KEY_STAT) & 0x02) == 0)
    ;
  outb (KEY_COM, 0x16);
  key_code = inb (0x41);

  if (driver_mode & RAWMODE)
    {
      put_entry (key_code);
      set_flg (waitflag, ONKEYS);
    }

  if ((key_code & 0x70) == 0x70)
    {
      switch (key_code)
	{
	case 0xf1:
	  capskey = NORMAL;
	  break;
	case 0x70:
	  shiftkey_code |= SHIFT_DOWN;
	  break;
	case 0x71:
	  capskey = CAPS_DOWN;
	  break;
	case 0x73:
	  shiftkey_code |= GRAPH_DOWN;
	  break;
	case 0x74:
	  shiftkey_code |= CONTROL_DOWN;
	  break;
	case 0xf3:
	  shiftkey_code &= ~GRAPH_DOWN;
	  break;
	case 0xf4:
	  shiftkey_code &= ~CONTROL_DOWN;
	  break;
	case 0xf0:
	  shiftkey_code &= ~SHIFT_DOWN;
	  break;
	}
      return;
    }
  
  /* もし、キーを離したところならば、無視する */
  if (key_code & 0x80)
    return;
  
  /* マトリックステーブルから、キーコードを取り出す。
   */
  if (shiftkey_code & CONTROL_DOWN)
    ch = key_table[CONTROL_CODE][key_code];
  else if (capskey)
    {
      if (shiftkey_code & SHIFT_DOWN)
	ch = key_table[NORMAL_CODE][key_code];
      else
	ch = key_table[SHIFT_CODE][key_code];
    }
  else if (shiftkey_code & SHIFT_DOWN)
    ch = key_table[SHIFT_DOWN][key_code];
  else
    ch = key_table[NORMAL_CODE][key_code];

  if (ch == NULL)
    return;
  
  /* イベントをバッファに溜める
   */
  put_entry (ch);
  set_flg (waitflag, ONKEYS);

#ifdef notdef
  /*
   * デバッグ用
   */
  dbg_printf ("<%d>", key_code);
#endif /* notdef */
}
