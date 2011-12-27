/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/sh/sh.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */

/* $Log: sh.h,v $
/* Revision 1.1  2011/12/27 17:13:35  liu1
/* Initial Version.
/*
/* Revision 1.2  1995-09-21 15:52:09  night
/* ソースファイルの先頭に Copyright notice 情報を追加。
/*
 * Revision 1.1  1995/03/18  14:15:13  night
 * 最初の登録
 *
 *
 */


#ifndef _SH_H_
#define _SH_H_	1

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define	MAXLINE	1000


struct internal_command
{
  char	*cmd;
  int	(*func)();
};

extern int	stdin_fd;
extern int	stdout_fd;
extern int	stderr_fd;

extern struct internal_command	command_table[];

#endif /* _SH_H_ */
