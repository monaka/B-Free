/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/utime.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: utime.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:02  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:51:11  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 *
 *
 *
 */

#ifndef __SYS_UTIME_H__
#define __SYS_UTIME_H__	1

/*
 * utime �����ƥॳ����ǻ��Ѥ��빽¤��
 *
 */
struct utimbuf
{
  time_t	actime;		/* ������������ 	*/
  time_t	modtime;	/* �ѹ�����		*/
};


#endif /* __SYS_UTIME_H__ */

