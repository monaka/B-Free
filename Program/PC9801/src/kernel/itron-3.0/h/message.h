/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* message.h --- メッセージバッファ管理
 *
 *
 */

#ifndef __H_MESSAGE_H__
#define __H_MESSAGE_H__		1

/*******************************************************************************
 * メッセージバッファのヘッダ
 *
 */
typedef struct message_head_t
{
  ATR			mbfatr;
  int			total_size;	/* 空きバッファのサイズ */
  struct message_t	*message_ptr;
  struct message_t	*last_message_ptr;
  T_TCB			*wait_sendtask;	/* メッセージを待っているタスク(送信側) */
  T_TCB			*wait_recvtask;	/* メッセージを待っているタスク(受信側) */
  INT			bufsz;		/* バッファの最大長	*/
  INT			msgsz;		/* メッセージの最大長	*/
} T_MSGHEAD;



/*******************************************************************************
 * メッセージエントリ
 *
 *	メッセージはこのパケットに詰められる。
 *
 */
typedef struct message_t
{
  INT			size;
  struct message_t	*next;
  VP			buf;
} T_MSG;


#endif /* __H_MESSAGE_H__ */
