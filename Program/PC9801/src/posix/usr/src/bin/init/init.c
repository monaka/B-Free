/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/init.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/bin/init/init.c,v 1.1 2011/12/27 17:13:36 liu1 Exp $";

/*

 POSIX �Ķ�������ץ����


 POSIX �Ķ���Ω���夲���
 ------------------------

 1) BOOT�ץ���ब Portmanager, POSIX server(s) �������˥���
 2) portmanager �μ¹�
 3) POSIX server �ε�ư
    src/kernel/make/config.tab �˽񤤤Ƥ�����֤ǵ�ư
    (1) �ץ����ޥ͡�����ε�ư
    (2) ����ޥ͡�����ε�ư
    (3) �ե�����ޥ͡�����ε�ư
 4) init (�ܥץ����)�ε�ư
    (1) root �ե����륷���ƥ��ޥ���Ȥ���
    (2) /etc/init.sh �����Ƥ�¹Ԥ���

 */
#include "init.h"

/*
 * init �ץ����� main �ؿ���
 * 
 *
 */
__main ()
{
  init_program ();
  banner ("POSIX Environment started.\n");
  mount_root (ROOTDEV);
  do_inittab ();
  for (;;)
    {
      slp_tsk ();
    }    
}

init_program ()
{
  dbg_printf ("init: start\n");
}


do_inittab (void)
{
#ifdef notdef
  interpret (INITTAB);
#endif /* notdef */
}
