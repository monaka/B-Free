/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/lowlib.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: lowlib.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.7  1995-09-21 15:52:57  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.6  1995/07/06  15:23:30  night
 * ���ѿ� user_stack_top �� user_stack_bottom �Ȥ���̾�����ѹ�������
 * ���桼�������å��ΰ�� BTRON �Ķ��˹�碌�� 0x40000000 ����祢�ɥ쥹
 *   �Ȥʤ�褦���ѹ�������
 *
 * Revision 1.5  1995/03/18  14:27:42  night
 * errno ��������ɲá�
 * setup_trap_entry() �ؿ����ɲá�
 * posix_exit() �ؿ����ɲá�
 *
 * Revision 1.4  1995/02/21  15:20:53  night
 * �桼���Υ����å��˰�����ʸ����ν�üʸ���������Ȥ��ˡ�
 * ���ߤΥ桼�������å��ݥ��󥿤Υȥåפ򼨤��Ƥ��� user_stack_top �ѿ���
 * ���㥹�Ȥ��������Ԥ��褦���ѹ�������
 *
 * Revision 1.3  1995/02/21  15:14:47  night
 * �����ʥ뵡�����������뤿��δؿ��� init_signal_handler_table () ����
 * init_signal() ���ѹ�������
 * �ޤ����桼���ץ����Υ����å����������뤿��ν������ɲä�����
 *
 * Revision 1.2  1995/02/20  15:20:56  night
 * ;ʬ����� (�Ť� RCS �� LOG �ޥ������äƤ�����ʬ) ������
 *
 * Revision 1.1  1995/02/20  15:16:40  night
 * �Ϥ���Ƥ���Ͽ
 *
 */

/* lowlib for posix subsystem.
 *
 *
 * POSIX ���֥����ƥ��Ѥ� LOWLIB.
 *
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <server/memory.h>
#include "funcs.h"

/*								*/
/* ����ѿ�������򤹤뤿����ΰ� (�ºݤ˾�����ݤ���)	*/
/*								*/


/*
 * ���� lowlib ��ư���Ƥ���ץ����� ID 
 * (�ץ����ޥ͡����㤫���餦)
 */
pid_t	my_pid;

/*
 * �桼�����֤����顼�ֹ�
 */
int	errno;

/*
 * �ݡ��ȥޥ͡����㤫�������������뤿����ΰ衣
 */
struct recv_port_message_t		recv_msg;


/*
 * �ɽ�ؿ��������
 */

static VP	make_stack_frame (VP user_stack_bottom, int argc, 
				  char **argv, char *envp);
static void	init_user_memory (VP *user_stack_bottom, VP start_heap);
static void	setup_trap_entry (int (*posix_entry)());

/* 
 * POSIX LOWLIB �ν������Ԥ���
 * ���������λ�������Ȥϡ��桼���ץ����� startup �ؿ���ƤӤ�����
 *
 * LOWLIB �ϡ�POSIX �ץ����ޥ͡����㤫�鵯ư����롣
 * ���ΤȤ��ˡ�POSIX �ץ����ޥ͡�����ϡ�����������å��˥ץ�
 * ���ؤΰ������Ϥ�(�����å��˰����ξ����񤭹���Ǥ���)��
 *
 */
lowlib_start (B stack_top)
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
      pid_t	pid;		/* ���Υץ����� ID */
    } *args = (struct a *)&stack_top;

  VP		user_stack_bottom;	/* �桼�������å��Υܥȥ�q���ɥ쥹 */
  extern int	posix_entry();


  my_pid = args->pid;	/* LOWLIB ��Ž��Ĥ��Ƥ���ץ����� */
                        /* ID �򵭲����롣*/

  /*
   * �桼���ץ����Τ���Υ����ΰ�Υ��åȥ��å�
   */
  init_user_memory (&user_stack_bottom, args->start_heap);

  /*
   * �桼���� entry �롼����Τ���Υ����å��ե졼���������롣 
   */
  user_stack_bottom = make_stack_frame (user_stack_bottom,
				     args->argc, 
				     args->argv, 
				     args->envp);

  /* 
   * �����ǡ������ʥ뵡������������
   * �����ʥ�ϥ�ɥ�ν���ͤ� POSIX �λ��ͤ˽�����
   * �����ʥ������Ȥ뤿��Υ�������ư���롣
   */
  init_signal ();

  /*
   * POSIX LOWLIB �γ����ߥ���ȥ�롼��������ꤹ�롣
   * POSIX �����ƥॳ����ϡ����������ꤷ�� posix_entry () �������Ȥʤ롣
   */
  setup_trap_entry (posix_entry);

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
  posix_exit ();
}


/*
 * �桼���ץ����Τ���Υ����ΰ�Υ��åȥ��å�
 * o �����å��ΰ�Τ���� region �����
 * o �ҡ��פ˻��Ѥ��뤿��� region �����
 * (����ޥ͡�������׵᤹��)
 * ���δؿ��ϡ����������桼�������å��Υȥåץ��ɥ쥹���֤���
 */
static void
init_user_memory (VP *user_stack_bottom, VP start_heap)
{
  W	error;

  /*
   * �桼�������å��Ѥ� region ��������롣
   * �桼�������å����ΰ�ϼ��Τ褦�˷��롣
   *  start: 0x30000000
   *  size:  256MB
   * �ʤ���vm_create() �ϥ���ޥ͡�������ΰ���ݤ��׵��Ԥ���
   */
  error = vm_create (get_tid(), 
		     0x30000000,
		     256 * 1024 * 1024, 
		     (VM_READ | VM_WRITE),
		     VM_USER);
  if (error != PE_OK)
    {
      /* 
       * �����ΰ�γ��ݤ˼��Ԥ�����
       * ���Υ������Ͻ�λ���롣
       */
      posix_exit ();
    }
  user_stack_bottom = (void *)0x3ffffff4;

  /*
   * �ҡ����ѤΥ�������ꤹ�롣
   *  start: ���� start_heap �ǻ���
   *  size:  500M - start_heap
   */
  error = vm_create (get_tid (),
		     start_heap,
		     (500 * 1024 * 1024) - (int)start_heap,
		     (VM_READ | VM_WRITE),
		     VM_USER);
  if (error != PE_OK)
    {
      /* 
       * �����ΰ�γ��ݤ˼��Ԥ�����
       * ���Υ������Ͻ�λ���롣
       */
      posix_exit ();
    }
}

/*
 * �����ǡ��桼���� entry �롼����Τ���Υ����å��ե졼���������롣 
 * �桼�������å��Υȥåפ򼨤��ݥ��� user_stack_bottom ������Ȥꡢ
 * �����å��ե졼��� argc, argv, envp �����Ƥ�����
 * (envp �ˤĤ��Ƥϡ��Ķ��ѿ����Τ򥹥��å��ե졼�����������ߡ�
 * �����ΰ�򤵤�˥����å������ꤹ��)
 * ���δؿ��ϡ��������������å��ե졼��Υȥåץ��ɥ쥹���֤���
 *
 * ���δؿ���¹Ԥ������Ȥϡ��桼���Υ����å��ϼ��Τ褦�ˤʤ롣
 *
 *	+----------------------+ <-- return address.
 *	|    Argument Count    | 
 *	+----------------------+
 *	|    Argument Vector   |
 *	+----------------------+
 *	|    Environment       |
 *	+----------------------+
 *	|                      |
 *      //   ������ʸ����     //
 *	|    �ơ��֥�          |
 *	+----------------------+
 *	|                      |
 *	//   �Ķ��ѿ�          //
 *	|      �μ�Ǽ�ΰ�      | <- user_stack_bottom (����)
 *	+----------------------+
 *
 */
static VP
make_stack_frame (VP user_stack_bottom, int argc, char **argv, char *envp)
{
  int	count;

  /*
   * �ޤ����Ķ��ѿ��Ѥ��ΰ����ݤ���
   */
  (char *)user_stack_bottom = (char *)user_stack_bottom - (strlen (envp) + 1);

  /*
   * ���ݤ����ΰ�˴Ķ��ѿ��򥳥ԡ����� 
   */
  strcpy (user_stack_bottom, envp);

  /*
   * �Ķ��ѿ����ΰ�򼨤��ݥ��󥿤򥹥��å��� push ���� 
   */
  envp = user_stack_bottom;
  (char **)user_stack_bottom = (char **)user_stack_bottom - 1;
  bcopy (envp, user_stack_bottom, sizeof (char *));

  /*
   * �Ķ��ѿ���Ʊ�ͤ˰�����ʸ�����Ѥ��ΰ����ݤ��롣
   */
  for (count = argc - 1; count >= 0; count--)
    {
      (char *)user_stack_bottom = (char *)user_stack_bottom - (strlen (argv[count]) + 1);
      strcpy (user_stack_bottom, argv[count]);
      ((char *)user_stack_bottom)[strlen (argv[count])] = '\0';
    }

  /*
   * �Ǹ�˰����ο��򥹥��å��� push ���롣
   */
  (int *)user_stack_bottom += 1;
  *(int *)user_stack_bottom = argc;

  /*
   * �桼�������å��Υȥåפ��֤���
   */
  return (user_stack_bottom);
}

/*
 * POSIX LOWLIB �γ����ߥ���ȥ�롼��������ꤹ�롣
 * POSIX �����ƥॳ����ϡ����������ꤷ�� posix_entry () �������Ȥʤ롣 
 */
static void
setup_trap_entry (int (*posix_entry)())
{
  /*
   * ITRON �ˤγ����ߥ٥�����Ͽ�����ƥॳ�������Ѥ��롣
   */

}

/*
 * POSIX �ץ�����λ���롣
 * POSIX �ץ���(��������)�����Ѥ��Ƥ����񸻤򥷥��ƥ���ֵѤ���
 * ���٤ƤΥ�������λ�����롣
 */
void
posix_exit (void)
{
  /*
   * �桼���ץ������ʬ�����Ѥ��Ƥ������۵����ΰ��������롣
   */
  

  /*
   * �����ơ��Ǹ�˼�ʬ���Ȥ�λ���롣
   */
  exd_tsk ();
}

