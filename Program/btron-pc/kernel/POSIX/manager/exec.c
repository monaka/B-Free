/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


/* $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/manager/exec.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $ */


/* exec.c --- exec �����ƥॳ�����Ϣ�Υե�����
 *
 *
 * $Log: exec.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.10  2000-05-06 03:52:22  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.9  2000/01/08 09:10:55  naniwa
 * minor fix
 *
 * Revision 1.8  1999/11/10 10:48:31  naniwa
 * to implement execve
 *
 * Revision 1.7  1999/03/24 03:54:41  monaka
 * printf() was renamed to printk().
 *
 * Revision 1.6  1998/07/01 13:57:27  night
 * �ƥ��ȤΤ���˰��Ū�� exec ����¹ԥե�������ɤ߹��߽����򥹥��åפ�
 * ��褦���ѹ���
 *
 * Revision 1.5  1998/06/11 15:29:57  night
 * alloca ����Ѥ��ʤ��褦�ѹ���
 *
 * Revision 1.4  1998/02/16 14:20:28  night
 * �����Ȥ��ѹ���
 *
 * Revision 1.3  1998/01/06 16:39:54  night
 * �ǥХå��Ѥ� print ʸ���ɲá�
 *
 * Revision 1.2  1997/10/24 13:55:41  night
 * �ե��������ʸ�������ɤ� SJIS ���� EUC ���ѹ���
 *
 * Revision 1.1  1997/10/23 14:32:32  night
 * exec �����ƥॳ����ط��ν����ι���
 *
 *
 *
 */


#include "posix.h"

static W		read_exec_header (struct inode *ip, struct ELFheader *elfp,
					  struct ELF_Pheader *text, struct ELF_Pheader *data);
static W		load_text (W procid, struct inode *ip, struct ELF_Pheader *text, ID task);
static W		load_data (W procid, struct inode *ip, struct ELF_Pheader *data, ID task);



/* exec_program - 
 *
 */
W
exec_program (struct posix_request *req, W procid, B *pathname)
{
  struct inode		*ip;
  W			errno;
  struct access_info	acc;
  struct ELFheader	elf_header;
  struct ELF_Pheader	text, data;
  ID			main_task, signal_task;
  struct proc		*procp;


#ifdef EXEC_DEBUG
  printk ("[PM] exec_program: path = \"%s\"\n", pathname);	/* */
#endif

  /* �ץ����ξ���μ����� */
  errno = proc_get_procp (procid, &procp);
  if (errno) {
    return(errno);
  }

  /* �оݤȤʤ�ץ����ե�����򥪡��ץ󤹤� */
  proc_get_euid (procid, &(acc.uid));
  proc_get_egid (procid, &(acc.gid));
  if (pathname[0] == '/')
    {
#ifdef EXEC_DEBUG
      printk ("[PM] exec_program: call fs_open_file ()\n");	/* */
#endif
      errno = fs_open_file (pathname, O_RDONLY, 0, &acc, rootfile, &ip);
    }
  else
    {
      struct inode	*startip;
      
      errno = proc_get_cwd (procid, &startip);
      if (errno)
	{
	  return (errno);
	}
      errno = fs_open_file (pathname, O_RDONLY, 0, &acc, startip, &ip);
    }
  if (errno)
    {
#ifdef EXEC_DEBUG
      printk ("[PM] Cannot open file. -> return from exec_program().\n");
#endif
      return (errno);
    }

  /* �¹Ե��ĤΥ����å� */
  errno = permit(ip, &acc, X_BIT);
  if (errno) {
#ifdef EXEC_DEBUG
      printk ("[PM] Permission denied. -> return from exec_program().\n");
#endif
    return(errno);
  }

  errno = read_exec_header (ip, &elf_header, &text, &data);
  if (errno)
    {
      fs_close_file (ip);
      return (errno);
    }

  /* �ƤӽФ����ץ����Τ��٤Ƥ�(����)������������
   */
  errno = proc_destroy_memory (procid);
  if (errno)
    {
      fs_close_file (ip);
      return (errno);
    }
  
#ifdef notdef
  /* ����������������������
   */
  /* ��¸�Υ������Υ��᡼�����ѹ����뤳�ȤȤ����������������ϻߤ�� */
  errno = proc_renew_task (procid, req->param.par_execve.start_main,
			   req->param.par_execve.start_signal,
			   &main_task, &signal_task);
  if (errno)
    {
      fs_close_file (ip);
      return (errno);
    }
#endif
  /* ���۶��֤����� */
  errno = setup_vm_proc (procp);

  /* �ƥ������ΰ�����������
   */
  errno = load_text (procid, ip, &text, req->caller);
  if (errno)
    {
      fs_close_file (ip);
      return (errno);
    }

  /* �ǡ����ΰ�����������
   */
  errno = load_data (procid, ip, &data, req->caller);
  if (errno)
    {
      fs_close_file (ip);
      return (errno);
    }

  /* �������� context.eip �� elf_header.e_entry �����ꤹ�� */
  vset_ctx(req->caller, elf_header.e_entry);

  fs_close_file (ip);
  
  return (EP_OK);
}



/* read_exec_header - �¹ԥե�����Υإå����ɤ߼��
 *
 *
 */
static W
read_exec_header (struct inode *ip,
		  struct ELFheader *elfp,
		  struct ELF_Pheader *text,
		  struct ELF_Pheader *data)
{
  W			errno;
  W			rlength;
#ifdef USE_ALLOCA
  struct ELF_Pheader	*ph_table;
#else
  struct ELF_Pheader	ph_table[10];
#endif
  W			ph_index;


  errno = fs_read_file (ip, 0, (B*)elfp, sizeof (struct ELFheader), &rlength);
  if (errno)
    {
      return (errno);
    }

  /* �ޥ��å��ʥ�ФΥ����å�
   */
  if ((elfp->e_ident[0] != 0x7f) ||
      (elfp->e_ident[1] != 'E') ||
      (elfp->e_ident[2] != 'L') ||
      (elfp->e_ident[3] != 'F'))
    {
      /* ELF �ե����ޥåȤΥե�����ǤϤʤ��ä�
       */
      return (EP_NOEXEC);
    }

  if (elfp->e_type != ET_EXEC)
    {
      /* �¹ԥե�����ǤϤʤ��ä�
       */
      return (EP_NOEXEC);
    }

  if (elfp->e_machine != EM_386)
    {
      /* ELF �ե�������б��ޥ�����ब��ä�
       */
      return (EP_NOEXEC);
    }

  if (elfp->e_version < 1)
    {
      /* ELF �ե�����ΥС����������
       */
      return (EP_NOEXEC);
    }

  if (sizeof (struct ELF_Pheader) != elfp->e_phentsize)
    {
      /* �ץ����إå��Υ�����������Ȱ�äƤ���
       */
      return (EP_NOEXEC);
    }
      
#ifdef USE_ALLOCA
  ph_table = alloca (elfp->e_phentsize * elfp->e_phnum);
  if (ph_table == NULL)
    {
      /* ���꤬�����Ǥ��ʤ��ä�
       */
      return (EP_NOMEM);
    }
#endif

  errno = fs_read_file (ip, elfp->e_phoff, (B*)ph_table, elfp->e_phentsize * elfp->e_phnum, &rlength);
  if (errno)
    {
      return (errno);
    }
  else if (rlength != elfp->e_phentsize * elfp->e_phnum)
    {
      return (EP_NOEXEC);
    }

  bzero ((VP)text, sizeof (struct ELF_Pheader));
  bzero ((VP)data, sizeof (struct ELF_Pheader));
  for (ph_index = 0; ph_index < elfp->e_phnum; ph_index++)
    {
      /* �ץ����إå��ơ��֥��硹�˸��Ƥ������ƥ��������Υ����פˤ�ä�
       * �ƥ����ȡ��ǡ�����BSS �γƾ���ν������Ԥ���
       */
      if (ph_table[ph_index].p_type == PT_LOAD)
	{
	  if (ph_table[ph_index].p_flags == (PF_R + PF_X))
	    {
	      /* TEXT */
	      if (text->p_type == 0)
		{
		  *text = ph_table[ph_index];
		}
	    }
	  /* else if (ph_table[ph_index].p_flags == (PF_R + PF_W + PF_X))*/
	  /* �ǡ������������Υե饰�� PF_X ��ޤޤʤ��褦�� */
	  else if (ph_table[ph_index].p_flags == (PF_R + PF_W))
	    {
	      /* DATA */
	      if (data->p_type == 0)
		{
		  *data = ph_table[ph_index];
		}
	    }
	}
    }
  
  return (EP_OK);
}



/* �ƥ������ΰ�������˥��ɤ��롣
 *
 *
 */
static W
load_text (W procid, struct inode *ip, struct ELF_Pheader *text, ID task)
{
  W		errno;
  W		rest_length;
  W		offset;
  W		read_size;
  W		vaddr;
  static B	buf[PAGE_SIZE];


  errno = alloc_memory (procid,
			CUTDOWN (text->p_vaddr, PAGE_SIZE),
			ROUNDUP (text->p_memsz + (text->p_vaddr - CUTDOWN (text->p_vaddr, PAGE_SIZE)), PAGE_SIZE),
			VM_READ | VM_EXEC);
  if (errno)
    {
#ifdef EXEC_DEBUG
      printk("ERROR: alloc memory\n");
#endif
      return (errno);
    }

  for (rest_length = text->p_filesz, offset = text->p_offset, vaddr = text->p_vaddr;
       rest_length > 0; 
       rest_length -= PAGE_SIZE, vaddr += PAGE_SIZE, offset += read_size)
    {
      errno = fs_read_file (ip, offset, buf, (PAGE_SIZE < rest_length) ? PAGE_SIZE : rest_length, &read_size);
      if (errno)
	{
#ifdef EXEC_DEBUG
	  printk("ERROR: fs_read_file\n");
#endif
	  return (EP_NOMEM);
	}

      errno = vput_reg (task, (B*)vaddr, read_size, buf);
      if (errno)
	{
#ifdef EXEC_DEBUG
	  printk("ERROR: vput_reg %d %d %x %d %x\n", errno,
		 task, vaddr, read_size, buf);
#endif
	  return (EP_NOMEM);
	}
    }
  
  return (EP_OK);
}



/* �ǡ����ΰ�������˥��ɤ��롣
 *
 *
 */
static W
load_data (W procid, struct inode *ip, struct ELF_Pheader *data, ID task)
{
  W		errno;
  W		rest_length;
  W		offset;
  W		read_size;
  W		vaddr;
  static B	buf[PAGE_SIZE];


  errno = alloc_memory (procid,
			CUTDOWN (data->p_vaddr, PAGE_SIZE),
			ROUNDUP (data->p_memsz + (data->p_vaddr - CUTDOWN (data->p_vaddr, PAGE_SIZE)), PAGE_SIZE),
			VM_READ | VM_WRITE);
  if (errno)
    {
#ifdef EXEC_DEBUG
      printk("ERROR: alloc memory\n");
#endif
      return (errno);
    }

  for (rest_length = data->p_filesz, offset = data->p_offset, vaddr = data->p_vaddr;
       rest_length > 0; 
       rest_length -= PAGE_SIZE, vaddr += PAGE_SIZE, offset += read_size)
    {
      errno = fs_read_file (ip, offset, buf, (PAGE_SIZE < rest_length) ? PAGE_SIZE : rest_length, &read_size);
      if (errno)
	{
#ifdef EXEC_DEBUG
	  printk("ERROR: fs_read_file\n");
#endif
	  return (EP_NOMEM);
	}

      errno = vput_reg (task, (B*)vaddr, read_size, buf);
      if (errno)
	{
#ifdef EXEC_DEBUG
	  printk("ERROR: vput_reg\n");
#endif
	  return (EP_NOMEM);
	}
    }

  return (EP_OK);
}


