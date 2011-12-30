/*

B-Free Project §Œ¿∏¿Æ ™§œ GNU Generic PUBLIC LICENSE §ÀΩæ§§§ﬁ§π°£

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* <errno.h> --- •®•È°º»÷πÊ§ŒƒÍµ¡
 *
 * $Date: 2011/12/30 00:57:06 $
 * $Revision: 1.2 $
 * $Author: liu1 $
 * $Log: errno.h,v $
 * Revision 1.2  2011/12/30 00:57:06  liu1
 * „Ç≥„É≥„Éë„Ç§„É´„Ç®„É©„Éº„ÅÆ‰øÆÊ≠£„ÄÇ
 *
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-05-11 10:45:01  night
 * 2nd boot (IBM-PC »« B-FREE OS) §Œ•Ω°º•π°£
 *
 * Revision 1.3  1995/09/21 15:50:37  night
 * •Ω°º•π•’•°•§•Î§Œ¿Ë∆¨§À Copyright notice æ Û§Úƒ…≤√°£
 *
 * Revision 1.2  1994/07/30  17:37:14  night
 * •’•°•§•Î√Ê§Œ∆¸À‹∏Ï ∏ª˙§Ú§π§Ÿ§∆ EUC •≥°º•…§À —ππ°£
 *
 * Revision 1.1  1993/10/11  21:28:53  btron
 * btron/386
 *
 * Revision 1.1.1.1  93/01/14  12:30:18  btron
 * BTRON SYSTEM 1.0
 * 
 * Revision 1.1.1.1  93/01/13  16:50:33  btron
 * BTRON SYSTEM 1.0
 * 
 *
 * Discription
 *
 */

#ifndef __ERRNO_H__
#define __ERRNO_H__	1

#include "types.h"

#define E_OK		(0)	/* ¿µæÔΩ™Œª */

#define E_ADR		(-1)
#define E_PAR		(-2)
#define E_NOSPT		(-3)
#define E_NOSPC		(-4)
#define E_ID		(-5)
#define E_EXS		(-6)
#define E_NOEXS		(-7)
#define E_NOMEM		(-8)
#define E_CTX		(-9)
#define E_SYS		(-10)
#define E_UNF		(-11)
#define E_SZOVR		(-12)
#define E_MPTR		(-13)
#define E_AKEY		(-14)
#define E_MPURG		(-15)
#define E_MRELOC	(-16)
#define E_PPRI		(-17)
#define E_NOPRC		(-18)
#define E_SELF		(-19)
#define E_LEVEL		(-20)
#define E_MINTR		(-21)
#define E_DLT		(-22)
#define E_SEM		(-23)
#define E_NONE		(-24)
#define E_INIT		(-25)
#define E_FNAME		(-26)
#define E_FD		(-27)
#define E_ACCES		(-28)
#define E_PERM		(-29)
#define E_RONLY		(-30)
#define E_PWD		(-31)
#define E_ENDR		(-32)
#define E_REC		(-33)
#define E_NOLINK	(-34)
#define E_LOCK		(-35)
#define E_LIMIT		(-36)
#define E_XFS		(-37)
#define E_NOFS		(-38)
#define E_NODSK		(-39)
#define E_TRON		(-40)
#define E_DD		(-41)
#define E_DEV		(-42)
#define E_BUSY		(-43)
#define E_NODEV		(-44)
#define E_ERDEV		(-45)
#define E_NOMDA		(-46)
#define E_IO		(-47)

typedef	UWORD32		ERRNO;

#endif /* __ERRNO_H__ */



