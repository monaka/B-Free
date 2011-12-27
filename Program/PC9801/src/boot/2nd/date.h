/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT date
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/date.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 * $Log: date.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:50:36  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
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

void	init_clock (void);
int	read_clock (struct date_t *date);

#endif __DATE_H__
