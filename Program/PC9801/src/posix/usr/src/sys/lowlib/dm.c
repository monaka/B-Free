/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/dm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/lowlib/dm.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: dm.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:55  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/03/04  14:23:10  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX �ǥХ��������ޥ͡����� (DM) ���̿���Ԥ�����δؿ���
 *
 * ����Ū�˥ǥХ��������ޥ͡����㤬��������ѥ��åȤμ���ʬ�����ؿ���¸
 * �ߤ��Ƥ��롣
 *
 * ���Υե����������������ؿ��ϰʲ��ΤȤ��ꡧ
 *
 * dm_open
 * dm_close
 * dm_add
 * dm_delete
 *
 */


#include <sys/types.h>
#include <errno.h>
#include <sys/portmanager.h>
#include <sys/mm.h>
#include <sys/pm.h>
#include <sys/fm.h>
#include "funcs.h"

/*
 *
 */
ID
dm_open (char *device_name, unsigned int permission)
{
}


/*
 *
 */
ID
dm_close (ID port)
{
}

/*
 *
 */
ID
dm_add (char *device_name, ID port)
{
}

/*
 *
 */
ID
dm_delete (ID port)
{
}


