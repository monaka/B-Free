/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dircache.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/dircache.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: dircache.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:38  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:14:30  night
 * dircache �ε�ǽ��Ȥ�ʤ����꤬�Ǥ���褦�� #ifdef DIRCACHE ... #endif
 * ���ɲá�
 *
 * Revision 1.1  1995/03/18  14:01:11  night
 * �ǽ����Ͽ
 *
 *
 */

#ifdef DIRCACHE
/*
 * ���Υե�����ϡ�B-Free OS/POSIX �Ķ���ư���ե����륵���Фΰ����Ǥ���
 * ���Υե�����Ǥϡ��ǥ��쥯�ȥꥭ��å���δ�����Ԥ��ޤ���
 *
 */

#include "FM.h"

/*
 * �ǥ��쥯�ȥ�Υ���å���Ρ���
 */
struct dir_entry
{
  char	dirname[PATH_MAX];	/* 1024 */
  ID	port;
};


/*
 * ����ѿ������ (�ºݤ˾�����ݤ���)
 */
/*
 * �ǥ��쥯�ȥꥭ��å���ơ��֥�
 */
struct dir_entry	dircache[MAXDIRCACHE];



/* 
 * �ǥ��쥯�ȥꥭ��å���ν����
 */
void
init_dircache (void)
{
  return;
}


/*
 * �ǥ��쥯�ȥꥭ��å���˥���ȥ���ɲä��롣
 */
void
add_dircache (char *dir, ID port)
{
  return;
}


/*
 * �ǥ��쥯�ȥꥭ��å��夫����ꤷ���ѥ�̾���ĥǥ��쥯�ȥ��ߤĤ�
 * �롣
 * �ǥ��쥯�ȥ�Υ���ȥ꤬����å������ˤ��ä����ˤϡ����Υǥ���
 * ���ȥ�ؤĤʤ���ݡ��Ȥ��֤����ߤĤ���ʤ��ä����ˤϡ�0 ���֤���
 *
 */
ID
find_dircache (char *dir)
{
  return (0);
}


#endif DIRCACHE
