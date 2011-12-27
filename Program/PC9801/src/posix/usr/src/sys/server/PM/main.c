/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/PM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.5  1996-01-02 13:59:01  night
 * Ω���夲���Υ�å�������ɽ������������ɲá�
 * �ɲôؿ���banner()
 *
 * Revision 1.4  1996/01/02 13:54:18  night
 * ���󥽡���Υ����ץ�������ɲá�
 * �ºݤ˥����ץ������Ԥ��Τϡ�open_console �Ȥ��� libkernel.a ����δ�
 * ���Ǥ��롣
 *
 * Revision 1.3  1995/09/21  15:53:44  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:25:38  night
 * ��å������� proc_message_t �� pm_msg_t ���ѹ���
 *
 * Revision 1.1  1995/03/18  14:02:36  night
 * �ǽ����Ͽ
 *
 *
 */


/*
 * ���Υե�����ϡ�B-Free OS/POSIX �Ķ���ư���ץ��������Фΰ����Ǥ���
 *
 *
 */

#include "pm.h"

ID	recvport;	/* �׵��������뤿��Υ�å������ݡ��� */

/*
 * ���Υե���������ǻ��Ѥ���ؿ�
 */
static void	init_process_manager (void);
static void	banner (void);

/*
 * ���Υե������������ǻ��Ѥ����ѿ�
 */
static B	banner_message[] = "POSIX environment (Process manager). Version 1.0\n";


/*
 * �ץ����ޥ͡������ main �ؿ���
 *
 */
int
_main (void)
{
  pm_msg_t	request;

  init_process_manager ();

  banner ();

  /*
   * ��å������μ��� - ���� - �����Υ롼��
   */
  for (;;)
    {
      /* ��å������μ��� (get_req �� libkernel.a �ˤ���ؿ�) */
      if (get_req (recvport, &request, sizeof (request)) > 0)
	{
	  /*
	   * �ꥯ�����Ȥν����������� doit() �ǹԤ���
	   */
	  doit (&request);
	}
      else
	{
	  /* 
	   * �����ǥ��顼�Ȥʤä���
	   * ���顼�Ȥʤä������� reject ���ơ������׵������Ĥ��롣
	   */
	}
    }
  /* DO NOT REACHED */
}

/*
 * �ץ����ޥ͡�����ν����
 *
 */
static void
init_process_manager ()
{
  /*
   *
   */
  int	error;

  open_console ();	/* ���󥽡���Υ����ץ� */
  init_processinfo ();	/* �ץ�������ν���� */

  recvport = get_port (sizeof (struct pm_msg), sizeof (struct pm_msg));
  if (recvport == 0)	/* ��å������ݡ��Ȥμ����˼��Ԥ��� */
    {
      /* error */
    }
  error = regist_port (PROCESS_MANAGER_NAME, recvport);
  if (error != E_OK)	/* ��å������ݡ��Ȥ���Ͽ�˼��Ԥ��� */
    {
      /* error */
    }

}

static void
banner (void)
{
  write_console (banner_message, sizeof (banner_message));
}
