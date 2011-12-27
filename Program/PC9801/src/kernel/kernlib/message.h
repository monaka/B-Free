/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/kernlib/message.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

#ifndef __KERNLIB_MESSAGE_H__
#define __KERNLIB_MESSAGE_H__	1

/*
 * $Log: message.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:41  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/09/14  04:38:10  night
 * 最初の登録
 *
 *
 */

#include "../itron-3.0/h/types.h"

/*
 * メッセージ
 */
typedef struct {
  ID	mbfid;		/* 応答メッセージを送信するメッセージバッファ	*/
  W	msgtyp;		/* メッセージ識別番号				*/
  ID	tskid;		/* メッセージを送信したタスク			*/
  W	len;		/* 独自パラメタのサイズ（バイト）		*/
  UW	arg[0];		/* 独自パラメタ 				*/
} SVC_REQ;







#endif /* __KERNLIB_MESSAGE_H__	*/
