/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* ctype.h
 *
 *
 */

#ifndef __H_CTYPE_H__
#define __H_CTYPE_H__	1

/* ISCONTROL --- 引数がコントロール文字かどうかを調べる
 */
#define ISCONTROL(ch)	(!(ch & 0xe0))

/* CONTROL ---  引数で渡された文字をコントロール文字に変換する。
 */
#define CONTROL(ch)	(ch & 0x1f)

#define isspace(ch)	((ch == ' ') || (ch == '\n') || (ch == '\t'))



#endif /* __H_CTYPE_H__ */
