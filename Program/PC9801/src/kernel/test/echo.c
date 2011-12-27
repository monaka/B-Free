/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* echo.c --- echo プログラム
 *
 * このデバイスドライバは、システムのデバッグ用に作成した。
 * 
 */

#include <itron.h>
#include <errno.h>

static void echo_server ();

/* デバッグ用の printf 
 */
#define DPRINTF(x)	{ printf x; }

Main ()
{
  dbg_puts ("hello world.\n");
  slp_tsk ();
}

