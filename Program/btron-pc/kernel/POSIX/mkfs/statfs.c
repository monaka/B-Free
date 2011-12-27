/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* 
 * $Header: /cvsroot/bfree-info/B-Free/Program/btron-pc/kernel/POSIX/mkfs/statfs.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 *
 *
 * $Log: statfs.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.25  2000-07-02 15:29:36  kishida0
 * add command line argument counter
 *
 * Revision 1.24  2000/06/02 10:29:34  naniwa
 * to show error message when disk is full
 *
 * Revision 1.23  2000/05/06 03:51:00  naniwa
 * fixed file parmission
 *
 * Revision 1.22  2000/02/20 09:40:45  naniwa
 * to implement mknod
 *
 * Revision 1.21  2000/02/16 08:19:57  naniwa
 * minor fix on error message
 *
 * Revision 1.20  1999/12/23 07:04:15  naniwa
 * fixed alloc_inode
 *
 * Revision 1.19  1999/12/21 10:54:58  naniwa
 * fixed alloc_inode()
 *
 * Revision 1.18  1999/05/28 15:52:13  naniwa
 * modified to support sfs ver 1.1
 *
 * Revision 1.17  1999/05/13 08:25:20  naniwa
 * fixed free_indirect(), free_dindirect()
 *
 * Revision 1.16  1999/05/06 23:17:22  naniwa
 * fixed to keep file system consistent
 *
 * Revision 1.15  1999/04/23 18:41:52  night
 * Add free_inode function.
 *
 * Revision 1.14  1999/04/23 12:00:20  monaka
 * Added include directive (errno.h)
 *
 * Revision 1.13  1999/04/13 04:15:34  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.12  1997/08/31 13:37:21  night
 * ヘルプメッセージの変更。
 *
 * Revision 1.11  1997/04/24 15:43:54  night
 * 註釈の修正。
 *
 * Revision 1.10  1996/11/18  13:47:53  night
 * write コマンドを実行したとき、ファイルが存在しない場合に新たに作成する
 * ように機能を変更した。
 *
 * Revision 1.9  1996/11/16  13:01:23  night
 * f_mkdir () 関数の中で write_file() を呼び出しているが、第6引数の
 * 型をキャストして正しいもの (B*) に合わせた。
 * (ワーニング出力抑制のため)
 *
 * Revision 1.8  1996/11/16  12:58:43  night
 * write_file() の中で write() システムコールの返り値としてチェックする
 * 値として struct inode のサイズを使っていたが、正しく struct sfs_inode
 * を使うように変更した。
 *
 * Revision 1.7  1996/11/14  13:19:22  night
 * 一重および二重間接ブロックの処理を追加。
 *
 * Revision 1.6  1996/11/13  12:54:41  night
 * コマンド処理機能の追加。
 *
 * Revision 1.5  1996/11/12  11:33:07  night
 * mount_fs()、alloc_inode()、alloc_block() の関数を追加した。
 *
 * Revision 1.4  1996/11/11  13:53:01  night
 * read_file() 関数をメモリを消費しないものに修正。
 *
 * Revision 1.3  1996/11/11  13:38:46  night
 * ディレクトリの内容を取り出す機能の追加。
 *
 * Revision 1.2  1996/11/10  11:57:12  night
 * ファイルシステムの root ディレクトリの中身を表示するような処理を追加。
 *
 * Revision 1.1  1996/11/08  11:07:42  night
 * 最初の登録
 *
 *
 */


#define MAX_MODULE_NAME	40

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "../../ITRON/h/types.h"
#include "../manager/sfs/sfs_fs.h"


static char rcsid[] = "@(#)$Id: statfs.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";


#define BLOCKSIZE 512

#define RDONLY			1
#define RDWR			2


#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
#define MIN(x,y)		((x > y) ? y : x)

int mount_fs (char *path, struct sfs_superblock *sb, struct sfs_inode *root, int mode);
int lookup_file (int fd, struct sfs_superblock *sb, struct sfs_inode *cwd, char	*path, struct sfs_inode *ip);
int create_file (int fd, struct sfs_superblock *sb, struct sfs_inode *parent_dir, const char *name, int mode, struct sfs_inode *newinode);
int read_file (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int start, int size, B *buf);
int write_inode (int fd, struct sfs_superblock *sb, struct sfs_inode *ip);
int remove_file (int fd, struct sfs_superblock *sb, struct sfs_inode *dir, char *fname);
void print_superblock (struct sfs_superblock *sb);
int truncate_file (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int newsize);

int read_block (int fd, int blockno, int blocksize, B *buf);
int write_block (int fd, int blockno, int blocksize, B *buf);
int alloc_block (int fd, struct sfs_superblock *sb);
int free_block (int fd, struct sfs_superblock *sb, int blockno);
void free_indirect(int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int offset, int inblock);
void free_dindirect(int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int offset, int dinblock, int inblock);
void free_all_dindirect(int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int inblock);
int get_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);
int get_indirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);
int set_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno, int newblock);
int set_indirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno, int newblock);
int set_dindirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno, int newblock);
int locallookup_file (int fd, struct sfs_superblock *sb, struct sfs_inode *parent, struct sfs_inode *ip, char *name);
int get_dindirect_block_num (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int blockno);


extern W read_dir (int fd, struct sfs_superblock *sb, struct sfs_inode *ip, int nentry, struct sfs_dir *dirp);

extern W read_inode (int fd, struct sfs_superblock *sb, int ino, struct sfs_inode *ip);
extern W read_rootdir (int fd, struct sfs_superblock *sb, struct sfs_dir **dirp, int *nentry);
extern int write_file (int fd, struct sfs_superblock *sb, struct sfs_inode *ip,
		       int start, int size, B *buf);

struct sfs_inode	rootdir_buf;
struct sfs_inode	*rootdirp;

extern int	f_create_file ();
extern int	f_write_file ();
extern int	f_read_file ();
extern int	f_dir ();
extern int	f_statfs ();
extern int	f_mkdir ();
extern int	f_rmdir ();
extern int 	f_remove_file ();
extern int	f_truncate ();
extern int	f_mknod ();
extern int	f_chmod ();

struct cmd
{
  char	*name;
  int   options;
  int	(*funcp)();
} cmdtable[] =
{
  { "create", 1,f_create_file },
  { "rm",     1,f_remove_file },
  { "write",  2,f_write_file },
  { "read",   1,f_read_file },
  { "dir",    1,f_dir },
  { "mkdir",  1,f_mkdir },
  { "rmdir",  1,f_rmdir },
  { "statfs", 0,f_statfs },
  { "trunc",  2,f_truncate },
  { "mknod",  2,f_mknod },
  { "chmod",  2,f_chmod },
  { NULL,     0,NULL }
};


void
usage (void)
{
  fprintf (stderr, "usage: statfs device command args\n");
  fprintf (stderr, "Command:\n");
  fprintf (stderr, "\tcreate file\n");
  fprintf (stderr, "\trm file\n");
  fprintf (stderr, "\twrite to from\n");
  fprintf (stderr, "\tread filename\n");
  fprintf (stderr, "\tdir directory\n");
  fprintf (stderr, "\tmkdir directory\n");
  fprintf (stderr, "\trmdir directory\n");
  fprintf (stderr, "\ttrunc\n");
  fprintf (stderr, "\ttrunc file size\n");
  fprintf (stderr, "\tmknod path maj/min_num\n");
  fprintf (stderr, "\tchmod mode path\n");
}

int
main (int ac, char **av)
{
  int			fd;
  struct sfs_superblock	sb;
  int			i;

  printf("%s\n",rcsid);
  if (ac < 3)
    {
      usage ();
      return (0);
    }

  fd = mount_fs (av[1], &sb, &rootdir_buf, RDWR);
  rootdirp = &rootdir_buf;

#ifdef notdef
  print_superblock (&sb);
  fprintf (stderr, "root dir: index = %d\n", rootdirp->sfs_i_index);

  nentry = read_dir (fd, &sb, rootdirp, 0, NULL);
  rootdir = alloca (sizeof (struct sfs_dir) * nentry);
  read_dir (fd, &sb, rootdirp, nentry, rootdir);
  for (i = 0; i < nentry; i++)
    {
      fprintf (stderr, "rootdir[%d] inode = %d, name = %-14s\n", i, rootdir[i].sfs_d_index, rootdir[i].sfs_d_name);
    }
#endif

  for (i = 0; cmdtable[i].name != NULL; i++)
    {
      int errno;

      if (strcmp (av[2], cmdtable[i].name) == 0)
	{
          printf("opt = %d %d\n",cmdtable[i].options, ac);
          if ((cmdtable[i].options) + 3 != ac)
            {
              fprintf (stderr, "command line option error\n");
              fprintf (stderr, "-----------------\n");
              usage ();
              return (0);
            }
	  errno = (*cmdtable[i].funcp)(fd, &sb, av[3], av[4], av[5]);
	  if (errno)
	    {
	      fprintf (stderr, "errno = %d\n", errno);
	    }
	  exit (0);
	}
    }
  fprintf (stderr, "Unknown command: %s\n", av[2]);
  exit (1);
}



/* ============================================================
 *  コマンド
 *
 */

/* f_create_file
 *
 */
int
f_create_file (int fd, struct sfs_superblock *sb, char *path)
{
  struct sfs_inode parent_ip, ip;
  int	errno;
  char	*pdirname, *fname;
  int	i;

  for (i = strlen (path); i > 0; i--)
    {
      if (path[i] == '/')
	break;
    }
  
  pdirname = alloca (i + 1);
  strncpy (pdirname, path, i);
  pdirname[i] = '\0';

  fname = alloca (strlen (path) - i + 1);
  strncpy (fname, &path[i + 1], strlen (path) - i);
  fname[strlen (path) - i] = '\0';

  errno = lookup_file (fd, sb, rootdirp, pdirname, &parent_ip);
  if (errno)
    {
      fprintf(stderr,"cannot lookup parent directory\n");
      return (errno);
    }

  errno = create_file (fd, sb, &parent_ip, fname, 0666, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot create file\n");
      return (errno);
    }
  return (0);
}



/* f_write_file
 *
 */
int
f_write_file (int fd, struct sfs_superblock *sb, char *path, char *src_file)
{
  struct sfs_inode	ip;
  int			errno;
  int			dfd;
  struct stat		st;
  char			*buf;


  dfd = open (src_file, O_RDONLY);
  if (dfd < 0)
    {
      fprintf (stderr, "cannot open src file.\n");
      return (-1);
    }
  fstat (dfd, &st);
  buf = (char *)malloc (st.st_size);
  if (buf == NULL)
    {
      fprintf (stderr, "cannot allocate buffer\n");
      return(ENOMEM);
    }
  read (dfd, buf, st.st_size);

  errno = lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      if (errno == ENOENT)
	{
	  errno = f_create_file (fd, sb, path);
	  if (errno)
	    {
	      return (errno);
	    }
	  errno = lookup_file (fd, sb, rootdirp, path, &ip);
	  if (errno)
	    {
	      return (errno);
	    }
	}
      else
	{
	  fprintf (stderr, "cannot open file.\n");
	  free (buf);
	  return (errno);
	}
    }

  errno = write_file (fd, sb, &ip, 0, st.st_size, buf);
  if (errno)
    {
      fprintf (stderr, "cannot write to file.\n");
      free (buf);
      return (errno);
    }

  free (buf);
  return (0);
}


int
f_read_file (int fd, struct sfs_superblock *sb, char *path)
{
  struct sfs_inode	ip;
  int			errno;
  char			*buf;
  int			i, rsize;
  int			total;


  errno = lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot open file.\n");
      return (errno);
    }

  
  buf = alloca (sb->sfs_blocksize);
  rsize = ip.sfs_i_size;
  total = 0;
  for (i = 0; rsize > 0; i += sb->sfs_blocksize)
    {
      errno = read_file (fd, sb, &ip, i, MIN (sb->sfs_blocksize, rsize), buf);
      if (errno)
	{
	  fprintf (stderr, "cannot read to file.\n");
	  return (errno);
	}
      fwrite (buf, MIN (sb->sfs_blocksize, rsize), 1, stdout);
      total += MIN (sb->sfs_blocksize, rsize);
      rsize -= sb->sfs_blocksize;
    }
  return (0);
}


int
f_dir (int fd, struct sfs_superblock *sb, char *path)
{
  struct sfs_inode	ip;
  int			errno;
  int			nentry;
  struct sfs_dir	*dirp;
  char			*name;
  int			i;

  errno = lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot open file.\n");
      return (errno);
    }
  if ((ip.sfs_i_perm & SFS_FMT_DIR) == 0)
    {
      fprintf (stderr, "%3ld  %4.4lo\t%4.4ld\t%4.4ld\t%-14s\t%ld bytes\n", ip.sfs_i_nlink, ip.sfs_i_perm, ip.sfs_i_uid, ip.sfs_i_gid, path, ip.sfs_i_size);
      return (0);
    }

  nentry = read_dir (fd, sb, &ip, 0, NULL);
  dirp = alloca (nentry * sizeof (struct sfs_dir));
  if (read_dir (fd, sb, &ip, nentry, dirp) != 0)
    {
      fprintf (stderr, "cannot read directory\n");
      return (-1);
    }
  name = alloca (strlen (path) + SFS_MAXNAMELEN + 2);
  for (i = 0; i < nentry; i++)
    {
      struct sfs_inode	ip;

      strcpy (name, path);
      strcat (name, "/");
      strcat (name, dirp[i].sfs_d_name);
      errno = lookup_file (fd, sb, rootdirp, name, &ip);
      if (errno)
	{
	  fprintf (stderr, "errno = %d\n", errno);
	  return (errno);
	}
      if ((ip.sfs_i_perm & SFS_FMT_DEV) != 0) {
	fprintf (stderr, "%3ld  %4.4lo\t%4.4ld\t%4.4ld\t%-14s\t%08lx\n",
		 ip.sfs_i_nlink, ip.sfs_i_perm, ip.sfs_i_uid, ip.sfs_i_gid,
		 dirp[i].sfs_d_name, ip.sfs_i_direct[0]);
      }
      else {
	fprintf (stderr, "%3ld  %4.4lo\t%4.4ld\t%4.4ld\t%-14s\t%ld bytes\n",
		 ip.sfs_i_nlink, ip.sfs_i_perm, ip.sfs_i_uid, ip.sfs_i_gid,
		 dirp[i].sfs_d_name, ip.sfs_i_size);
      }
    }
  return (0);
}


int
f_mkdir (int fd, struct sfs_superblock *sb, char *path)
{
  struct sfs_inode parent_ip, ip;
  int	errno;
  char	*pdirname, *fname;
  int	i;
  static struct sfs_dir	dir[2] =
    {
      { 0, "." },
      { 0, ".." }
    };

  for (i = strlen (path); i > 0; i--)
    {
      if (path[i] == '/')
	break;
    }
  
  pdirname = alloca (i + 1);
  strncpy (pdirname, path, i);
  pdirname[i] = '\0';

  fname = alloca (strlen (path) - i + 1);
  strncpy (fname, &path[i + 1], strlen (path) - i);
  fname[strlen (path) - i] = '\0';

/*  fprintf (stderr, "parent = %s, file = %s\n", pdirname, fname); */
  errno = lookup_file (fd, sb, rootdirp, pdirname, &parent_ip);
  if (errno)
    {
      fprintf(stderr,"cannot lookup parent directory\n");
      return (errno);
    }

  errno = create_file (fd, sb, &parent_ip, fname, 0777, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot create file\n");
      return (errno);
    }
  ip.sfs_i_nlink = 2;
  ip.sfs_i_perm |= (SFS_FMT_DIR);
  errno = write_inode (fd, sb, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot write inode\n");
      return (errno);
    }
  /* parent_ip のリンク数の更新は create_file では行わない．*/
  parent_ip.sfs_i_nlink += 1;
  errno = write_inode (fd, sb, &parent_ip);
  if (errno)
    {
      fprintf (stderr, "cannot write inode\n");
      return (errno);
    }

  dir[0].sfs_d_index = ip.sfs_i_index;
  dir[1].sfs_d_index = parent_ip.sfs_i_index;
  errno = write_file (fd, sb, &ip, 0, sizeof (dir), (B*)dir);
  if (errno)
    {
      fprintf (stderr, "cannot write to directory.\n");
      return (errno);
    }

  return (0);
}


int
f_rmdir (int fd, struct sfs_superblock *sb, char *path)
{
  struct sfs_inode parent_ip, ip;
  int	errno;
  char	*pdirname, *fname;
  int	i;

  for (i = strlen (path); i > 0; i--)
    {
      if (path[i] == '/')
	break;
    }
  
  errno = lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      fprintf (stderr, "Cannot found entry.\n");
      return (errno);
    }
  if ((ip.sfs_i_perm & SFS_FMT_DIR) == 0)
    {
      fprintf (stderr, "Not directory\n");
      return (ENOTDIR);
    }
  if (read_dir(fd, sb, &ip, 0, NULL) > 2)
    {
      fprintf (stderr, "Directory not empty.\n");
      return (EEXIST);
    }

  pdirname = alloca (i + 1);
  strncpy (pdirname, path, i);
  pdirname[i] = '\0';

  fname = alloca (strlen (path) - i + 1);
  strncpy (fname, &path[i + 1], strlen (path) - i);
  fname[strlen (path) - i] = '\0';

  errno = lookup_file (fd, sb, rootdirp, pdirname, &parent_ip);
  if (errno)
    {
      printf("cannot lookup parent directory\n");
      return (errno);
    }

  errno = remove_file (fd, sb, &parent_ip, fname);
  if (errno)
    {
      fprintf (stderr, "cannot remove file\n");
      return (errno);
    }
  return (0);
}



int
f_statfs (int fd, struct sfs_superblock *sb)
{
  print_superblock (sb);
  return (0);
}


int
f_truncate (int fd, struct sfs_superblock *sb, char *path, char *newsize)
{
  struct sfs_inode ip;
  int	errno;

  errno = lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      fprintf (stderr, "Cannot lookup file\n");
      return (errno);
    }

  errno = truncate_file (fd, sb, &ip, atoi (newsize));
  if (errno)
    {
      fprintf (stderr, "Cannot truncate file.\n");
      return (errno);
    }
  return (0);
}


int
f_remove_file (int fd, struct sfs_superblock *sb, char *path)
{
  struct sfs_inode parent_ip;
  int	errno;
  char	*pdirname, *fname;
  int	i;

  for (i = strlen (path); i > 0; i--)
    {
      if (path[i] == '/')
	break;
    }
  
  errno = lookup_file (fd, sb, rootdirp, path, &parent_ip);
  if (errno)
    {
      fprintf (stderr, "Cannot found entry.\n");
      return (errno);
    }
  if ((parent_ip.sfs_i_perm & SFS_FMT_DIR) != 0)
    {
      fprintf (stderr, "Is a directory\n");
      return (EISDIR);
    }

  pdirname = alloca (i + 1);
  strncpy (pdirname, path, i);
  pdirname[i] = '\0';

  fname = alloca (strlen (path) - i + 1);
  strncpy (fname, &path[i + 1], strlen (path) - i);
  fname[strlen (path) - i] = '\0';

  errno = lookup_file (fd, sb, rootdirp, pdirname, &parent_ip);
  if (errno)
    {
      printf("cannot lookup parent directory\n");
      return (errno);
    }

  errno = remove_file (fd, sb, &parent_ip, fname);
  if (errno)
    {
      fprintf (stderr, "cannot remove file\n");
      return (errno);
    }
  return (0);
}

int
f_mknod (int fd, struct sfs_superblock *sb, char *path, char *num)
{
  struct sfs_inode parent_ip, ip;
  int	errno;
  char	*pdirname, *fname;
  int	i;

  for (i = strlen (path); i > 0; i--)
    {
      if (path[i] == '/')
	break;
    }
  
  pdirname = alloca (i + 1);
  strncpy (pdirname, path, i);
  pdirname[i] = '\0';

  fname = alloca (strlen (path) - i + 1);
  strncpy (fname, &path[i + 1], strlen (path) - i);
  fname[strlen (path) - i] = '\0';

  errno = lookup_file (fd, sb, rootdirp, pdirname, &parent_ip);
  if (errno)
    {
      printf("cannot lookup parent directory\n");
      return (errno);
    }

  errno = create_file (fd, sb, &parent_ip, fname, 0666, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot create file\n");
      return (errno);
    }
  ip.sfs_i_perm |= (SFS_FMT_DEV);
  if (strncmp(num, "0x", 2) == 0) {
    sscanf(&num[2], "%x", &i);
  }
  else {
    sscanf(num, "%d", &i);
  }
  ip.sfs_i_direct[0] = i;
  errno = write_inode (fd, sb, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot write inode\n");
      return (errno);
    }
  return (0);
}

int
f_chmod (int fd, struct sfs_superblock *sb, char *num, char *path)
{
  struct sfs_inode ip;
  int	errno;
  int	i;

  errno = lookup_file (fd, sb, rootdirp, path, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot open file.\n");
      return (errno);
    }

  sscanf(num, "%o", &i);
  ip.sfs_i_perm = i;
  errno = write_inode (fd, sb, &ip);
  if (errno)
    {
      fprintf (stderr, "cannot write inode\n");
      return (errno);
    }
  return (0);
}


/* ファイルシステム全体に関係する処理
 *
 * mount_fs()
 * print_superblock()
 *
 */


int
mount_fs (char *path, struct sfs_superblock *sb, struct sfs_inode *root, int mode)
{
  int	fd;

  if (mode == RDONLY)
    {
      fd = open (path, O_RDONLY);
    }
  else
    {
      fd = open (path, O_RDWR);
    }

  if (fd < 0)
    {
      fprintf (stderr, "Cannot open file.\n");
      return (-1);
    }

  if (lseek (fd, BLOCKSIZE, 0) < 0)
    {
      return (-1);
    }

  if (read (fd, sb, sizeof (struct sfs_superblock)) != sizeof (struct sfs_superblock))
    {
      return (-1);
    }

  read_inode (fd, sb, 1, root);
  return (fd);  
}


void
print_superblock (struct sfs_superblock	*sb)
{
  if (sb->sfs_magic != SFS_MAGIC)
    {
      fprintf (stderr, "Invalid Magic ID\n");
      exit (1);
    }

  fprintf (stderr, "*STATUS* \n\n");
  fprintf (stderr, "FS type\t\tSFS\n");
  fprintf (stderr, "version\t\t%d.%d\n", sb->sfs_version_hi, sb->sfs_version_lo);
  fprintf (stderr, "total size\t%ld\n", sb->sfs_nblock * sb->sfs_blocksize);
  fprintf (stderr, "size\t\t%ld\n", sb->sfs_freeblock * sb->sfs_blocksize);
  fprintf (stderr, "mount count\t%ld\n", sb->sfs_mountcount);
  fprintf (stderr, "blocksize\t%d bytes\n", sb->sfs_blocksize);
  fprintf (stderr, "block\t\t%ld block, %ld free\n", sb->sfs_nblock, sb->sfs_freeblock);
  fprintf (stderr, "bitmap\t\t%ld bytes\n", sb->sfs_bitmapsize * sb->sfs_blocksize);
  fprintf (stderr, "inode\t\t%ld inode, %ld free\n", sb->sfs_ninode, sb->sfs_freeinode);
  fprintf (stderr, "isearch\t\t%ld, bsearch\t\t%ld\n", sb->sfs_isearch, sb->sfs_bsearch);
}


/* ディレクトリに関係する処理
 *
 * read_rootdir()
 * read_dir()
 *
 */

W
read_rootdir (int fd,
	      struct sfs_superblock *sb,
	      struct sfs_dir **dirp,
	      int *nentry)
{
  struct sfs_inode	ip;

  
  if (read_inode (fd, sb, 1, &ip) != 0)
    {
      fprintf (stderr, "Cannot read rootdir.\n");
      exit (0);
    }

  *dirp = (struct sfs_dir *)malloc (ip.sfs_i_size);
  if (*dirp == NULL)
    {
      fprintf (stderr, "Memory over flow.\n");
      exit (0);
    }

  *nentry = ip.sfs_i_size / sizeof (struct sfs_dir);
  read_file (fd, sb, &ip, 0, ip.sfs_i_size, (B *)dirp);
  return (0);
}


W
read_dir (int fd,
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

  read_file (fd, sb, ip, 0, size, (B *)dirp);	/* エラーチェックが必要! */
  return (0);
}


/* inode に関係している処理
 *
 * get_inode_offset()
 * read_inode()
 * alloc_inode()
 * write_inode()
 */
int
get_inode_offset (struct sfs_superblock *sb, int ino)
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


W
read_inode (int fd, struct sfs_superblock *sb, int ino, struct sfs_inode *ip)
{
  int	offset;
  offset = get_inode_offset (sb, ino);
  lseek (fd, offset, 0);
  read (fd, ip, sizeof (struct sfs_inode));
  return (0);
}


int
alloc_inode (int fd, struct sfs_superblock *sb)
{
  int	 i;
  struct sfs_inode	ipbuf;

  if (sb->sfs_freeinode <= 0)
    {
      return (0);
    }

  if (lseek (fd, get_inode_offset (sb, sb->sfs_isearch), 0) < 0)
    {
      perror ("lseek");
      exit (1);
    }

  for (i = sb->sfs_isearch; i <= sb->sfs_ninode; i++)
    {
      if (read (fd, &ipbuf, sizeof (struct sfs_inode)) < sizeof (struct sfs_inode))
	{
	  return (0);
	}
      if (ipbuf.sfs_i_index != i)
	{
	  bzero (&ipbuf, sizeof (ipbuf));
	  ipbuf.sfs_i_index = i;
	  lseek (fd, get_inode_offset (sb, ipbuf.sfs_i_index), 0);
	  write (fd, &ipbuf, sizeof (ipbuf));
	  sb->sfs_freeinode--;
	  sb->sfs_isearch = (i+1);
	  lseek (fd, 1 * sb->sfs_blocksize, 0);
	  write (fd, sb, sizeof (struct sfs_superblock));

	  return (i);
	}
    }

  return (0);
}	


/* free_inode - 
 *
 * 
 */
int
free_inode (int fd, struct sfs_superblock *sb, int inode_index)
{
  struct sfs_inode	ipbuf;

  if (lseek (fd, get_inode_offset (sb, 1), 0) < 0)
    {
      perror ("lseek");
      exit (1);
    }

  bzero (&ipbuf, sizeof (ipbuf));
  if (lseek (fd, get_inode_offset (sb, inode_index), 0) < 0)
    {
      perror ("lseek");
      exit (1);
    }
  write (fd, &ipbuf, sizeof (ipbuf));

  sb->sfs_freeinode++;
  if (sb->sfs_isearch >= inode_index)
    sb->sfs_isearch = inode_index-1;
  lseek (fd, 1 * sb->sfs_blocksize, 0);
  write (fd, sb, sizeof (struct sfs_superblock));

  return (0);
}


int
write_inode (int fd, struct sfs_superblock *sb, struct sfs_inode *ip)
{
  lseek (fd, get_inode_offset (sb, ip->sfs_i_index), 0);
  if (write (fd, ip, sizeof (struct sfs_inode)) < sizeof (struct sfs_inode))
    {
      return (EIO);
    }
  /* rootdir_buf の内容の更新 */
  if (ip->sfs_i_index == 1) {
    memmove(rootdirp, ip, sizeof(struct sfs_inode));
  }
  return (0);
}



/* ファイルに関係している処理
 *
 * write_file()
 * read_file()
 * create_file()
 * lookup()
 */
int
write_file (int fd,
	   struct sfs_superblock *sb,
	   struct sfs_inode *ip,
	   int start,
	   int size,
	   B *buf)
{
  B	*blockbuf;
  int	copysize;
  int	offset;
  int	retsize;
  int	filesize;


  retsize = size;
  filesize = start + retsize;

  blockbuf = (B *)alloca (sb->sfs_blocksize);
  while (size > 0)
    {
      if (get_block_num (fd, sb, ip, start / sb->sfs_blocksize) <= 0)
	{
	  /* ファイルサイズを越えて書き込む場合には、新しくブロックをアロケートする
	   */
	  set_block_num (fd, sb, ip, start / sb->sfs_blocksize, alloc_block (fd, sb));
/*
 *   ip->sfs_i_direct[start / sb->sfs_blocksize] = alloc_block (fd, sb);
 */
	  bzero (blockbuf, sb->sfs_blocksize);
	}
      else
	{
	  read_block (fd, 
		      get_block_num (fd, sb, ip, start / sb->sfs_blocksize), 
		      sb->sfs_blocksize,
		      blockbuf);
	}

      /* 読み込んだブロックの内容を更新する
       */
      offset = start % sb->sfs_blocksize;
      copysize = MIN (sb->sfs_blocksize - offset, size);
      bcopy (buf, &blockbuf[offset], copysize);

      /* 更新したブロックを書き込む
       */
      write_block (fd, 
		   get_block_num (fd, sb, ip, start / sb->sfs_blocksize), 
		   sb->sfs_blocksize,
		   blockbuf);

      buf += copysize;
      start += copysize;
      size -= copysize;
    }

  /* もし、書き込みをおこなった後にファイルのサイズが増えていれば、
   * サイズを更新して inode を書き込む。
   * ファイルのサイズが減っていればファイルを切り詰める．
   */
  if (filesize > ip->sfs_i_size)
    {
      ip->sfs_i_size = filesize;
      ip->sfs_i_size_blk = ROUNDUP(filesize, sb->sfs_blocksize)/sb->sfs_blocksize;
      write_inode (fd, sb, ip);
    }
  else {
    truncate_file (fd, sb, ip, filesize);
  }
  return (0);
}

	    
int
read_file (int fd,
	   struct sfs_superblock *sb,
	   struct sfs_inode *ip,
	   int start,
	   int size,
	   B *buf)
{
  B	*blockbuf;
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

/*  fprintf (stderr, "read_file: offset = %d, size = %d\n", start, size); */
  blockbuf = (B *)alloca (sb->sfs_blocksize);
  while (size > 0)
    {
      read_block (fd, 
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
  return (0);
}



int
create_file (int fd,
	     struct sfs_superblock *sb,
	     struct sfs_inode *parent_dir,
	     const char *name,
	     int mode,
	     struct sfs_inode *newinode)
{
  int	inode_index;
  struct sfs_dir	*dirp;
  struct sfs_dir	newdir_entry;
  int	nentry;
  int	dir_index;

  if (strlen (name) > SFS_MAXNAMELEN)
    return (ENAMETOOLONG);

  nentry = read_dir (fd, sb, parent_dir, 0, NULL);
  dirp = alloca (nentry * sizeof (struct sfs_dir));
  if (read_dir (fd, sb, parent_dir, nentry, dirp) != 0)
    {
      return (ENOENT);
    }

  for (dir_index = 0; dir_index < nentry; dir_index++)
    {
      if (dirp[dir_index].sfs_d_index <= 0)
	{
	  /* 削除したエントリがある */
	  break;
	}
      if (strncmp (dirp[dir_index].sfs_d_name, name, SFS_MAXNAMELEN) == 0)
	{
	  return (EEXIST);
	}
    }

  /*
   * Inode の作成
   */
  if ((inode_index = alloc_inode (fd, sb)) == 0)
    {
      return (ENOMEM);
    }
  bzero(newinode, sizeof(struct sfs_inode));
  newinode->sfs_i_index = inode_index;
  newinode->sfs_i_nlink = 1;
  newinode->sfs_i_size = 0;
  newinode->sfs_i_size_blk = 0;
  newinode->sfs_i_perm = mode;
  newinode->sfs_i_uid = newinode->sfs_i_gid = 0;
  newinode->sfs_i_atime = newinode->sfs_i_mtime = newinode->sfs_i_ctime = time (NULL);
#ifdef notdef
  bzero (newinode->sfs_i_direct, sizeof (newinode->sfs_i_direct));
  bzero (newinode->sfs_i_indirect, sizeof (newinode->sfs_i_indirect));
  bzero (newinode->sfs_i_dindirect, sizeof (newinode->sfs_i_dindirect));
#endif
  write_inode (fd, sb, newinode);

  /* parent_dir のリンクカウントは増やさない．
  parent_dir->sfs_i_nlink += 1;
  write_inode (fd, sb, parent_dir);
  */

  /* 親ディレクトリの更新
   */
  newdir_entry.sfs_d_index = newinode->sfs_i_index;
  strcpy (newdir_entry.sfs_d_name, name);
  write_file (fd, 
	      sb, 
	      parent_dir, 
	      sizeof (struct sfs_dir) * dir_index, 
	      sizeof (struct sfs_dir),
	      (char *)&newdir_entry);
  return (0);
}


int
lookup_file (int fd,
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

  if (strcmp (path, "/") == 0)
    {
      bcopy (cwd, ip, sizeof (struct sfs_inode));
      return (0);
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

/*      fprintf (stderr, "local lookup = %s\n", name); */
      errno = locallookup_file (fd, sb, pdirp, dirp, name);
      if (errno)
	{
	  return (errno);
	}

      pdirp = dirp;
      dirp = pdirp;
    }

  bcopy (pdirp, ip, sizeof (struct sfs_inode));
  return (0);
}


int
locallookup_file (int fd,
		   struct sfs_superblock *sb, 
		   struct sfs_inode *parent,
		   struct sfs_inode *ip,
		   char *name)
{
  int	nentry;
  struct sfs_dir *dirp;
  int	i;
  int	errno;

  nentry = read_dir (fd, sb, parent, 0, NULL);
  dirp = alloca (sizeof (struct sfs_dir) * nentry);
  read_dir (fd, sb, parent, nentry, dirp);
  for (i = 0; i < nentry; i++)
    {
      if (strcmp (name, dirp[i].sfs_d_name) == 0)
	{
	  errno = read_inode (fd, sb, dirp[i].sfs_d_index, ip);
	  if (errno)
	    {
	      return (errno);
	    }
	  return (0);
	}
    }
  return (ENOENT);      
}


int
truncate_file (int fd,
	       struct sfs_superblock *sb,
	       struct sfs_inode *ip,
	       int	newsize)
{
  int	nblock, blockno, inblock, offset, dinblock;
  int	i;

  nblock = ROUNDUP (newsize, sb->sfs_blocksize);
  if (nblock < ROUNDUP (ip->sfs_i_size, sb->sfs_blocksize)) {
      /* 余分なブロックを解放する
       */
      for (blockno = i = nblock / sb->sfs_blocksize;
	   i < ROUNDUP (ip->sfs_i_size, sb->sfs_blocksize) / sb->sfs_blocksize;
	   i++)
	{
	  free_block (fd, sb, get_block_num (fd, sb, ip, i));
	}

      /* 間接ブロックの block の開放 */
      if (blockno < SFS_DIRECT_BLOCK_ENTRY) {
	/* 直接ブロックの範囲内 */
	for(i = blockno; i < SFS_DIRECT_BLOCK_ENTRY; ++i) {
	  ip->sfs_i_direct[i] = 0;
	}
	free_indirect(fd, sb, ip, 0, 0);
	free_all_dindirect(fd, sb, ip, 0);
      }
      else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
			  + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK))) {
	/* 一重間接ブロックの範囲内 */
	inblock = (blockno - SFS_DIRECT_BLOCK_ENTRY);
	offset = inblock % SFS_INDIRECT_BLOCK;
	inblock = inblock / SFS_INDIRECT_BLOCK;
	free_indirect(fd, sb, ip, offset, inblock);
	free_all_dindirect(fd, sb, ip, 0);
      }
      else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
			  + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)
			  + (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK * SFS_INDIRECT_BLOCK))) {
	/* 二重間接ブロックの範囲内 */
	blockno = blockno -
	  (SFS_DIRECT_BLOCK_ENTRY + SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);
	
	inblock = blockno / (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);
	dinblock = (blockno % (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK))
	  / SFS_INDIRECT_BLOCK;
	offset = blockno % SFS_INDIRECT_BLOCK;
	free_dindirect(fd, sb, ip, offset, dinblock, inblock);
	free_all_dindirect(fd, sb, ip, inblock+1);
      }
  }

  ip->sfs_i_size = newsize;
  ip->sfs_i_size_blk = nblock / sb->sfs_blocksize;
  return (write_inode (fd, sb, ip));
}

void free_indirect(int fd, struct sfs_superblock *sb, struct sfs_inode *ip,
		   int offset, int inblock) {
  int i;
  struct sfs_indirect	inbuf;

  if (offset != 0) {
    read_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize,
		(B*)&inbuf);
    for (i = offset; i < SFS_INDIRECT_BLOCK; ++i) {
      inbuf.sfs_in_block[i] = 0;
    }
    write_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize,
		 (B*)&inbuf);

    ++inblock;
  }
  for(i = inblock; i < SFS_INDIRECT_BLOCK_ENTRY; ++i) {
    if (ip->sfs_i_indirect[i] > 0) {
      free_block(fd, sb, ip->sfs_i_indirect[i]);
      ip->sfs_i_indirect[i] = 0;
    }
  }
}

void free_dindirect(int fd, struct sfs_superblock *sb, struct sfs_inode *ip,
		    int offset, int dinblock, int inblock) {
  int i;
  struct sfs_indirect	inbuf, inbuf2;

  if (ip->sfs_i_dindirect[inblock] <= 0) {
      return;
  }
  read_block (fd, ip->sfs_i_dindirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
  if (offset != 0) {
    read_block(fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize,
	       (B*)&inbuf2);
    for(i = offset; i < SFS_INDIRECT_BLOCK; ++i) {
      inbuf2.sfs_in_block[i] = 0;
    }
    write_block(fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize,
		(B*)&inbuf2);
    ++dinblock;
  }
  for(i = dinblock; i < SFS_INDIRECT_BLOCK; ++i) {
    if (inbuf.sfs_in_block[i] > 0) {
      free_block(fd, sb, inbuf.sfs_in_block[i]);
      inbuf.sfs_in_block[i] = 0;
    }
  }
  if (dinblock > 0) {
    write_block (fd, ip->sfs_i_dindirect[inblock],
		 sb->sfs_blocksize, (B*)&inbuf);
  }
}

void free_all_dindirect(int fd, struct sfs_superblock *sb,
			struct sfs_inode *ip, int inblock) {
  int i;

  for(i = inblock; i < SFS_DINDIRECT_BLOCK_ENTRY; ++i) {
    if (ip->sfs_i_dindirect[i] > 0) {
      free_dindirect(fd, sb, ip, 0, 0, i);
      free_block(fd, sb, ip->sfs_i_dindirect[i]);
      ip->sfs_i_dindirect[i] = 0;
    }
  }
}

int
remove_file (int fd,
	     struct sfs_superblock *sb,
	     struct sfs_inode *dir,
	     char *fname)
{
  int	nentry;
  int	i;
  struct sfs_dir	*buf;
  int	inodeindex;
  struct sfs_inode	ip;

  nentry = read_dir (fd, sb, dir, 0, NULL);
  if (nentry < 0)
    {
      return (ENOENT);
    }
  
  buf = alloca (sizeof (struct sfs_dir) * nentry);
  if (buf == NULL)
    {
      return (ENOMEM);
    }
  if (read_dir (fd, sb, dir, nentry, buf) != 0)
    {
      return (EIO);
    }

  for (i = 0; i < nentry; i++)
    {
      if (strcmp (fname, buf[i].sfs_d_name) == 0)
	{
	  inodeindex = buf[i].sfs_d_index;
	  break;
	}
    }
  if (i >= nentry)
    {
      return (ENOENT);
    }
  while (i < nentry)
    {
      buf[i].sfs_d_index = buf[i + 1].sfs_d_index;
      strcpy (buf[i].sfs_d_name, buf[i + 1].sfs_d_name);
      i++;
    }
  dir->sfs_i_size -= sizeof (struct sfs_dir);
  write_file (fd, sb, dir, 0, dir->sfs_i_size, (char *)buf);
  truncate_file (fd, sb, dir, dir->sfs_i_size);

  read_inode (fd, sb, inodeindex, &ip);
  ip.sfs_i_nlink--;
  if (ip.sfs_i_nlink <= 0)
    {
      truncate_file (fd, sb, &ip, 0);
      free_inode(fd, sb, ip.sfs_i_index);
    }
  else if ((ip.sfs_i_perm & SFS_FMT_DIR) != 0) {
    truncate_file (fd, sb, &ip, 0);
    free_inode(fd, sb, ip.sfs_i_index);

    /* 親ディレクトリの nlink の更新 */
    dir->sfs_i_nlink--;
    write_inode(fd, sb, dir);
  }
  return (0);
}


/* ブロックに関係している処理
 *
 * read_block()
 * write_block()
 * alloc_block()
 * get_block_num()
 * set_block_num()
 *
 */
int
read_block (int fd, int blockno, int blocksize, B *buf)
{
  if (lseek (fd, blockno * blocksize, 0) < 0)
    {
      return (0);
    }

  if (read (fd, buf, blocksize) < blocksize)
    {
      return (0);
    }

  return (blocksize);
}


int
write_block (int fd, int blockno, int blocksize, B *buf)
{
  if (lseek (fd, blockno * blocksize, 0) < 0)
    {
      return (0);
    }

  if (write (fd, buf, blocksize) < blocksize)
    {
      return (0);
    }

  return (blocksize);
}

int
alloc_block (int fd, struct sfs_superblock *sb)
{
  int	startoffset;
  int	i, j, k, s;
  char	*buf;
  int	free_block;
  unsigned char	mask;


  if (sb->sfs_freeblock <= 0)
    {
      fprintf(stderr, "cannot allocate block\n");
      return (-1);
    }


  startoffset = ((1 + 1) * sb->sfs_blocksize);
  lseek (fd, startoffset, 0);
  buf = alloca (sb->sfs_blocksize);
  s = (sb->sfs_bsearch-1)/(8*sb->sfs_blocksize);
  for (i = s; i < sb->sfs_bitmapsize; i++)
    {
      lseek (fd, (i * sb->sfs_blocksize) + startoffset, 0);
      if (read (fd, buf, sb->sfs_blocksize) < 0)
	{
	  return (-1);
	}
      if (i == s)
	j = ((sb->sfs_bsearch-1)/8)%sb->sfs_blocksize;
      else
	j = 0;
      for (; j < sb->sfs_blocksize; j++)
	{
	  if ((buf[j] & 0xff) != 0xff)
	    {
	      mask = 1;
	      for (k = 0; k < 8; k++)
		{
		  if ((mask & buf[j]) != mask)
		    {
		      free_block = (i * sb->sfs_blocksize * 8) 
			           + (j * 8)
				   + k;
		      buf[j] = buf[j] | mask;
		      lseek (fd, (i * sb->sfs_blocksize) + startoffset, 0);
		      if (write (fd, buf, sb->sfs_blocksize) < 0)
			{
			  fprintf (stderr, "write fail\n");
			  return (-1);
			}
		      sb->sfs_freeblock--;
		      sb->sfs_bsearch = free_block;
		      lseek (fd, 1 * sb->sfs_blocksize, 0);
		      write (fd, sb, sizeof (struct sfs_superblock));
		      return (free_block);
		    }
		  mask = mask << 1;
		}
	    }
	}
    }
  return (-1);
}	


int
free_block (int fd, struct sfs_superblock *sb, int blockno)
{
  unsigned char	block;
  int		startoffset;
  int		mask;

  startoffset = ((1 + 1) * sb->sfs_blocksize) + (blockno / 8);
  lseek (fd, startoffset, 0);
  read (fd, &block, 1);
  mask = 0x01;
  mask = mask << (blockno % 8);
  block = block & ((~mask) & 0xff);
  lseek (fd, startoffset, 0);
  write (fd, &block, 1);

  sb->sfs_freeblock++;
  if (sb->sfs_bsearch >= blockno && blockno > 0)
    sb->sfs_bsearch = blockno-1;
  lseek (fd, 1 * sb->sfs_blocksize, 0);
  write (fd, sb, sizeof (struct sfs_superblock));
  return(0);
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
      /* 一重間接ブロックの範囲内
       */
      return (get_indirect_block_num (fd, sb, ip, blockno));
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
  		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)
		      + (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK * SFS_INDIRECT_BLOCK)))
    {
      /* 二重間接ブロックの範囲内
       */
      return (get_dindirect_block_num (fd, sb, ip, blockno));
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

  read_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
#ifdef notdef
  fprintf (stderr, "get_ind: inblock = %d, offset = %d, blocknum = %d\n",
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

#ifdef notdef
  fprintf (stderr, "GET: blockno = %d, inblock = %d, dinblock = %d, dinblock_offset = %d\n",
	   blockno, inblock, dinblock, dinblock_offset);
#endif
  if (ip->sfs_i_dindirect[inblock] <= 0)
    {
      return (0);
    }

  read_block (fd, ip->sfs_i_dindirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
  if (inbuf.sfs_in_block[dinblock] <= 0)
    {
      return (0);
    }

  read_block (fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize, (B*)&inbuf);

#ifdef notdef
  fprintf (stderr, "get_ind: inblock = %d, dinblock = %d, offset = %d, blocknum = %d\n",
	  inblock, dinblock, dinblock_offset, inbuf.sfs_in_block[dinblock_offset]);
#endif
  return (inbuf.sfs_in_block[dinblock_offset]);
}


int
set_block_num (int fd,
	       struct sfs_superblock *sb,
	       struct sfs_inode *ip,
	       int blockno,
	       int newblock)
{
  if (newblock < 0)
    {
      return (-1);
    }
  if (blockno < (SFS_DIRECT_BLOCK_ENTRY))
    {
      /* 直接ブロックの範囲内
       */
      ip->sfs_i_direct[blockno] = newblock;
      return (ip->sfs_i_direct[blockno]);
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)))
    {
      /* 一重間接ブロックの範囲内
       */
      return (set_indirect_block_num (fd, sb, ip, blockno, newblock));
    }
  else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
  		      + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)
		      + (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK * SFS_INDIRECT_BLOCK)))
    {
      /* 二重間接ブロックの範囲内
       */
      return (set_dindirect_block_num (fd, sb, ip, blockno, newblock));
    }

  return (-1);
}



int
set_indirect_block_num (int fd,
			struct sfs_superblock *sb,
			struct sfs_inode *ip,
			int blockno,
			int newblock)
{
  int	inblock;
  int	inblock_offset;
  struct sfs_indirect	inbuf;
  int	newinblock;

  inblock = (blockno - SFS_DIRECT_BLOCK_ENTRY);
  inblock_offset = inblock % SFS_INDIRECT_BLOCK;
  inblock = inblock / SFS_INDIRECT_BLOCK;
  if (ip->sfs_i_indirect[inblock] <= 0)
    {
      newinblock = alloc_block (fd, sb);
      ip->sfs_i_indirect[inblock] = newinblock;
      bzero ((B*)&inbuf, sizeof (inbuf));
    }
  else
    {
      read_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
    }

  inbuf.sfs_in_block[inblock_offset] = newblock;
  write_block (fd, ip->sfs_i_indirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
  write_inode (fd, sb, ip);

#ifdef notdef
  fprintf (stderr, "set_ind: inblock = %d, offset = %d, newblock = %d\n",
	  inblock, inblock_offset, newblock);
#endif

  return (inbuf.sfs_in_block[inblock_offset]);
}


int
set_dindirect_block_num (int fd,
			 struct sfs_superblock *sb,
			 struct sfs_inode *ip,
			 int blockno, int newblock)
{
  int	dinblock;
  int	dinblock_offset;
  int	inblock;
  struct sfs_indirect	inbuf;	/* 一番目の間接ブロックの情報 */
  struct sfs_indirect	dinbuf;	/* 二番目の間接ブロックの情報 */
  int	newinblock;		
  int	newdinblock;		

  blockno = blockno - (SFS_DIRECT_BLOCK_ENTRY + (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK));

  inblock = blockno / (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK);
  dinblock = (blockno % (SFS_DINDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK)) / SFS_INDIRECT_BLOCK;
  dinblock_offset = blockno % SFS_INDIRECT_BLOCK;

  bzero (&inbuf, sizeof (inbuf));
  bzero (&dinbuf, sizeof (dinbuf));
  if (ip->sfs_i_dindirect[inblock] <= 0)
    {
      /* 一重目の間接ブロックの更新(アロケート)
       */
      newinblock = alloc_block (fd, sb);
      ip->sfs_i_dindirect[inblock] = newinblock;
      bzero ((B*)&inbuf, sizeof (inbuf));
    }
  else
    {
      read_block (fd, ip->sfs_i_dindirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
    }

  if (inbuf.sfs_in_block[dinblock] <= 0)
    {
      /* 二番目の間接ブロックの更新
       * (アロケート)
       */
      newdinblock = alloc_block (fd, sb);
      inbuf.sfs_in_block[dinblock] = newdinblock;
      bzero ((B*)&dinbuf, sizeof (dinbuf));
    }
  else
    {
      read_block (fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize, (B*)&dinbuf);
    }

  dinbuf.sfs_in_block[dinblock_offset] = newblock;

  write_block (fd, ip->sfs_i_dindirect[inblock], sb->sfs_blocksize, (B*)&inbuf);
  write_block (fd, inbuf.sfs_in_block[dinblock], sb->sfs_blocksize, (B*)&dinbuf);
  write_inode (fd, sb, ip);

  return (newblock);
}


