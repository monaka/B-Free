/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/pcmcia/pcmcia.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: pcmcia.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-07-31 18:36:05  kishida0
 * test version
 *
 * Revision 1.3  1999/07/06 13:40:12  kishida0
 * for debug
 *
 * Revision 1.2  1999/05/13 16:23:59  kishida0
 * デバッグ用にコード追加、特に大きな変更ではない
 *
 * Revision 1.1  1999/04/21 17:56:28  kishida0
 * for debug
 *
 * Revision 1.1  1998/12/19 07:50:25  monaka
 * Pre release version.
 *
 */

/*
 *
 */
#ifndef	__PCMCIA_H__
#define	__PCMCIA_H__	1


#define PCMCIA_DRIVER	"driver.pcmcia"

#define MAX_PCMCIA	4	
#define PCICREG 0x3e0

#define PCMCIA_READ_ATRIB_MEM   0
#define PCMCIA_WRITE_ATRIB_MEM  1
#define PCMCIA_READ_MEM         2
#define PCMCIA_WRITE_MEM        3

extern void dump_cis(void);

#endif /* __PCMCIA_H__ */
