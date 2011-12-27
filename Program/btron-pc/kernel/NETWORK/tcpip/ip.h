/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/tcpip/ip.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: ip.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1999-12-11 16:45:58  kishida0
 * add for debug
 *
 * Revision 1.2  1997/05/08 15:10:06  night
 * �ե��������ʸ�������ɤ� SJIS ���� EUC ���ѹ�������
 *
 * Revision 1.1  1997/05/06 12:46:22  night
 * �ǽ����Ͽ
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


#define IPVERSION	4


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

#endif /* __IP_H__ */
