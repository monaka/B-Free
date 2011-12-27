/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/***************************************************************************
 * 
 */

#include "types.h"
#include "location.h"
#include "config.h"
#include "interrupt.h"
#include "idt.h"
#include "asm.h"
#include "keyboard.h"

static int get_one_char (void);

/* define keyboard command list */
#define KEY_RESET		0x40
#define KEY_DEFAULT_MODE	0x5e

#define NORMAL_MODE	0
#define SHIFT_MODE	1
#define ALT_MODE	2
#define CTL_MODE	3


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


static int	keyboard_map_table[4][255] =
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
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '_', '+',  BS, TAB,
/* 1 */   'Q', 'W',  'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']','\n', CTL, 'A', 'S',
/* 2 */   'D', 'F',  'G', 'H', 'J', 'K', 'L', ';', '\'','~', SFT, NOK, 'Z', 'X', 'C', 'V',
/* 3 */   'B', 'N',  'M', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},

{/* Alt */
/*         0     1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',  BS, TAB,
/* 1 */   'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',CTL, 'a', 's',
/* 2 */   'd', 'f',  'g', 'h', 'j', 'k', 'l', ';', '\'','~', SFT, NOK, 'z', 'x', 'c', 'v',
/* 3 */   'b', 'n',  'm', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},

{/* control */
/*         0     1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
/* 0 */  NOK,  ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',  BS, TAB,
/* 1 */   'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',CTL, 'a', 's',
/* 2 */   'd', 'f',  'g', 'h', 'j', 'k', 'l', ';', '\'','~', SFT, NOK, 'z', 'x', 'c', 'v',
/* 3 */   'b', 'n',  'm', '<', '>', '/', SFT, '-', ALT, ' ',    0,   0,   0,   0,   0,   0,
},

};

int	cbuf;
int	input_flag;


/*
 *
 */
int
get_mode (int ch)
{
  if (ch & CTL)
    {
      return (CTL_MODE);
    }
  else if (ch & SFT)
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


/*************************************************************************
 *
 */
void
init_keyboard (void)
{
  BYTE status;

  set_idt (INT_KEYBOARD, 0x08, (int)int33_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (1);
  
  status = inb (KEY_STAT);
#ifdef nodef
  boot_printf ("keyboard status: 0x%x\n", status);
#endif
}

/**************************************************************************
 *
 */
int
read_keyboard (void)
{
  int	ch;
  int	mode;

  do
    {
      ch = get_one_char ();
/*      get_one_char (); */
      mode = get_mode (ch);
    }	
  while (keyboard_map_table[mode][(ch & 0x7f)] == 0);
  return (keyboard_map_table[mode][ch & 0x7f]);
}

/**************************************************************************
 *
 */
int
read_keyboard_no_hung (void)
{

  int	ch;
  int	mode;

  ch = get_one_char ();
  mode = get_mode (ch);
  return (keyboard_map_table[mode][ch & 0x7f]);
}

void
intr_keyboard ()
{
  int		ch;
  int		i;
  static int	mode = 0;

  ch = inb (KEY_DATA);	/* get scan code */
  for (i = 0; i < 100; i++)
    {
      if ((inb (KEY_STAT) & 0x02) == 0)
        break;
    }
  if (i == 100)
    {
      boot_printf ("keyboard wait overrun\n");
    }
  outb (KEY_COM, 0xad);
  outb (KEY_COM, 0xae);

  if (ch == 0xE0)
    {
      ch = inb (KEY_DATA);	/* get scan code */
      ch = ch | ENCAP;
      if ((ch & 0xff) == 0x53)	/* delete key */
	{
	  if (mode & (SFT | CTL))
	    {
	      boot_printf ("reset\n");
	    }
	}
    }
  else if ((ch == 0x2a) || (ch == 0x36))
    {
      mode |= SFT;
      return;
    }
  else if (ch == 0x1d)
    {
      mode |= CTL;
      return;
    }
  else if (ch == 0x38)
    {
      mode |= ALT;
      return ;
    }
  else if (ISBREAK (ch))
    {
      ch = ch & 0x7f;

      /* シフトキー等の場合、状態を変更する */
      if ((ch == 0x2a) || (ch == 0x36))
	{
	  mode &= ~SFT;
	}
      else if (ch == 0x1d)
	{
	  mode &= ~CTL;
	}
      else if (ch == 0x38)
	{
	  mode &= ~ALT;
	}
      return;
    }

  cbuf = ch | mode;
  input_flag = 1;

  return ;
}  
 
static int
get_one_char (void)
{
  int	ch;
  
/*
  while (input_flag == 0)
    ;
  lock ();
  input_flag = 0;
  unlock ();
*/
  wait_int (&input_flag);
  ch = cbuf;
  return (ch);
}  

