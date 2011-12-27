/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/include/keycode.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: keycode.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  2000-07-02 15:03:11  kishida0
 * add #ifndef and #define line
 *
 * Revision 1.2  2000/02/27 15:36:23  naniwa
 * to work as multi task OS
 *
 * Revision 1.1  2000/01/30 18:54:08  kishida0
 * add keyboard keytop define
 *
 *
 *
 */
/**/
#ifndef __KEYCODE_H__
#define __KEYCODE_H__

#define CTRL    (0x0100)
#define SHIFT   (0x0200)
#define ALT     (0x0400)
#define ENCAP   (0x0800)

/* キー */
#define NOKEY	0

#define ESC	0x1b
#define BS	0x0b
#define TAB	'\t'
#define SPACE   ' '
#define ENTER	'\n'
#define	DEL	0x0b

#define KANJI   ESC /*  */

/* normal function key */
#define FN01   0x01
#define FN02   0
#define FN03   0
#define FN04   0
#define FN05   0
#define FN06   0
#define FN07   0
#define FN08   0
#define FN09   0
#define FN10   0
#define FN11   0
#define FN12   0

/* +shift function key */
#define SFN01   0
#define SFN02   0
#define SFN03   0
#define SFN04   0
#define SFN05   0
#define SFN06   0
#define SFN07   0
#define SFN08   0
#define SFN09   0
#define SFN10   0
#define SFN11   0
#define SFN12   0

/* +control function key */
#define CFN01   0
#define CFN02   0
#define CFN03   0
#define CFN04   0
#define CFN05   0
#define CFN06   0
#define CFN07   0
#define CFN08   0
#define CFN09   0
#define CFN10   0
#define CFN11   0
#define CFN12   0

/* +alt function key  */
#define AFN01   0
#define AFN02   0
#define AFN03   0
#define AFN04   0
#define AFN05   0
#define AFN06   0
#define AFN07   0
#define AFN08   0
#define AFN09   0
#define AFN10   0
#define AFN11   0
#define AFN12   0

#define NEXF   0   /*  */
#define XFER   0   /*  */
#define KANA   0   /*  */
#define MENU   0   /* Windoze menu key */

#define HOME 0 
#define UpAr 0
#define DnAr 0
#define L_Ar 0
#define R_Ar 0

#define PgUp 0
#define PgDw 0

#define LWinK 0
#define RWinK 0

#define PRINT 0
#define SCROL 0
#define PAUSE 0

#define INSER 0
#define HOME  0

/* number key pad */
#define NLOCK  0
#define NMIN   0

#define NUM1 END

#endif //__KEYCODE_H__
