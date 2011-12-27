/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/***************************************************************************
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/memory.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 */

#ifndef	__MEMORY_H__
#define __MEMORY_H__

#include "lib.h"

extern void	*last_addr;
extern UWORD32 real_mem, ext_mem, base_mem;

void	init_memory (void);

#endif /* __MEMORY_H__ */
