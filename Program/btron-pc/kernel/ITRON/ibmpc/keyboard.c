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
#include "func.h"

#include "keyboard_def.h"
#include "keyboard_type.h"

#define MAX_KEYENTRY	1000


#define KEY_COM		0x64
#define KEY_DATA	0x60
#define KEY_STAT	0x64

/* 文字をコントロールキーに変換する */
#define C(x)		(x & 0x1f)

#define INT_KEYBOARD	33

#define NORMAL_MODE	0
#define SHIFT_MODE	1
#define CONTROL_MODE	2
#define ALT_MODE	3

#define ISBREAK(ch)     (ch & 0x80)

#if 0 /* include btron-pc/include/keyboard.h */
  /* 下記の記述はいずれ消去する予定 */

/*******************************************************************************
 *	シフトキーの状態を表すマクロ定数
 *
 */
#define CTL	(0x0100)
#define SFT	(0x0200)
#define ALT	(0x0400)
#define ENCAP	(0x0800)

#define NORMAL_MODE	0
#define SHIFT_MODE	1
#define ALT_MODE	2
#define CTL_MODE	3

#define CAPS_DOWN	1

/* 特殊キー */
#define NOK	0
#define ESC	0x1b
#define BS	0x0b
#define TAB	'\t'
#define CTL	(0x0100)
#define SFT	(0x0200)
#define ALT	(0x0400)
#define ENCAP	(0x0800)

#define ISBREAK(ch)	(ch & 0x80)

static int	key_table[4][255] =
{

{/* normal */
/*         0     1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',  BS, TAB,
/* 1 */   'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',CTL, 'a', 's',
/* 2 */   'd', 'f',  'g', 'h', 'j', 'k', 'l', ';', '\'','~', SFT, NOK, 'z', 'x', 'c', 'v',
/* 3 */   'b', 'n',  'm', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},

{/* Shift */
/*         0     1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+',  BS, TAB,
/* 1 */   'Q', 'W',  'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']','\n', CTL, 'A', 'S',
/* 2 */   'D', 'F',  'G', 'H', 'J', 'K', 'L', ';', '\'','~', SFT, NOK, 'Z', 'X', 'C', 'V',
/* 3 */   'B', 'N',  'M', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},

{/* control */
/*         0     1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',  BS, TAB,
/* 1 */   'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',CTL, 'a', 's',
/* 2 */   'd', 'f',  'g', 'h', 'j', 'k', 'l', ';', '\'','~', SFT, NOK, 'z', 'x', 'c', 'v',
/* 3 */   'b', 'n',  'm', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},

{/* Alt */
/*         0     1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',  BS, TAB,
/* 1 */   'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',CTL, 'a', 's',
/* 2 */   'd', 'f',  'g', 'h', 'j', 'k', 'l', ';', '\'','~', SFT, NOK, 'z', 'x', 'c', 'v',
/* 3 */   'b', 'n',  'm', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},
};
#endif  /* include btron-pc/include/keyboard.h */

UW	keyboard_type;
  
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
static struct key_entry		*freeentry;

static void	cmd_kbd (UB cmd);



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
init_keyboard (void)
{
  INT		i;
  T_CTSK	par_task;
  T_CMBF	par_msg;
  W		status;

  keyboard_type = K_101US;

  set_idt (INT_KEYBOARD, 0x08, (int)int33_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (1);
  status = inb (KEY_STAT);
  printk ("keyboard status: 0x%x\n", status);

  return (E_OK);
}


#if 0
static void
read_keyid (void)
{
  cmd_kbd (0x60);
  outb (KEY_DATA, 0xf2);
}
#endif

static void
kbd_wait (void)
{
  W	i;

  for (i = 0; i < 100000; i++)
    {
      if ((inb (KEY_STAT) & 0x02) == 0)
	return;
    }
  printk ("keyboard overflow.\n");
}

static void
cmd_kbd (UB cmd)
{
#if 0
  UH	ret;
#endif

  kbd_wait ();
  outb (KEY_COM, cmd);
}

/*
 *
 */
int
get_mode (int ch)
{
  if (ch & CTRL)
    {
      return (CONTROL_MODE);
    }
  else if (ch & SHIFT)
    {
      return (SHIFT_MODE);
    }
  else if (ch & ALT)
    {
      return (ALT_MODE);
    }
  else if (ch & ENCAP)
    {
      return (NORMAL_MODE);	/* 本当は違う値が返る */
    }

  return (NORMAL_MODE);    
}

void
set_keyboard_type(UW type)
{
   keyboard_type = type; 
}

W
getchar (void)
{
  struct key_entry	ch;
  INT			size;
  
  rcv_mbf ((VP)&ch, &size, ITRON_KEYBOARD_MBF);
#ifdef notdef
  printk ("getchar: key = 0x%x, mode = %d, converted key = 0x%x\n",
	  ch.ch & 0x7f, get_mode (ch.ch), key_table[get_mode (ch.ch)][(ch.ch) & 0xff]);
#endif

  if(keyboard_type==K_101US){
    return (key_table_101[get_mode (ch.ch)][ch.ch & 0x7f]);
  }else{
    return (key_table_106[get_mode (ch.ch)][ch.ch & 0x7f]);
  }
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
#if 0
	  printk ("-- %d --\n", p->ch);	/** **/
#endif
	}
    }
}

void
intr_kbd_test (void)
{
  UW			key_code;

  kbd_wait ();
  key_code = inb (KEY_DATA);
  if (key_code == 0xfe)
    return;
  printk ("- key_code = 0x%x\n", key_code);
  cmd_kbd (0xad);
  cmd_kbd (0xae);
  return;
}

/*
 * キーボード割り込みによって起動される関数。
 *
 * キーボードデバイスからキーイベントを読み取り、入力バッファ (input_buffer) 
 * に追加する。
 */
W
intr_kbd (void)
{
  W			ch;
  struct key_entry	*key;
  ER			error;
  W			i;
  static W	mode = 0;
  

  ch = inb(KEY_DATA);
  if (ch == 0xE0)
    {
      for (i = 0; i < 100; i++)
	{
	  if ((inb (KEY_STAT) & 0x02) == 0)
	    break;
	}
      ch = inb (KEY_DATA);	/* get scan code */
      for (i = 0; i < 100; i++)
	{
	  if ((inb (KEY_STAT) & 0x02) == 0)
	    break;
	}
      if (i == 100)
	{
	  printk ("keyboard wait overrun\n");
	}
      
#ifdef KEYBOARD_DEBUG
      printk ("keycode = 0x%x\n", ch);
#endif
      ch = ch | ENCAP;
      if ((ch & 0xff) == 0x53)	/* delete key */
	{
	  if (mode & (SHIFT | CTRL))
	    {
	      printk ("reset\n");
	    }
	}
    }
  else if ((ch == 0x2a) || (ch == 0x36))
    {
      mode |= SHIFT;
      return E_OK;
    }
  else if (ch == 0x1d)
    {
      mode |= CTRL;
      return E_OK;
    }
  else if (ch == 0x38)
    {
      mode |= ALT;
      return E_OK;
    }
  else if (ISBREAK (ch))
    {
      ch = ch & 0x7f;

      /* シフトキー等の場合、状態を変更する */
      if ((ch == 0x2a) || (ch == 0x36))
	{
	  mode &= ~SHIFT;
	}
      else if (ch == 0x1d)
	{
	  mode &= ~CTRL;
	}
      else if (ch == 0x38)
	{
	  mode &= ~ALT;
	}
      return E_OK;
    }

  dis_int ();
  key = alloc_key (ch);
  if (key == NULL)		/* キーがない */
    {
      ena_int ();
      return E_OK;
    }

  key->ch = ch | mode;
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

#if 0
  printk ("key code = 0x%x, char = 0x%x", key_code, ch);	/* */
#endif

  if ((error = wup_tsk (ITRON_KEYBOARD)) != E_OK)
    {
      if (error != E_OBJ)
	{
	  printk ("errno = %d\n", error);
	  falldown ("intr_kbd: error on wup_tsk.\n");
	}
    }
  return E_OK;
}

