/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#) $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/BTRON/manager/MM/assert.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/*
 * $Log: assert.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1997-06-29 15:44:30  night
 * �����Ȥ��ѹ�
 *
 * �����������ɤ� SJIS ���� EUC ���ѹ�������
 * ��RCS �� Log �ޥ�������Υ����Ȥ��ɲä���(�ޤ�����Ƥ��ʤ��ä��ե�����Τ�)��
 *
 *
 */


#ifndef __MM_ASSERT_H__
#define __MM_ASSERT_H__		1


#ifdef DEBUG

#define ASSERT(ex)	((ex) ? 0 : __assert__(#ex, __FILE__, __LINE__))

#else

#define ASSERT(ex)

#endif


#endif /* __MM_ASSERT_H__ */
