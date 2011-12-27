/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* keyboard.c
 *
 *
 */

#include "itron.h"
#include "config.h"
#include "version.h"
#include "errno.h"
#include "task.h"
#include "func.h"
#include "misc.h"
#include "interrupt.h"

#define MAX_KEYENTRY	1000


#define KEY_COM		0x43
#define KEY_DATA	0x41
#define KEY_STAT	0x43

/* 文字をコントロールキーに変換する */
#define C(x)		(x & 0x1f)

#define INT_KEYBOARD	33

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

struct key_entry
{
  struct key_entry	*next;
  UW			ch;
};


struct keyboard
{
  UW			 shiftkey;	/* シフトキーの状態を表す		*/
  UW			 total;		/* 現在キューイングしているエントリ数	*/
  struct key_entry	*first;		/* キューイングリストの先頭エントリ	*/
  struct key_entry	*last;		/* キューイングリストの最終エントリ	*/
};


static struct keyboard		input_buffer;
static struct key_entry		keyentry[MAX_KEYENTRY];
static struct key_entry		*freeentry;

static struct key_entry *
alloc_key (UW code)
{
  struct key_entry *p;

  if (freeentry == NULL)
    {
      return (NULL);
    }
  p = freeentry;
  freeentry = freeentry->next;
  p->ch = code;
  p->next = NULL;
  return (p);
}

static void
dealloc_key (struct key_entry *key)
{
  key->next = freeentry;
  freeentry = key;
}

/****************************************************************************
 * init_keyboard --- 
 *
 */
ER
init_keyboard ()
{
  INT		i;
  T_CTSK	par_task;
  T_CMBF	par_msg;

  for (i = 0; i < MAX_KEYENTRY - 1; i++)
    {
      keyentry[i].next = &keyentry[i + 1];
    }
  keyentry[MAX_KEYENTRY - 1].next = NULL;
  freeentry = keyentry;

  /* KBD の初期化 --- 8251A の初期化 */
  set_idt (INT_KEYBOARD, 0x08, (int)int33_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (1);

  outb (KEY_COM, 0);
  outb (KEY_COM, 0);
  outb (KEY_COM, 0);
  outb (KEY_COM, 0x40);
  outb (KEY_COM, 0x5e);
  outb (KEY_COM, 0x3a);
/*  busywait (20); */
  outb (KEY_COM, 0x32);
/*  busywait (20); */
  outb (KEY_COM, 0x16);
/*  string_shift_to_tron (device_table[id].name); */
  shiftkey_code = NORMAL;

  par_msg.mbfatr = TA_TFIFO;
  par_msg.bufsz = (INT)(sizeof (struct key_entry) * 100);
  par_msg.maxmsz = (INT)sizeof (struct key_entry);
  cre_mbf (ITRON_KEYBOARD_MBF, &par_msg);
  par_task.exinf = 0;
  par_task.startaddr = keyboard_task;
  par_task.itskpri = 1;
  par_task.stksz = PAGE_SIZE;
  par_task.addrmap = NULL;
  cre_tsk (ITRON_KEYBOARD, &par_task);
  sta_tsk (ITRON_KEYBOARD, NULL);
  return (E_OK);
}

W
getchar (void)
{
  struct key_entry	ch;
  INT			size;
  
  rcv_mbf ((VP)&ch, &size, ITRON_KEYBOARD_MBF);
  return (ch.ch);
}



/***********************************************************************************
 * keyboard_task --- 
 *
 */
void
keyboard_task (void)
{
  struct key_entry *p;

  for (;;)
    {
      slp_tsk ();	/* 割り込みによって目覚めるのを待つ */
      if (input_buffer.first)
	{
	  p = input_buffer.first;
	  if (input_buffer.first == input_buffer.last)
	    {
	      input_buffer.first = input_buffer.last = NULL;
	    }
	  else
	    {
	      input_buffer.first = input_buffer.first->next;
	    }
	  snd_mbf (ITRON_KEYBOARD_MBF, sizeof (struct key_entry), p);	/** */
	  dealloc_key (p);
/*	  printf ("-- %d --\n", p->ch);	/** **/
	}
    }
}

/*
 * キーボード割り込みによって起動される関数。
 *
 * キーボードデバイスからキーイベントを読み取り、入力バッファ (input_buffer) 
 * に追加する。
 */
W
intr_kbd ()
{
  W			key_code;
  W			ch;
  struct key_entry	*key;
  ER			error;
  
  while ((inb (KEY_STAT) & 0x02) == 0)
    ;
  outb (KEY_COM, 0x16);
  key_code = inb (0x41);

  if ((key_code & 0x70) == 0x70)
    {
      switch (key_code)
	{
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
	case 0xf0:
	  shiftkey_code &= ~SHIFT_DOWN;
	  break;
	case 0xf1:
	  capskey = NORMAL;
	  break;
	case 0xf3:
	  shiftkey_code &= ~GRAPH_DOWN;
	  break;

	case 0xf4:
	  shiftkey_code &= ~CONTROL_DOWN;
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
  
  dis_int ();
  key = alloc_key (ch);
  if (key == NULL)		/* キーがない */
    {
      ena_int ();
      return;
    }

  if (input_buffer.last)
    {
      input_buffer.last->next = key;
      input_buffer.last = key;
    }
  else
    {
      input_buffer.first = input_buffer.last = key;
    }
  ena_int ();

/*  printf ("key code = 0x%x, char = 0x%x", key_code, ch);	/* */
#if 0  
  if (ch != 0)
    {
      B buf[2];

      buf[0] = ch;
      buf[1] = '\0';
      printf ("(");
      printf (buf);
      printf (")\n");
    }
  else
    printf ("(NULL)\n", ch);
#endif /* 0 */
  if (ch == ('s' - 'a' + 1))
    {
      falldown ("control S\n");
    }
  else if (ch == ('l' - 'a' + 1))
    {
      print_task_list ();
    }

  if ((error = wup_tsk (ITRON_KEYBOARD)) != E_OK)
    {
      printf ("errno = %d\n", error);
      falldown ("intr_kbd: error on wup_tsk.\n");
    }
}

