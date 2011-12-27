#ifndef __POSIX_IF_H__
#define __POSIX_IF_H__
/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: posix_if.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: posix_if.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: posix_if.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1998-11-26 11:55:24  monaka
 *
 * First version.
 *
 */

#include "init.h"

#define LOWLIB_INFO_ADDR	0x88030000
#define POSIX_MIN_PRIORITY	10
#define X_OK			1


struct posix_process
{
  W	procid;

  ID	maintask;
  ID	signaltask;

  UW	text_start;
  UW	text_size;

  UW	data_start;
  UW	data_size;

  UW	stack_start;
  UW	stack_size;

  UW	entry;
};


extern W posix_init (W root_device);
extern W posix_access (B *path, W oflag);
extern W posix_open (B *path, W oflag, W mode);
extern W posix_close (W fileid);
extern W posix_read (W fileid, B *buf, W length);
extern W posix_write (W fileid, B *buf, W length);
extern W posix_opendir (B *path);
extern W posix_closedir (W fileid);
extern W posix_readdir (W fileid, B *buf, W length);
extern W posix_lseek (W fileid, W offset, UW mode);
extern W posix_stat (W fileid, struct stat *st);
extern W posix_newprocess (B *program);
extern W posix_load_program (W fileid, struct posix_process *procinfo);
extern W posix_exec (B *path);
extern W posix_fork (void *main_func, void *signal_func);

#endif /* __POSIX_IF_H__ */
