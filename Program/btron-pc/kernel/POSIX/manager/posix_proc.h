/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* posix_proc.h - POSIX �ץ�������
 *
 *
 * $Log: posix_proc.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.16  2000-05-06 03:52:26  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.15  2000/01/15 15:29:29  naniwa
 * minor fix
 *
 * Revision 1.14  1999/07/23 14:39:01  naniwa
 * modified to implement exit
 *
 * Revision 1.13  1999/07/21 15:10:06  naniwa
 * modified to implement waitpid
 *
 * Revision 1.12  1999/03/24 04:52:09  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.11  1998/05/23 15:32:43  night
 * enum ����κǸ�����ǤˤĤ��Ƥ��� ',' ����������
 * (ANSI ��ʸˡ�Ǥϡ��Ǹ�����Ǥ� �ϡ�',' �ϤĤ����ʤ�)
 *
 * Revision 1.10  1998/01/06 16:37:11  night
 * proc ��¤�Τ˥ץ������ȤäƤ�������ΰ�ξ����ѤΥ���ȥ���ɲä�
 * ����
 *
 * �ƥ������ΰ�
 * UW			text_start_addr;
 * UW			text_length;
 *
 * �ǡ����ΰ�
 * UW			data_start_addr;
 * UW			data_length;
 *
 * BSS �ΰ�
 * UW			bss_start_addr;
 * UW			bss_length;
 *
 * Revision 1.9  1997/12/17 14:16:21  night
 * �ؿ� fork() �ΰ����ѹ���ȼ�����ؿ��ץ�ȥ����פ��ѹ���
 *
 * Revision 1.8  1997/10/24 13:59:26  night
 * enum proc_status ���ɲá�
 * �ؿ�������ɲá�
 *
 * Revision 1.7  1997/10/23 14:32:33  night
 * exec �����ƥॳ����ط��ν����ι���
 *
 * Revision 1.6  1997/05/08 15:11:30  night
 * �ץ����ξ�������ꤹ�뵡ǽ���ɲá�
 * (syscall misc �� proc_set_info ���ޥ��)
 *
 * Revision 1.5  1997/03/25 13:34:53  night
 * ELF �����μ¹ԥե�����ؤ��б�
 *
 * Revision 1.4  1996/11/18  13:43:47  night
 * �ե����빽¤�Τ˥ե�����μ���(�̾�ե����뤫�ѥ��פ�)�򼨤���
 * f_flag ���ɲá�
 *
 * Revision 1.3  1996/11/17  16:55:28  night
 * ʸ�������ɤ� EUC �ˤ�����
 *
 * Revision 1.2  1996/11/17  16:47:49  night
 * �ʲ������Ǥ��ɲ�
 *
 *    struct proc		*proc_next;
 *    W			proc_euid;
 *    W			proc_egid;
 *    W			proc_umask;
 *
 * Revision 1.1  1996/11/14  13:18:15  night
 * �ǽ����Ͽ
 *
 *
 */

#ifndef __POSIX_PROC_H__
#define __POSIX_PROC_H__	1


#define F_FILE		0x0001
#define F_PIPE		0x0002


struct file
{
  struct proc		*f_procp;
  struct inode		*f_inode;
  W			f_flag;
  W			f_offset;	/* current offset */
  W			f_omode;
};


enum proc_status
{
  PS_DORMANT,		/* ̤�������� */
  PS_SLEEP,		/* sleep ���� */
  PS_WAIT,		/* wait ���� */
  PS_RUN,		/* run ���� */
  PS_ZOMBIE		/* zombie ���� */
};

struct proc
{
  struct proc		*proc_next;

  enum proc_status	proc_status;		/* �ץ����ξ��֤򼨤� */

  ID			proc_maintask;		/* �ᥤ�󥿥��� */
  ID			proc_signal_handler;	/* �����ʥ�ϥ�ɥ饿���� */

  W			proc_uid;
  W			proc_gid;
  W			proc_euid;
  W			proc_egid;

  W			proc_umask;

  struct inode		*proc_workdir;
  struct file		proc_open_file[MAX_OPEN];

  UW			proc_pid;		/* my process ID 
						 * �����ͤ� 0 �ΤȤ��ϡ����Υ���ȥ�ϡ�
						 * �ȤäƤ��ʤ���
						 */
  
  UW			proc_ppid;		/* parent process ID */

  UW			proc_pgid;		/* process group ID */
  UW			proc_wpid;		/* pid parameter of waitpid */
  UW			proc_exst;		/* exit status */
  UW			proc_rvpt;		/* receive_port */
 

  struct vm_tree	*vm_tree;		/* ���۶��֤ξ��� 
						 * ���ۥڡ�����ʪ���ڡ���/����å׾�����������
						 */

  UW			text_start_addr;
  UW			text_length;

  UW			data_start_addr;
  UW			data_length;

  UW			bss_start_addr;
  UW			bss_length;
};



#ifdef KERNEL

/* process.c */

extern struct proc	proc_table[MAX_PROCESS];

extern W		init_process (void);
extern W		proc_set_info (struct posix_request *req);
extern W		proc_get_uid (W procid, W *uid);
extern W		proc_get_euid (W procid, W *uid);
extern W		proc_set_euid (W procid, W uid);
extern W		proc_get_gid (W procid, W *gid);
extern W		proc_get_egid (W procid, W *gid);
extern W		proc_get_pid (W procid, W *pid);
extern W		proc_get_ppid (W procid, W *ppid);
extern W		proc_set_egid (W procid, W gid);
extern W		proc_set_gid (W procid, W gid);
extern W		proc_alloc_fileid (W procid, W *retval);
extern W		proc_get_cwd (W procid, struct inode **cwd);
extern W		proc_set_cwd (W procid, struct inode *cwd);
extern W		proc_get_file (W procid, W fileid, struct file **fp);
extern W		proc_set_file (W procid, W fileid, W flag, struct inode *ip);
extern W		proc_rewind_file (W procid, W fileid);
extern W		proc_get_cwd (W procid, struct inode **cwd);
extern W		proc_set_umask (W procid, W umask);
extern W		proc_get_umask (W procid, W *umask);
extern W		proc_renew_task (W procid, FP main_funcp, FP signal_funcp, ID *new_main_task, ID *new_signal_task);
extern W		proc_dump (struct posix_request *req);
extern W		proc_get_procp (W procid, struct proc **procp);
extern W		proc_destroy_memory (W procid);
extern W		proc_exit (W procid);
extern W		proc_vm_dump (struct posix_request *req);
extern struct vm_tree	*proc_get_vmtree (W procid);
extern W		proc_alloc_proc (struct proc **procp);



/* exec.c */
extern W		exec_program (struct posix_request *req, W procid, B *pathname);


/* fork.c */
extern W		fork (struct proc *parent, W *childid, ID main_task, ID signal_task);
extern W		proc_duplicate (struct proc *source, struct proc *destination);

/* filesystem.c */
extern W		open_special_dev(struct proc *procp);

#endif

#endif /* __POSIX_PROC_H__ */
