/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/***********************************************************************
 * idt.c
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/idt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: idt.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:39  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:29:07  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:21  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:30  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/idt.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "idt.h"
#include "interrupt.h"

/*********************************************************************
 * init_idt
 */
void
init_idt (void)
{
  int	i;

  for (i = 0; i < MAX_IDT; i++)
    {
      set_idt (i, 0x08, (int)ignore_handler, TRAP_DESC, 0);
    }
}

/*********************************************************************
 * set_idt
 */
int
set_idt (int entry, int selector, int offset, int type, int dpl)
{
  struct idt_t	*table;
  unsigned int	*tmp;
  
  table = (struct idt_t *)IDT_TABLE_ADDR;
  SET_OFFSET_IDT (table[entry], offset);
  table[entry].p = 1;
  table[entry].selector = selector;
  table[entry].dpl = dpl;
  table[entry].type = type;
  table[entry].dt0 = 0;
  table[entry].zero = 0;
#ifdef DEBUG
  printf ("set_idt: entry = %d, selector = %d, type = %d\n",
	  entry, selector, type);
  tmp = (unsigned int *)&table[entry];
  printf ("idt[%d] = 0x%x, 0x%x\n", entry, tmp[0], tmp[1]);
#endif DEBUG
}


/*********************************************************************
 *	get_idt
 */
struct idt_t *
get_idt (int entry)
{
  struct idt_t *table;

  table = (struct idt_t *)IDT_TABLE_ADDR;
  return (&table[entry]);
}
