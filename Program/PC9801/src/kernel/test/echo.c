/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* echo.c --- echo �ץ����
 *
 * ���ΥǥХ����ɥ饤�Фϡ������ƥ�ΥǥХå��Ѥ˺���������
 * 
 */

#include <itron.h>
#include <errno.h>

static void echo_server ();

/* �ǥХå��Ѥ� printf 
 */
#define DPRINTF(x)	{ printf x; }

Main ()
{
  dbg_puts ("hello world.\n");
  slp_tsk ();
}

