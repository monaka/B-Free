/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/udp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/NETWORK/etcpip/udp.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* 
 * $Log: udp.c,v $
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

#include "udp.h"

/* UDP パケットの処理を行う。
 */

ER UDP_CRE_CEP(ID cepid, T_UDP_CCEP *pk_ccep)
{
	ID_CHECK(cepid);
	NUL_CHECK(pk_ccep);
	
	return E_NOSPT;
}

ER udp_cre_cep(ID cepid, T_UDP_CCEP *pk_ccep)
{
	ID_CHECK(cepid);
	NUL_CHECK(pk_ccep);
	
	return E_NOSPT;
}

ER udp_del_cep(ID cepid)
{
	ID_CHECK(cepid);

	return E_NOSPT;
}

ER udp_snd_dat(ID cepid, T_IPV4EP *p_dstaddr, VP data, INT len, TMO tmout)
{
	ID_CHECK(cepid);
	NUL_CHECK(p_dstaddr);
	NUL_CHECK(data);
	LEN_CHECK(len);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER udp_rcv_dat(ID cepid, /*out*/T_IPV4EP *p_dstaddr, VP data, INT len, TMO tmout)
{
	ID_CHECK(cepid);
	NUL_CHECK(data);
	LEN_CHECK(len);
	TMO_CHECK(tmout);
	
	return E_NOSPT;
}

ER udp_can_cep(ID cepid, FN fncd)
{
	ID_CHECK(cepid);

	switch (fncd) {
		case TFN_UDP_SND_DAT:
			break;
		case TFN_UDP_RCV_DAT:
			break;
		case TFN_UDP_ALL:
			break;
		default:
			return E_OBJ;
	}
	
	return E_NOSPT;
}

ER udp_set_opt(ID cepid, INT optname, VP optval, INT optlen)
{
	ID_CHECK(cepid);
	/* optname 	って、どんな値??? */
	NUL_CHECK(optval);
	LEN_CHECK(optlen);
	
	return E_NOSPT;
}

ER udp_get_opt(ID cepid, INT optname, VP optval, INT optlen)
{
	ID_CHECK(cepid);
	/* optname 	って、どんな値??? */
	NUL_CHECK(optval);
	LEN_CHECK(optlen);
	
	return E_NOSPT;
}
