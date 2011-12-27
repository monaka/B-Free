/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/manager.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 */

/*
 *
 *
 *
 * $Log: manager.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-10-29 15:48:08  night
 * 最初の登録
 *
 *
 */


#ifndef __MANAGER_H__
#define __MANAGER_H__	1

#include <h/types.h>
#include <h/errno.h>
#include <ibmpc/ibmpc.h>

#define MANAGER		"btron.manager"

/* メッセージに入る各機能(PM や MM など)が
 * 自由に使えるデータのサイズ
 * メッセージの最後に入る領域。
 */
#define MAX_PRIVATE_SIZE	1024


#define PM_MASK			0x0100
#define FM_MASK			0x0200
#define MM_MASK			0x0300


/*
 * マネージャが受けとるメッセージ形式
 */
struct manager_request
{
  UW	command;		/* コマンド */
  B	data[MAX_PRIVATE_SIZE];
};


/*
 * マネージャが返すメッセージの形式
 */
struct manager_response
{
  UW	result;			/* 結果 */
  B	data[MAX_PRIVATE_SIZE];
};


struct manager_message
{
  ID	caller_port;	/* メッセージの返答用のポート */
  ID	caller_task;	/* メッセージの送り手の task ID */
  W	length;		/* メッセージの最大サイズ */

  union
    {
      struct manager_request	request;
      struct manager_response	response;
    } body;
};


typedef struct manager_message	MANAGER_MSG;



#endif /* __MANAGER_H__ */
