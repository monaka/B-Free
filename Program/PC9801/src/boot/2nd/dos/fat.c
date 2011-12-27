/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

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
 * fat �ơ��֥�ν����
 */
void
init_fattable (struct dos_fsinfo *fs)
{
  unsigned short	*fat;
  unsigned char		*buf;

  fat = (unsigned short *)malloc (sizeof (unsigned short) * DOS_FATENTRY (fs));
  if (fat == NULL)
    {
      /* ������­���ä� */
      printf ("No more memory!\n");
      return;
    }

  buf = (unsigned char *)malloc (DOS_FATSIZE (fs));
  if (buf == NULL)
    {
      /* ������­���ä� */
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
 * �ǥ���������Ƭ�ˤ��� fattable ���ɤ߼��
 * �ɤ߼�ä� FAT ����ϡ�fattable ������롣
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


/* �����Ƥ��� FAT ����ȥ���������
 * �⤷�������Ƥ��� FAT ����ȥ꤬�ʤ��ä����ˤ� 0 ���֤���
 * DOS �Υե����륷���ƥ�ξ�硢FAT �Ϥ��Τޤޥ֥�å��������ͤƤ���Τǡ�
 * �����֥�å����֤����Ȥˤʤ롣
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

  fs->fattable[i] = LAST_FATID;	/* ���� FAT ����ȥ�ϻ�����Ǥ���Ȥ�������Ĥ��� */
  (void)sync_fat (fs);
  return (i);
}

/* FAT �Υ���ȥ��������롣
 * FAT �ϥꥹ�ȤȤʤäƤ���Τǡ����Τ��٤ƤΥ���ȥ��������롣
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
 * FAT �����ǥ������˽񤭹���
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
  write (fs->device, fatblock, DOS_FATSIZE (fs));	/* ͽ���� FAT �ơ��֥��񤭹��� */
  free (fatblock);
  return (SUCCESS);
}

#endif /* notdef */
