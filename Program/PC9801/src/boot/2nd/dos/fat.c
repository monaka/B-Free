/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include "dosfs.h"



/*
 * fat テーブルの初期化
 */
void
init_fattable (struct dos_fsinfo *fs)
{
  unsigned short	*fat;
  unsigned char		*buf;

  fat = (unsigned short *)malloc (sizeof (unsigned short) * DOS_FATENTRY (fs));
  if (fat == NULL)
    {
      /* メモリ不足だった */
      printf ("No more memory!\n");
      return;
    }

  buf = (unsigned char *)malloc (DOS_FATSIZE (fs));
  if (buf == NULL)
    {
      /* メモリ不足だった */
      printf ("No more memory!\n");
      return;
    }

#ifdef __BOOT__
  printf ("boot: read_device (%d, %d, %d, 0x%x)\n",
	  fs->device, DOS_IPLSIZE(fs), DOS_FATSIZE (fs), buf);	/* */
  read_device (fs->device, DOS_IPLSIZE(fs), DOS_FATSIZE (fs), buf);
#else
  lseek (fs->device, DOS_IPLSIZE(fs), SEEK_SET);
  read (fs->device, buf, DOS_FATSIZE (fs));
#endif __BOOT__
  (void)make_fattable (buf, fat, DOS_FATENTRY (fs));
  fs->fattable = fat;
  (void)free (buf);
}

/* 
 * ディスクの先頭にある fattable の読み取り
 * 読み取った FAT 情報は、fattable に入れる。
 *
 */
void
make_fattable (unsigned char *fat, unsigned short *fattable, int entry)
{
  unsigned int *p;
  int	i, j;

  for (i = 0, j = 0; j < entry; i++, j += 2)
    {
      p = (unsigned int *)&(fat[i * 3]);
      fattable[j] =  (*p & 0x0fff);
      fattable[j + 1] =  ((*p >> 12) & 0x0fff);
#ifdef notdef
      if (fat[j] != 0)
	printf ("fat[%d]: %d\n", j,     fattable[j]);

      if (fat[j + 1] != 0)
	printf ("fat[%d]: %d\n", j + 1, fattable[j + 1]);
#endif /* notdef */
    }
}


int
next_fatid (struct dos_fsinfo *fs, int entry)
{
  if (entry != 0x0fff)
    return (fs->fattable[entry]);
  return (0);
}

int
print_fat (struct dos_fsinfo *fs)
{
  unsigned short	*fat;
  int			i;

  fat = fs->fattable;
  for (i = 0; i <= 5; i++)
    {
      printf ("fat[%d] = %d(0x%x)\n", i, fat[i], fat[i]);
    }
}


#ifdef notdef


/* 空いている FAT エントリを取得する
 * もし、空いている FAT エントリがなかった場合には 0 を返す。
 * DOS のファイルシステムの場合、FAT はそのままブロック管理を兼ねているので、
 * 空きブロックを返すことになる。
 */
int
alloc_fat (struct dos_fsinfo *fs)
{
  int	i;

  for (i = 0; (fs->fattable[i] != 0) && (DOS_FATENTRY (fs) > i); i++)
    ;

  if (DOS_FATENTRY (fs) > i)
    {
      return (0);	
    }

  fs->fattable[i] = LAST_FATID;	/* この FAT エントリは使用中であるという印をつける */
  (void)sync_fat (fs);
  return (i);
}

/* FAT のエントリを解放する。
 * FAT はリストとなっているので、そのすべてのエントリを解放する。
 */
void
free_fat (struct dos_fsinfo *fs, int startentry)
{
  int	next;
  int	current;

  current = startentry;
  next = next_fatid (fs, current);
  while (next != 0xfff)
    {
      fs->fattable[current] = FREE_FATID;
      current = next;
      next = next_fatid (fs, current);
    }
  sync_fat (fs);
}

/*
 * FAT 情報をディスクに書き込む
 */
int
sync_fat (struct dos_fsinfo *fs)
{
  unsigned char	*fatblock;
  int		i;

  fatblock = (unsigned char *)malloc (DOS_FATSIZE (fs));
  if (fatblock == NULL)
    {
      printf ("cannot memory allocate.\n");
      exit (0);
    }

  for (i = 0; i < DOS_FATENTRY (fs); i += 2)
    {
      (*(short *)fatblock) = (fs->fattable[i + 1] << 12) | (fs->fattable[i]);
      fatblock += 3;
    }

  lseek (fs->device, DOS_IPLSIZE (fs), SEEK_SET);
  write (fs->device, fatblock, DOS_FATSIZE (fs));
  write (fs->device, fatblock, DOS_FATSIZE (fs));	/* 予備の FAT テーブルも書き込む */
  free (fatblock);
  return (SUCCESS);
}

#endif /* notdef */
