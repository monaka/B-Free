/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* memory.c - POSIX �Ķ��ޥ͡�����Υ��������ʬ
 *
 *
 *
 * $Log: memory.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-07-23 14:38:58  naniwa
 * modified to implement exit
 *
 * Revision 1.3  1997/10/24 13:57:42  night
 * duplicate_memory() �� destroy_proc_memory () ���ɲá�
 *
 * Revision 1.2  1997/10/23 14:32:32  night
 * exec �����ƥॳ����ط��ν����ι���
 *
 * Revision 1.1  1996/11/05 15:13:46  night
 * �ǽ����Ͽ
 *
 */

#include "posix.h"



/* init_filesystem
 *
 */
ER
init_memory (void)
{
  return (E_OK);
}



/* setup_vm_proc - �ץ����β��ۥ������ν���� 
 *
 *
 */
W
setup_vm_proc (struct proc *procp)
{
  W	errno;

  errno = create_vm_tree (procp, (VM_READ | VM_WRITE | VM_USER | VM_SHARE), LOW_USER_ADDR, USER_MEM_SIZE);
  if (errno != EP_OK)
    {
      return (errno);
    }

  return (EP_OK);
}



/* alloc_memory - �ץ������Ф���Ǥ�դβ��ۥ���������Ƥ롣
 *
 * ������
 * 	procp	����������Ƥ�ץ���(�ץ���������¤�ΤؤΥݥ���)
 *	start	������Ƥ����Υ��ɥ쥹 (4KB alignment)
 *	size	���ꥵ���� (4KB alignment)
 *	access	����Υ��������⡼��
 *
 */
W
alloc_memory (W procid, UW start, UW size, UW access)
{
  W		i;
  W		errno;
  struct proc	*procp;


  errno = proc_get_procp (procid, &procp);
  for (i = 0; i < size / PAGE_SIZE; i++)
    {
      errno = grow_vm (procp, start + (i * PAGE_SIZE), access);
      if (errno)
	{
	  return (errno);
	}
    }

  return (EP_OK);
}


/* free_memory - ���ۥ�����������
 *
 */
W
free_memory (W procid, UW start, UW size)
{
  W	i;
  W	errno;
  struct proc	*procp;


  errno = proc_get_procp (procid, &procp);
  for (i = 0; i < size / PAGE_SIZE; i++)
    {
      errno = shorten_vm (procp, start + (i * PAGE_SIZE));
      if (errno)
	{
	  return (errno);
	}
    }

  return (EP_OK);
}


/* duplicate_memory - ���۶��֤Υǡ����򤹤٤ƥ��ԡ�����
 *
 *
 */
W
duplicate_memory (struct proc *source,
		  struct proc *destination)
{
  W	errno;

  errno = duplicate_tree (source, destination);
  return (errno);
}



/* destroy_proc_memory - �ץ����Τ�Ĥ��٤ƤΥ�����������
 *
 */
W
destroy_proc_memory (struct proc *procp, W unmap)
{
  W	errno;

  errno = destroy_vmtree (procp, procp->vm_tree, unmap);
  if (errno)
    {
      return (errno);
    }

  procp->vm_tree = NULL;
  return (EP_OK);
}



W
vm_page_fault_handler ()
{
  return (EP_NOSUP);
}
