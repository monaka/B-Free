/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*************************************************************************
 *
 *		2nd BOOT types definition.
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/boot/2nd-opts/types.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 * $Log: types.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1999-03-29 22:47:51  kishida0
 * for test use (2nd boot optional)
 *
 * Revision 1.2  1996/11/13 16:29:05  night
 * TCODE �η��� unsigned short ���� unsigned char ���ѹ�������
 *
 * Revision 1.1  1996/05/11  10:45:07  night
 * 2nd boot (IBM-PC �� B-FREE OS) �Υ�������
 *
 * Revision 1.3  1995/09/21  15:50:44  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1994/07/30  17:37:20  night
 * �ե�����������ܸ�ʸ���򤹤٤� EUC �����ɤ��ѹ���
 *
 * Revision 1.1  1993/10/11  21:30:02  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:28  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:28  btron
 * BTRON SYSTEM 1.0
 * 
 */

#ifndef	__TYPES_H__
#define	__TYPES_H__	1

typedef	char		SBYTE;
typedef unsigned char	BYTE;
typedef	short		WORD16;
typedef	unsigned short	UWORD16;
typedef long		WORD32;
typedef unsigned long	UWORD32;
typedef long		WORD;
typedef unsigned long	UWORD;
typedef long		LONG;
typedef unsigned long	ULONG;
typedef float		FLOAT;
typedef double		DOUBLE;
typedef void		VOID;

typedef enum { FALSE=0, TRUE=1 } BOOL;
typedef unsigned char	SCODE;			/* ASCII code */
typedef unsigned char	TCODE;			/* TRON code */

typedef char		*SBPTR;
typedef unsigned char	*BPTR;
typedef short		*WPTR;
typedef unsigned short	*UWPTR;
typedef long		*LPTR;
typedef unsigned long	*ULPTR;

typedef TCODE		*TPTR;
typedef SCODE		*SPTR;
typedef int		(*FUNCP)();

#define SNULL		(BYTE)0
#define TNULL		(WORD)0
#define NULL		(LONG)0

#define LOCAL		static
#define GLOBAL		/* ����������ʤ� */
#define IMPORT		extern

#define MAXINT		(0x7fffffff)

#endif __TYPES_H__

