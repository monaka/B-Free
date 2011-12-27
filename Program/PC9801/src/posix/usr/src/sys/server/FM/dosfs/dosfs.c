/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dosfs/dosfs.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dosfs/dosfs.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: dosfs.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:53:40  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  11:03:01  night
 * �ǽ����Ͽ
 *
 *
 */


/*
 * ���Υե�����ϡ�B-Free OS/POSIX �Ķ���ư�� MS-DOS �ե����륷���ƥ�
 * �����Фΰ����Ǥ���
 *
 */


#include "../FM.h"

static W  dos_null();
static W  dos_lookup();
static W  dos_getattr();
static W  dos_putattr();
static W  dos_read();
static W  dos_write();
static W  dos_trunc();
static W  dos_mkdir();
static W  dos_rmdir();
static W  dos_mknod();
static W  dos_rmnod();
static W  dos_link();
static W  dos_remove();
static W  dos_mount();
static W  dos_unmount();


struct subfs_proc dos_proc =
{
  dos_null,
  dos_lookup,
  dos_getattr,
  dos_putattr,
  dos_read,
  dos_write,
  dos_trunc,
  dos_mkdir,
  dos_rmdir,
  dos_mknod,
  dos_rmnod,
  dos_link,
  dos_remove,
  dos_mount,
  dos_unmount,
};


/*
 * ����ѿ��������������ݤ��롣
 *
 */


/*
 * DOS �ե����륷���ƥ�ν������Ԥ��ؿ���
 */
struct subfs_t *
dos_init(ID device)
{
}


/*
 * ���⤷�ʤ��ؿ�
 */
static W
dos_null()
{
}



static W
dos_lookup()
{
}


static W
dos_getattr()
{
}


static W
dos_putattr()
{
}


static W
dos_read()
{
}


static W
dos_write()
{
}


static W
dos_trunc()
{
}


static W
dos_mkdir()
{
}


static W
dos_rmdir()
{
}


static W
dos_mknod()
{
}


static W
dos_rmnod()
{
}


static W
dos_link()
{
}


static W
dos_remove()
{
}


static W
dos_mount()
{
}


static W
dos_unmount()
{
}
