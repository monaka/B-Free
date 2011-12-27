/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/doit.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: doit.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:41  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:22:04  night
 * ��å�������¤�Τ� posix_message_t ���� mm_args ���ѹ���
 *
 * Revision 1.1  1995/03/18  14:01:51  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX �Ķ��ǤΥ���ޥ͡����㡣
 * ���Υե�����Ǥϡ����������׵��������롣
 */

#include <sys/types.h>
#include <server/memory.h>

/*
 * �����Ȥä��׵�ϡ����� request ��������롣
 * 
 */
doit (struct mm_msg *request)
{
  struct mm_res	res;

  switch (request->prog)
    {
    default:	/* unknown mesages */
      bzero (&res, sizeof res);
      (enum mm_error)(res.errorcode) = EMM_UNKNOWNMSG;
      snd_mbf (request->recvport, &res, sizeof (res));
      break;

    case MM_CREATE:	/* ���۶��֤����� */
    case MM_REMOVE:	/* ���۶��֤κ�� */
    case MM_WRITE:	/* ���۶��֤˾����񤭹��� */
    case MM_READ:	/* ���۶��֤����Ƥ��ɤߤ��� */
    case MM_STAT:	/* ���۶��֤ˤĤ��Ƥξ����������� */
      bzero (&res, sizeof res);

      res.errorcode = EMM_NOSUPPORT;
      snd_mbf (request->recvport, &res, sizeof (res));
      break;
    }
}


