/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT interrupt routines.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/cinterrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: cinterrupt.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:50:35  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1994/07/30  17:37:13  night
 * ファイル中の日本語文字をすべて EUC コードに変更。
 *
 * Revision 1.1  1993/10/11  21:28:38  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:16  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:24  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/cinterrupt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "interrupt.h"

/****************************************************************************
 * init_8259A ---
 */
void
init_8259A (void)
{
/* init master 8259A */
  outb (MASTER_8259A_COM, 0x11);
  outb (MASTER_8259A_DATA, 0x20);
  outb (MASTER_8259A_DATA, 0x80);
  outb (MASTER_8259A_DATA, 0x1d);
  
/* init slave 8259A */
  outb (SLAVE_8259A_COM, 0x11);
  outb (SLAVE_8259A_DATA, 0x28);
  outb (SLAVE_8259A_DATA, 0x07);
  outb (SLAVE_8259A_DATA, 0x09);

/* set mask */
  outb (MASTER_8259A_DATA, 0x7f);
  outb (SLAVE_8259A_DATA,  0xff);
}  

void
reset_intr_mask (int intn)
{
  lock ();
  if (intn < 8)
    outb (MASTER_8259A_DATA, inb (MASTER_8259A_DATA) & ~(1 << intn));
  else
    outb (SLAVE_8259A_DATA, inb (SLAVE_8259A_DATA) & ~(1 << (intn - 8)));
  unlock ();
}    

/***************************************************************************
 * interrupt --- 外部割り込みの処理
 */
void
interrupt (int intn)
{
  switch (intn)
    {
    default:
      /* error!! */
      printf ("unknown interrupt from %d\n", intn);
      break;
    case INT_KEYBOARD:
      intr_keyboard ();
      break;
    case INT_FD:
      intr_fd ();
      break;
    case INT_HD:
      hd_intr ();
      break;
    }
}

void
trap (int intn)
{
}

void
fault (int intn)
{

}

/**************************************************************************
 *
 */
int
wait_int (int *flag)
{
  while (*flag == FALSE)
    ;
  lock ();
  *flag = FALSE;
  unlock ();
  return (*flag);
}

