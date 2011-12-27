/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/memory_manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

static char rcs[] = "@(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/memory_manager.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/*
 * $Log: memory_manager.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1997-07-06 11:54:25  night
 * banner() �ν������ѹ���
 *
 * Revision 1.3  1997/07/02 13:10:32  night
 * malloc �ν�����
 *
 * Revision 1.2  1997/06/29 15:44:32  night
 * �����Ȥ��ѹ�
 *
 * �����������ɤ� SJIS ���� EUC ���ѹ�������
 * ��RCS �� Log �ޥ�������Υ����Ȥ��ɲä���(�ޤ�����Ƥ��ʤ��ä��ե�����Τ�)��
 *
 *
 */


#include "memory_manager.h"


void
start ()
{
  struct mm_request_t	request;

  init ();
  banner ();

  for (;;)
    {
      /* �׵�μ����դ��Ƚ��� */
      if (get_request (&request) == FAIL)
	{
	  /* �ꥯ�����ȼ����˼��Ԥ��� */
#ifdef DEBUG
	  printf ("Cannot get request.\n");
#endif
	  continue;
	}

#ifdef DEBUG
      printf ("OP = %d\n ", request.operation);
#endif /* DEBUG */

#ifdef notdef
      /* ���������ꥯ�����Ȥ�������� */
      if ((request.operation < 0) || (request.operation > NR_POSIX_SYSCALL))
	{
	  /* �ꥯ�������׵�ˤ��륪�ڥ졼�����ϡ����ݡ��Ȥ��Ƥ��ʤ� */
	  error_response (&request, EP_NOSUP);
	}
      else
	{
	  printf ("systemcall: %s\n", syscall_table[request.operation].name);
	  (*syscall_table[request.operation].syscall)(&request);
	}
#endif
    }

}


banner ()
{
  printf ("BTRON MEMORY MANAGER Version %d.%d. start\n", VERSION_ID_HIGH, VERSION_ID_LOW);
}


void
init (void)
{
  /* port �ν���� / ��Ͽ */
  init_port ();

  /* ��å��������ϵ����ν���� */
  init_log ();

  /* �����������(malloc)�ν���� */
  if (init_malloc () != E_OK)
    {
      printf ("MM: cannot init memory allocate system(malloc).\n");
      stop ();
    }
}


void
stop (void)
{
  for (;;)
    {
      slp_tsk ();
    }
}
