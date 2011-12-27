/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/ne2000/global.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: global.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1999-03-07 12:43:24  night
 * 次の大域変数を追加。
 * read_buffer[MAX_MTU_LENGTH];
 * read_buffer_length;
 *
 * Revision 1.4  1999/03/02 15:14:29  night
 * 大域変数 max_adaptor についての定義を追加。
 *
 * extern int			max_adaptor;
 *
 * Revision 1.3  1997/10/28 12:44:17  night
 * dev_table の定義の追加。
 *
 * Revision 1.2  1997/05/17 10:41:15  night
 * ファイルの文字コードを SJIS から EUC に変更。
 *
 * Revision 1.1  1997/05/06 12:43:30  night
 * 最初の登録
 *
 *
 */

/*
 *
 */
#ifndef __GLOBAL_H__
#define	__GLOBAL_H__	1



/* ne2000.c */
extern struct ne2000_device	dev_table[MAX_NE2000];
extern int			max_adaptor;
extern UB			read_buffer[MAX_MTU_LENGTH];
extern W			read_buffer_length;

#endif /* __GLOBAL_H__ */
