/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/times.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: times.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:02  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:51:26  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 *
 *
 */

#ifndef __SYS_TIMES_H__
#define __SYS_TIMES_H__		1

/*
 *
 */
typedef unsigned long	clock_t;

/*
 * 
 */
struct tms
{
  clock_t	tms_utime;	/* User CPU time. */
  clock_t	tms_stime;	/* System CPU time. */
  clock_t	tms_cutime;	/* User CPU time of terminated child proceses. */
  clock_t	tms_cstime;	/* Sytem CPU time of terminated child proceses. */
};


#endif /* __SYS_TIMES_H__ */
