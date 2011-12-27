/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 * $Log: unlink.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  2000-05-06 03:54:33  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.3  2000/02/04 15:16:35  naniwa
 * minor fix
 *
 * Revision 1.2  1999/12/26 11:19:12  naniwa
 * minor fix
 *
 * Revision 1.1  1999/03/21 14:01:52  monaka
 * They are separated from syscall.c. Still no major changes available.
 *
 */

#include "posix.h"

/* psc_unlink_f - �ե������������
 *
 * ����:
 * 	req->param.par_unlink.path	�������ե�����Υѥ�̾
 *	req->param.par_unlink.pathlen	�ѥ�̾��Ĺ��
 *
 */
W
psc_unlink_f (struct posix_request *req)
{
#ifdef USE_ALLOCA
  B			*pathname;
#else
  B			pathname[MAX_NAMELEN];
#endif
  W			errno;
  struct inode		*startip;
  struct access_info	acc;


  /* �ѥ�̾��ƤӽФ����Υץ���(������)����
   * ���Ф���
   * �ƤӽФ����Υ����� ID �ϡ���å������ѥ�᡼����
   * ������äƤ��롣
   */
#ifdef USE_ALLOCA
  pathname = alloca (req->param.par_unlink.pathlen + 1);
  if (pathname == NULL)
    {
      /* ����������顼 */
      put_response (req, EP_NOMEM, 0, 0, 0);
      return (FAIL);
    }
#endif
#if 0
  bzero (pathname, req->param.par_open.pathlen + 1);
#endif
  errno = vget_reg (req->caller,
		    req->param.par_unlink.path,
		    req->param.par_unlink.pathlen + 1,
		    pathname);
  if (errno)
    {
      /* �ѥ�̾�Υ��ԡ����顼 */
      if (errno == E_PAR)
	put_response (req, EP_INVAL, 0, 0, 0);
      else
	put_response (req, EP_FAULT, 0, 0, 0);
	
      return (FAIL);
    }


  /* �ե����뤬���Хѥ��ǻ��ꤵ��Ƥ��뤫�ɤ�����
   * �����å����롣
   * (�ѥ�̾�� '/' �ǤϤ��ޤäƤ��ʤ����ˤϡ�
   * ���Хѥ��ȸ��ʤ�)
   * ���Хѥ��ǤϤ��ޤäƤ�����ˤϡ�
   * �ץ����Υ����ȥǥ��쥯�ȥ����Ф��ơ�
   * ������ե�����Υѥ��κǽ�Υǥ��쥯�ȥ��
   * ���롣
   */
  if (*pathname != '/')
    {
      errno = proc_get_cwd (req->procid, &startip);
      if (errno)
	{
	  put_response (req, errno, 0, 0, 0);
	  return (FAIL);
	}
    }
  else
    {
      startip = rootfile;
    }


  /* �ץ����Υ桼�� ID �ȥ��롼�� ID ��
   * ���Ф���
   * ���ξ���˴�Ť��ơ��ե���������Ǥ��뤫�ɤ�����
   * ���ꤹ�롣
   */
  errno = proc_get_euid (req->procid, &(acc.uid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  errno = proc_get_egid (req->procid, &(acc.gid));
  if (errno)
    {
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }

  errno = fs_remove_file (startip,
			  pathname,
			  &acc);
  if (errno)
    {
      /* �ե����뤬�����ץ�Ǥ��ʤ� */
      put_response (req, errno, 0, 0, 0);
      return (FAIL);
    }
  
  put_response (req, EP_OK, 0, 0, 0);
  return (SUCCESS);
} 
