/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

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
#define BOOT_BLOCK	2		/* boot block のサイズ */
#define SSYS		1
#define NBMP		128
#define SFIDT		1024
#define SFMNT		1024
#define FRAG_ENT	32
#define RECORD_ENT	40
#define ROOT_FILE_ADDR	8		/* root file のアドレス */
#define ROOT_FILE_ID	0

/**************************************************************************
 *
 *	ファイルシステムヘッダ;
 *
 *	この構造体で示されるデータは、各ファイルシステムの先頭ブロックに
 *	入れられる。各要素は、リトルインディアンとなっている。
 *
 *
 */
struct file_system_header
{
  unsigned short	os_disk_id;	/* ＯＳディスクＩＤ		*/
  unsigned short	disk_id;	/* ディスク形式ＩＤ		*/
  unsigned short	sysblock_size;	/* システムブロックのサイズ(=1)	*/
  unsigned short	file_id_table_size; /* file ID テーブルのサイズ(=1) */
  unsigned short	shortname_table_size;	/* ファイル短縮名テーブルサイズ	*/
  					/* (=1)				*/
  unsigned short	bitmap_size;	/*ビットマップテーブルサイズ(=1)*/ 
  unsigned char		reserved[12];	/* 使用しない			*/
  unsigned short	block_size;	/* 論理ブロックのバイト数	*/
  unsigned short	max_file;	/* 最大ファイル数		*/
  unsigned short	language;	/* 使用言語			*/
  unsigned short	access_level;	/* アクセスレベル		*/
  unsigned long	max_block;		/* 最大ブロック数		*/
  unsigned long	free_block;		/* 未使用論理ブロック数		*/
  unsigned long	last_modify_time;	/* 最新のシステムブロックの更新日付 */
  unsigned long	create_time;		/* 最新のシステムブロックの更新日付 */
  unsigned char	file_system_name[MAX_FSNAME];	/* ファイルシステム名	*/
  unsigned char	device_name[MAX_DEV_NAME];	/* デバイス名	*/
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

/* F_TIME --- ファイル日時構造体
 *
 */
typedef struct f_time
{
  long	f_ltime;		/* ファイルの保存期限（日時）	*/
  long	f_atime;		/* 最新のアクセス日時		*/
  long	f_mtime;		/* 最新の更新日時		*/
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
  unsigned long  f_start;	/* ファイルヘッダ開始 ID = 0x54726f6e	*/
  unsigned short f_type;	/* ファイルタイプ／所有者		*/
  unsigned short f_atype;	/* アプリケーションタイプ		*/
  unsigned short f_owner[14];	/* ファイル所有者（１２文字）＋０	*/
  unsigned short f_group[14];	/* 所有グループ名（１２文字）＋０	*/
  short		f_grpacc;	/* グループアクセスレベル		*/
  short		f_pubacc;	/* 一般アクセスレベル			*/
  short		f_nlink;	/* 含んでいるリンク数			*/
  short		f_index;	/* インデックスレベル			*/
  long		f_size;		/* ファイルの総バイト数			*/
  long		f_nblk;		/* 総使用論理ブロック数			*/
  long		f_nrec;		/* 総レコード数				*/
  char		reserved0[8];	/* 予約 8 バイト			*/
  long		f_ltime;	/* ファイルの保存期限（日時）		*/
  long		f_atime;	/* 最新のアクセス日時			*/
  long		f_mtime;	/* 最新の更新日時			*/
  long		f_ctime;	/* ファイルの作成日時			*/
  unsigned short fname[MAX_FNAME];
  unsigned short keyword[MAX_KEYWORD];
  char		reserved1[12];
  unsigned short	f_id;
  unsigned long		f_end;	/* ファイルヘッダ終了 ID = 0x82dde96b */
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
