/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/fork.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


/* fork.c --- �ץ����� fork ��Ԥ�
 *
 *
 * $Log: fork.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.15  2000-05-06 03:52:24  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.14  2000/02/27 15:33:39  naniwa
 * minor change
 *
 * Revision 1.13  2000/01/26 08:24:33  naniwa
 * to prevent memory leak
 *
 * Revision 1.12  2000/01/21 14:22:30  naniwa
 * fixed copying file descriptors, etc
 *
 * Revision 1.11  2000/01/15 15:29:28  naniwa
 * minor fix
 *
 * Revision 1.10  2000/01/08 09:08:40  naniwa
 * fixed to copy working directory
 *
 * Revision 1.9  1999/11/10 10:48:09  naniwa
 * minor fix
 *
 * Revision 1.8  1999/07/23 14:39:48  naniwa
 * modfied to call lod_low
 *
 * Revision 1.7  1999/07/21 15:10:04  naniwa
 * modified to implement waitpid
 *
 * Revision 1.6  1999/03/24 03:54:44  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.5  1998/02/24 14:12:48  night
 * �ǥХå��ѥץ���ʸ���ɲá�
 *
 * Revision 1.4  1998/02/23 14:48:14  night
 * �ǥХå��ѥץ���ʸ���ɲá�
 *
 * Revision 1.3  1998/02/16 14:21:31  night
 * �ǥХå��ѥץ���ʸ���ɲá�
 *
 * Revision 1.2  1997/12/17 14:15:47  night
 * �ؿ� fork() ����Ȥ��ä���
 * �Ǥ�ޤ����������Ƥ��ʤ���
 *
 * Revision 1.1  1997/10/24 14:01:40  night
 * �ǽ����Ͽ
 *
 *
 *
 */

#include "posix.h"
#include "h/lowlib.h"



/* fork - �ץ����� fork ��Ԥ�
 *
 *
 */
W
fork (struct proc *parent, W *childid, ID main_task, ID signal_task)
{
  struct proc	*newproc;
  W		errno;
/*   ID		main_task, signal_task;
 */
  struct lowlib_data lowlib_data;

#ifdef FKDEBUG
  printk ("fork: call alloc_proc: (%s file, %d line), parent = 0x%x, parent->vm_tree = 0x%x\n",
	  __FILE__, __LINE__, parent, parent->vm_tree);	/* */
#endif

  /* �������ץ�����¤�Τ���ݤ���
   */
  errno = proc_alloc_proc (&newproc);
  if (errno)
    {
      return (errno);
    }
  *childid = newproc->proc_pid;
  newproc->proc_maintask = main_task;
  newproc->proc_signal_handler = signal_task;

  /* �ץ�������Υ��ԡ� */
  /* �������� (�ץ�������ӥե�����)�ι��� */
#ifdef FKDEBUG
  printk ("fork(): parent = 0x%x, parent->vm_tree = 0x%x\n", parent, parent->vm_tree);	/* */
  printk ("fork(): child = 0x%x, child->vm_tree = 0x%x\n", newproc, newproc->vm_tree);	/* */
#endif
  errno = proc_duplicate (parent, newproc);
  if (errno)
    {
      destroy_proc_memory (newproc, 0);
      return (errno);
    }

  newproc->proc_status = PS_RUN;
  newproc->proc_ppid = parent->proc_pid;
  lod_low(main_task, "lowlib.posix");
  errno = vget_reg (main_task, LOWLIB_DATA,
		    sizeof (struct lowlib_data), &lowlib_data);
  lowlib_data.main_task = main_task;
  /*  lowlib_data.signal_task = signal_task;*/
  lowlib_data.my_pid = *childid;
  errno = vput_reg (main_task, LOWLIB_DATA,
		    sizeof (struct lowlib_data), &lowlib_data);
  return (EP_OK);  
}



/* proc_duplicate - �ץ�������Υ��ԡ�
 *
 *	Lowlib �ˤĤ��Ƥξ���ϡ��ƤӽФ���Ȥ����ꤷ�Ƥ���Τ�ɬ�פʤ���
 *	(�������μ¹Գ��ϥ��ɥ쥹�ϡ�Lowlib ��� fork �Ѵؿ������ꤵ��Ƥ���)
 *
 */
W
proc_duplicate (struct proc *source,
		struct proc *destination)
{
  W	errno;
  W	index;


  /* �ץ����β��۶��֤Υ��ԡ�
   *
   *	���������۶��֤������������ԡ����β��۶��֤���ǡ����򥳥ԡ����롣
   */

#ifdef FKDEBUG
  printk ("fork: proc_duplicate: (%s file, %d line)\n", __FILE__, __LINE__);	/* */
#endif

  /* ���۶��֤����� */
  errno = setup_vm_proc (destination);
  if (errno)
    {
      /* ���۶��֤������˼��Ԥ���
       */
#ifdef FKDEBUG
      printk ("cannot create virtual memory space.\n");	/* */
#endif
      return (errno);
    }
#ifdef FKDEBUG
  printk ("fork: proc_duplicate: (parent = 0x%x, parent->vm_tree = 0x%x)\n", 
	  source, source->vm_tree);
#endif
  /* �ץ����Τ�Ĳ��۶��֤ξ���򥳥ԡ����� */
  errno = duplicate_memory (source, destination);
  if (errno)
    {
      /* �ǡ����Υ��ԡ��˼��Ԥ��� */
#ifdef FKDEBUG
      printk ("cannot duplicate memory\n");		/* */ 
#endif
      return (errno);
    }

#ifdef FKDEBUG
  printk ("fork: proc_duplicate: (%s file, %d line)\n", __FILE__, __LINE__);	/* */
#endif

  /* �����ץ�ե�����ξ���Υ��ԡ�
   */
  for (index = 0; index < MAX_OPEN; index++)
    {
      if (source->proc_open_file[index].f_inode != NULL) {
	destination->proc_open_file[index] = source->proc_open_file[index];
#ifdef notdef
	bcopy(&(source->proc_open_file[index]),
	      &(destination->proc_open_file[index]),
	      sizeof(struct file));
#endif
	destination->proc_open_file[index].f_inode->i_refcount++;
      }
    }

  /* copy of working directory */
  destination->proc_workdir = source->proc_workdir;
  destination->proc_workdir->i_refcount++;

  /* copy of uid/gid */
  destination->proc_uid = source->proc_uid;
  destination->proc_euid = source->proc_euid;
  destination->proc_gid = source->proc_gid;
  destination->proc_euid = source->proc_euid;

  /* copy of umask */
  destination->proc_umask = source->proc_umask;

  return (EP_OK);
}

