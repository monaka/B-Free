/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* This is part of the BTRON/386
   Define ANSI C.



   This file defienition BASIC types for BTRON/386.

 */

#ifndef _ITRON_TYPES_H_
#define _ITRON_TYPES_H_	1

typedef char		B;	/* 符号付き8ビット整数 */
typedef short		H;
typedef long		W;
typedef unsigned char	UB;
typedef unsigned short	UH;
typedef unsigned long	UW;

typedef long		VW;
typedef short		VH;
typedef char		VB;
typedef void		*VP;
typedef void		(*FP)(void);


typedef int		INT;
typedef unsigned int	UINT;

typedef enum { FALSE=0, TRUE=1 } BOOL;

typedef short		FN;
typedef int		ID;
typedef int		BOOL_ID;
typedef int		HNO;
typedef int		RNO;
typedef int		NODE;
typedef unsigned int	ATR;
typedef int		ER;
typedef int		PRI;
typedef int		TMO;

typedef UH		TC;
/*typedef TCODE		*TPTR;*/

#ifndef NULL
#define NULL		(0)
#endif
#ifndef SNULL
#define SNULL		((B)0)
#endif
#ifndef TNULL
#define TNULL		((TC)0)
#endif

#endif /* _ITRON_TYPES_H_ */
