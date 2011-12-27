/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/tcp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/tcp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: tcp.c,v $
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

#include "tcp.h"

/* TCP プロトコルの処理を行う。
 */

ER TCP_CRE_REP(ID repid, T_TCP_CREP *pk_crep)
{
	ID_CHECK(repid);
	NUL_CHECK(pk_crep);
	
	return E_NOSPT;
}

ER tcp_cre_rep(ID repid, T_TCP_CREP *pk_crep)
{
	ID_CHECK(repid);
	NUL_CHECK(pk_crep);

	return E_NOSPT;
}

ER tcp_del_rep(ID repid)
{
	ID_CHECK(repid);

	return E_NOSPT;
}

ER TCP_CRE_CEP(ID cepid, T_TCP_CCEP *pk_ccep)
{
	ID_CHECK(cepid);
	NUL_CHECK(pk_ccep);
	NUL_CHECK(pk_ccep->sbuf);
	LEN_CHECK(pk_ccep->sbufsz);
	NUL_CHECK(pk_ccep->rbuf);
	LEN_CHECK(pk_ccep->rbufsz);

	return E_NOSPT;
}

ER tcp_cre_cep(ID cepid, T_TCP_CCEP *pk_ccep)
{
	ID_CHECK(cepid);
	NUL_CHECK(pk_ccep);
	NUL_CHECK(pk_ccep->sbuf);
	LEN_CHECK(pk_ccep->sbufsz);
	NUL_CHECK(pk_ccep->rbuf);
	LEN_CHECK(pk_ccep->rbufsz);

	return E_NOSPT;
}

ER tcp_del_cep(ID cepid)
{
	ID_CHECK(cepid);

	return E_NOSPT;
}

ER tcp_acp_cep(ID cepid, ID repid, /*out*/ T_IPV4EP *p_dstaddr, TMO tmout)
{
	ID_CHECK(cepid);
	ID_CHECK(repid);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_con_cep(ID cepid, T_IPV4EP *myaddr, T_IPV4EP *dstaddr, TMO tmout)
{
	ID_CHECK(cepid);
	NUL_CHECK(myaddr);
	NUL_CHECK(dstaddr);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_sht_cep(ID cepid)
{
	ID_CHECK(cepid);

	return E_NOSPT;
}

ER tcp_cls_cep(ID cepid, TMO tmout)
{
	ID_CHECK(cepid);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_snd_dat(ID cepid, VP data, INT len, TMO tmout)
{
	ID_CHECK(cepid);
	NUL_CHECK(data);
	LEN_CHECK(len);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_rcv_dat(ID cepid, VP data, INT len, TMO tmout)
{
	ID_CHECK(cepid);
	NUL_CHECK(data);
	LEN_CHECK(len);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_get_buf(ID cepid, /*out*/ VP *p_buf, TMO tmout)
{
	ID_CHECK(cepid);
	TMO_CHECK(tmout);
	
	p_buf = NULL;
	return E_NOSPT;
}

ER tcp_snd_buf(ID cepid, INT len)
{
	ID_CHECK(cepid);
	LEN_CHECK(len);
	
	return E_NOSPT;
}

ER tcp_rcv_buf(ID cepid, /*out*/ VP *p_buf, TMO tmout)
{
	ID_CHECK(cepid);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_rel_buf(ID cepid, INT len)
{
	ID_CHECK(cepid);
	LEN_CHECK(len);
	
	return E_NOSPT;
}

ER tcp_snd_oob(ID cepid, VP data, INT len, TMO tmout)
{
	ID_CHECK(cepid);
	NUL_CHECK(data);
	LEN_CHECK(len);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER tcp_rcv_oob(ID cepid, VP data, INT len)
{
	ID_CHECK(cepid);
	NUL_CHECK(data);
	LEN_CHECK(len);
	
	return E_NOSPT;
}

ER tcp_can_cep(ID cepid, FN fncd)
{
	ID_CHECK(cepid);

	switch (fncd) {
		case TFN_TCP_ACP_CEP:
			break;
		case TFN_TCP_CON_CEP:
			break;
		case TFN_TCP_CLS_CEP:
			break;
		case TFN_TCP_SND_DAT:
			break;
		case TFN_TCP_RCV_DAT:
			break;
		case TFN_TCP_GET_BUF:
			break;
		case TFN_TCP_RCV_BUF:
			break;
		case TFN_TCP_SND_OOB:
			break;
		case TFN_TCP_ALL:
			break;
		default:
			return E_OBJ;
	}
	
	
	return E_NOSPT;
}

ER tcp_set_opt(ID cepid, INT optname, VP optval, INT optlen)
{
	ID_CHECK(cepid);
	/* optname って、どういう値??? */
	NUL_CHECK(optval);
	LEN_CHECK(optlen);
	
	return E_NOSPT;
}

ER tcp_get_opt(ID cepid, INT optname, VP optval, INT optlen)
{
	ID_CHECK(cepid);
	/* optname って、どういう値??? */
	NUL_CHECK(optval);
	LEN_CHECK(optlen);

	return E_NOSPT;
}
