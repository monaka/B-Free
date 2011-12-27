/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/types.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: types.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:02  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:47:58  night
 * �ǽ����Ͽ
 *
 *
 */

/* 
 * POSIX �Ķ��ǻ��Ѥ�����ܷ������
 *
 */

#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__	1

#include "../../../../kernel/itron-3.0/h/types.h"


/*
 * �ǥХ����ֹ�򼨤��� 
 */
typedef unsigned short		dev_t;

/*
 * ���롼���ֹ�򼨤���
 */
typedef	unsigned short		gid_t;

/*
 * �ե�������ֹ�
 */
typedef unsigned long		ino_t;


typedef unsigned long		mode_t;
typedef unsigned short		nlink_t;
typedef unsigned long long	off_t;
typedef unsigned short		pid_t;
typedef unsigned int		size_t;
typedef unsigned int		ssize_t;
typedef unsigned short		uid_t;

#endif /* __SYS_TYPES_H__ */
