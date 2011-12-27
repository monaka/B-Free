/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/server/memory.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: memory.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:58  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:47:57  night
 * 最初の登録
 *
 *
 */


#ifndef __SYS_MM_H__
#define __SYS_MM_H__	1

/*
 * ポートマネージャに登録する POSIX のメモリマネージャの名前
 */
#define MEMORY_MANAGER_NAME	"posix.memory_manager"


/*
 * 仮想空間の属性
 */
#define	VM_READ		0x00000001	/* 参照可属性 */
#define VM_WRITE	0x00000002	/* 更新可属性 */
#define VM_EXEC		0x00000004	/* 実行可属性 */
#define VM_GROW		0x00000008	/* 仮想空間の拡張属性 */
#define VM_PAGEOUT	0x00000010	/* ページアウト可属性 */
#define VM_NOPAGEOUT	0x00000000	/* ページアウト不可属性 */
#define VM_USER		0x00010000	/* ユーザモード */
#define VM_KERNEL	0x00020000	/* カーネルモード */


/*
 * メモリ管理マネージャに送る要求 (command) の定義
 *
 */
enum mm_command
{
  MM_NULL,	/* 何もしない */
  MM_CREATE,	/* 仮想空間の生成 */
  MM_REMOVE,	/* 仮想空間の削除 */
  MM_WRITE,	/* 仮想空間に情報を書き込む */
  MM_READ,	/* 仮想空間の内容を読みこむ */
  MM_STAT,	/* 仮想空間についての情報を取得する */
};


/* メモリサーバに処理を要求するときに使用する送信パケットと
 * 受信パケット(返信)の定義。
 * これらのパケットは、要求毎に別々に定義する。
 */

/*
 * 仮想空間の生成時に使用するパケット
 */
struct mm_create_arg
{
  ID		taskid;		/* 仮想空間を生成するタスク ID */
  void		*start;		/* 仮想空間のスタートアドレス */
  unsigned int	size;		/* 仮想空間の大きさ */
  int		attr;		/* 仮想空間の属性 */
};

/*
 * 削除要求
 */
struct mm_remove_arg
{
  ID		taskid;		/* 仮想空間を生成するタスク ID */
  void		*start;		/* 仮想空間のスタートアドレス */
  unsigned int	size;		/* 仮想空間の大きさ */
};

/*
 * 書き込み要求
 */
struct mm_write_arg
{
  ID		srctaskid;	/* 書き込み元タスク ID */
  ID		destaskid;	/* 書き込み先タスク ID */
  void		*srcstart;	/* 書き込み元スタートアドレス */
  void		*desstart;	/* 書き込み先スタートアドレス */
  unsigned int	size;		/* 領域のサイズ */
};

/*
 * 読み込み要求
 */
struct mm_read_arg
{
  ID		srctaskid;	/* 読み込み元タスク ID */
  ID		destaskid;	/* 読み込み先タスク ID */
  void		*srcstart;	/* 読み込み元スタートアドレス */
  void		*desstart;	/* 読み込み先スタートアドレス */
  unsigned int	size;		/* 領域のサイズ */
};

/*
 * 情報取得要求 
 */  
struct mm_stat_arg
{
  ID		taskid;		/* 仮想空間を生成するタスク ID */
  void		*start;		/* 仮想空間のスタートアドレス */
  unsigned int	size;		/* 仮想空間の大きさ */
};

/*
 * 引数
 */
struct mm_msg
{
  enum mm_command		prog;		/* 要求の種類を示す */
  ID				recvport;	/* 返信用ポート */
  union
    {
      struct mm_create_arg	mm_create_arg;	/* 生成要求 */
      struct mm_remove_arg	mm_remove_arg;	/* 削除要求 */
      struct mm_write_arg	mm_write_arg;	/* 書き込み要求 */
      struct mm_read_arg	mm_read_arg;	/* 読み込み要求 */
      struct mm_stat_arg	mm_stat_arg;	/* 情報取得要求 */
    } a;
};

/*
 * レスポンス
 */
enum mm_error
{
  EMM_OK = 0,
  EMM_SYSTEM,		/* システムエラー */
  EMM_UNKNOWNMSG,	/* 知らない要求が来た */
  EMM_NOSUPPORT,	/* この要求はサポートしていない */
  EMM_GARBADGE,		/* メッセージが破損している可能性がある */
};

/*
 * 仮想空間生成要求のレスポンス
 */
struct mm_create_res
{
  ID		taskid;		/* 仮想空間を生成するタスク ID */
  void		*start;		/* 仮想空間のスタートアドレス */
  unsigned int	size;		/* 仮想空間の大きさ */
};

/*
 * 仮想空間削除要求のレスポンス
 */
struct mm_remove_res
{
  ID		taskid;		/* 仮想空間を生成するタスク ID */
  void		*start;		/* 仮想空間のスタートアドレス */
  unsigned int	size;		/* 仮想空間の大きさ */
};

/*
 *
 */
struct mm_write_res
{
  ID		srctaskid;	/* 書き込み元タスク ID */
  ID		destaskid;	/* 書き込み先タスク ID */
  void		*srcstart;	/* 書き込み元スタートアドレス */
  void		*desstart;	/* 書き込み先スタートアドレス */
  unsigned int	size;		/* 領域のサイズ */
};

/*
 *
 */
struct mm_read_res
{
  ID		srctaskid;	/* 読み込み元タスク ID */
  ID		destaskid;	/* 読み込み先タスク ID */
  void		*srcstart;	/* 読み込み元スタートアドレス */
  void		*desstart;	/* 読み込み先スタートアドレス */
  unsigned int	size;		/* 領域のサイズ */
};

/*
 *
 */
struct mm_stat_res
{
  ID		taskid;		/* 仮想空間を管理しているタスク ID */
  void		*start;		/* 仮想空間のスタートアドレス */
  unsigned int	size;		/* 仮想空間の大きさ */
  int		attr;		/* 仮想空間の属性 */
};


/*
 *
 */
struct mm_res
{
  enum mm_error	errorcode;	/* エラー番号 */
  unsigned int	syserror;	/* EMM_SYSTEM のときに返すエラー */

  union 
    {
      struct mm_create_res	mm_create_res;	/* ファイル作成 */
      struct mm_remove_res	mm_remove_res;	/* 削除要求 */
      struct mm_write_res	mm_write_res;	/* 書き込み要求 */
      struct mm_read_res	mm_read_res;	/* 読み込み要求 */
      struct mm_stat_res	mm_stat_res;	/* 情報取得要求 */
    } r;
};


#endif /* __SYS_MM_H__ */
