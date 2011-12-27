/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/errno.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: errno.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:51:52  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.2  1995/08/05  12:10:46  night
 * インクルード文 #include "../../../kernel/itron-3.0/h/errno.h" を追加。
 *
 * Revision 1.1  1995/03/18  14:14:37  night
 * 最初の登録
 *
 *
 */

#ifndef __POSIX_ERRNO_H__
#define __POSIX_ERRNO_H__	1

#include <sys/perrno.h>
#include "../../../kernel/itron-3.0/h/errno.h"

/*
 * POSIX で定義している error 番号
 */
#define E2BIG		1	/* Arg list too long */
#define EACCESS		2	/* Permission denied */
#define EAGAIN		3	/* Resouce temporarily unavailable */
#define EBADF		4	/* Bad file descriptor */
#define EBUSY		5	/* Resouce busy */
#define ECHILD		6	/* No child process */
#define EDEADLK		7	/* Resouce deadlock avoided */
#define EDOM		8	/* Domain error */
#define FEXIST		9	/* File exists */
#define EFAULT		10	/* Bad address */
#define EFBIG		11	/* File too large */
#define EINTR		12	/* Interrupted function call */
#define EINVAL		13	/* Invalid argument */
#define EIO		14	/* Input/output error */
#define EISDIR		15	/* Is a directory */
#define EMFILE		16	/* Too many open files */
#define EMLINK		17	/* Too many links */
#define ENAMETOOLONG	18	/* Filename too long */
#define ENFILE		19	/* Too many open files in system */
#define ENODEV		20	/* No such device */
#define ENOENT		21	/* No such file or directory */
#define ENOEXEC		22	/* Exec format error */
#define ENOLCK		23	/* No locks available */
#define ENOMEM		24	/* Not enough space */
#define ENOSPC		25	/* No space left on device */
#define ENOSYS		26	/* Function not implemented */
#define ENOTDIR		27	/* Not a directory */
#define ENOTEMPTY	28	/* Directory not empty */
#define ENOTTY		29	/* Inappropriate I/O control operation */
#define ENXIO		30	/* No such device or address */
#define EPERM		31	/* Operation not permitted */
#define EPIPE		32	/* Broken pipe */
#define ERANGE		33	/* Result too large */
#define EROFS		34	/* Read-only file system */
#define ESPIPE		35	/* Invalid seek */
#define ESRCH		36	/* No such process */
#define EXDEV		37	/* Improper link */


extern int	errno;

extern void	perror (const char *msg);
extern char	*strerror (int errno);


#endif /* __POSIX_ERRNO_H__ */

