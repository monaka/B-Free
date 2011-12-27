/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/**************************************************************************
 *
 * file.c
 *
 */

#include "types.h"
#include "config.h"
#include "macros.h"
#include "file.h"

static	char	buf[BLOCK_SIZE];
static int	get_record2 (union record_entry *table,
			     int ent_size,
			     int level,
			     union record_entry *buf,
			     int no);
static int	get_record3 (union record_entry *table,
			     int ent_size,
			     union record_entry *buf,
			     int no);



/************************************************************************
 *
 */
struct file_system *
attach_filesystem (int drive, struct file_system *fs)
{
  int	i;
  char	*p;
  
  init_fd ();
  fd_reset ();
  fd_read (drive, HEADER_LOCATE, &(fs->fs_block));
  if (fs->fs_block.header.os_disk_id != 0x42fe)
    {
      return (0);
    }
  p = (char *)fs->id_table;
  printf ("file id table_size = %d\n", fs->fs_block.header.file_id_table_size);
  for (i = 0;
       i < fs->fs_block.header.file_id_table_size;
       i++)
    {
/*
      printf ("ファイルテーブルの読み取り中 (%d)\n", i);
 */
      fd_read (drive,
	       i + SSYS + BOOT_BLOCK,
	       &(p[i * BLOCK_SIZE]));
    }
  fd_read (drive, ROOT_FILE_ADDR + 1, &root);
  return (fs);
}

/****************************************************************************
 *
 */
int
get_file_addr (struct file_system *fs,
	       int	id)
{
  return (fs->id_table[id].baddr);
}

/***************************************************************************
 *
 */
int
open_file (int drive, int id, struct file_system *fs, struct file *fp)
{
  int	baddr;
  int	result;

  baddr = get_file_addr (fs, id);
  result = fd_read (drive, baddr + 1, fp);
  if (result == E_OK)
    return (result);

/* あとしまつ */

  return (result);
}

/*****************************************************************************
 * read_file --- ファイルの指定されたレコードから指定されたオフセット、
 *		 サイズを読み込む
 *
 * 引数
 *	drive	--- ドライブ番号
 *	fp	--- ファイル構造体
 *	record	--- レコード番号
 *	offset	--- オフセット値
 *	size	--- 読み込みサイズ
 *
 * 返値
 *	0 <	読み込んだデータのバイト数
 *	0 =	データの終了
 *	0 >	エラー番号
 */
int
read_file (int drive,
	   struct file *fp,
	   int record,
	   int offset,
	   int size,
	   char *buf)
{
  int	i,j;
  static char	tmp[BLOCK_SIZE];
  int	read_size;
  
  bzero (tmp, sizeof (tmp));
  for (i = j = 0; (i < RECORD_ENT) && (j < record); i++)
    {
      if ((fp->record_table[i].normal_record.zero == 0)
	  && (fp->record_table[i].normal_record.type != 0))
	j++;
    }
  printf ("record index = %d\n", i);
  printf ("start_offset = %d\n",
	  fp->record_table[i].normal_record.start_offset);
  printf ("size = %d\n", 
	  fp->record_table[i].normal_record.size);
  printf ("count = %d\n", 
	  fp->record_table[i].normal_record.count);
  printf ("block address = %d\n", 
	  fp->record_table[i].normal_record.baddr);
  fd_read (drive,
	   fp->record_table[i].normal_record.baddr + 1,
	   tmp);
  bcopy (tmp + fp->record_table[i].normal_record.start_offset,
	 buf,
	 BLOCK_SIZE - (fp->record_table[i].normal_record.start_offset
		       + size % 1024));
  read_size = fp->record_table[i].normal_record.size - offset;
  return (read_size);  
}

/*************************************************************************
 *
 */
int
get_record (struct file *fp, int recordno, union record_entry *ent)
{
  int	i,j;
  
  if (fp->f_index == 0)
    {
      for (i = j = 0; j < recordno; i++)
	{
	  if ((fp->record_table[i].normal_record.type != 0) &&
	      (fp->record_table[i].normal_record.zero == 0))
	    j++;
	  if (i > 40)
	    return (NULL);
	}
      bcopy (&(fp->record_table[recordno]), ent, sizeof (union record_entry));
      return ((int)ent);
    }

  return (get_record2 (fp->record_table,
		       TABLE_SIZE (fp->record_table),
		       fp->f_index,
		       ent,
		       recordno));
}

/****************************************************************************
 *
 */
static int
get_record2 (union record_entry *table,
	     int ent_size,
	     int level,
	     union record_entry *buf,
	     int no)
{
  int	i, j;
  union record_entry	buffer[64];
  int	total = 0;

#ifdef RECORD_DEBUG
  printf ("get_record2: level %d; recordno = %d\n", level, no);
#endif
  for (i = 0; i < ent_size; i++)
    {
      for (j = 0; j < 2; j++)
	{
	  if ((total + table[i].indirect_record[j].count) >= no)
	    {
	      level--;
	      if (level <= 0)
		{
#ifdef RECORD_DEBUG
		  printf ("get_record2: blockno = %d\n", 
			   table[i].indirect_record[j].baddr + 1);
#endif		  
		  fd_read (0,
			   table[i].indirect_record[j].baddr + 1,
			   buffer);
		  get_record3 (buffer, 64, buf, no - total);
		  return ((int)buf);
		}
	      else
		{
		  fd_read (0,
			   table[i].indirect_record[j].baddr + 1,
			   buffer);
		  get_record2 (buffer, 128, level, buf, no - total);
		  return ((int)buf);
		}
	    }
	  total += table[i].indirect_record[j].baddr;
	}
    }
}

static int
get_record3 (union record_entry *table,
	     int ent_size,
	     union record_entry *buf,
	     int no)
{
  int	i, j;

#ifdef RECORD_DEBUG
  printf ("get_record3: no = %d, entsize =  %d\n", no, ent_size);
#endif
  for (i = j = 0; i < ent_size; i++)
    {
      if ((table[i].normal_record.zero == 0)
	  && (table[i].normal_record.type != 0))
	{
#ifdef RECORD_DEBUG
	  printf ("get_record3: i = %d;j = %d; type = %d\n",
		  i, j, table[i].normal_record.type);
#endif
	  if (j >= no)
	    {
	      bcopy (&table[i].normal_record, buf, sizeof(union record_entry));
	      return ((int)buf);
	    }
	  j++;
	}
    }
#ifdef RECORD_DEBUG
  printf ("not found record.\n");
#endif
  return (NULL);
}
  


