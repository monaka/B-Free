/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


#include "../init.h"
#include "dosfs.h"



/*
 * fat �ơ��֥�ν����
 */
void
init_fattable (struct dosfs *fs)
{
  unsigned short	*fat;
  unsigned char		*buf;
  W			magicid;
  W			foo;
  W	type;

  magicid = fs->dosparam.magicid;
  
  type = (magicid == 0xf9) ? 12 : 16;
  printf ("type = %d\n", type);	/* */
  printf ("fs->dosparam.fatsize = %d\n", fs->dosparam.fatsize);		/* */
  printf ("fs->dosparam.blocksize = %d\n", fs->dosparam.fatcount);	/* */
  printf ("malloc (%d)\n", sizeof (unsigned short) * DOS_FATENTRY (fs, type));	/* */
  printf ("DOS_FATENTRY = %d\n", DOS_FATENTRY (fs, type));

  fat = (unsigned short *)malloc (sizeof (unsigned short) * DOS_FATENTRY (fs, type));
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

/*  printf ("dofs: read_device (%d, %d, %d, 0x%x)\n",
	  fs->port, DOS_IPLSIZE (fs), DOS_FATSIZE (fs), buf);	/* */
  read_device (fs->port,
	       DOS_IPLSIZE(fs), 
	       fs->dd, 
	       DOS_FATSIZE (fs), 
	       buf);
  if (magicid == 0xf9)
    {
      (void)make_fat12table (buf, fat, DOS_FATENTRY (fs, 12));
    }
  else
    {
      (void)make_fat16table (buf, fat, DOS_FATENTRY (fs, 16));
    }
  fs->dosparam.fattable = fat;
  (void)free (buf);
}

/* 
 * �ǥ���������Ƭ�ˤ��� fattable ���ɤ߼��
 * �ɤ߼�ä� FAT ����ϡ�fattable ������롣
 *
 */
void
make_fat16table (UB *fat, UH *fattable, W entry)
{
  UH	*p;
  int	i, j;

  printf ("fat 16\n");
  printf ("fat count is %d\n", entry);
  p = (UH *)fat;
#ifdef notdef
  for (i = 0, j = 0; j < entry; i++, j++)
    {
      fattable[j] =  (p[i] & 0xffff);
      if (fat[j] != 0)
	printf ("fat[%d]: %d, ", j, fattable[j]);
    }
  printf ("\n");
#endif
}

/* 
 * �ǥ���������Ƭ�ˤ��� fattable ���ɤ߼��
 * �ɤ߼�ä� FAT ����ϡ�fattable ������롣
 *
 */
void
make_fat12table (UB *fat, UH *fattable, W entry)
{
  unsigned int *p;
  int	i, j;

  printf ("fat 12\n");
  printf ("fat count is %d\n", entry);
#ifdef notdef
  for (i = 0, j = 0; j < entry; i++, j += 2)
    {
      p = (unsigned int *)&(fat[i * 3]);
      fattable[j] =  (*p & 0x0fff);
      fattable[j + 1] =  ((*p >> 12) & 0x0fff);
      if (fat[j] != 0)
	printf ("fat[%d]: %d, ", j,     fattable[j]);

      if (fat[j + 1] != 0)
	printf ("fat[%d]: %d, ", j + 1, fattable[j + 1]);
    }
#endif /* notdef */
}



int
next_fatid (struct dosfs *fs, int entry)
{
  if (entry != 0x0fff)
    return (fs->dosparam.fattable[entry]);
  return (0);
}

int
print_fat (struct dosfs *fs)
{
  unsigned short	*fat;
  int			i;

  fat = fs->dosparam.fattable;
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
  W	i;
  W	type;

  type = (fs->dosparam.magicid == 8) ? 16 : 12;
  for (i = 0; (fs->fattable[i] != 0) && (DOS_FATENTRY (fs, type) > i); i++)
    ;

  if (DOS_FATENTRY (fs, type) > i)
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
  W	type;

  type = (fs->dosparam.magicid == 8) ? 16 : 12;
  fatblock = (unsigned char *)malloc (DOS_FATSIZE (fs));
  if (fatblock == NULL)
    {
      printf ("cannot memory allocate.\n");
      exit (0);
    }

  for (i = 0; i < DOS_FATENTRY (fs, type); i += 2)
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
