/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#ifndef __STDLIB_H__
#define __STDLIB_H__	1

#include <h/types.h>
#include <h/errno.h>
#ifdef notdef
#include <ibmpc/ibmpc.h>
#endif
#include "ITRON/h/itron_module.h"
#ifdef notdef
#include "ITRON/kernlib/device.h"
#include "ITRON/servers/port-manager.h"
/* #include "../kernlib/libkernel.h" */
#include "BTRON/device/wconsole/wconsole.h"
#include "BTRON/device/keyboard/keyboard.h"
#include "BTRON/device/ide/ide.h"
#endif
#include "POSIX/manager/posix_syscall.h"
#include "POSIX/manager/sfs/sfs_fs.h"
#include "POSIX/manager/posix_fs.h"
#include "POSIX/manager/posix_mm.h"
#include "POSIX/lowlib/lowlib.h"

#define putchar(ch)	putc (ch, stdout)

#define BUFSIZE		1024
#define MAXFILENAME	255
#define NFILE		10
#define MAXFSNAME	8
#define MAXDIRENTRY	100

#ifndef ROUNDUP
#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
#endif


/*
 * SCSI デバイスの ID 2、パーティション 1 を root にする
 */
#define ROOT_DEV	"driver.scsi"
#define ROOT_PART	((0x2 << 7) | 1)
#define ROOT_TYPE	"dosfs"


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


#ifdef notdef
extern ID	console;
extern ID	dev_recv;	/* デバイスドライバからの返答用 */
#endif

extern W	getc (FILE *port);
extern W	putc (W ch, FILE *port);
extern W	fputs (B *line, FILE *port);
extern W	readchar (ID port);
extern W 	writechar (ID port, UB *buf, W count);
extern W	printf (B *fmt,...);
extern W	fprintf (FILE *port, B *fmt,...);

#endif /* __STDLIB_H__ */
