/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/keyboard/keyboard.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: keyboard.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.4  1996-01-06 15:59:49  night
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
#include <pc9801/pc98.h>
#include <h/itron.h>
#include "../../kernlib/device.h"

#define KEYBOARD_DRIVER	"driver.keyboard"

#define KEYBOARD_INT		33

#define KEYBOARD_CLEAR		1
#define KEYBOARD_CHANGEMODE	2

#define KEY_COM		0x43
#define KEY_DATA	0x41
#define KEY_STAT	0x43

/* 文字をコントロールキーに変換する */
#define C(x)		(x & 0x1f)

#define	NOKEYS	0
#define ONKEYS	1		/* キーが入力している */

/* keydevice mode */
#define NOWAITMODE	0x0001
#define WAITMODE	0x0002
#define RAWMODE		0x0004


extern ID	recvport;
extern W	initialized;
extern ID	waitflag;	/* キーボードからキー入力を待つ時に */
				/* 使用するイベントフラグの ID */
extern W	driver_mode;

/* main.c */
extern W    	init_keyboard(void);	/* 初期化		*/
extern W    	open_keyboard();	/* オープン		*/
extern W  	close_keyboard();	/* クローズ		*/
extern W    	read_keyboard();	/* 読み込み		*/
extern W    	write_keyboard();	/* 書き込み		*/
extern W    	control_keyboard();	/* コントロール		*/


#endif __KEYBOARD_H__


