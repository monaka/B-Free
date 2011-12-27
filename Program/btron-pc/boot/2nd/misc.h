/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT main routine.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd/misc.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: misc.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-05-11 10:45:06  night
 * 2nd boot (IBM-PC 版 B-FREE OS) のソース。
 *
 * Revision 1.2  1995/09/21  15:50:42  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1993/10/11  21:29:41  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:25  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:32  btron
 * BTRON SYSTEM 1.0
 * 
 */

#ifndef __MISC_H__
#define __MISC_H__	1

extern int	getchar (void);
extern char	*gets (char *);
extern unsigned short	shift_to_jis (unsigned short shift);
extern unsigned short	jis_to_shift (unsigned short shift);
extern char		*string_shift_to_jis (char *s);


#endif __MISC_H__
