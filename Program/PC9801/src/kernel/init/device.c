/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Id: device.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "$Id: device.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*
 * $Log: device.c,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-12-05 14:32:41  night
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


FILE	__file_table__[NFILE];


W
init_device (void)
{
  if (find_port (KEYBOARD_DRIVER, &keyboard) != E_PORT_OK)
    {
      dbg_printf ("Init: Cannot open keyboard device.\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }
  
  if (find_port (CONSOLE_DRIVER, &console) != E_PORT_OK)
    {
      dbg_printf ("Init: Cannot open console device.\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

  dev_recv = get_port (sizeof (DDEV_RES), sizeof (DDEV_RES));
  if (dev_recv <= 0)
    {
      dbg_printf ("Init: Cannot allocate port\n");
      slp_tsk ();
      /* DO NOT REACHED */
    }

  __file_table__[STDIN].device = keyboard;
  __file_table__[STDIN].count = 0;
  __file_table__[STDIN].length = 0;
  __file_table__[STDIN].bufsize = BUFSIZE;

  __file_table__[STDOUT].device = console;
  __file_table__[STDOUT].count = 0;
  __file_table__[STDOUT].length = 0;
  __file_table__[STDOUT].bufsize = BUFSIZE;

  __file_table__[STDERR].device = console;
  __file_table__[STDERR].count = 0;
  __file_table__[STDERR].length = 0;
  __file_table__[STDERR].bufsize = BUFSIZE;

  console_clear ();
}


void
console_clear (void)
{
  DDEV_REQ		req;		/* 要求パケット */
  DDEV_RES		res;		/* 返答パケット */
  W			rsize;
  ER			error;
  W			i;
  
  req.header.mbfid = dev_recv;
  req.header.msgtyp = DEV_CTL;
  req.body.ctl_req.dd = 0xAA;
  req.body.ctl_req.cmd = 1;	/* clear console */
  error = snd_mbf (console, sizeof (req), &req);
  if (error != E_OK)
    {
      printf ("cannot send packet. %d\n", error);
      return;
    }
  rsize = sizeof (res);
  rcv_mbf (&res, (INT *)&rsize, dev_recv);
}

