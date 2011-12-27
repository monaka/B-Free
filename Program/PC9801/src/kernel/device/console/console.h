/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/device/console/console.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: console.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.5  1995-12-05 14:16:13  night
 * device.h の #include 文を変更した。
 *
 * Revision 1.4  1995/09/21  15:50:55  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/09/17  16:49:26  night
 * MAX_COLUMN と MAX_LINE の定義。
 * コンソール制御用のコマンド (CONSOLE_CLEAR、CONSOLE_MOVE) の定義。
 * もろもろの関数の定義。
 * を行った。
 *
 * Revision 1.2  1995/09/14  04:27:47  night
 * init_console() を大域関数に変更。
 * send_reply() の追加。
 *
 * 
 *
 *
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__	1


#include <h/types.h>
#include <h/errno.h>
#include <pc9801/pc98.h>
#include "../../kernlib/device.h"

#define CONSOLE_DRIVER	"driver.console"

#define MAX_COLUMN	80
#define MAX_LINE	25

#define CONSOLE_CLEAR	1
#define CONSOLE_MOVE	2


/* main.c */
extern W    	init_console(void);	/* 初期化		*/
extern W    	open_console();		/* オープン		*/
extern W  	close_console();	/* クローズ		*/
extern W    	read_console();		/* 読み込み		*/
extern W    	write_console();	/* 書き込み		*/
extern W    	control_console();	/* コントロール		*/

extern void	goto_cursol (W x, W y);
extern void	set_curpos (W x, W y);

#endif __CONSOLE_H__


