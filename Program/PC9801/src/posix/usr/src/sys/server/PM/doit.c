/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: doit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:43  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:25:18  night
 * メッセージを proc_message_t を pm_msg_t に変更。
 *
 * Revision 1.1  1995/03/18  14:02:38  night
 * 最初の登録
 *
 *
 */


/*
 * このファイルは、B-Free OS/POSIX 環境で動くプロセスサーバの一部です。
 *
 *
 */

#include "pm.h"


/*
 * プロセスマネージャへ送られた要求を処理する関数。
 *
 * 実際の処理は別の関数が行う。この関数は、単に実際の処理をする関数を
 * リクエストによって振り分ける。
 *
 */
doit (pm_msg_t *req)
{
  proc_error_t	error;

  switch (req->request)
    {
    case PROC_FORK:	/* fork システムコール */
      error = do_proc_fork (req);
      send_reply (req, error);
      break;

    default:
      /* 
       * 該当するメッセージのタイプが見つからなかった。
       * エラーを返す。
       */
      break;
    }
}


/*
 * 返答メッセージを呼び出し元へ送る。
 * 引数で指定してある返答メッセージを ITRON の snd_mbf システムコール
 * を使用して、呼び出しもとに返答する。
 */
ER
send_reply (pm_msg_t *req, struct proc_reply *reply)
{
  return (snd_mbf (req->caller, sizeof (*reply), reply));
}

/*
 * エラーとなったことを返す。
 */
ER
send_error_reply (pm_msg_t *req, enum proc_error errno)
{
  struct proc_reply	reply;

  reply.error = errno;
  return (send_reply (req, &reply));
}

/*
 * NULL を返す。
 *
 */
ER
send_null_reply (pm_msg_t *req)
{
  struct proc_reply	reply;

  bzero (&reply, sizeof (reply));
  return (snd_mbf (req->caller, sizeof (reply), &reply));
}
