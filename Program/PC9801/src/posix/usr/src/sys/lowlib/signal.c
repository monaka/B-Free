/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/signal.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/signal.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: signal.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  1995-09-21 15:52:58  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.6  1995/09/02  09:34:54  night
 * �������ɲ�
 *
 * Revision 1.5  1995/03/18  14:28:41  night
 * sig_stop() �ؿ������Ƥ򵭽ҡ�
 *
 * Revision 1.4  1995/02/21  15:23:17  night
 * �����ʥ�ϥ�ɥ�ơ��֥���ѿ��������ʬ����������
 * �ޤ���sig_user() �ǥ桼���Υ����ʥ�ϥ�ɥ��ɽ��ѿ��˵�Ͽ����Ȥ��ˡ�
 * ���㥹�Ȥ��������Ԥ��Ƥ��ʤ��ä��Τǽ���������
 *
 * Revision 1.3  1995/02/21  15:13:39  night
 * �����ʥ뵡���ν�����ؿ� (init_signal()) �ε��Ҥ��ɲäȥ����ʥ�ϥ��
 * ���ѤΥơ��֥� (signal_handler[]) �������Ԥʤä���
 * �ޤ��������ʥ�ϥ�ɥ�Ȥ��ưʲ��δؿ��Υ�����ȥ��񤤤���
 * (��Ȥ��ʤ���ñ�˴ؿ��������������)
 *
 * 	sig_null	ñ�˥����ʥ��̵�� (IGNORE)���롣
 * 	sig_user	�桼����������������ʥ�ϥ�ɥ��¹Ԥ��롣
 * 			�桼���Υ����ʥ�ϥ�ɥ�ؤΥݥ��󥿤ϡ�
 * 			�����Ȥ��� sig_user() ���Ϥ���
 * 	sig_core	��������פ����Τ����ץ�����λ���롣
 * 			�⤷���ץ��������餫��ü���ǥХ����ȷ�ӤĤ�
 * 			�Ƥ�����ˤϡ�ü�����Ф��� ��������פ�����
 * 			�������Ȥ�ɽ�����롣
 * 	sig_kill	ñ�˥ץ�����λ����(��������פ⤷�ʤ�)��
 * 	sig_stop	�ץ�������Ū����ߤ����롣
 * 	sig_cont	sig_stop �ˤ�äư��Ū����ߤ����ץ����ν�
 * 			����Ƴ����롣
 *
 * Revision 1.2  1995/02/20  15:26:34  night
 * �����ʥ�ϥ�ɥ��¹Բ�ǽ���֤ˤ���ؿ���init_signal_handler_table()
 * ����������������������ΤȤ����δؿ��ϲ��⤷�Ƥ��ʤ���
 *
 * Revision 1.1  1995/02/20  15:16:44  night
 * �Ϥ���Ƥ���Ͽ
 *
 *
 */

/*
 * POSIX ���ͤ�������륷���ʥ��¸����뤿��δؿ��������äƤ���ե�
 * ���� 
 *
 * ���Υե���������������Ƥ�������ѿ� signal_handler_table �ϡ�
 * �����ʥ� (���� 32 �Ĥޤ�����Ǥ���) �Τ��Τ��ΤΥϥ�ɥ��������
 * �Ƥ��롣
 *
 * �ϥ�ɥ�ؿ��ˤϡ����μ��ब���롧
 *
 *	sig_null	ñ�˥����ʥ��̵�� (IGNORE)���롣
 *	sig_user	�桼����������������ʥ�ϥ�ɥ��¹Ԥ��롣
 *			�桼���Υ����ʥ�ϥ�ɥ�ؤΥݥ��󥿤ϡ�
 *			�����Ȥ��� sig_user() ���Ϥ���
 *	sig_core	��������פ����Τ����ץ�����λ���롣
 *			�⤷���ץ��������餫��ü���ǥХ����ȷ�ӤĤ�
 *			�Ƥ�����ˤϡ�ü�����Ф��� ��������פ�����
 *			�������Ȥ�ɽ�����롣
 *	sig_kill	ñ�˥ץ�����λ����(��������פ⤷�ʤ�)��
 *	sig_stop	�ץ�������Ū����ߤ����롣
 *	sig_cont	sig_stop �ˤ�äư��Ū����ߤ����ץ����ν�
 *			����Ƴ����롣
 * 
 * �����Υϥ�ɥ�ϡ��桼���ץ�����¹Ԥ��Ƥ���祿�����Ȥ��̸�
 * �Υ����ʥ�����ѥ�������Ǽ¹Ԥ��뤳�Ȥ���դ��뤳�ȡ�
 *
 * POSIX �ץ����ޥ͡�����ϡ��桼���ץ����˥����ʥ��������ˤϡ�
 * �����ʥ�����ѥ��������Ф��ƥ����ʥ��å����������롣
 * ���Τ���˻��Ѥ����å������ݡ��Ȥϡ�init_signal() �ؿ���������/��
 * �������롣
 * 
 */

#include <sys/types.h>
#include <limits.h>
#include <sys/perrno.h>
#include <signal.h>
#include "global.h"


/*
 * �桼���������ʥ�ϥ�ɥ������Ǥ��뤫�ɤ����򼨤��������
 */
#define USER_OK		1
#define USER_NOK	0


/*
 * ���Υե��������ǤΤ߻��Ѥ��빽¤��
 */
struct signal_handler
{
  int	(*handler)();	/* �����ʥ����������Ȥ��ƤӽФ��ϥ�ɥ�ؿ� */
  void	*argp;		/* �ϥ�ɥ�ؿ����Ϥ����� (�ǥե���Ȥ� NULL) */
  int	user_flag;	/* �桼���������ʥ�ϥ�ɥ������Ǥ��뤫�Υ� */
			/* �饰 */
};


/*
 * ���Υե��������ǤΤ߻��Ȳ�ǽ�ʴؿ� (static function)
 */
static int	sig_null (void *arg);	/* ���⤷�ʤ� */
static int	sig_user (void *arg);	/* �桼���ϥ�ɥ��¹� */
static int	sig_core (void *arg);	/* ��������פ��� */
static int	sig_kill (void *arg);	/* ��λ���� */
static int	sig_stop (void *arg);	/* �ץ���������ߤ��� */
static int	sig_cont (void *arg);	/* �ץ����μ¹Ԥ�Ƴ����� */

/*
 * ����ѿ������ (�ºݤ˾�����ݤ���)
 */

struct signal_handler signal_handler_table[MAX_SIGNAL] =
{
  { sig_core, NULL }, /* SIGHUP */
  { sig_core, NULL }, /* SIGINT */
  { sig_core, NULL }, /* SIGQUIT */
  { sig_core, NULL }, /* SIGILL */
  { sig_core, NULL }, /* SIGABRT */
  { sig_core, NULL }, /* SIGFPE */
  { sig_stop, NULL }, /* SIGTTIN */
  { sig_stop, NULL }, /* SIGTTOU */
  { sig_kill, NULL }, /* SIGKILL */
  { sig_core, NULL }, /* SIGSEGV */
  { sig_core, NULL }, /* SIGALRM */
  { sig_stop, NULL }, /* SIGSTOP */
  { sig_null, NULL }, /* SIGUSR1 */
  { sig_null, NULL }, /* SIGUSR2 */
  { sig_core, NULL }, /* SIGTERM */
  { sig_null, NULL }, /* SIGCHLD */
  { sig_stop, NULL }, /* SIGTSTP */
  { sig_cont, NULL }, /* SIGCONT */
  { sig_core, NULL }, /* SIGPIPE */
};



/*
 * �����ǥ����ʥ뵡������������
 * 
 * ������
 *	(1) �����ʥ�����ѤΥ�å������ݡ��Ȥν������
 *	    ITRON �ˤΥ����ƥॳ����ǥ�å������Хåե���������롣
 *	    ����������å������Хåե��ϡ��ץ����ޥ͡������ (�ץ�
 *	    ������Ȥ���) ��Ͽ���롣
 *	(2) �����ʥ�����ѤΥ�������ư���롣
 */
int
init_signal (void)
{
  ID	sigport;

  /*
   * �����ʥ�����ѤΥ�å������ݡ��Ȥ��������롣
   */
  sigport = alloc_port ();		/* ��å������Хåե��μ��� */

  /*
   * �����ʥ���������ݡ��Ȥ�ץ����ޥ͡��������Ͽ���롣
   * ���� proc_set_signal_port () �ϡ�POSIX �ץ����ޥ͡�������̿���
   * ���뤿��δؿ���
   */
  proc_set_signal_port (my_pid, sigport);

  /*
   * �����ʥ��������뤿��Υ�������ư���롣
   * �����󡢥����ʥ�Ϣ��δؿ�������/��Ͽ�����ݡ��Ȥ���Ѥ��롣
   */
  
}

/*
 * �����ʥ�������äƤ⡢���⤷�ʤ��ؿ�
 */
static int
sig_null (void *arg)
{
  return (PE_OK);  /* ������ｪλ�������Ȥ������֤���*/
}

/*
 * �桼���ϥ�ɥ�� (�祿�����Υ���ƥ����Ȥ�) �¹Ԥ��롣
 *
 * �桼���ϥ�ɥ�ϡ������Ĥ�����ͳ����桼���ץ�����¹Ԥ��Ƥ���
 * �祿�������¹Ԥ���ɬ�פ����롣
 * ���Τ��� sig_user() �ϡ�
 *
 *   (1) �祿����������ߤ��� (SUS_TSK)��
 *   (2) ���¹Ԥ��Ƥ��륢�ɥ쥹�����褦�����å���񤭴�����
 *   (3) �Ǹ�˥����ʥ�ϥ�ɥ��¹Ԥ���褦����ƥ����Ⱦ����
 *       �ѹ����롣
 *
 */
static int
sig_user (void *arg)
{
  int	(*user_handler)();

  user_handler = (int (*)())arg;

  sus_tsk (main_task);	/* �祿����������� */

  /* �����å��ν񤭴��� */

  /* ����ƥ����Ⱦ���ν񤭴��� */

  return (PE_OK);
}


/*
 * ��������פ��롣
 */
static int
sig_core (void *arg)
{
  /* �桼���Υǡ����ΰ褪��ӥ����å��ΰ��core�ե�������Ǥ��Ф� */

}


/*
 * �ץ�����λ���롣
 */
static int
sig_kill (void *arg)
{


}


/*
 * �ץ�������Ū����ߤ��롣
 *
 * �ᥤ�󥿥����򥷥��ʥ뤬���äƤ���ޤ���ߤ����롣
 *
 */
static int
sig_stop (void *arg)
{
  /*
   * �ᥤ�󥿥������Ф��� slp_tsk () ��¹Ԥ��롣
   */
  slp_tsk (main_task);

  /*
   * �ץ����ޥ͡�������Ф��ƥץ����� stop ���֤ˤʤä����Ȥ��Τ�
   * ���롣
   */
  
  return (PE_OK);
}


/*
 * �����ߤ����ץ�����Ƴ����롣
 */
static int
sig_cont (void *arg)
{

}


