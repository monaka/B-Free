/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* error.h
 */

#ifndef _ITRON_ERROR_H_
#define _ITRON_ERROR_H_	1

#define ASSERT(expr,mes)	\
if (expr) {\
  printf ("ASSERT: %s %d %s\n", __FILE__, __LINE__, mes); \
  fatal("system stop.\n"); \
}




#endif /* _ITRON_ERROR_H_ */
