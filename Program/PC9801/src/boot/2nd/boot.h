/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *	BOOT �ǻ��Ѥ��빽¤�Τ����
 *
 * $Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/boot/2nd/boot.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $
 *
 *
 * $Log: boot.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.7  1995-09-21 15:50:34  night
 * �������ե��������Ƭ�� Copyright notice ������ɲá�
 *
 * Revision 1.6  1995/06/04  03:35:04  night
 * machine_info ��¤�Τ��ɲá�
 *
 * Revision 1.5  1995/01/13  13:54:52  night
 * ���� RCS �ޥ����ޤޤ�Ƥ����Τǡ�����Ͽ���뤿�Ӥ���᤬���������ʤ�
 * �Ƥ����������ѹ�������
 *
 * Revision 1.4  1995/01/13  13:51:47  night
 * module_info ��¤�Τ� name ���Ǥ��ɲá�
 * module_type �� driver �� lowlib ���ɲá�
 *
 * Revision 1.3  1995/01/09  14:10:11  night
 * Header: /usr/local/src/master/btron/src/boot/2nd/boot.h,v 1.4 1995/01/13 13:51:47 night Exp $ �ȡ� Log: boot.h,v $
 * ���ɲá�
 *
 */

#ifndef __BOOT_H__
#define __BOOT_H__	1

#define MAX_MODULE_NAME	40

enum module_type 
{
  kernel	= 0,
  user		= 1,
  lowlib	= 2, 
  server      	= 3,
  driver	= 4
};


/* ʣ���Υ⥸�塼����ɤ߹��ि��Υإå�
 *
 *	�إå��ϰʲ������Ǥ���Ǥ��Ƥ���
 *
 *	1) cookie:	�ޥ��å��ֹ�(�إå�������)
 *	2) count:	�⥸�塼���
 *	3) module_info:	�ƥ⥸�塼��ξ���
 *	   length
 *	   vaddr
 *	   entry
 *	   mode
 */
struct module_info
{
  int		length;		/* �ǥ�������Ǥ��礭�� */
  int		mem_length;	/* ������Ǥ��礭��; 
				   �إå���ޤޤ���BSS �ΰ��ޤ� */
  unsigned int		vaddr;	/* ���ۥ��ɥ쥹	*/
  unsigned int		paddr;	/* �¥��ɥ쥹	*/
  unsigned int		entry;
  enum module_type	type;
  char		name[MAX_MODULE_NAME];
};

struct machine_info
{
  unsigned int	ext_mem;
  unsigned int	real_mem;
  unsigned int	base_mem;
};

struct boot_header
{
  unsigned char		cookie;		/* 0001 �˸��� */
  int			count;
  struct machine_info	machine;
  struct module_info	modules[0];
};


#endif /* __BOOT_H__ */
