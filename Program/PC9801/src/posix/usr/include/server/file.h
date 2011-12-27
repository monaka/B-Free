/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/server/file.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: file.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1996-01-06 16:12:58  night
 * ヘッダファイルのインクルードパスを変更。
 *
 * Revision 1.2  1995/09/21  15:51:58  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:47:57  night
 * 最初の登録
 *
 *
 */


#ifndef __SERVER_FILE_H__
#define __SERVER_FILE_H__	1

#include "../../../../kernel/kernlib/message.h"
#include <sys/types.h>
#include <limits.h>

/*
 * ファイルマネージャがポートマネージャに登録するときに
 * 使用する名前。
 */
#define POSIX_FILE_SERVER	"posix.server.file"


/*
 * ファイルマネージャへ送るリクエスト
 */
enum file_request
{
  FILE_NULL,		/* 何もしない 				*/
  FILE_ACCESS,		/* ファイルのアクセス権をチェック	*/
  FILE_LOOKUP,		/* パス名を辿る 			*/
  FILE_CLOSE,		/* ファイルをクローズする		*/
  FILE_GETATTR,		/* ファイル属性を得る			*/
  FILE_SETATTR,		/* ファイル属性を設定			*/
  FILE_READ,		/* ファイルの読み取り   	   	*/
  FILE_WRITE,		/* ファイルの書き込み      		*/
  FILE_TRANC,		/* ファイルのサイズを変更		*/
  FILE_MKDIR,		/* ディレクトリを作成       		*/
  FILE_RMDIR,		/* ディレクトリを削除       		*/
  FILE_MKNOD,		/* スペシャルファイルの作成 		*/
  FILE_RMNOD,           /* スペシャルファイルの削除 		*/
  FILE_LINK,            /* ファイルのハードリンク   		*/
  FILE_REMOVE,		/* ファイルの削除			*/
  FILE_SELECT,		/* デバイスの select       		*/
  FILE_CONTROL,		/* ファイルの制御           		*/
  FILE_MOVE,		/* ファイルの移動(名前の変更)  		*/

  FILE_MOUNTROOT,	/* root ファイルシステムのマウント	*/
  FILE_MOUNT,		/* ファイルシステムのマウント		*/
  FILE_UNMOUNT,		/* ファイルシステムのアンマウント	*/
};

typedef enum file_request file_request_t;


/*
 * ファイル ID
 */
struct fileid
{
  unsigned short	hostid;		/* ホスト ID */
  unsigned short	fsid;		/* ファイルシステム ID */
  unsigned long		fileno;		/* ファイル番号 */
};


/*
 * ファイルマネージャが返すエラー番号
 */
enum file_error
{
  FILE_EOK,		/* 正常終了 				*/
  FILE_ENOMEM,		/* システム領域のメモリ(資源)が足りない	*/
  FILE_EUNKNOWN,	/* メッセージが理解できない             */
  FILE_ENOSPT,		/* 未サポート */
};

typedef enum file_error	file_error_t;


/*
 * ここで、各要求パケットと返答パケットを定義する。
 */
struct file_access_req
{
  char	path[PATH_MAX];
  int	mode;
};

typedef struct file_access_req	file_access_req_t;

/*
 * ファイルに任意のパーミッションを設定するための要求パケット
 *
 */
struct file_setattr_req
{
  char	path[PATH_MAX];
  int	mode;
};

typedef struct file_setattr_req	file_setattr_req_t;


/*
 * ファイルサーバへ送る要求パケット
 *
 */
struct fm_msg
{
  SVC_REQ	header;

  union
    {
      struct file_access_req	file_access_req;
      struct file_setattr_req	file_setattr_req;
    } body;
};

typedef struct fm_msg	fm_msg_t;


/*
 * ファイルサーバから返ってくる返答パケット
 */
struct file_reply
{
  SVC_REQ	header;

  file_error_t		error;

  /*
   * 各要求ごとに異なる領域
   */
};

typedef struct file_reply	file_reply_t;


#endif /* __SERVER_FILE_H__ */
