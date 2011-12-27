/*

B-Free Project ÇÃê∂ê¨ï®ÇÕ GNU Generic PUBLIC LICENSE Ç…è]Ç¢Ç‹Ç∑ÅB

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#ifndef __MKFS_H__
#define __MKFS_H__	1


#include <fcntl.h>
#include "../../ITRON/h/types.h"
#include "../manager/sfs/sfs_fs.h"
#include "../manager/posix_fs.h"


#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))


#endif /* __MKFS_H__ */
