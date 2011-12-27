/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: lowlib.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.9  2000-02-16 08:16:48  naniwa
 * not to depend on sys/types.h
 *
 * Revision 1.8  1999/07/18 10:50:30  naniwa
 * fixed setup_port()
 *
 * Revision 1.7  1999/04/18 18:43:53  monaka
 * Fix for moving Port-manager and libkernel.a  to ITRON. I guess it is reasonable. At least they should not be in BTRON/.
 *
 * Revision 1.6  1999/03/30 13:19:05  monaka
 * Minor changes.
 *
 * Revision 1.5  1998/02/25 12:50:06  night
 * �ץ�����θ�ͭ�ǡ�������򡢤��줾������ѿ��Ǹ���Ū�ˤ�ĤΤǤϤʤ���
 * lowlib_data �Ȥ����ݥ��󥿤Ǵ�������褦�ˤ�����
 * �����ѹ���ȼ�����ץ�����˰�ä����������/���Ȥ�������ϡ��ݥ���
 * lowlib_data ��𤷤ƹԤ��褦���ѹ�������
 *
 * lowlib_data ���ͤϡ��ޥ��� LOWLIB_DATA �ǻ��ꤷ�����ɥ쥹 (0x7fff0000)
 * �����ꤵ��롣���Υ��ɥ쥹�ϡ��ץ�����˰�ä�ʪ���ڡ����˥ޥåԥ�
 * ����롣���ۥ��ɥ쥹�ϸ���������ºݤ��ΰ�ϥץ�����˰�ä�����¸
 * �ߤ��Ƥ��롣LOWLIB ����������ץ�����ξ���Ϥ����ΰ�������褦��
 * ���롣
 *
 * Revision 1.4  1997/10/11 16:24:05  night
 * �ǥХå��� print ʸ�� DEBUG �ޥ����������Ƥ���Ȥ��Τ�ͭ���ˤʤ��
 * ��������
 *
 * Revision 1.3  1997/09/23 13:52:34  night
 * �ǥХå�ʸ���ɲá�
 *
 * Revision 1.2  1997/08/31 13:13:43  night
 * �Ȥꤢ������OS �ե�����������Ȥ���ޤǤǤ�����
 *
 * Revision 1.1  1996/11/11  13:36:06  night
 * IBM PC �Ǥؤκǽ����Ͽ
 *
 * ----------------
 *
 *
 */

/* lowlib for posix subsystem.
 *
 *
 * POSIX ���֥����ƥ��Ѥ� LOWLIB.
 *
 *
 */

#include "lowlib.h"
#include "servers/port-manager.h"




/* 
 * POSIX LOWLIB �ν������Ԥ���
 */
ER
lowlib_init (struct lowlib_info *lowlib)
{
  ER	error;


  dbg_printf ("POSIX lowlib: now loading...\n");

  /* POSIX �ޥ͡������ port �μ��� */

  lowlib->intr = SYSCALL_INTR;
  lowlib->start = lowlib_start;
  lowlib->quit = lowlib_exit;
  lowlib->intr_func = lowlib_syscall;


  return (E_OK);
}



/* 
 * �桼���ץ����� startup �ؿ���ƤӤ�����
 *
 * LOWLIB �ϡ�POSIX �ץ����ޥ͡����㤫�鵯ư����롣
 * ���ΤȤ��ˡ�POSIX �ץ����ޥ͡�����ϡ�����������å��˥ץ�
 * ���ؤΰ������Ϥ�(�����å��˰����ξ����񤭹���Ǥ���)��
 *
 */
ER
lowlib_start (VP stack_top)
{
  /* 
   * lowlib �������Ȥ����������� 
   */
  struct a
    {
      int	(*ap_start)();	/* �桼���ץ����Υ���ȥꥢ�ɥ쥹 */
      int	argc;		/* �桼���ץ������Ϥ� argc */
      char	**argv;		/* �桼���ץ������Ϥ� argv */
      char	*envp;		/* �桼���ץ����δĶ��ѿ����� */
      VP	start_heap;	/* �桼���ץ����Υҡ������ΰ���� */
				/* Ƭ���ɥ쥹 */
      int	pid;		/* ���Υץ����� ID */
    } *args = (struct a *)stack_top;

  VP		user_stack_bottom;	/* �桼�������å��Υܥȥ�q���ɥ쥹 */


  lowlib_data->my_pid = args->pid;	/* LOWLIB ��Ž��Ĥ��Ƥ���ץ����� 
					 * ID �򵭲����롣
					 */


  /* �ץ�����ͭ����Τ�����ΰ����ݤ���
   */


  /* 
   * �����ǡ������ʥ뵡������������
   * �����ʥ�ϥ�ɥ�ν���ͤ� POSIX �λ��ͤ˽�����
   * �����ʥ������Ȥ뤿��Υ�������ư���롣
   */
  init_signal ();

  /*
   * �����ǡ��桼���ץ����Υ������ȥ��å״ؿ��򥳡��뤹�롣
   * lowlib �ϥ����ͥ�⡼�ɤ�ư���Ƥ���Τǡ��桼���⡼�ɤ����ܤ��ʤ�
   * ��Ф����ʤ���
   * ���Τ���ˡ�������֥�Ǻ��������ؿ� jmp_user_entry () ��Ƥ�Ǥ�
   * �롣
   */
  jmp_user_entry ();

  /*
   * �����ˤϡ���äƤ��ʤ����⤷����ɤäƤ������ˤϡ��桼���ץ�
   * �����˴���Ԥ���
   */
  lowlib_exit ();
}



B	*posix_name;

ER
setup_port (void)
{
  ER	error;

#ifdef DEBUG
  dbg_printf ("setup_port\n");
#endif
  /* port �ν���� */
  posix_name = "manager.posix";
  if (posix_manager == 0) {
    error = find_port (posix_name, &posix_manager);
    if (error != E_OK)
      {
#ifdef DEBUG
	dbg_printf ("Cannot find port.\n");
#endif
	return (error);
      }
  }
#ifdef DEBUG
  dbg_printf("Port found.(%d)\n", lowlib_data);
#endif
  if (lowlib_data->recv_port == 0) {
    lowlib_data->recv_port = get_port (sizeof (struct posix_response), sizeof (struct posix_response));
#ifdef DEBUG
    dbg_printf("Port got.\n");
#endif
    if (lowlib_data->recv_port == 0)
      {
#ifdef DEBUG
	dbg_printf ("Cannot allocate port\n");
#endif
	return (E_NOMEM);
      }
  }
  return (E_OK);
} 


/*
 *	POSIX �����ƥॳ����ν����ؿ� (����)
 */
ER
lowlib_syscall (W syscallno, VP arg)
{
  int	(*func)();
  ER errno;

  if ((syscallno < 0) || (syscallno >= nsyscall))
    {
      return (EP_INVAL);
    }

  func = syscalls[syscallno].func;
  if (func == NULL)
    {
      return (EP_NOSYS);
    }

  if (lowlib_data->recv_port == 0)
    {
      errno = setup_port ();
      if (errno != E_OK) return(errno);
    }

  return ((*func)(arg));
}


/*
 * POSIX �ץ�����λ���롣
 * POSIX �ץ���(��������)�����Ѥ��Ƥ����񸻤򥷥��ƥ���ֵѤ���
 * ���٤ƤΥ�������λ�����롣
 */
ER
lowlib_exit ()
{
  /*
   * �桼���ץ������ʬ�����Ѥ��Ƥ������۵����ΰ��������롣
   */
  

  /*
   * �����ơ��Ǹ�˼�ʬ���Ȥ�λ���롣
   */
  exd_tsk ();
}

