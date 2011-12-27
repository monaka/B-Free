/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/tcp.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* 
 * $Log: tcp.h,v $
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

/*
 *
 */
#ifndef	__TCP_H__
#define	__TCP_H__	1

#include "ip.h"

/* Constants */

/* special port number */
#define TCP_PORTANY  0

#define TFN_TCP_CRE_REP	(-0x201)
#define TFN_TCP_DEL_REP (-0x202)
#define TFN_TCP_CRE_CEP (-0x203)
#define TFN_TCP_DEL_CEP (-0x204)
#define TFN_TCP_ACP_CEP (-0x205)
#define TFN_TCP_CON_CEP (-0x206)
#define TFN_TCP_SHT_CEP (-0x207)
#define TFN_TCP_CLS_CEP (-0x208)
#define TFN_TCP_SND_DAT (-0x209)
#define TFN_TCP_RCV_DAT (-0x20a)
#define TFN_TCP_GET_BUF (-0x20b)
#define TFN_TCP_SND_BUF (-0x20c)
#define TFN_TCP_RCV_BUF (-0x20d)
#define TFN_TCP_REL_BUF (-0x20e)
#define TFN_TCP_SND_OOB (-0x20f)
#define TFN_TCP_RCV_OOB (-0x210)
#define TFN_TCP_CAN_CEP (-0x211)
#define TFN_TCP_SET_OPT (-0x212)
#define TFN_TCP_GET_OPT (-0x213)
#define TFN_TCP_ALL     (0)

#define TEV_TCP_RCV_OOB (0x201)


/* structs */

struct tcphdr
{
};

typedef struct t_tcp_ip_crep {
	ATR repatr;
	T_IPV4EP myaddr;
} T_TCP_CREP;

typedef struct t_tcp_ccep {
	ATR cepatr;
	VP sbuf;
	INT sbufsz;
	VP rbuf;
	INT rbufsz;
	FP callback;
} T_TCP_CCEP;


/* Functions */

extern ER TCP_CRE_REP(ID repid, T_TCP_CREP *pk_crep);
extern ER tcp_cre_rep(ID repid, T_TCP_CREP *pk_crep);
extern ER tcp_del_rep(ID repid);
extern ER TCP_CRE_CEP(ID cepid, T_TCP_CCEP *pk_ccep);
extern ER tcp_cre_cep(ID cepid, T_TCP_CCEP *pk_ccep);
extern ER tcp_del_cep(ID cepid);
extern ER tcp_acp_cep(ID cepid, ID repid, T_IPV4EP *p_dstaddr, TMO tmout);
extern ER tcp_con_cep(ID cepid, T_IPV4EP *myaddr, T_IPV4EP *dstaddr, TMO tmout);
extern ER tcp_sht_cep(ID cepid);
extern ER tcp_cls_cep(ID cepid, TMO tmout);
extern ER tcp_snd_dat(ID cepid, VP data, INT len, TMO tmout);
extern ER tcp_rcv_dat(ID cepid, VP data, INT len, TMO tmout);
extern ER tcp_get_buf(ID cepid, VP *p_buf, TMO tmout);
extern ER tcp_snd_buf(ID cepid, INT len);
extern ER tcp_rcv_buf(ID cepid, VP *p_buf, TMO tmout);
extern ER tcp_rel_buf(ID cepid, INT len);
extern ER tcp_snd_oob(ID cepid, VP data, INT len, TMO tmout);
extern ER tcp_rcv_oob(ID cepid, VP data, INT len);
extern ER tcp_can_cep(ID cepid, FN fncd);
extern ER tcp_set_opt(ID cepid, INT optname, VP optval, INT optlen);
extern ER tcp_get_opt(ID cepid, INT optname, VP optval, INT optlen);

#endif /* __TCP_H__ */
