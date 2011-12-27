/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/***********************************************************************
 * idt.c
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/idt.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: idt.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.1  1996/05/11 10:45:03  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * Revision 1.2  1995/09/21 15:50:39  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:29:10  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:22  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:29  btron
 * BTRON SYSTEM 1.0
 * 
 */

#ifndef __IDT_H__
#define __IDT_H__	1

struct idt_t
{
  int	offset0:16;
  int	selector:16;
  int	zero:8;
  int	type:4;
  int	dt0:1;
  int	dpl:2;
  int	p:1;
  int	offset1:16;
};

#define GET_OFFSET_IDT(desc)		\
  (desc.offset0 | desc.offset1 << 16);

#define SET_OFFSET_IDT(desc,x)		\
(desc.offset0 = x & 0xffff);		\
  (desc.offset1 = x >> 16 & 0xffff);

#define IDT_TABLE_ADDR	0x2000

#define	TRAP_DESC	0xf
#define INTERRUPT_DESC	0xe

#define MAX_IDT		256


void	init_idt (void);
int	set_idt (int entry, int selector, int offset, int type, int dpl);
struct idt_t	*get_idt (int entry);
void	ignore (void);



#endif __IDT_H__
