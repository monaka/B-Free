/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: doit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:41  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:22:04  night
 * メッセージ構造体を posix_message_t から mm_args に変更。
 *
 * Revision 1.1  1995/03/18  14:01:51  night
 * 最初の登録
 *
 *
 */

/*
 * POSIX 環境でのメモリマネージャ。
 * このファイルでは、受信した要求を処理する。
 */

#include <sys/types.h>
#include <server/memory.h>

/*
 * 受けとった要求は、引数 request の中に入る。
 * 
 */
doit (struct mm_msg *request)
{
  struct mm_res	res;

  switch (request->prog)
    {
    default:	/* unknown mesages */
      bzero (&res, sizeof res);
      (enum mm_error)(res.errorcode) = EMM_UNKNOWNMSG;
      snd_mbf (request->recvport, &res, sizeof (res));
      break;

    case MM_CREATE:	/* 仮想空間の生成 */
    case MM_REMOVE:	/* 仮想空間の削除 */
    case MM_WRITE:	/* 仮想空間に情報を書き込む */
    case MM_READ:	/* 仮想空間の内容を読みこむ */
    case MM_STAT:	/* 仮想空間についての情報を取得する */
      bzero (&res, sizeof res);

      res.errorcode = EMM_NOSUPPORT;
      snd_mbf (request->recvport, &res, sizeof (res));
      break;
    }
}


