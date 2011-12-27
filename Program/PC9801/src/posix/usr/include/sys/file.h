/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/file.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: file.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:01  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:51:25  night
 * �ǽ����Ͽ
 *
 *
 */

#ifndef __SYS_FILE_H__
#define __SYS_FILE_H__	1

/*
 * �ե����륿����
 */
enum file_t
{
  f_regular,
  f_directory,
  f_special
};

typedef enum file_t	file_t;


#endif /* __SYS_FILE_H__ */
