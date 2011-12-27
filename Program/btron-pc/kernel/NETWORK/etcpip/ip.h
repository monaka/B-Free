/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/ip.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: ip.h,v $
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
#ifndef	__IP_H__
#define	__IP_H__	1

#include "../../ITRON/h/types.h"
#include "../../ITRON/h/errno.h"

/* Constants */

#define IPVERSION	4

/* special IP address */
#define IPV4_ADDRANY 0

/* Additional Main Error Code
 * These may include ITRON specification.
 */
#define E_WBLK (-83) /* Non blocking call */
#define E_CLS  (-87) /* Connection was cleard */
#define E_BOVR (-89) /* Buffer overflow */

/* Additional Timeout constant.
 * These may include ITRON specification.
 */
#define TMO_NBLK -2 /* non-blocking call */


struct iphdr
{
  unsigned char		ip_hl:4,
                        ip_v:4;			/* always 4 */
  unsigned char		ip_tos;
  unsigned short	ip_len;
  unsigned short	ip_id;
  unsigned short	ip_off;
  unsigned char		ip_ttl;
  unsigned char		ip_p;
  unsigned short	ip_csum;
  unsigned long		ip_src, ip_dst;
};

typedef struct t_ipv4ep {
	UW ipaddr;
	UH portno;
} T_IPV4EP;

#endif /* __IP_H__ */
