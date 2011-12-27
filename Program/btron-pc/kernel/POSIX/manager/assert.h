/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* assert.h - Assert �ط��������Ԥ���
 *
 * ������������� ASSERT �ϡ�POSIX ��������Ƥ��� assert �ޥ����
 * �����褦�ʵ�ǽ���Ĥ��������Ĥ����¤����롣
 */

#ifndef __ASSERT_H__
#define	__ASSERT_H__	1

#ifdef NDEBUG

#define	ASSERT(test)

#else

#define _TOSTR_(x)	#x
#define ASSERT(test)	(test) ? (void)0 : \
	((_assert (__FILE__ ":" _TOSTR_(__LINE__) ":" _TOSTR_(test))))


#endif /* NDEBUG */
#endif /* __ASSERT_H__ */
