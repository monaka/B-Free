/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* version.cj
 */

#include "itron.h"
#include "errno.h"
#include "version.h"

T_VER	version = { 0, 0, MAJOR_VERSION, MINOR_VERSION, { 0, 0, 0, 0 }, CPU };

/*******************************************************************************
 * get_ver --- �С�����󻲾�
 *
 */
ER
get_ver (T_VER *ver)
{
  bcopy ((B *)&version, (B *)ver, sizeof (T_VER));
  return (E_OK);
}

