/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/lowlib/lowlib.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

#ifndef __POSIX_LOWLIB_H__
#define __POSIX_LOWLIB_H__	1

#include "POSIX/manager/posix.h"
#include "POSIX/manager/posix_fs.h"
#include "POSIX/manager/posix_mm.h"
#include "POSIX/manager/posix_syscall.h"
#include "POSIX/libc/errno.h"
#include "ITRON/h/types.h"
#include "ITRON/h/lowlib.h"

#include "syscall.h"
#include "global.h"
#include "funcs.h"

#ifdef notdef
struct posix_process_info
{
  UW		errno;
  ID		recv_port;

  UW		procid;
  UW		entry_point;

  ID		main_task;
  ID		signal_task;
};

#endif

#ifdef notdef

/* UNIQ_PAGE: プロセス毎に別々の物理ページを割り当てる固定アドレス(ページ)
 * プロセス毎に固有の情報(プロセス ID など)を割り当てるために使用する。
 */
#define UNIQ_PAGE	0x7FFFFC00
#define PROC_INFO	((struct posix_process_info *)UNIQ_PAGE)

#define MY_PID	(PROC_INFO->proc_pid)
#define ERRNO	(PROC_INFO->errno)

#else

#define PROC_INFO	(LOWLIB_DATA)
#define MY_PID	(PROC_INFO->my_pid)
#define ERRNO	(PROC_INFO->errno)

#endif


#endif	/* __POSIX_LOWLIB_H__ */

