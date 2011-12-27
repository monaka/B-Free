/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*********************************************************************
 *
 * file.h
 *
 */

#ifndef __FILE_H__
#define __FILE_H__	1

#include "errno.h"

#define BLOCK_SIZE	1024
#define HEADER_LOCATE	2
#define MAX_FSNAME	40
#define MAX_DEV_NAME	40
#define MAX_FNAME	20
#define MAX_KEYWORD	12
#define BOOT_BLOCK	2		/* boot block �Υ����� */
#define SSYS		1
#define NBMP		128
#define SFIDT		1024
#define SFMNT		1024
#define FRAG_ENT	32
#define RECORD_ENT	40
#define ROOT_FILE_ADDR	8		/* root file �Υ��ɥ쥹 */
#define ROOT_FILE_ID	0

/**************************************************************************
 *
 *	�ե����륷���ƥ�إå�;
 *
 *	���ι�¤�ΤǼ������ǡ����ϡ��ƥե����륷���ƥ����Ƭ�֥�å���
 *	������롣�����Ǥϡ���ȥ륤��ǥ�����ȤʤäƤ��롣
 *
 *
 */
struct file_system_header
{
  unsigned short	os_disk_id;	/* �ϣӥǥ������ɣ�		*/
  unsigned short	disk_id;	/* �ǥ����������ɣ�		*/
  unsigned short	sysblock_size;	/* �����ƥ�֥�å��Υ�����(=1)	*/
  unsigned short	file_id_table_size; /* file ID �ơ��֥�Υ�����(=1) */
  unsigned short	shortname_table_size;	/* �ե�����û��̾�ơ��֥륵����	*/
  					/* (=1)				*/
  unsigned short	bitmap_size;	/*�ӥåȥޥåץơ��֥륵����(=1)*/ 
  unsigned char		reserved[12];	/* ���Ѥ��ʤ�			*/
  unsigned short	block_size;	/* �����֥�å��ΥХ��ȿ�	*/
  unsigned short	max_file;	/* ����ե������		*/
  unsigned short	language;	/* ���Ѹ���			*/
  unsigned short	access_level;	/* ����������٥�		*/
  unsigned long	max_block;		/* ����֥�å���		*/
  unsigned long	free_block;		/* ̤���������֥�å���		*/
  unsigned long	last_modify_time;	/* �ǿ��Υ����ƥ�֥�å��ι������� */
  unsigned long	create_time;		/* �ǿ��Υ����ƥ�֥�å��ι������� */
  unsigned char	file_system_name[MAX_FSNAME];	/* �ե����륷���ƥ�̾	*/
  unsigned char	device_name[MAX_DEV_NAME];	/* �ǥХ���̾	*/
};

struct file_id
{
  unsigned int	baddr:24;
  unsigned int	count:8;
};

struct file_system_block 
{
  struct file_system_header	header;
  unsigned char			using_bitmap[NBMP];
  unsigned char			mal_bitmap[NBMP];
  unsigned char			reserve[640];
};

struct file_system
{  
  struct file_system_block	fs_block;
  struct file_id		id_table[SFIDT];
  unsigned int			fs_short[SFMNT];
};

/* F_TIME --- �ե�����������¤��
 *
 */
typedef struct f_time
{
  long	f_ltime;		/* �ե��������¸���¡�������	*/
  long	f_atime;		/* �ǿ��Υ�����������		*/
  long	f_mtime;		/* �ǿ��ι�������		*/
} F_TIME;

struct frag_entry
{
  unsigned short	size;
  unsigned short	offset0;
  unsigned short	offset1;
};

struct normal_record
{
  unsigned char		zero;
  unsigned char		type;
  unsigned short	sub_type;
  short			reserve;
  unsigned short	start_offset;
  unsigned long		size;
  unsigned char		count;
  unsigned int		baddr:24;
};

struct connect_record
{
  struct
    {
      unsigned int	count:8;
      unsigned int	baddr:24;
    } data[4];
};

struct link_record
{
  unsigned char		zero;	/* always 0 */
  unsigned char		mark;	/* always 0x80 */
  unsigned short	sub_type;
  unsigned short	file_id;
  unsigned short	assoc_data[5];
};

struct unused_record
{
  unsigned char		zero;
  unsigned char		zero2;
  unsigned char		unused[14];
};

struct indirect
{
  unsigned int		count;
  unsigned int		baddr;
};

union record_entry
{
  struct normal_record	normal_record;
  struct connect_record	connect_record;
  struct link_record	link_record;
  struct unused_record	unused_record;
  struct indirect	indirect_record[2];
};


/*************************************************************************
 * 
 *
 */
struct file
{
  unsigned long  f_start;	/* �ե�����إå����� ID = 0x54726f6e	*/
  unsigned short f_type;	/* �ե����륿���ס���ͭ��		*/
  unsigned short f_atype;	/* ���ץꥱ������󥿥���		*/
  unsigned short f_owner[14];	/* �ե������ͭ�ԡʣ���ʸ���ˡܣ�	*/
  unsigned short f_group[14];	/* ��ͭ���롼��̾�ʣ���ʸ���ˡܣ�	*/
  short		f_grpacc;	/* ���롼�ץ���������٥�		*/
  short		f_pubacc;	/* ���̥���������٥�			*/
  short		f_nlink;	/* �ޤ�Ǥ����󥯿�			*/
  short		f_index;	/* ����ǥå�����٥�			*/
  long		f_size;		/* �ե��������Х��ȿ�			*/
  long		f_nblk;		/* ����������֥�å���			*/
  long		f_nrec;		/* ��쥳���ɿ�				*/
  char		reserved0[8];	/* ͽ�� 8 �Х���			*/
  long		f_ltime;	/* �ե��������¸���¡�������		*/
  long		f_atime;	/* �ǿ��Υ�����������			*/
  long		f_mtime;	/* �ǿ��ι�������			*/
  long		f_ctime;	/* �ե�����κ�������			*/
  unsigned short fname[MAX_FNAME];
  unsigned short keyword[MAX_KEYWORD];
  char		reserved1[12];
  unsigned short	f_id;
  unsigned long		f_end;	/* �ե�����إå���λ ID = 0x82dde96b */
  struct frag_entry	frag_table[FRAG_ENT];
  union record_entry	record_table[RECORD_ENT];    
};


extern struct file_system *attach_filesystem (int, struct file_system *);
extern int	get_file_addr (struct file_system *fs, int id);
extern int	open_file (int drive, int id,
			   struct file_system *fs, struct file *fp);
extern int	read_file (int drive, struct file *fp,
			   int record, int offset, int size, char *buf);
extern void	print_record_type (int);
extern int	get_record (struct file *fp,
			    int recordno,
			    union record_entry *ent);

#define FILE_ID_SIZE(x)		(x->fs_block.header.file_id_table_size)
#define FILE_SNAME_SIZE(x)	(x->fs_block.header.shortname_table_size)


#endif __FILE_H__
