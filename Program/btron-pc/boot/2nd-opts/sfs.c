/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include "types.h"
#include "config.h"
#include "file.h"
#include "location.h"
#include "errno.h"
#include "page.h"
#include "lib.h"
#include "console.h"
#include "sfs.h"


#define MAXBUFFER	100
#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
#define MIN(x,y)		((x > y) ? y : x)


struct buffer
{
  int	blockno;
  char	data[512];
};


struct ic_sfs_superblock	fs_table[1];
struct sfs_inode	rootdir_buf;
struct sfs_inode	*rootdirp;

int sfs_read_dev (int dev, int start, int size, char *buf);
int sfs_read_inode (int fd, struct sfs_superblock *sb, int ino, struct sfs_inode *ip);
int sfs_lookup_file (int fd,
		 struct sfs_superblock *sb, 
		 struct sfs_inode *cwd,
		 char	*path,
		 struct sfs_inode *ip);
int sfs_read_dir (int fd,
	      struct sfs_superblock *sb,
	      struct sfs_inode *ip,
	      int nentry,
	      struct sfs_dir *dirp);
int cat_buf (char *buf, int length);
int sfs_blseek (int dev, int offset, int mode);
int sfs_bread (int dev, char *buf, int length);
int sfs_read_block (int fd, int blockno, int blocksize, char *buf);
int get_block_num (int fd,
	       struct sfs_superblock *sb,
	       struct sfs_inode *ip,
	       int blockno);
int sfs_locallookup_file (int fd,
		      struct sfs_superblock *sb, 
		      struct sfs_inode *parent,
		      struct sfs_inode *ip,
		      char *name);
int get_indirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);
int get_dindirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);


int
sfs_init_buffer (struct ic_sfs_superblock *sb)
{
#ifdef nodef
  int	i;

  for (i = 0; i < MAXBUFFER; i++)
    {
      sb->buffer[i].blockno = 0;
    }
#endif

  return E_OK;
}


int
f_sfs_mountroot (int dev)
{
  boot_printf ("f_sfs_mountroot: device is 0x%x\n", dev);

  fs_table[0].devid = dev;

  sfs_init_buffer (&fs_table[0]);
  sfs_read_dev (dev, SFS_BLOCK_SIZE, sizeof (struct sfs_superblock), &(fs_table[0].sb));

  boot_printf ("readed super block.\n");
  boot_printf ("magic: 0x%x, version %d.%d\n",
	  fs_table[0].sb.sfs_magic, 
	  fs_table[0].sb.sfs_version_hi, 
	  fs_table[0].sb.sfs_version_lo);
  boot_printf ("blocksize: %d\n", fs_table[0].sb.sfs_blocksize);
  boot_printf ("nblock: %d\n", fs_table[0].sb.sfs_nblock);
  boot_printf ("freeblock: %d\n", fs_table[0].sb.sfs_freeblock);

  if (fs_table[0].sb.sfs_magic != SFS_MAGIC)
    {
      boot_printf ("Invalid file system.\n");
      return (-1);
    }
  rootdirp = &rootdir_buf;

  sfs_read_inode (dev, &(fs_table[0].sb), 1, rootdirp);

  return E_OK;
}


int
f_sfs_mount (char **av)
{
  ULONG devid = 0;

  if (av[2] != NULL)
    {
      if (strcmp (av[1], "hd") == 0)
	{
	  devid = 0x010000;
	}
      else if (strcmp (av[1], "fd") == 0)
	{
	  devid = 0x000000;
	}
      else
	{
	  boot_printf ("unknown device. Valid device is \"fd\" or \"hd\"\n");
	  return E_SYS;
	}

      devid += atoi (av[2]);
      boot_printf ("devid = 0x%x (device = %d, partition = %d)\n", devid, (devid >> 8) & 0xff, devid & 0xff);
    }
  else
    {
      devid = atoi (av[1]);
    }

  if (f_sfs_mountroot (devid) == -1)
    {
      boot_printf ("cannot mount\n");
      return E_SYS;
    }

  return E_OK;
}



int
f_sfs_dir (char **av)
{
  struct sfs_inode	ip;
  int			errno;
  int			nentry;
#ifdef USE_ALLOCA
  struct sfs_dir	*dirp;
#else
  static struct sfs_dir	dirp[sizeof (struct sfs_dir) * 256];
#endif

#ifdef USE_ALLOCA
  char			*name;
#else
  static char		name[SFS_MAXNAMELEN + 2];
#endif
  int			i;
  int			fd;
  struct sfs_superblock	*sb;
  char			*path;

  fd = fs_table[0].devid;
  sb = &(fs_table[0].sb);
  path = av[1];

  errno = sfs_lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      boot_printf ("cannot open file.\n");
      return (errno);
    }

  if ((ip.sfs_i_perm & SFS_FMT_DIR) == 0)
    {
      boot_printf ("%d  0x%x\t%d\t%d\t%s\t%d bytes\n", ip.sfs_i_nlink, ip.sfs_i_perm, ip.sfs_i_uid, ip.sfs_i_gid, path, ip.sfs_i_size);
      return (0);
    }

  nentry = sfs_read_dir (fd, sb, &ip, 0, NULL);
#ifdef USE_ALLOCA
  dirp = alloca (nentry * sizeof (struct sfs_dir));
#endif
  if (sfs_read_dir (fd, sb, &ip, nentry, dirp) != 0)
    {
      boot_printf ("cannot read directory\n");
      return (-1);
    }
#ifdef USE_ALLOCA
  name = alloca (strlen (path) + SFS_MAXNAMELEN + 2);
#endif
  for (i = 0; i < nentry; i++)
    {
      struct sfs_inode	ip;

      strcpy (name, path);
      strcat (name, "/");
      strcat (name, dirp[i].sfs_d_name);
      errno = sfs_lookup_file (fd, sb, rootdirp, name, &ip);
      if (errno)
	{
	  boot_printf ("errno = %d\n");
	  return errno;
	}
      boot_printf ("%d  0x%x\t%d\t%d\t%s\t%d bytes\n", ip.sfs_i_nlink, ip.sfs_i_perm, ip.sfs_i_uid, ip.sfs_i_gid, dirp[i].sfs_d_name, ip.sfs_i_size);
    }
  return E_OK;
}


int
f_sfs_cat (char **av)
{
  char	*path;
  struct sfs_inode	ip;
  int			fd;
  struct sfs_superblock	*sb;
  int			errno;
#ifdef USE_ALLOCA
  char			*buf;
#else
  static   char		buf[BLOCK_SIZE];
#endif
  int			rsize;
  int			start;

  fd = fs_table[0].devid;
  sb = &(fs_table[0].sb);
  path = av[1];

  errno = sfs_lookup_file (fd, path, rootdirp, path, &ip);
  if (errno)
    {
      boot_printf ("Cannot open file.\n");
      return (errno);
    }
  rsize = ip.sfs_i_size;
#ifdef USE_ALLOCA
  buf = alloca (sb->sfs_blocksize);
#endif
  if (buf == 0)
    {
      boot_printf ("Cannot alloc memory.\n");
      return (ENOMEM);
    }

  while (rsize > 0)
    {
      errno = sfs_read_file (fd, sb, &ip, start, MIN (sb->sfs_blocksize, rsize), buf);
      if (errno)
	{
	  boot_printf ("Cannot read file.\n");
	  return (errno);
	}

      cat_buf (buf, sb->sfs_blocksize);
      start += sb->sfs_blocksize;
    }

  return E_OK;
}


int
cat_buf (char *buf, int length)
{
  int	i;

  for (i = 0; i < length; i++)
    {
      putchar ((int)buf[i]);
    }
  return (i);
}


/* ディレクトリに関係する処理
 *
 * sfs_read_rootdir()
 * sfs_read_dir()
 *
 */
int
sfs_read_dir (int fd,
	      struct sfs_superblock *sb,
	      struct sfs_inode *ip,
	      int nentry,
	      struct sfs_dir *dirp)
{
  int	size;

  if ((nentry <= 0) || (dirp == NULL))
    {
      return (ip->sfs_i_size / sizeof (struct sfs_dir));
    }
  
  size = (nentry * sizeof (struct sfs_dir) <= ip->sfs_i_size) ? 
          nentry * sizeof (struct sfs_dir) :
	  ip->sfs_i_size;

  sfs_read_file (fd, sb, ip, 0, size, dirp);	/* エラーチェックが必要! */
  return (0);
}


/* inode に関係している処理
 *
 * sfs_get_inode_offset()
 * sfs_read_inode()
 */
int
sfs_get_inode_offset (struct sfs_superblock *sb, int ino)
{
  int	offset;
  int	nblock;
  int	blocksize;

  nblock = sb->sfs_nblock;
  blocksize = sb->sfs_blocksize;
  offset = 1 + 1 + (ROUNDUP (nblock / 8, blocksize) / blocksize);
  offset *= blocksize;
  return (offset + ((ino - 1) * sizeof (struct sfs_inode)));
}


int
sfs_read_inode (int fd, struct sfs_superblock *sb, int ino, struct sfs_inode *ip)
{
  int	offset;

  offset = sfs_get_inode_offset (sb, ino);
  sfs_blseek (fd, offset, 0);
  sfs_bread (fd, ip, sizeof (struct sfs_inode));

  return E_OK;
}




/* ファイルに関係している処理
 *
 * read_file()
 * create_file()
 * lookup()
 */
int
sfs_read_file (int fd,
	   struct sfs_superblock *sb,
	   struct sfs_inode *ip,
	   int start,
	   int size,
	   char *buf)
{
#ifdef USE_ALLOCA
  char	*blockbuf;
#else
  static char	  blockbuf[BLOCK_SIZE];
#endif
  int	copysize;
  int	offset;
  int	retsize;
  char	*bufp;

  if (start + size > ip->sfs_i_size)
    {
      size = ip->sfs_i_size - start;
    }

  retsize = size;

  bufp = buf;

#ifdef USE_ALLOCA
  blockbuf = (char *)alloca (sb->sfs_blocksize);
#endif
  while (size > 0)
    {
      sfs_read_block (fd, 
		      get_block_num (fd, sb, ip, start / sb->sfs_blocksize),
		      sb->sfs_blocksize,
		      blockbuf);
      offset = start % sb->sfs_blocksize;
      copysize = MIN (sb->sfs_blocksize - offset, size);
      bcopy (&blockbuf[offset], buf, copysize);

      buf += copysize;
      start += copysize;
      size -= copysize;
    }
  return E_OK;
}


int
sfs_lookup_file (int fd,
		 struct sfs_superblock *sb, 
		 struct sfs_inode *cwd,
		 char	*path,
		 struct sfs_inode *ip)
{
  char name[SFS_MAXNAMELEN + 1];
  struct sfs_inode	*dirp;
  struct sfs_inode	*pdirp;
  struct sfs_inode	dirbuf;
  int	i;
  int	errno;

  if (strcmp (path, "/") == 0)
    {
      bcopy (cwd, ip, sizeof (struct sfs_inode));
      return E_OK;
    }

  if (*path == '/')
    {
      path++;
    }

  pdirp = cwd;
  dirp = &dirbuf;

  while (*path)
    {
      if (*path == '/')
	{
	  path++;
	}

      for (i = 0; ; i++)
	{
	  if (i > SFS_MAXNAMELEN)
	    {
	      return (ENAMETOOLONG);
	    }
	  if ((*path == '/') || (*path == '\0'))
	    {
	      break;
	    }
	  name[i] = *path++;
	}
      if (i == 0)
	break;

      name[i] = '\0';

/*      boot_printf ("local lookup = %s\n", name); */
      errno = sfs_locallookup_file (fd, sb, pdirp, dirp, name);
      if (errno)
	{
	  return (errno);
	}

      pdirp = dirp;
      dirp = pdirp;
    }

  bcopy (pdirp, ip, sizeof (struct sfs_inode));

  return E_OK;
}


int
sfs_locallookup_file (int fd,
		      struct sfs_superblock *sb, 
		      struct sfs_inode *parent,
		      struct sfs_inode *ip,
		      char *name)
{
  int	nentry;
#ifdef USE_ALLOCA
  struct sfs_dir *dirp;
#else
  static struct sfs_dir	dirp[sizeof (struct sfs_dir) * 256];
#endif
  int	i;
  int	errno;

  nentry = sfs_read_dir (fd, sb, parent, 0, NULL);
#ifdef USE_ALLOCA
  dirp = alloca (sizeof (struct sfs_dir) * nentry);
#endif
  sfs_read_dir (fd, sb, parent, nentry, dirp);
  for (i = 0; i < nentry; i++)
    {
      if (strcmp (name, dirp[i].sfs_d_name) == 0)
	{
	  errno = sfs_read_inode (fd, sb, dirp[i].sfs_d_index, ip);
	  if (errno)
	    {
	      return (errno);
	    }
	  return E_OK;
	}
    }
  return (ENOENT);      
}


/* ブロックに関係している処理
 *
 * sfs_read_block()
 * sfs_alloc_block()
 */
int
sfs_read_block (int fd, int blockno, int blocksize, char *buf)
{
  if (sfs_blseek (fd, blockno * blocksize, 0) < 0)
    {
      return 0;
    }

  if (sfs_bread (fd, buf, blocksize) < blocksize)
    {
      return 0;
    }

  return (blocksize);
}

/* =============================================================== */

int
sfs_bread (int dev, char *buf, int length)
{
  sfs_read_dev (dev, fs_table[0].position, length, buf);
  fs_table[0].position += length;

  return E_OK;
}


int
sfs_blseek (int dev, int offset, int mode)
{
  fs_table[0].position = offset;

  return E_OK;
}



int
get_block_num (int fd,
	       struct sfs_superblock *sb,
	       struct sfs_inode *ip,
	       int blockno)
{
  if (blockno < SFS_DIRECT_BLOCK_ENTRY)
    {
      /* 直接ブロックの範囲内
       */
      return (ip->sfs_i_direct[blockno]);
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
  		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)))
    {
      int result;
      /* 一重間接ブロックの範囲内
       */
      result = get_indirect_block_num (fd, sb, ip, blockno);
      return result;
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
  		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)
		      + (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK * SFS_INDIRECT_BLOCK)))
    {
      int result;
      /* 二重間接ブロックの範囲内
       */
      result =  get_dindirect_block_num (fd, sb, ip, blockno);
      return result;
    }

  return (-1);
}


int
get_indirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno)
{
  int	inblock;
  int	inblock_offset;
  struct sfs_indirect	inbuf;

  inblock = (blockno - SFS_DIRECT_BLOCK_ENTRY);
  inblock_offset = inblock % SFS_INDIRECT_BLOCK;
  inblock = inblock / SFS_INDIRECT_BLOCK;
  if (ip->sfs_i_indirect[inblock] <= 0)
    {
      return (0);
    }

  sfs_read_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize, (char *)&inbuf);
#ifdef nodef
  boot_fprintf (stderr, "get_ind: inblock = %d, offset = %d, blocknum = %d\n",
	  inblock, inblock_offset, inbuf.sfs_in_block[inblock_offset]);
#endif
  return (inbuf.sfs_in_block[inblock_offset]);
}

int
get_dindirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno)
{
  int	dinblock;
  int	dinblock_offset;
  int	inblock;
  struct sfs_indirect	inbuf;

  blockno = blockno - (SFS_DIRECT_BLOCK_ENTRY + SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);

  inblock = blockno / (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);
  dinblock = (blockno % (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)) / SFS_INDIRECT_BLOCK;
  dinblock_offset = blockno % SFS_INDIRECT_BLOCK;

#ifdef nodef
  boot_fprintf (stderr, "GET: blockno = %d, inblock = %d, dinblock = %d, dinblock_offset = %d\n",
	   blockno, inblock, dinblock, dinblock_offset);
#endif
  if (ip->sfs_i_dindirect[inblock] <= 0)
    {
      return (0);
    }

  sfs_read_block (fd, ip->sfs_i_dindirect[inblock], sb->sfs_blocksize, (char *)&inbuf);
  if (inbuf.sfs_in_block[dinblock] <= 0)
    {
      return (0);
    }

  sfs_read_block (fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize, (char *)&inbuf);

#ifdef nodef
  boot_fprintf (stderr, "get_ind: inblock = %d, dinblock = %d, offset = %d, blocknum = %d\n",
	  inblock, dinblock, dinblock_offset, inbuf.sfs_in_block[dinblock_offset]);
#endif
  return (inbuf.sfs_in_block[dinblock_offset]);
}




/*
 *
 */
int
sfs_read_dev (int dev, int start, int size, char *buf)
{
  int	device;
  int	drive;
  int	part;
#ifdef USE_ALLOCA
  char	*blockbuf;
#else
  static char	blockbuf[BLOCK_SIZE];
#endif
  int	copysize;
  int	offset;
  int	retsize;
  char	*bufp;
  int	blockno;
  int	(*read_func)();
  extern int	fd_read_with_partition ();
  extern int	ide_read_block_1k ();

  device = (dev >> 16) & 0xff;
  drive = (dev >> 8) & 0xff;
  part = dev & 0xff;
  if ((device & 0xff) == 0)
    {
      /* floppy disk */
      read_func = fd_read_with_partition;
    }
  else
    {
      /* hard disk */
      read_func = ide_read_block_1k;
    }

  retsize = size;
  bufp = buf;

#ifdef USE_ALLOCA
  blockbuf = (char *)alloca (BLOCK_SIZE);
#endif
  blockno = start / BLOCK_SIZE;
  while (size > 0)
    {
#ifdef nodef
      fd_read (device,
	       blockno,
	       blockbuf);
#else
      (*read_func)(drive, part, blockno, blockbuf, 1);
#endif
      offset = start % BLOCK_SIZE;
      copysize = MIN (BLOCK_SIZE - offset, size);
      bcopy (&blockbuf[offset], buf, copysize);

      buf += copysize;
      start += copysize;
      size -= copysize;
      blockno++;
    }
  return (0);
  
}

/*
 *
 */
int
sfs_write_dev (int dev, int start, int size, char *buf)
{
#ifdef nodef

#ifdef USE_ALLOCA
  char	*blockbuf;
#else
  static char	blockbuf[BLOCK_SIZE];
#endif

  device = dev >> 8;
  part = dev & 0xff;

  retsize = size;
  bufp = buf;

#ifdef USE_ALLOCA
  blockbuf = (char *)alloca (BLOCK_SIZE);
#endif
  blockno = start / BLOCK_SIZE;
  while (size > 0)
    {
      fd_write (device,
	       blockno,
	       blockbuf);
      boot_printf ("fd_write: fd = %d, blockno = %d, buf = 0x%x\n",
	      device, blockno, blockbuf);
      offset = start % BLOCK_SIZE;
      copysize = MIN (BLOCK_SIZE - offset, size);
      bcopy (&blockbuf[offset], buf, copysize);

      buf += copysize;
      start += copysize;
      size -= copysize;
      blockno++;
    }
  return E_OK;

#endif 

  return ENODEV;
  
}


int
sfs_read_buf (char **av)
{
  char	buf[SFS_BLOCK_SIZE];
  struct sfs_superblock	*sb;

  sfs_read_dev (0, 512, sizeof (struct sfs_superblock), buf);
  sb = (struct sfs_superblock *)buf;
  boot_printf ("magic ID  0x%x\n", sb->sfs_magic);

  return E_OK;
}
