/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Id: device.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "$Id: device.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: device.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-05-15 09:57:58  naniwa
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

#include "init.h"
#include "../device/wconsole/wconsole.h"

static void InitFileTable(void);

FILE	__file_table__[NFILE];


void
init_device (void)
{
  ID keyboard;
  ER result;
  W dev_desc;

  InitFileTable();

  result = open_device(KEYBOARD_DRIVER, &keyboard, &dev_desc);
  if (result != E_OK || dev_desc != STDIN)
    {
      dbg_printf ("Init: Cannot open keyboard device.\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

  result = open_device(WCONSOLE_DRIVER, &console, &dev_desc);
  if (result != E_OK || dev_desc != STDOUT)
    {
      dbg_printf ("Init: Cannot open console device.\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

  __file_table__[STDERR].device = console;
  __file_table__[STDERR].count = 0;
  __file_table__[STDERR].length = 0;
  __file_table__[STDERR].bufsize = BUFSIZE;


  dev_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (dev_recv <= 0)
    {
      dbg_printf ("Init: Cannot allocate port\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

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
      dbg_printf ("init: File table exhaust.");
      return (E_NOMEM);
    }

  if (find_port (dev_name, id) != E_PORT_OK)
    {
      dbg_printf ("init: Port not found.");
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
