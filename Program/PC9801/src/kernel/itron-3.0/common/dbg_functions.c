/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* システムデバッグ用の関数群 
 *
 */

#include "itron.h"
#include "errno.h"
#include "syscall.h"
#include "func.h"


ER
sys_dbg_puts (W *args)
{
  struct a
    {
      B *msg;
    } *arg = (struct a *)args;
  
  printf ("%s", arg->msg);
  return (E_OK);
}
