/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/arp.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: arp.h,v $
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

/*
 *
 */
#ifndef	__ARP_H__
#define	__ARP_H__	1


struct arphdr
{
  unsigned short	ar_hrd;		/* format of hardware address	*/
  unsigned short	ar_pro;		/* format of protocol address	*/
  unsigned char		ar_hln;		/* length of hardware address	*/
  unsigned char		ar_pln;		/* length of protocol address	*/
  unsigned short	ar_op;		/* ARP opcode (command)		*/
};



#endif /* __ARP_H__ */
