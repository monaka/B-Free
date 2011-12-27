/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* POSIX Library for stdio.
 *
 */

/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/stdio.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */


#ifndef __STDIO_H__
#define __STDIO_H__	1

#include <sys/types.h>
#include <native/port.h>
#include <stdlib.h>


struct file
{
  port_t	fid;
  void		*buf;
  int		bufsize;
};

typedef struct file	FILE;


extern FILE	_file_buf[];


#define stdin	(&_file_buf[0])
#define stdout	(&_file_buf[1])
#define stderr	(&_file_buf[2])


/*
 * バッファード I/O 用の関数群の定義
 */
extern FILE	*fopen (char *path, char *mode);
extern FILE	*freopsn (const char *path, char *mode, FILE *stream);
extern int	*fclose (FILE *stream);
extern int	fputc (int c, FILE *stream);
extern int	putc (int c, FILE *stream);
extern int	putchar (int c);
extern int	fputs (const char s, FILE *stream);
extern int	putw (int w, FILE *stream);
extern int	fgetc (FILE *stream);
extern int	getc (FILE *stream);
extern int	getchar (void);
extern int	getw (FILE *stream);
extern char	*fgets (char *line, int n, FILE *stream);
extern char	*gets (char *s);
extern int	printf (const char *format, ...);
extern int	fprintf (FILE *stream, const char *format, ...);
extern int	sprintf (char *s, const char *format, ...);


#endif /* __STDIO_H__ */

