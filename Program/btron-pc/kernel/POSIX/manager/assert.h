/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* assert.h - Assert 関係の定義を行う。
 *
 * ここで定義する ASSERT は、POSIX に定義してある assert マクロと
 * 似たような機能をもつが、いくつか制限がある。
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
