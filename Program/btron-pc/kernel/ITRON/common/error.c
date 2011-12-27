/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* error.cj
 *
 */

#include "itron.h"
#include "errno.h"
#include "version.h"
#include "func.h"


/* falldown --- ITRON �Ǥ� [panic] �ؿ���
 *
 * @desc
 *	falldown() �ϡ������ǻ��ꤵ�줿�ե����ޥåȤ˽�����(���󥽡���)���̤�
 *	��å�������ɽ�����롣
 *	�ե����ޥåȤΰ�̣�ϡ�printk () ��Ʊ����
 *
 *	��å�������ɽ��������ϡ�̵�¥롼�פ����롣
 *
 */
void
falldown (B *fmt, ...)
{
  VP arg0;
  vprintk (fmt, (VP)&arg0);

/* �⤷��ITRON_DBG �ޥ����������Ƥ���С��ǥХå���ư���롣
 */
#ifdef ITRON_DBG
  itron_debugger ();
#else
  dis_int ();
  for (;;)
    ;
#endif /* ITRON_DBG */
}
