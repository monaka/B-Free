/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/***************************************************************************
 *
 */

#ifndef __MACROS_H__
#define __MACROS_H__	1

#define ROUNDUP(x,align)	((((x) + ((align) - 1)) / (align)) * (align))
#define TABLE_SIZE(table)	(sizeof(table) / sizeof(table[0]))


#define ISDIGIT(x)	((x >= '0') && (x <= '9'))


#endif /* __MACROS_H__ */
