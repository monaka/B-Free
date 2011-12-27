/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/tcpip/global.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: global.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-12-11 16:45:56  kishida0
 * add for debug
 *
 * Revision 1.2  1997/05/08 15:10:06  night
 * ファイル内の文字コードを SJIS から EUC に変更した。
 *
 * Revision 1.1  1997/05/06 12:46:22  night
 * 最初の登録
 *
 *
 */

/* 大域変数の定義を行う。
 *
 */
#ifndef __GLOBAL_H__
#define	__GLOBAL_H__	1

#ifndef GLOBAL
#define GLOBAL	extern
#endif

GLOBAL ID	link_task;
GLOBAL ID	ip_task;
GLOBAL ID	tcp_task;
GLOBAL ID	udp_task;
GLOBAL ID	timer_task;

#ifdef __TCPIP_C__
	W	initialized = 0;
#else
GLOBAL	W	initialized;
#endif

#endif /* __GLOBAL_H__ */
