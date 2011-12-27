/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* misc.h ---
 *
 *
 */

#ifndef __ITRON_MISC_H__
#define __ITRON_MISC_H__	1

#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
#define CUTDOWN(x,align)	(((long)x) * ((long)align) / ((long)align))
#define TABLE_SIZE(table)	(sizeof(table) / sizeof(table[0]))
#define ISDIGIT(x)		((x >= '0') && (x <= '9'))
#define MAX(a,b)		((a > b) ? a : b)
#define MIN(a,b)		((a < b) ? a : b)


#define ASSERT(exp,mes)		\
if (exp) \
{\
   printf ("ASSERT (%s %d): %s\n", __FILE__, __LINE__, mes); \
}
  

/* rtov --- 実メモリアドレスを仮想メモリアドレスに変換する。
 */
#define RTOV(real)	((real) | 0x80000000)

/* ptov --- 実メモリのページ番号から仮想メモリアドレスを割り出す。
 */
#define PTOV(page)	((page << PAGE_SHIFT) | 0x80000000)


#endif /* __ITRON_MISC_H__ */
