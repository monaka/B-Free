/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
 * �ե��������ʸ�������ɤ� SJIS ���� EUC ���ѹ�������
 *
 * Revision 1.1  1997/05/06 12:46:23  night
 * �ǽ����Ͽ
 *
 *
 */


#define GLOBAL
#include "tcpip.h"


/* TCP/IP �ץ�ȥ��륹���å��� main �롼���� 
 */
ER
startup ()
{
  if (init_tcpip () != E_OK)
    {
      /* ��ư�˼���(��å���������) */
      dbg_printf ("TCP/IP: Can not start task(s).\n");
      slp_tsk ();
    }

  /* ��ν����ϡ��ҥ��������Ԥ���
   */
  for (;;)
    {
      slp_tsk ();
    }
}



/* TCPIP �ޥ͡�����ν���� 
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

