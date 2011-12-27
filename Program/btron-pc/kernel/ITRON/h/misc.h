/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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

/* 
#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
*/
#ifndef ROUNDUP
#define ROUNDUP(x,align)	((((int)x) + (align) - 1) & ~((align) - 1))
#endif

#define CUTDOWN(x,align)	(((unsigned long)x) / ((unsigned long)align) * ((unsigned long)align))
#define TABLE_SIZE(table)	(sizeof(table) / sizeof(table[0]))
#define ISDIGIT(x)		((x >= '0') && (x <= '9'))
#define MAX(a,b)		((a > b) ? a : b)
#define MIN(a,b)		((a < b) ? a : b)


#define ASSERT(exp,mes)		\
if (exp) \
{\
   printk ("ASSERT (%s %d): %s\n", __FILE__, __LINE__, mes); \
}
  

/* rtov --- �¥��ꥢ�ɥ쥹���ۥ��ꥢ�ɥ쥹���Ѵ����롣
 */
#define RTOV(real)	((real) | 0x80000000)

/* ptov --- �¥���Υڡ����ֹ椫�鲾�ۥ��ꥢ�ɥ쥹����Ф���
 */
#define PTOV(page)	((page << PAGE_SHIFT) | 0x80000000)


#endif /* __ITRON_MISC_H__ */
