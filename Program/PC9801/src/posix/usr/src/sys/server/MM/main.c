/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */
static char rcsid[] = "@(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/src/sys/server/MM/main.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

/*
 * $Log: main.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.3  1995-09-21 15:53:42  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.2  1995/08/05  12:22:51  night
 * ��å�������¤�Τ� posix_message_t ���� mm_args ���ѹ���
 *
 * Revision 1.1  1995/03/18  14:01:50  night
 * �ǽ����Ͽ
 *
 *
 */

/*
 * POSIX �Ķ��ǤΥ���ޥ͡�����
 *
 */

#include <sys/types.h>
#include <sys/errno.h>
#include <server/memory.h>


/*
 * ����ѿ������ (�ºݤ˾�����ݤ���)
 */
ID	recvport;	/* �׵��������뤿��Υ�å������ݡ��� */

/*
 * ���Υե���������ǻ��Ѥ���ؿ�
 */
static void	init_memory_manager (void);


/*
 * ����ޥ͡�����ν�����Ԥ���
 *
 */
void
_main (int ac, char **av)
{
  struct mm_msg	request;
  int		error;

  init_memory_manager ();	/* ����ޥ͡��������Τν���� */

  /*
   * ��å������μ��� - ���� - �����Υ롼��
   */
  for (;;)
    {
      /* ��å������μ��� */
      error = get_req (recvport, &request, sizeof (request));
      if (error >= 0)
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
	  switch (error)
	    {
	    default:	/* Unknown error */
	      dbg_puts ("POSIX/MM: Unknown error\n");
	      break;

	    case EMM_SYSTEM:
	      break;
	    }
	}
    }
  /* DO NOT REACHED */
}


/*
 * ����ޥ͡�����ν����
 *
 *
 */
static void
init_memory_manager (void)
{
  int	error;

  init_pageinfo ();	/* ���ۥ������ν���� */

  recvport = get_port (sizeof (struct mm_msg), sizeof (struct mm_msg));
  if (recvport == 0)	/* ��å������ݡ��Ȥμ����˼��Ԥ��� */
    {
      /* error */
    }
  error = regist_port (MEMORY_MANAGER_NAME, recvport);
  if (error != E_OK)	/* ��å������ݡ��Ȥ���Ͽ�˼��Ԥ��� */
    {
      /* error */
    }
}

#ifdef notdef
/*
 * ��å������μ���������Ԥ�����δؿ���
 */
get_req (ID recvport, void *req, int size)
{
  int		rsize;
  struct mm_res	res;

  rcv_mbf (req, &size, recvport);
  if (size != rsize)
    {
      /* �����������פ��ʤ��ä���������»������ǽ�������� */
      /* ���顼���֤� */
      return (EMM_GARBADGE);
    }
  return (EMM_OK);
}
#endif
