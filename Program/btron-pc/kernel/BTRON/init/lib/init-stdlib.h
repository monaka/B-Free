/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#ifndef __INIT_STDLIB_H__
#define __INIT_STDLIB_H__

#include "../init.h"

extern W putc (W ch, FILE *port);
extern void fflush (FILE *port);
extern W fputs (B *line, FILE *port);
extern W writechar (ID port, ID resport, UB *buf, W length);
extern W isprint (W ch);
extern W isspace (W ch);
extern W isnum (W ch);
extern W getc (FILE *port);
extern W readchar (ID port, ID resport);


#endif

