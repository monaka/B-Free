/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/tcpip.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/tcpip.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: tcpip.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1998-11-24 15:28:31  monaka
 * Some files has copied from NETWORK/tcpip and modified.
 *
 * Revision 1.2  1997/05/08 15:10:06  night
 * ファイル内の文字コードを SJIS から EUC に変更した。
 *
 * Revision 1.1  1997/05/06 12:46:23  night
 * 最初の登録
 *
 *
 */


#define GLOBAL
#include "tcpip.h"


/* TCP/IP プロトコルスタックの main ルーチン 
 */
ER
startup ()
{
  if (init_tcpip () != E_OK)
    {
      /* 起動に失敗(メッセージ出力) */
      dbg_printf ("TCP/IP: Can not start task(s).\n");
      slp_tsk ();
    }

  /* 後の処理は、子タスクが行う。
   */
  for (;;)
    {
      slp_tsk ();
    }
}



/* TCPIP マネージャの初期化 
 */
ER
init_tcpip ()
{
  if (!initialized)
    {
      init_timer ();
      init_link ();
      init_ip ();
      init_udp ();
      init_tcp ();
      init_arp ();

      initialized = 1;
    }

  return (E_OK);
}

