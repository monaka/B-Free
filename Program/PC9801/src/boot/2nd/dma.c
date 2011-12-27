/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT floppy driver routines.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/dma.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: dma.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:36  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:28:47  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:18  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:34  btron
 * BTRON SYSTEM 1.0
 * 
 */

static char	rcsid[] = "$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/dma.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

#include "types.h"
#include "dma.h"

/****************************************************************************
 *
 */
int
init_dma (void)
{
}

/****************************************************************************
 * setup_dma --- ＤＭＡの設定を行う
 *
 */
int
setup_dma (void *addr, int mode, int length, int mask)
{
  unsigned long	p;
  
  length -= 1;
  p = (unsigned long)addr;
#ifdef DMADEBUG
  printf ("setup_dma: mode = 0x%x, addr = 0x%x, length = %d, mask = 0x%x\n",
	  mode, addr, length, mask);
#endif

  outb (DMA_WRITE_MODE, mode);
  outb (DMA_CLEAR_BYTE, mode);
  outb (DMA_CHANNEL2_ADDR, p & 0xff);
  outb (DMA_CHANNEL2_ADDR, (p >> 8) & 0xff);
  outb (DMA_CHANNEL2_BANK, (p >> 16) & 0x0f);
  outb (DMA_CHANNEL2_COUNT, length & 0xff);
  outb (DMA_CHANNEL2_COUNT, (length >> 8) & 0xff);
  outb (DMA_WRITE_SINGLE_MASK, mask);
}

