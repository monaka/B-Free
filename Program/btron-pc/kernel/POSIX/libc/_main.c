/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Id: _main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: _main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: _main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.6  2000-01-29 16:21:21  naniwa
 * to clean up
 *
 * Revision 1.5  2000/01/18 14:39:03  naniwa
 * to make independent of libkernel.a
 *
 * Revision 1.4  2000/01/15 15:23:53  naniwa
 * to set file descriptor 0, 1 and 2
 *
 * Revision 1.3  1999/12/19 11:01:55  naniwa
 * modified to suit to libc
 *
 * Revision 1.2  1999/11/10 10:58:29  naniwa
 * correction
 *
 * Revision 1.4  1999/05/15 09:57:58  naniwa
 * modified to use wconsole
 *
 * Revision 1.3  1999/02/17 09:43:08  monaka
 * Modified for 'device descriptor'.
 *
 * Revision 1.2  1996/11/06 12:37:12  night
 * 実行開始時のコンソールクリア処理を変更した。
 *
 * Revision 1.1  1996/07/25  16:01:59  night
 * IBM PC 版用への最初の登録
 *
 * Revision 1.2  1995/12/05 14:32:41  night
 * init_device (void) の関数を追加。
 *
 * Revision 1.1  1995/10/10  16:21:47  night
 * 最初の登録.
 * システム起動後、プロンプトを出力してコマンド入力 - 実行ができるところ
 * まで出来た。ただし、コマンドは echo だけ。
 *
 *
 */

#include "others/stdlib.h"
#include "BTRON/device/wconsole/wconsole.h"

void init_device();
#ifdef notdef
void del_device();
#endif

W _main()
{
  W ac;
  B **av;

  init_device();
  main(ac, av);
#ifdef notdef
  del_device();
#endif
  _exit(1);
}

static void InitFileTable(void);

FILE	__file_table__[NFILE];
#ifdef notdef
ID dev_recv;
ID console;
#endif

void
init_device (void)
{
  ID keyboard;
  ER result;
  W dev_desc;

  InitFileTable();
#ifdef notdef
  result = open_device(KEYBOARD_DRIVER, &keyboard, &dev_desc);
  if (result != E_OK || dev_desc != STDIN)
    {
      dbg_printf ("LIBC: Cannot open keyboard device.\n");
      _exit(-1);
      /* DO NOT REACHED */
    }

  result = open_device(WCONSOLE_DRIVER, &console, &dev_desc);
  if (result != E_OK || dev_desc != STDOUT)
    {
      dbg_printf ("LIBC: Cannot open console device.\n");
      _exit(-1);
      /* DO NOT REACHED */
    }

  __file_table__[STDERR].device = console;
  __file_table__[STDERR].count = 0;
  __file_table__[STDERR].length = 0;
  __file_table__[STDERR].bufsize = BUFSIZE;
#else
#ifdef notdef
  if (find_port (WCONSOLE_DRIVER, &console) != E_PORT_OK) {
#ifdef DEBUG
    dbg_printf ("LIBC: Cannnot open console device.");
#endif
    _exit(-1);
  }
#endif
  __file_table__[STDIN].device = 0;
  __file_table__[STDIN].count = 0;
  __file_table__[STDIN].length = 0;
  __file_table__[STDIN].bufsize = BUFSIZE;
  __file_table__[STDOUT].device = 1;
  __file_table__[STDOUT].count = 0;
  __file_table__[STDOUT].length = 0;
  __file_table__[STDOUT].bufsize = BUFSIZE;
  __file_table__[STDERR].device = 1;
  __file_table__[STDERR].count = 0;
  __file_table__[STDERR].length = 0;
  __file_table__[STDERR].bufsize = BUFSIZE;
#endif

#ifdef notdef
  dev_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (dev_recv <= 0)
    {
#ifdef DEBUG
      dbg_printf ("LIBC: Cannot allocate port\n");
#endif
      _exit(-1);
      /* DO NOT REACHED */
    }
#endif
}

#ifdef notdef
void del_device()
{
  del_mbf(dev_recv);
}


ER
open_device (B *dev_name, ID *id, W *dev_desc)
{
  for(*dev_desc = 0;
      (*dev_desc < NFILE) && (__file_table__[*dev_desc].device != -1);
      (*dev_desc)++)
    {
      /* do nothing */
    }

  /* out of file table. */
  if (*dev_desc == NFILE)
    {
#ifdef DEBUG
      dbg_printf ("LIBC: File table exhaust.");
#endif
      return (E_NOMEM);
    }

  if (find_port (dev_name, id) != E_PORT_OK)
    {
#ifdef DEBUG
      dbg_printf ("LIBC: Port not found.");
#endif
      return (E_OBJ);
    }

  __file_table__[*dev_desc].device = *id;
  __file_table__[*dev_desc].count = 0;
  __file_table__[*dev_desc].length = 0;
  __file_table__[*dev_desc].bufsize = BUFSIZE;

  return (E_OK);
}

ER
close_device (W dev_desc)
{
  __file_table__[dev_desc].device = -1;

  return (E_OK);
}
#endif

static void
InitFileTable(void)
{
  int i;

  /* initialize __file_table__[] */
  for(i = 0; i < NFILE; i++)
    {
      __file_table__[i].device = -1;
      __file_table__[i].count = 0;
      __file_table__[i].length = 0;
      __file_table__[i].bufsize = BUFSIZE;
    }
}
