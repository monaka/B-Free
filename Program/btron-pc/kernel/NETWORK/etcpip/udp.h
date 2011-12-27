/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/udp.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: udp.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1998-11-24 15:28:31  monaka
 * Some files has copied from NETWORK/tcpip and modified.
 *
 * Revision 1.2  1997/05/08 15:10:07  night
 * ファイル内の文字コードを SJIS から EUC に変更した。
 *
 * Revision 1.1  1997/05/06 12:46:23  night
 * 最初の登録
 *
 *
 */

/*
 *
 */
#ifndef	__UDP_H__
#define	__UDP_H__	1

#include "ip.h"

/* Constants */

/* special port number */
#define UDP_PORTANY  0

#define TFN_UDP_CRE_CEP (-0x221)
#define TFN_UDP_DEL_CEP (-0x222)
#define TFN_UDP_SND_DAT (-0x223)
#define TFN_UDP_RCV_DAT (-0x224)
#define TFN_UDP_CAN_CEP (-0x225)
#define TFN_UDP_SET_OPT (-0x226)
#define TFN_UDP_GET_OPT (-0x227)
#define TFN_UDP_ALL     (0)

#define TEV_UDP_RCV_DAT (0x221)


/* Structs */

struct udphdr
{
};

typedef struct t_udp_ccep {
	ATR cepatr;
	T_IPV4EP myaddr;
	FP callback;
} T_UDP_CCEP;

/* Functions */
extern ER UDP_CRE_CEP(ID cepid, T_UDP_CCEP *pk_ccep);
extern ER udp_cre_cep(ID cepid, T_UDP_CCEP *pk_ccep);
extern ER udp_del_cep(ID cepid);
extern ER udp_snd_dat(ID cepid, T_IPV4EP *p_dstaddr, VP data, INT len, TMO tmout);
extern ER udp_rcv_dat(ID cepid, T_IPV4EP *p_dstaddr, VP data, INT len, TMO tmout);
extern ER udp_can_cep(ID cepid, FN fncd);
extern ER udp_set_opt(ID cepid, INT optname, VP optval, INT optlen);
extern ER udp_get_opt(ID cepid, INT optname, VP optval, INT optlen);

#endif /* __UDP_H__ */
