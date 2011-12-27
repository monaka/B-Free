/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/ITRON/kernlib/message.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

#ifndef __KERNLIB_MESSAGE_H__
#define __KERNLIB_MESSAGE_H__	1

/*
 * $Log: message.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-04-18 17:48:33  monaka
 * Port-manager and libkernel.a is moved to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.3  1996/07/25 17:23:55  night
 * ITRON/h/types.h のインクルードパスを変更。
 *
 * Revision 1.2  1996/07/23  17:17:07  night
 * IBM PC 用の make 環境に merge
 *
 * Revision 1.1  1996/07/22  23:52:05  night
 * 最初の登録
 *
 * Revision 1.2  1995/09/21  15:51:41  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/09/14  04:38:10  night
 * 最初の登録
 *
 *
 */

#include "../../ITRON/h/types.h"

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
