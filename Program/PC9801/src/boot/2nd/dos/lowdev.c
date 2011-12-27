/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include "../types.h"
#include "../file.h"
#include "../macros.h"
#include "dosfs.h"


/*
 * ���ꤷ�����饹���ֹ椫����ꤷ���Х��ȿ�ʬ�����ɤ߹��ࡣ
 * �⤷���ꤷ���Х��ȿ������饹���Υ�������ۤ��Ƥ������ˤϡ�
 * ���� FAT ����ȥ����Ф��ơ�����ɤ߿ʤࡣ
 *
 */
int
dos_read (struct dos_fsinfo *fs, int fatent, int size, unsigned char *buf)
{
  int		total_size = 0;;

#ifdef DEBUG
  printf ("dos_read: fatent = %d\n", fatent);
#endif /* DEBUG */

  /*
   * �ޤ����ǽ���ɤ߹��߰��֤ޤǰ�ư�������������ɤ߹��ࡣ
   */
  while (size > 0)
    {
#ifdef DEBUG
      printf ("dos_read: start = %d, size = %d, fatent = %d\n",
	      DOS_CLUSTER_TO_BLOCK (fs, fatent), size, fatent);
#endif /* DEBUG */

#ifdef __BOOT__
      if (DOS_BLOCK_TO_BYTE (fs, DOS_CLUSTER_TO_BLOCK (fs, fatent)) 
	  > (1024 * 1024))
	{
	  printf ("Block Number overflow. (%d)\n", 
		  DOS_BLOCK_TO_BYTE (fs, DOS_CLUSTER_TO_BLOCK (fs, fatent))); 
  	}	
      else
	{
	  read_device (fs->device, 
		       DOS_BLOCK_TO_BYTE (fs, DOS_CLUSTER_TO_BLOCK (fs, fatent)),
		       min (size, fs->clustersize),
		       buf);
	}
      total_size += min (size, fs->clustersize);
#else
      lseek (fs->device, DOS_BLOCK_TO_BYTE (fs, DOS_CLUSTER_TO_BLOCK (fs, fatent)), SEEK_SET);
      total_size += read (fs->device, buf, min (size, fs->clustersize));
#endif /* __BOOT__ */
      size -= (fs->clustersize);
      fatent = next_fatid (fs, fatent);
      buf += fs->clustersize;
    }
  return (total_size);
}



/*
 * �����ǻ��ꤷ�� FAT ����ȥ�Υꥹ�Ȥ�é�äơ�
 * �����ĤΥ���ȥ����Ѥ��Ƥ��뤫�򥫥���Ȥ��롣
 */
int
fat_list_count (struct dos_fsinfo *fs, int entry)
{
  int	count;

  /*
   * ���� FAT �ϻ��ѤǤ��ʤ���
   */
  if (next_fatid (fs, entry) == BAD_FATID)
    {
      return (0);
    }

  for (count = 1;
       (!IS_LAST_FATID (next_fatid (fs, entry))) && (next_fatid (fs, entry) != BAD_FATID);
       count++)
    {
      entry = next_fatid (fs, entry);
    }
#ifdef DEBUG
  printf ("fat_list_count: %d\n", count);
#endif /* DEBUG */
  return (count);
}


/*
 *
 */
int
dos_cluster_to_block (struct dos_fsinfo *fs, int fat)
{
  int	offset;

  offset =  DOS_ROOTDIROFFSET (fs) + DOS_ROOTDIRSIZE(fs);
  offset += ((fat - 2) * (fs->clustersize));
#ifdef DEBUG
  printf ("fat: %d, offset (byte): %d\n", fat, offset);
#endif /* DEBUG */
  return (offset / fs->blocksize);
}

/* ���ꤷ���ǥХ�������ǡ������ɤ߹���
 * ���ΤȤ��� fd �����б����Ƥ��ʤ���
 */
int
read_device (int device, int offset, int size, void *buf)
{
  char	*tmpbuf;
  int	 i;
  int    align_start, align_end, align_size;
  
  /* �֥�å������줿�������ͤη׻� */
  align_start = (offset == 0) ? 0 : offset / BLOCK_SIZE * BLOCK_SIZE;
  align_end   = ROUNDUP (offset + size, BLOCK_SIZE);
  align_size = align_end - align_start;

  /* �ƥ�ݥ��Хåե������� */
  tmpbuf = (char *)malloc (align_size * sizeof (char));
  if (tmpbuf == NULL)
    {
      return (0);
    }
/*  printf ("tmpbuf = 0x%x\n", tmpbuf);				/* */

  init_fd ();
  
  /* FD ������ɤ߹��� */
  for (i = 0; i < align_size; i += BLOCK_SIZE)
    {
/*      printf ("fd_read (%d, %d, 0x%x)\n",
	      device, (align_start + i) / BLOCK_SIZE, tmpbuf + i);	/* */
      fd_read (device, (align_start + i) / BLOCK_SIZE, tmpbuf + i);
    }

  /* �ɤ߹�����ǡ�����������Ϥ���Ƥ����ΰ�˥��ԡ����� */
/*  printf ("read_device: bcopy (0x%x, 0x%x, %d)\n",
	  tmpbuf + (offset - align_start), buf, size);		/* */
/*  printf ("read_device: offset = %d\n", (offset - align_start));	/* */
  bcopy (tmpbuf + (offset - align_start), buf, size);
/*  printf ("buf[0] = 0x%x\n", ((char *)buf)[0]);			/* */
/*  printf ("tmpbuf[512] = 0x%x\n", tmpbuf[512]);			/* */
  free (tmpbuf);
  return (size);
}
