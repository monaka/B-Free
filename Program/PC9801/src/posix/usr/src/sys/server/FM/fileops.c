/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/fileops.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/fileops.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * ファイルオペレーション関数群。
 *
 */
/*
 * $Log: fileops.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1995-09-21 15:53:38  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.3  1995/08/12  16:27:24  night
 * オペレーション関数の全面変更。
 * (名前をすべて置き換えた)
 *
 * Revision 1.2  1995/08/12  16:11:41  night
 * 構造体 file_message_t を fm_msg_t に変更。
 *
 * Revision 1.1  1995/08/12  16:05:37  night
 * 最初の登録
 *
 *
 */


#include <sys/types.h>
#include <sys/errno.h>
#include <server/file.h>

#include "FM.h"
#include "dosfs/dosfs.h"


/* ファイルのアクセス権をチェック	*/
void
do_access (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* パス名を辿る 			*/
void
do_lookup (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルをクローズする		*/
void
do_close (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイル属性を得る			*/
void
do_getattr (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイル属性を設定			*/
void
do_setattr (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルの読み取り   	   	*/
void
do_read (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルの書き込み      		*/
void
do_write (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルのサイズを変更		*/
void
do_tranc (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ディレクトリを作成       		*/
void
do_mkdir (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ディレクトリを削除       		*/
void
do_rmdir (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* スペシャルファイルの作成 		*/
void
do_mknod (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* スペシャルファイルの削除 		*/
void
do_rmnod (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルのハードリンク   		*/
void
do_link (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルの削除			*/
void
do_remove (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* デバイスの select       		*/
void
do_select (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルの制御           		*/
void
do_control (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルの移動(名前の変更)  		*/
void
do_move (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* root ファイルシステムのマウント	*/
void
do_mountroot (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルシステムのマウント		*/
void
do_mount (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

/* ファイルシステムのアンマウント	*/
void
do_unmount (fm_msg_t *req)
{
  send_error_reply (req, FILE_ENOSPT);
}

