/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* posix.h - POSIX 環境マネージャ用のヘッダファイル
 *
 *
 *
 */

#ifndef __POSIX_H__
#define __POSIX_H__	1


#include "h/itron.h"
#include "h/errno.h"
#include "kernlib/device.h"
#include "../../BTRON/device/console/console.h"
#include "servers/port-manager.h"

#define ROUNDUP(x,align)	((((int)x) + (align) - 1) & ~((align) - 1))
#define CUTDOWN(x,align)	(((unsigned long)x) / ((unsigned long)align) * ((unsigned long)align))


/* =========================== マクロ定義 =========================== */

#define POSIX_MANAGER	"manager.posix"


/* 各種制限値 */
#define MAX_BODY_SIZE	1024
#define MAX_LINE_LENGTH	512
#define MAXFS		1
#define MAX_MOUNT	5
#define MAX_INODE	200
#define MAX_OPEN	100
#define MAX_NAMELEN	255
#define MAX_SIGNAL	32
#define MAX_PROCESS	100

#define MAX_SYSTEM_NAMELENGTH		

#define POSIX_TASK_LEVEL	10


/* POSIX 環境マネージャが要求元に返すエラー番号 */
#define EP_OK			(0)	/* 成功		*/
#define EP_2BIG			(-1)	/* Arg list too long */
#define EP_ACCESS		(-2)	/* Permission denied */
#define EP_AGAIN		(-3)	/* Resouce temporarily unavailable */
#define EP_BADF			(-4)	/* Bad file descriptor */
#define EP_BUSY			(-5)	/* Resouce busy */
#define EP_CHILD		(-6)	/* No child process */
#define EP_DEADLK		(-7)	/* Resouce deadlock avoided */
#define EP_DOM			(-8)	/* Domain error */
#define EP_EXIST		(-9)	/* File exists */
#define EP_FAULT		(-10)	/* Bad address */
#define EP_FBIG			(-11)	/* File too large */
#define EP_INTR			(-12)	/* Interrupted function call */
#define EP_INVAL		(-13)	/* Invalid argument */
#define EP_IO			(-14)	/* Input/output error */
#define EP_ISDIR		(-15)	/* Is a directory */
#define EP_MFILE		(-16)	/* Too many open files */
#define EP_MLINK		(-17)	/* Too many links */
#define EP_NAMETOOLONG		(-18)	/* Filename too long */
#define EP_NFILE		(-19)	/* Too many open files in system */
#define EP_NODEV		(-20)	/* No such device */
#define EP_NOENT		(-21)	/* No such file or directory */
#define EP_NOEXEC		(-22)	/* Exec format error */
#define EP_NOLCK		(-23)	/* No locks available */
#define EP_NOMEM		(-24)	/* Not enough space */
#define EP_NOSPC		(-25)	/* No space left on device */
#define EP_NOSYS		(-26)	/* Function not implemented */
#define EP_NOTDIR		(-27)	/* Not a directory */
#define EP_NOTEMPTY		(-28)	/* Directory not empty */
#define EP_NOTTY		(-29)	/* Inappropriate I/O control operation */
#define EP_NXIO			(-30)	/* No such device or address */
#define EP_PERM			(-31)	/* Operation not permitted */
#define EP_PIPE			(-32)	/* Broken pipe */
#define EP_RANGE		(-33)	/* Result too large */
#define EP_ROFS			(-34)	/* Read-only file system */
#define EP_SPIPE		(-35)	/* Invalid seek */
#define EP_SRCH			(-36)	/* No such process */
#define EP_XDEV			(-37)	/* Improper link */
#define EP_NOSUP		(-100)	/* not support	*/



#define SUCCESS		1
#define FAIL		0

/* macros for waitpid */
#define WNOHANG		1 /* 子プロセスの終了を待たない */
#define WUNTRACED	2 /* ジョブ制御用:未実装 */


/* dirent */
struct dirent
{
  long			d_ino;
  unsigned long		d_off;
  unsigned short	d_reclen;
  char			d_name[1];
};




#include "posix_syscall.h"

#ifdef KERNEL


#include "assert.h"
#include "sfs/sfs_fs.h"
#include "posix_fs.h"
#include "posix_proc.h"
#include "posix_mm.h"
#include "utsname.h"
#include "elf.h"


#include "kernlib/libkernel.h"




/* ===========================  関数定義  =========================== */

/* posix.c */
extern void	posix_start (void);
extern void	banner (void);
extern void	_assert(B *msg);

/* request.c */
extern W	init_port (void);
extern W	get_request (struct posix_request *req);
extern W	put_response (struct posix_request *req, 
			      W errno,
			      W status,
			      W ret1,
			      W ret2);
extern W	error_response (struct posix_request *req, W errno);


/* log.c */
extern void	init_log (void);
extern W	printk (B *fmt,...);


/* global.c */
struct utsname	system_name;


#endif /* KERNEL */



#endif /* __POSIX_H__ */
