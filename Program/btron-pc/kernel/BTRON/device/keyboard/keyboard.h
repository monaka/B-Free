/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/device/keyboard/keyboard.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: keyboard.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.8  2000-04-03 14:30:04  naniwa
 * to call timer handler in task
 *
 * Revision 1.7  2000/02/27 15:28:42  naniwa
 * to work as multi task OS
 *
 * Revision 1.6  2000/01/30 19:02:51  kishida0
 * add toplevel Makefile
 * use same keyboard keytop
 *   thanks nakanishi
 * now testing a scroll command (buggy!!)
 *
 * Revision 1.5  2000/01/24 23:28:53  kishida0
 * JIS -> EUC kanji code convert
 *
 * Revision 1.4  2000/01/23 15:50:51  kishida0
 * 106JP、101US キーボード切替え用のコマンドを追加した。
 * 関数にしているのがいまいちだが、とりあえずは使用可能
 * note-pc用にはもう少しキーのテーブルを確認する必要がある
 * 要レポート
 *
 * Revision 1.3  1999/04/18 18:43:31  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.2  1997/07/02 13:09:44  night
 * PC9801 依存したインクルードファイルを IBMPC 用のものに変更した。
 *
 * Revision 1.1  1996/07/24 16:28:44  night
 * IBM PC 版 B-Free OS への最初の登録
 *
 * Revision 1.4  1996/01/06 15:59:49  night
 * ヘッダのインクルードパスの一部変更
 *
 * Revision 1.3  1995/09/21  15:51:00  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/09/19  18:01:01  night
 * キーボードから入力できるところまでできた。
 *
 * Revision 1.1  1995/09/18  11:42:03  night
 * 最初の登録
 *
 * 
 *
 *
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__	1


#include <h/types.h>
#include <h/errno.h>
#ifdef PC98
#include <pc9801/pc98.h>
#else
#include <ibmpc/ibmpc.h>
#endif
#include <h/itron.h>
#include "../../../ITRON/kernlib/device.h"
#include "../../../POSIX/manager/posix.h"

#define KEYBOARD_DRIVER	"driver.keyboard"

#define KEYBOARD_INT		33

#define KEYBOARD_CLEAR		1
#define KEYBOARD_CHANGEMODE	2
#define KEYBOARD_CHANGE_106JP   3
#define KEYBOARD_CHANGE_101US   4

#define KEY_COM		0x64
#define KEY_DATA	0x60
#define KEY_STAT	0x64

/* 文字をコントロールキーに変換する */
#define C(x)		(x & 0x1f)

#define	NOKEYS	0
#define ONKEYS	1		/* キーが入力している */

/* keydevice mode */
#define NOWAITMODE	0x0001
#define WAITMODE	0x0002
#define RAWMODE		0x0004


extern W keyboard_select;

extern ID	recvport;
extern W	initialized;
extern ID	waitflag;	/* キーボードからキー入力を待つ時に */
				/* 使用するイベントフラグの ID */
extern W	driver_mode;
extern W	send_switch;

extern ID	my_tskid;

/* main.c */
extern W    	init_keyboard(void);	/* 初期化		*/
extern W    	open_keyboard();	/* オープン		*/
extern W  	close_keyboard();	/* クローズ		*/
extern W    	read_keyboard();	/* 読み込み		*/
extern W    	write_keyboard();	/* 書き込み		*/
extern W    	control_keyboard();	/* コントロール		*/
extern W	posix_read_keyboard();
extern W	send_switch_msg();

#endif __KEYBOARD_H__


