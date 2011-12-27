/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/signal.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: signal.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:52  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/03/18  14:14:38  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 *
 */

#ifndef __SIGNAL_H__
#define __SIGNAL_H__	1

/*
 * POSIX �Ķ��ǤϺ��� 32 �ĤޤǤΥ����ʥ������Ǥ��롣
 */
typedef unsigned long	sigset_t;

typedef void		*sigjmp_buf;

/*
 *
 */
struct sigaction
{
  void		(*sa_handler)();	/* �����ʥ�ϥ�ɥ� */
  sigset_t	sa_mask;		
  int		sa_flags;
};


#define SIGHUP		1
#define SIGINT		2
#define SIGQUIT		3
#define SIGILL		4
#define SIGABRT		5
#define SIGFPE		6
#define SIGTTIN		7
#define SIGTTOU		8
#define SIGKILL		9
#define SIGSEGV	       10
#define SIGALRM	       11
#define SIGSTOP	       12
#define SIGUSR1	       13
#define SIGUSR2	       14
#define SIGTERM	       15
#define SIGCHLD	       16
#define SIGTSTP	       17
#define SIGCONT	       18
#define SIGPIPE	       19





#endif /* __SIGNAL_H__ */
