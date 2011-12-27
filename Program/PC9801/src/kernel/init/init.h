/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#ifndef __INIT_H__
#define __INIT_H__	1

#include <h/types.h>
#include <h/errno.h>
#include <pc9801/pc98.h>
#include "../kernlib/device.h"
#include "../servers/port-manager.h"
#include "../kernlib/libkernel.h"
#include "../device/console/console.h"
#include "../device/keyboard/keyboard.h"

#define putchar(ch)	putc (ch, stdout)

#define MAJOR_VERSION	0
#define MINOR_VERSION	1


#define BUFSIZE	1024
#define NFILE	3

#define STDIN	0
#define STDOUT	1
#define STDERR	2


struct file
{
  ID	device;
  W	count;
  W	length;
  W	bufsize;
  UB	buf[BUFSIZE];
};

typedef struct file	FILE;

extern FILE	__file_table__[];

#define stdin	(&__file_table__[0])
#define stdout	(&__file_table__[1])
#define stderr	(&__file_table__[2])


struct command
{
  B	*name;
  W	(*func)();
};

extern struct command	command_table[];


extern ID	keyboard;
extern ID	console;
extern ID	dev_recv;	/* デバイスドライバからの返答用 */

extern void	banner (void);
extern void	eval (B *line);
extern W	read_line (FILE *port, B *line, W length);
extern W	parse_line (B *line, B **av, W size);
extern W	getc (FILE *port);
extern W	putc (W ch, FILE *port);
extern W	fputs (B *line, FILE *port);
extern W	readchar (ID port, ID resport);
extern W 	writechar (ID port, ID resport, UB *buf, W count);
extern W	printf (B *fmt,...);

extern void	console_clear ();

/* lib.c */
extern B	*strchr (B *s, W ch);
extern B	*strnchr (B *s, W ch, W size);


#endif /* __INIT_H__ */
