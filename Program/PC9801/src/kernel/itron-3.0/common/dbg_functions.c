/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* �����ƥ�ǥХå��Ѥδؿ��� 
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
