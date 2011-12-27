/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/request.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/request.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * このファイルは、B-Free OS/POSIX 環境で動くファイルサーバの一部です。
 *
 * ファイルサーバへのリクエストに関する処理を行います。
 */

/*
 * $Log: request.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1995-09-21 15:53:38  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.4  1995/08/12  16:27:28  night
 * オペレーション関数の全面変更。
 * (名前をすべて置き換えた)
 *
 * Revision 1.3  1995/08/12  16:16:26  night
 * RCS マクロの追加。
 *
 *
 */

#include <sys/types.h>
#include <server/file.h>

#include "FM.h"

/*
 * リクエストを受けとって、処理を行う。
 *
 */
void
doit (fm_msg_t *req)
{
  /* リクエストメッセージを受信して、適当な処理を行う。
   */
  switch (req->request)
    {
    default:
      /* メッセージが理解できなかった。
       * リプライを返す
       */
      send_error_reply (req, FILE_EUNKNOWN);
      break;

    case FILE_NULL:		/* 何もしない */
      send_null_reply (req);
      break;

#ifdef notdef
    case FILE_ACCESS:		/* ファイルのアクセス権をチェック */
      do_access (req);
      break;

    case FILE_LOOKUP:		/* パス名を辿る 		*/
      do_lookup (req);
      break;

    case FILE_CLOSE:		/* ファイルをクローズする	*/
      do_close (req);
      break;

    case FILE_GETATTR:		/* ファイル属性を得る		*/
      do_getattr (req);
      break;

    case FILE_SETATTR:		/* ファイル属性を設定		*/
      do_setattr (req);
      break;

    case FILE_READ:		/* ファイルの読み取り   	*/
      do_read (req);
      break;

    case FILE_WRITE:		/* ファイルの書き込み      	*/
      do_write (req);
      break;

    case FILE_TRANC:		/* ファイルのサイズを変更	*/
      do_tranc (req);
      break;

    case FILE_MKDIR:		/* ディレクトリを作成       	*/
      do_mkdir (req);
      break;

    case FILE_RMDIR:		/* ディレクトリを削除       	*/
      do_rmdir (req);
      break;

    case FILE_MKNOD:		/* スペシャルファイルの作成 	*/
      do_mknod (req);
      break;

    case FILE_RMNOD:           /* スペシャルファイルの削除 	*/
      do_rmnod (req);
      break;

    case FILE_LINK:            /* ファイルのハードリンク   	*/
      do_link (req);
      break;

    case FILE_REMOVE:		/* ファイルの削除		*/
      do_remove (req);
      break;

    case FILE_SELECT:		/* デバイスの select       	*/
      do_select (req);
      break;

    case FILE_CONTROL:		/* ファイルの制御           	*/
      do_control (req);
      break;

    case FILE_MOVE:		/* ファイルの移動(名前の変更)  	*/
      do_move (req);
      break;

    case FILE_MOUNTROOT:	/* root ファイルシステムのマウント */
      do_mountroot (req);
      break;

    case FILE_MOUNT:		/* ファイルシステムのマウント	*/
      do_mount (req);
      break;

    case FILE_UNMOUNT:		/* ファイルシステムのアンマウント */
      do_unmount (req);
      break;
#endif /* notdef */      
    }

}



/*
 * 返答メッセージを呼び出し元へ送る。
 * 引数で指定してある返答メッセージを ITRON の snd_mbf システムコール
 * を使用して、呼び出しもとに返答する。
 */
ER
send_reply (fm_msg_t *req, struct file_reply *reply)
{
  return (snd_mbf (req->caller, sizeof (*reply), reply));
}

/*
 * エラーとなったことを返す。
 */
ER
send_error_reply (fm_msg_t *req, enum file_error errno)
{
  struct file_reply	reply;

  reply.error = errno;
  return (send_reply (req, &reply));
}

/*
 * NULL を返す。
 *
 */
ER
send_null_reply (fm_msg_t *req)
{
  struct file_reply	reply;

  bzero (&reply, sizeof (reply));
  return (snd_mbf (req->caller, sizeof (reply), &reply));
}
