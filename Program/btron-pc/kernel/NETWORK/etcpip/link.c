/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
 * �ե��������ʸ�������ɤ� SJIS ���� EUC ���ѹ�������
 *
 * Revision 1.1  1997/05/06 12:46:22  night
 * �ǽ����Ͽ
 *
 *
 */

/* �ͥåȥ���Υ����
 *
 * �ɥ饤�ФȤΥǡ����Τ��Ȥ��Ԥ���
 * �ޤ���arp/rarp �ѥ��åȤν�����Ԥ���
 *
 */

#include "tcpip.h"


/* ����ؤν����
 */
ER
init_link ()
{
  /* ����ؤΤ�ĥ��塼����������
   */
  
  /* ����� task ������
   */

  return (E_OK);
}


/* driver_name �ǻ��ꤵ�줿�ͥåȥ���ɥ饤�Ф�
 * TCP/IP �ǻȤ���褦�ˤ��롣
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
