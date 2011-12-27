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

/* define keyboard command list */
#define KEY_RESET		0x40
#define KEY_DEFAULT_MODE	0x5e

static int	keyboard_map_table[255] =
{
/*        0   1   2   3   4   5   6   7   8   9   a   b   c   d    e   f
/* 0 */   0,'1','2','3','4','5','6','7','8','9','0','-','^','\\', 0,  0,
/* 1 */  'q','w','e','r','t','y','u','i','o','p','@','[','\n','a','s','d',
/* 2 */  'f','g','h','j','k','l',';',':',']','z','x','c','v','b','n','m',
/* 3 */  ',','.','/','-',' ', 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

int	cbuf;
int	input_flag;


/*************************************************************************
 *
 */
void
init_keyboard (void)
{
  set_idt (INT_KEYBOARD, 0x08, (int)int33_handler, INTERRUPT_DESC, 0);
  reset_intr_mask (1);

  outb (KEY_COM, 0);
  outb (KEY_COM, 0);
  outb (KEY_COM, 0);
  outb (KEY_COM, 0x40);
  outb (KEY_COM, 0x5e);
  outb (KEY_COM, 0x3a);
  busywait (20);
  outb (KEY_COM, 0x32);
  busywait (20);
  outb (KEY_COM, 0x16);
}

/**************************************************************************
 *
 */
int
read_keyboard (void)
{
  int	ch;
  
  do
    {
      ch = get_one_char ();
/*      get_one_char (); */
    }	
  while (keyboard_map_table[(ch & 0x7f)] == 0);
  return (keyboard_map_table[ch & 0x7f]);
}

/**************************************************************************
 *
 */
int
read_keyboard_no_hung (void)
{
  int	ch;
  
  ch = get_one_char ();
  return (keyboard_map_table[ch & 0x7f]);
}

void
intr_keyboard ()
{
  int	ch;
  
  while ((inb (KEY_STAT) & 0x02) == 0)
    ;
  outb (KEY_COM, 0x16);
  ch = inb (0x41);
  if (/*(ch & 0x7f) & ! */(ch & 0x80))
    {
      lock ();
      cbuf = ch;
      input_flag = 1;
      unlock ();
    }
}  
  
int
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

