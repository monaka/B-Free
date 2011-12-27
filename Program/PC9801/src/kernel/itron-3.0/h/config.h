/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* 
  This file is part of BTRON/386

  $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/kernel/itron-3.0/h/config.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $

  

*/

#ifndef _ITRON_CONFIG_H_
#define _ITRON_CONFIG_H_	1

#include "../i386/i386.h"

#define GDT_ADDR	0x80001000
#define IDT_ADDR	0x80002000
#define MAX_GDT		255		/* GDT の最大のインデックス */
#define MAX_IDT		255		/* IDT の最大のインデックス */

#define TASK_DESC_MIN	TSS_BASE
#define KERNEL_ESEG	0x0008		/* カーネルのコードセグメント */
#define KERNEL_DSEG	0x0010		/* カーネルのデータセグメント */
#define USER_ESEG	0x0018		/* ユーザのコードセグメント */
#define USER_DSEG	0x0020		/* ユーザのデータセグメント */

#define PAGE_DIR_ADDR	0x3000		/* 仮想ページディレクトリテーブル */
#define PAGE_ENTRY_ADDR	0x4000		/* 仮想ページテーブル */

/* タスク管理関係 */ 
#define MAX_TSKID	128		/* 最大のタスク ID 数	*/
#define MIN_TSKID	1		/* 最小のタスク ID 数	*/
#define NTASK		(MAX_TSKID - MIN_TSKID + 1)	/* タスク数	*/

#define MIN_PRIORITY	0		/* 最小のプライオリティ	*/
#define MAX_PRIORITY	31		/* 最大のプライオリティ	*/

#define MIN_USERTASKID	21
#define MAX_USERTASKID	MAX_TSKID

#define MAX_SUSPEND_NEST	10	/* サスペンドのネストのオーバーフロー */


/* セマフォ管理関係 */
#define MIN_SEMAID		1
#define MAX_SEMAID		256
#define NSEMAPHORE		(MAX_SEMAID - MIN_SEMAID + 1)
#define MIN_USERSEMAID		10
#define MAX_USERSEMAID		MAX_SEMAID


/* イベントフラグ管理関係 */
#define MIN_EVENTFLAG		1
#define MAX_EVENTFLAG		256
#define NEVENTFLAG		(MAX_SEMAID - MIN_SEMAID + 1)
#define MIN_USEREVENTID		10
#define MAX_USEREVENTID		MAX_EVENTFLAG


/* メッセージバッファ管理関係 */
#define MIN_MSGBUF		1
#define MAX_MSGBUF		256
#define NMSGBUF			(MAX_MSGBUF - MIN_MSGBUF + 1)
#define MAX_MSGENTRY		1000
/*
 * 自由に使えるメッセージバッファの最小の ID と最大の ID
 */
#define MIN_USERMBFID		100
#define MAX_USERMBFID		MAX_MSGBUF


/* メモリ管理関係 */
#define MAX_MEMPOOL	100
#define PAGE_SIZE	I386_PAGESIZE
#define PAGE_SHIFT	12
#define DIR_SHIFT	22
#define	MIN_KERNEL	0x80000000
#define MAX_KERNEL	0xffffffff

#define MEM_SIZE		0x100000	/* 拡張メモリは 1M バイト */
#define MAX_FREEMEM_ENTRY	1000	/* フリーメモリを管理するためのテーブル */
#define MAX_MEMORY_POOL		100
#define SYSTEM_MEMORY_SIZE	(2 * 1024 * 1024)	/* システムメモリは 2M */
#define MEMORY_POOL_BASE	(0x80010000U)		/* システムメモリとして管理する領域のベース */

#define SYSTEM_MEMORY_POOL	0	/* システムメモリプールの ID */

#define MAX_REGION		5	/* 各タスクに存在できる REGION の数 */

/* IO 管理関係 */
#define MAX_DEVICE_NAME		16	/* デバイス名の最大長 (16 バイト) */
#define MIN_DEVID		0
#define MAX_DEVID		19
#define MAX_DEVICE		(MAX_DEVID - MIN_DEVID + 1)	/* デバイスの最大数 */


/* インターバルタイマ */
#define MAX_TIMER	500


/* システムタスクのリスト */
#define NSYSTEM_TASK	20		/* システムタスクの数 */

#define ITRON_IDLE	1
#define ITRON_RS232C	2		/* ITRON 用 RS232C ドライバタスク */
#define ITRON_KEYBOARD	3
#define ITRON_CONSOLE	4		/* コンソールデバイスドライバ */
#define ITRON_DEBUG	5		/* デバッグ用プロセス: コマンドインタプリタ用 */

/*
 * ポートマネージャが要求受けつけ用に指定するメッセージバッファの ID 
 */
#define PORT_MANAGER_PORT	11


/* システムセマフォのリスト */
#define ITRON_KEYBOARD_SEM	3	/* キーボード入力用のセマフォ	 */


/* システムメッセージバッファのリスト */
#define ITRON_DEBUG_MBF		1	/* デバッグ用プロセス: コマンドインタプリタで使用 */
#define ITRON_RS232C_MBF	2	/* ITRON 用 RS232C ドライバタスクで使用 */
#define ITRON_KEYBOARD_MBF	3	/* キーボード入力用のメッセージバッファ */

extern W	ndevice;
extern ER	(*devices[])();


#endif /* _ITRON_CONFIG_H_ */
