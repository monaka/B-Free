/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/server/memory.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: memory.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:51:58  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.1  1995/08/27  10:47:57  night
 * �ǽ����Ͽ
 *
 *
 */


#ifndef __SYS_MM_H__
#define __SYS_MM_H__	1

/*
 * �ݡ��ȥޥ͡��������Ͽ���� POSIX �Υ���ޥ͡������̾��
 */
#define MEMORY_MANAGER_NAME	"posix.memory_manager"


/*
 * ���۶��֤�°��
 */
#define	VM_READ		0x00000001	/* ���Ȳ�°�� */
#define VM_WRITE	0x00000002	/* ������°�� */
#define VM_EXEC		0x00000004	/* �¹Բ�°�� */
#define VM_GROW		0x00000008	/* ���۶��֤γ�ĥ°�� */
#define VM_PAGEOUT	0x00000010	/* �ڡ��������Ȳ�°�� */
#define VM_NOPAGEOUT	0x00000000	/* �ڡ����������Բ�°�� */
#define VM_USER		0x00010000	/* �桼���⡼�� */
#define VM_KERNEL	0x00020000	/* �����ͥ�⡼�� */


/*
 * ��������ޥ͡�����������׵� (command) �����
 *
 */
enum mm_command
{
  MM_NULL,	/* ���⤷�ʤ� */
  MM_CREATE,	/* ���۶��֤����� */
  MM_REMOVE,	/* ���۶��֤κ�� */
  MM_WRITE,	/* ���۶��֤˾����񤭹��� */
  MM_READ,	/* ���۶��֤����Ƥ��ɤߤ��� */
  MM_STAT,	/* ���۶��֤ˤĤ��Ƥξ����������� */
};


/* ���ꥵ���Ф˽������׵᤹��Ȥ��˻��Ѥ��������ѥ��åȤ�
 * �����ѥ��å�(�ֿ�)�������
 * �����Υѥ��åȤϡ��׵�����̡���������롣
 */

/*
 * ���۶��֤��������˻��Ѥ���ѥ��å�
 */
struct mm_create_arg
{
  ID		taskid;		/* ���۶��֤��������륿���� ID */
  void		*start;		/* ���۶��֤Υ������ȥ��ɥ쥹 */
  unsigned int	size;		/* ���۶��֤��礭�� */
  int		attr;		/* ���۶��֤�°�� */
};

/*
 * ����׵�
 */
struct mm_remove_arg
{
  ID		taskid;		/* ���۶��֤��������륿���� ID */
  void		*start;		/* ���۶��֤Υ������ȥ��ɥ쥹 */
  unsigned int	size;		/* ���۶��֤��礭�� */
};

/*
 * �񤭹����׵�
 */
struct mm_write_arg
{
  ID		srctaskid;	/* �񤭹��߸������� ID */
  ID		destaskid;	/* �񤭹����西���� ID */
  void		*srcstart;	/* �񤭹��߸��������ȥ��ɥ쥹 */
  void		*desstart;	/* �񤭹����襹�����ȥ��ɥ쥹 */
  unsigned int	size;		/* �ΰ�Υ����� */
};

/*
 * �ɤ߹����׵�
 */
struct mm_read_arg
{
  ID		srctaskid;	/* �ɤ߹��߸������� ID */
  ID		destaskid;	/* �ɤ߹����西���� ID */
  void		*srcstart;	/* �ɤ߹��߸��������ȥ��ɥ쥹 */
  void		*desstart;	/* �ɤ߹����襹�����ȥ��ɥ쥹 */
  unsigned int	size;		/* �ΰ�Υ����� */
};

/*
 * ��������׵� 
 */  
struct mm_stat_arg
{
  ID		taskid;		/* ���۶��֤��������륿���� ID */
  void		*start;		/* ���۶��֤Υ������ȥ��ɥ쥹 */
  unsigned int	size;		/* ���۶��֤��礭�� */
};

/*
 * ����
 */
struct mm_msg
{
  enum mm_command		prog;		/* �׵�μ���򼨤� */
  ID				recvport;	/* �ֿ��ѥݡ��� */
  union
    {
      struct mm_create_arg	mm_create_arg;	/* �����׵� */
      struct mm_remove_arg	mm_remove_arg;	/* ����׵� */
      struct mm_write_arg	mm_write_arg;	/* �񤭹����׵� */
      struct mm_read_arg	mm_read_arg;	/* �ɤ߹����׵� */
      struct mm_stat_arg	mm_stat_arg;	/* ��������׵� */
    } a;
};

/*
 * �쥹�ݥ�
 */
enum mm_error
{
  EMM_OK = 0,
  EMM_SYSTEM,		/* �����ƥ२�顼 */
  EMM_UNKNOWNMSG,	/* �Τ�ʤ��׵᤬�褿 */
  EMM_NOSUPPORT,	/* �����׵�ϥ��ݡ��Ȥ��Ƥ��ʤ� */
  EMM_GARBADGE,		/* ��å���������»���Ƥ����ǽ�������� */
};

/*
 * ���۶��������׵�Υ쥹�ݥ�
 */
struct mm_create_res
{
  ID		taskid;		/* ���۶��֤��������륿���� ID */
  void		*start;		/* ���۶��֤Υ������ȥ��ɥ쥹 */
  unsigned int	size;		/* ���۶��֤��礭�� */
};

/*
 * ���۶��ֺ���׵�Υ쥹�ݥ�
 */
struct mm_remove_res
{
  ID		taskid;		/* ���۶��֤��������륿���� ID */
  void		*start;		/* ���۶��֤Υ������ȥ��ɥ쥹 */
  unsigned int	size;		/* ���۶��֤��礭�� */
};

/*
 *
 */
struct mm_write_res
{
  ID		srctaskid;	/* �񤭹��߸������� ID */
  ID		destaskid;	/* �񤭹����西���� ID */
  void		*srcstart;	/* �񤭹��߸��������ȥ��ɥ쥹 */
  void		*desstart;	/* �񤭹����襹�����ȥ��ɥ쥹 */
  unsigned int	size;		/* �ΰ�Υ����� */
};

/*
 *
 */
struct mm_read_res
{
  ID		srctaskid;	/* �ɤ߹��߸������� ID */
  ID		destaskid;	/* �ɤ߹����西���� ID */
  void		*srcstart;	/* �ɤ߹��߸��������ȥ��ɥ쥹 */
  void		*desstart;	/* �ɤ߹����襹�����ȥ��ɥ쥹 */
  unsigned int	size;		/* �ΰ�Υ����� */
};

/*
 *
 */
struct mm_stat_res
{
  ID		taskid;		/* ���۶��֤�������Ƥ��륿���� ID */
  void		*start;		/* ���۶��֤Υ������ȥ��ɥ쥹 */
  unsigned int	size;		/* ���۶��֤��礭�� */
  int		attr;		/* ���۶��֤�°�� */
};


/*
 *
 */
struct mm_res
{
  enum mm_error	errorcode;	/* ���顼�ֹ� */
  unsigned int	syserror;	/* EMM_SYSTEM �ΤȤ����֤����顼 */

  union 
    {
      struct mm_create_res	mm_create_res;	/* �ե�������� */
      struct mm_remove_res	mm_remove_res;	/* ����׵� */
      struct mm_write_res	mm_write_res;	/* �񤭹����׵� */
      struct mm_read_res	mm_read_res;	/* �ɤ߹����׵� */
      struct mm_stat_res	mm_stat_res;	/* ��������׵� */
    } r;
};


#endif /* __SYS_MM_H__ */
