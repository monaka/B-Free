/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/page_management.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/page_management.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: page_management.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-06-29 15:44:33  night
 * �����Ȥ��ѹ�
 *
 * �����������ɤ� SJIS ���� EUC ���ѹ�������
 * ��RCS �� Log �ޥ�������Υ����Ȥ��ɲä���(�ޤ�����Ƥ��ʤ��ä��ե�����Τ�)��
 *
 *
 */


#include "memory_manager.h"



/* Page �ĥ꡼�δ�����Ԥ��ؿ��� */



void
init_page_info ()
{
}


W
get_page_info (ID proc, UW addr, ID *devid, UW blockno)
{
}



W
set_pageinfo (ID proc, UW vaddr, UW paddr, ID devid, UW blockno)
{
}



W
alloc_new_pageinfo (ID proc, UW addr)
{
}


W
delete_pageinfo (ID proc, UW addr)
{

}
