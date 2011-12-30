/*

B-Free Project §Œ¿∏¿Æ ™§œ GNU Generic PUBLIC LICENSE §ÀΩæ§§§ﬁ§π°£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT date
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/date.h,v 1.2 2011/12/30 00:57:06 liu1 Exp $
 *
 * $Log: date.h,v $
 * Revision 1.2  2011/12/30 00:57:06  liu1
 * „Ç≥„É≥„Éë„Ç§„É´„Ç®„É©„Éº„ÅÆ‰øÆÊ≠£„ÄÇ
 *
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1998-11-20 08:02:24  monaka
 * *** empty log message ***
 *
 * Revision 1.1  1996/05/11 10:45:01  night
 * 2nd boot (IBM-PC »« B-FREE OS) §Œ•Ω°º•π°£
 *
 * Revision 1.2  1995/09/21 15:50:36  night
 * •Ω°º•π•’•°•§•Î§Œ¿Ë∆¨§À Copyright notice æ Û§Úƒ…≤√°£
 *
 * Revision 1.1  1993/10/11  21:28:45  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:18  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:30  btron
 * BTRON SYSTEM 1.0
 * 
 */



#ifndef __DATE_H__
#define __DATE_H__	1

struct date_t
{
  int	year;
  int	month;
  int	day;
  int	week;
  int	hour;
  int	minute;
  int	second;
};

extern void init_clock (void);
extern int  read_clock (struct date_t *date);
extern void busywait(int x);

#endif /* __DATE_H__ */
