/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/link.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/link.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: link.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1998-11-24 15:28:31  monaka
 * Some files has copied from NETWORK/tcpip and modified.
 *
 * Revision 1.2  1997/05/08 15:10:06  night
 * ファイル内の文字コードを SJIS から EUC に変更した。
 *
 * Revision 1.1  1997/05/06 12:46:22  night
 * 最初の登録
 *
 *
 */

/* ネットワークのリンク層
 *
 * ドライバとのデータのやりとりを行う。
 * また、arp/rarp パケットの処理も行う。
 *
 */

#include "tcpip.h"


/* リンク層の初期化
 */
ER
init_link ()
{
  /* リンク層のもつキューを初期化する
   */
  
  /* リンク用 task の生成
   */

  return (E_OK);
}


/* driver_name で指定されたネットワークドライバを
 * TCP/IP で使えるようにする。
 */
ER
attach_driver (B *driver_name)
{
  
}

/*
 *
 */
ER
detach_driver (B *driver_name)
{

}

/*
 *
 */
ER
receive_packet (W length, B *packet)
{

}

/*
 *
 */
ER
send_packet (W length, B *packet)
{

}
