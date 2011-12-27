/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *
 */

#ifndef __ITRON_TASK_H__
#define __ITRON_TASK_H__	1

#include "itron.h"
#include "i386.h"

#define KERNEL_TASK	1

struct t_task_wait
{
  W	time_wait:1;
  W	semaph_wait:1;
  W	event_wait:1;
  W	msg_wait:1;
};

typedef struct t_task_wait  T_TASK_WAIT;

/* t_tcb --- タスク構造体
 *
 *	この構造体には、タスクの固有情報が含まれる。
 *	ただし、ITRON レベルなので、ファイルの情報などは含まれていない。
 *
 */
typedef struct t_tcb
{
  struct t_tcb		*next;		/* 次のTCBへ			*/
  struct t_tcb		*before;	/* 前のTCBへ			*/

  W			tskid;		/* タスク ID			*/
  W			tsklevel;	/* タスクの優先順位		*/
  W			tsklevel0;	/* タスクの優先順位(初期化時の値) */
  					/* 終了したタスクが再度起動すると */
					/* きにはこの値が使われる。       */
  ATR			tskatr;		/* タスク属性			*/
  H			tskstat;	/* タスクの状態			*/

  /* タスクの待ち状態用の要素 */
  T_TASK_WAIT		tskwait;	/* タスクの待ち状態を示すフラグ */
  W			wakeup_count;	/* ウェイト状態のカウンタ       */
  W			suspend_count;	/* サスペンド状態のカウンタ     */

  ER			slp_err;	/* 待ち状態に関してのエラー	*/
					/* すべての同期機構で共通に使用 */
					/* する				*/

/* タスク同期機能 */
  /* セマフォ */
  struct t_tcb	 	*sem_next;	/* 次のセマフォ待ちタスクへ	*/
  ID			sem_id;		/* 今、現在待っているセマフォの ID */

  /* イベントフラグ */
  UINT			flag_pattern;	/* イベントフラグの待ちパターン	*/
  UINT			wfmode;		/* 待ち条件の指定		*/
  ID			event_id;	/* 今、現在待っているイベントフラグの ID */
  UINT			rflgptn;	/* 関数の値として返すフラグパターンq */

  /* メッセージ */
  struct t_tcb	 	*msg_next;	/* 次のメッセージ待ちタスクへ	*/
  ID			msg_id;		/* 今、現在待っているメッセージの ID */


/* スタック情報 */
  W			stksz;		/* タスクの持つスタックのサイズ	*/
  					/* これは、カーネルスタックのサ	*/
					/* イズとなる			*/

  B			*stackptr;	/* カーネルスタックの領域へのポ	*/
					/* インタ			*/


/* タスクの統計情報 */
  W			total;		/* 総実行時間			*/

#ifdef I386
  T_I386_CONTEXT	context;	/* コンテキスト情報 (CPU依存)	*/
  UW			tss_selector;	/* タスクのセレクタ (CPU依存)	*/
#endif

/* 仮想記憶情報 */
  T_REGION		regions[MAX_REGION];	/* タスクに結びついて */
						/* いる REGION のテー */
						/* ブル */ 
} T_TCB;


extern T_TCB		*run_task;		/* 現在、走行中のタスク */

#define GET_TSKWAIT(tcb)	(tcb.tskwait)


#endif /* __ITRON_TASK_H__ */
