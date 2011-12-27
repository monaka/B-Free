/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>

#define BLOCKSIZE	512

#define IPLSIZE		1
#define FATSIZE		3
#define FATCOUNT	2
#define ROOTDIRSIZE	7
#define ROOTDIRENTRY	112
#define DIRENTRY	ROOTDIRENTRY

struct dos_fsinfo
{
  unsigned char		magicid;
  unsigned short	iplsize;
  unsigned short	fatsize;
  unsigned short	rootdirsize;
  unsigned short	rootdirentry;
  unsigned short	clustersize;
  unsigned short	*fattable;
};

struct dos_direntry
{
  unsigned char	name[8];
  unsigned char	ext[3];
  unsigned char	attr;
  unsigned char	reserve[10];
  unsigned char	mtime[2];
  unsigned char	mdate[2];
  unsigned short fatent;
  unsigned char	fsize[4];
};

typedef struct dos_direntry	dos_directory[DIRENTRY];

unsigned short	fattable[FATSIZE * BLOCKSIZE * 2 / 3];

int		device;


/*
 * FAT バッファとFAT のエントリ番号からその内容を返す。
 */
int
fat_to_entry (unsigned char *fatid, int entry)
{
  unsigned int	start = (entry / 2) * 3;
  unsigned int	*result;

  result = (int *)&(fatid[start]);
  if ((entry % 2) == 0)		/* 偶数の場合 */
    {
      return (*result & 0xfff);
    }
  else				/* 奇数の場合 */
    {
      return (*result >> 12) & 0xfff;
    }
}	

/* 
 * ディスクの先頭にある fattable の読み取り
 * 読み取った FAT 情報は、fattable に入れる。
 *
 */
make_fattable (unsigned char *fat)
{
  unsigned int *p;
  int	i, j;

  for (i = 0, j = 0; i < 10; i++, j += 2)
    {
      p = (unsigned int *)&(fat[i * 3]);
      fattable[j] =  (*p & 0x0fff);
      fattable[j + 1] =  ((*p >> 12) & 0x0fff);
      printf ("fat = 0x%x\n", (*p) & 0xffffff);
      printf ("fattable[%d]: %d,0x%x\n", j, fattable[j], fattable[j]);
      printf ("fattable[%d]: %d,0x%x\n", j + 1, fattable[j + 1], fattable[j + 1]);
    }
}

/*
 * get fat data and analyzsis.
 */
void
main (int ac, char **av)
{
  int	i;
  struct dos_direntry	rootdir[ROOTDIRENTRY], *dir;
  unsigned char	fat[FATSIZE * BLOCKSIZE];

  device = open (av[1], O_RDONLY);
  if (device == -1)
    {
      perror ("open");
      exit (-1);
    }

#ifdef __BOOT__
  read_device (fs->device, IPLSIZE * BLOCKSIZE, sizeof (fat), fat);
#else
  lseek (device, IPLSIZE * BLOCKSIZE, SEEK_SET);
  read (device, fat, sizeof (fat));
#endif /* __BOOT__ */
  printf ("FAT ID: 0x%x\n", fat[0]);
  make_fattable (fat);

#ifdef __BOOT__
  read_device (fs->device, 
	       (IPLSIZE * BLOCKSIZE) + ((FATSIZE * BLOCKSIZE) * FATCOUNT), 
	       sizeof (dos_directory), 
	       fat);
#else
  lseek (device, (IPLSIZE * BLOCKSIZE) + ((FATSIZE * BLOCKSIZE) * FATCOUNT), SEEK_SET);
  read (device, rootdir, sizeof (dos_directory));
#endif /* __BOOT__ */
  for (i = 0; i < ROOTDIRENTRY; i++)
    {
      switch (rootdir[i].name[0])
	{
	default: /* used */
	  if ((fat_to_entry(fat, rootdir[i].fatent) == 0x0fff) || 
	      (rootdir[i].fatent == 0))
	    {
	      printf ("%d %8.8s, fatid = %d, next = END\n", i, rootdir[i].name, rootdir[i].fatent);
	    }
	  else
	    {
	      printf ("%d %8.8s, fatid = %d, next = %d, 0x%x\n", 
		      i, 
		      rootdir[i].name, 
		      rootdir[i].fatent, 
		      fat_to_entry(fat, rootdir[i].fatent),
		      fat_to_entry(fat, rootdir[i].fatent));
	    }
	  break;

	case 0xe5:
	  printf ("%3.3d: deleted\n", i);
	  break;

	case 0:	/* Unused */
	  break;
	}
    }
  close (device);
}

/* Local Varibales:
 * compile-command: "make getfat"
 * End:
 */
