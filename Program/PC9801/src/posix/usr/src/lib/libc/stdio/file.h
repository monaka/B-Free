/* POSIX Library for stdio.
 *
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/lib/libc/stdio/file.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

#ifndef __POSIX_FILE_H__
#define __POSIX_FILE_H__	1

#include <types.h>
#include <stdlib.h>


struct file
{
  port_t	fid;
  void		*buf;
  int		bufsize;
};

typedef struct file	FILE;


extern FILE	_file_buf[];


/*
 * バッファード I/O 用の関数群の定義
 */
extern FILE	*fopen (char *path, char *mode, ...);
extern FILE	*fclose (FILE *fp);
extern 


#endif /* __POSIX_FILE_H__ */

