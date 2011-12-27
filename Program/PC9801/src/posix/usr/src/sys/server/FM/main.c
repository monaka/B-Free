/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/FM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:38  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:15:15  night
 * �Ȥꤢ����ư���褦�ˤ����ѹ���
 *
 * Revision 1.1  1995/03/18  14:01:10  night
 * �ǽ����Ͽ
 *
 *
 */


/*
 * ���Υե�����ϡ�B-Free OS/POSIX �Ķ���ư���ե����륵���Фΰ����Ǥ���
 *
 *
 */

#include <sys/types.h>
#include <sys/errno.h>
#include <server/file.h>

#include "FM.h"
#include "dosfs/dosfs.h"


/*
 * ����ѿ������ (�ºݤ˾�����ݤ���)
 */
ID		recvport;		/* �׵�����Ĥ��ѥݡ��� */
struct subfs_t	*rootfs;

/*
 * ������ؿ������
 */
static void	init_file_manager (void);


/*
 *
 */
void
_main (void)
{
  fm_msg_t	request;

  init_file_manager ();		/* �ե�����ޥ͡��������Τν���� */

  /*
   * ��å������μ��� - ���� - �����Υ롼��
   */
  for (;;)
    {
      /* ��å������μ��� (get_req �� libkernel.a �ˤ���ؿ�) */
      if (get_req (recvport, &request, sizeof (request)) > 0)
	{
	  /*
	   * �ꥯ�����Ȥν����������� doit() �ǹԤ���
	   */
	  doit (&request);
	}
      else
	{
	  /* 
	   * �����ǥ��顼�Ȥʤä���
	   * ���顼�Ȥʤä������� reject ���ơ������׵������Ĥ��롣
	   */
	}
    }
  /* DO NOT REACHED */
}


/*
 * �ե�����ޥ͡�����ν����
 *
 * o �ե�����ơ��֥� (file_table) �ν����
 * o �׵�����Ĥ��ѤΥ�å������Хåե� ID ��ݡ��ȥޥ͡��������Ͽ
 * o root �ե����륷���ƥ�Υޥ����
 *   (root �ե����륷���ƥब�ɤΥǥХ������б����뤫�ϡ�config.c �ˤ�
 *    ���ꤹ��)
 */
static void
init_file_manager (void)
{
  int		i;
  ID		root_dev;
  ER		error;

  /*
   * �׵�����Ĥ��ѤΥݡ��Ȥ��������롣
   */
  recvport = get_port (sizeof (struct fm_msg), sizeof (struct fm_msg));
  if (recvport <= 0)
    {
      dbg_printf ("FM: cannot make receive porrt.\n");
      slp_tsk ();
      /* ��å������Хåե������˼��� */
    }

  error = regist_port (POSIX_FILE_SERVER, recvport);
  if (error != E_OK)
    {
      /* error */
    }

#ifdef notdef
  /*
   * (POSIX �Ķ��ˤȤäƤ�) root �ե����륷���ƥ��ޥ����
   *
   * �ǥХ����ɥ饤�Фϥե�����ޥ͡����㤬��ư�������ˤ��Ǥ˵�ư����Ƥ��ꡢ
   * �ɥ饤�Ф��׵�����դ��ݡ��Ȥϡ��ݡ��ȥޥ͡��������Ͽ����Ƥ��롣
   * config.h �� root �ե����륷���ƥ�Ȥʤ�ǥХ����ɥ饤�ФΥݡ���̾
   * �����ꤷ�Ƥ��롣
   * �ǥХ����ɥ饤�Ф��׵�����Ĥ��ݡ��Ȥ�ݡ��ȥޥ͡����㤫���������
   * root �ե����륷���ƥ�Υե����륷���ƥॿ���� (����� config.h ��
   * ����) �˹�ä��ե����륷���ƥ�ޥ͡������ư���ǥХ����ݡ��Ȥ�
   * �Ϥ���
   */

  /* 
   * �ǥХ����ɥ饤�Ф��׵�����դ��ݡ��Ȥ�ݡ��ȥޥ͡����㤫������� 
   * �롣
   */
  find_port (ROOTDEV, &root_dev);

  /*
   * DOS/FS ��ư���롣���줬 root �ե����륷���ƥ�Ȥʤ롣
   * �ѿ� root_fs �ϥե����륷���ƥ�δ����Ȥ��ơ��ե������ traverse 
   * ����Ԥ��Ȥ��γ������Ȥʤ롣
   * --- �����󡢤���ϥ���ե����졼�����ˤ�ä��ѹ��Ǥ���褦��
   * ����ɬ�פ����롣
   */
#ifdef ROOTDOS
  rootfs = dos_init (root_dev);
#endif /* ROOTDOS */
#endif /* notdef */

#ifdef DIRCACHE
  /*
   * �ǥ��쥯�ȥꥭ��å��嵡���ν����
   */
  init_dircache ();
#endif /* DIRCACHE */
}

