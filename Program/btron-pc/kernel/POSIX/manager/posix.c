/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

static char	rcsid[] = "@(#)$Id: posix.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


/* posix.c - POSIX �Ķ��ޥ͡�����
 *
 *
 *
 * $Log: posix.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.21  2000-04-03 14:32:33  naniwa
 * minor fix
 *
 * Revision 1.20  2000/02/19 03:02:14  naniwa
 * minor change on debug write
 *
 * Revision 1.19  2000/01/26 08:24:34  naniwa
 * to prevent memory leak
 *
 * Revision 1.18  1999/03/15 08:39:24  monaka
 * Some fixes for using dbg_printf().
 *
 * Revision 1.17  1997/10/24 13:58:14  night
 * �¹Գ��ϻ��� init_malloc () ��ƤӽФ��褦�ˤ�����
 *
 * Revision 1.16  1997/09/21 13:34:11  night
 * �饤�֥��� malloc �ޥ������Ѥ��ʤ��褦�ˤ�����
 *
 * Revision 1.15  1997/08/31 13:30:57  night
 * �ǥХå��ѽ��Ϥΰ����� #ifdef DEBUG ... #endif �ǿ������Ϥ����
 *
 * Revision 1.14  1997/07/06 11:56:41  night
 * malloc ��ǽ�ν����������ɲá�
 *
 * Revision 1.13  1997/07/04 15:07:39  night
 * �����ڥ����ե����� - �ǥХ����ɥ饤�Хݡ��Ȥ��б�ɽ�δ�Ϣ�������ɲá�
 * ���ե�������ɤ߹��߽����β�����
 *
 * Revision 1.12  1997/07/03 14:24:30  night
 * mountroot/open �����ΥХ�������
 *
 * Revision 1.11  1997/05/08 15:11:29  night
 * �ץ����ξ�������ꤹ�뵡ǽ���ɲá�
 * (syscall misc �� proc_set_info ���ޥ��)
 *
 * Revision 1.10  1997/04/28 15:28:18  night
 * �ǥХå��Ѥ�ʸ���ɲá�
 *
 * Revision 1.9  1997/04/24 15:40:30  night
 * mountroot �����ƥॳ����μ�����Ԥä���
 *
 * Revision 1.8  1997/03/25 15:45:23  night
 * �ǥХå��Ѥ��ɲä���̵�¥롼��ʸ(�������λ������ߤ���褦�ˤ��Ƥ���)��
 * ���������
 *
 * Revision 1.7  1997/03/25 13:34:53  night
 * ELF �����μ¹ԥե�����ؤ��б�
 *
 * Revision 1.6  1996/11/20  12:09:54  night
 * rcsid ���ɲá�
 *
 * Revision 1.5  1996/11/11  13:37:06  night
 * �������ɲ�
 *
 * Revision 1.4  1996/11/10  11:54:27  night
 * �����ƥॳ����ؿ��˥ꥯ�����Ⱦ�����Ϥ��Ȥ��ˡ��ݥ����Ϥ��Ǥʤ����
 * �����ʤ��Ȥ������Τ��Ϥ��Ƥ�����
 *
 * Revision 1.3  1996/11/07  21:11:49  night
 * Version �ֹ����Ϥ���Ȥ��˺Ǹ�˲��Ԥ���Ϥ���褦���ѹ�������
 *
 * Revision 1.2  1996/11/07  12:46:38  night
 * �����ƥॳ��������������������
 *
 * Revision 1.1  1996/11/05  15:13:45  night
 * �ǽ����Ͽ
 *
 */

#include "posix.h"


/*
 * POSIX �Ķ��ޥ͡�����Υᥤ��롼����
 *
 * ���ν�����Ԥ�
 *
 * �������
 *   �ݡ��ȥޥ͡�������׵�����դ��ѤΥݡ��Ȥ���Ͽ����
 *   �ե����륷���ƥ�/�ץ���/����γƽ����ν������Ԥ�
 *
 * ���׵�μ����դ�
 *   �׵�μ����դ��ϡ��׵�����դ��ݡ��Ȥˤ�äƹԤ���
 *
 * ���׵�ν���
 *
 *   �׵�μ����Ĥ���������������ޤǤ�¾���׵�ϼ����դ��ʤ���
 */
void
posix_start (void)
{
  struct posix_request	request;
#if 0
  extern B		_end;
#endif

  if (init_port () == FAIL)
    {
      dbg_printf ("Cannot allocate port.\n");
      slp_tsk ();
    }
  init_log ();
#if 0
  /* ����Ǥϥץ����κǸ��Ȥ뤳�ȤϤǤ��ʤ� */
  init_malloc ((UW)&_end);
#else
  /* ;͵�򸫤����ꤷ�Ƥ��뤬��manager ���礭���ʤä��Ȥ��ˤ�Ĵ����ɬ�� */
  init_malloc (0x100000);
#endif

  /* �Ƶ�ǽñ�̤Ǥν����
   */
  init_fs ();
  init_process ();
  init_memory ();

  banner ();

  for (;;)
    {
      /* �����׵��å��������Ԥ� */
      if (get_request (&request) == FAIL)
	{
	  /* �ꥯ�����ȼ����˼��Ԥ��� */
#ifdef DEBUG
	  dbg_printf ("Cannot get request.\n");
#endif
	  continue;
	}

#ifdef DEBUG
      dbg_printf ("OP = %d\n ", request.operation);
#endif

      /* ���������ꥯ�����Ȥ�������� */
      if ((request.operation < 0) || (request.operation > NR_POSIX_SYSCALL))
	{
	  /* �ꥯ�������׵�ˤ��륪�ڥ졼�����ϡ����ݡ��Ȥ��Ƥ��ʤ� */
	  error_response (&request, EP_NOSUP);
	}
      else
	{
#ifdef DEBUG
	  dbg_printf ("systemcall: %s\n", syscall_table[request.operation].name);
#endif

	   (*syscall_table[request.operation].syscall)(&request);
	}
#ifdef DEBUG
      dbg_printf ("posix: systemcall end.\n");
#endif
    }
  /* �����ˤ���ʤ� */
}


void
banner (void)
{
  extern char	*posix_version;
  ID		taskid;
#ifdef notdef
  W	      	pid, low, hi;
  static B     	num[] = "0123456789";
  static B     	buf[3];
#endif

  dbg_printf ("POSIX Manager ver %s started.", posix_version);
  if (get_tid (&taskid) == E_OK)
    {
#ifdef notdef
      low = pid % 10;
      hi = pid / 10;
      buf[0] = num[hi];
      buf[1] = num[low];
      buf[2] = '\0';
      printk ("(TASK ID = %d  / PID = %s)", taskid, buf);
#else
      dbg_printf ("(TASK ID = %d)", taskid);
#endif
    }
  dbg_printf ("\n");
}


