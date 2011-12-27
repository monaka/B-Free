/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_dir.c - SFS のディレクトリに関係する処理を行う。
 *
 * $Log: sfs_dir.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.11  2000-06-01 08:47:24  naniwa
 * to implement getdents
 *
 * Revision 1.10  1999/05/30 04:05:32  naniwa
 * modified to creat file correctly
 *
 * Revision 1.9  1999/05/28 15:48:34  naniwa
 * sfs ver 1.1
 *
 * Revision 1.8  1999/05/10 15:46:16  night
 * ディレクトリ内容の書き込みをする処理で、sfs_i_read () を呼びだしていた
 * のを sfs_i_write () を呼び出すように変更。
 *
 * Revision 1.7  1999/04/13 04:15:26  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.6  1999/03/24 04:52:16  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.5  1997/07/04 15:07:41  night
 * ・スペシャルファイル - デバイスドライバポートの対応表の関連処理の追加。
 * ・ファイルの読み込み処理の改訂。
 *
 * Revision 1.4  1997/07/03 14:24:35  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.3  1996/11/27 15:42:08  night
 * sfs_write_dir() を追加
 *
 * Revision 1.2  1996/11/18 13:46:10  night
 * ファイルの中身を作成。
 *
 * Revision 1.1  1996/11/17  14:52:57  night
 * 最初の登録
 *
 *
 */

static char rcsid[] = "$Id: sfs_dir.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../posix.h"
#include "sfs_fs.h"
#include "sfs_func.h"


/* ディレクトリに関係する処理
 *
 * sfs_read_dir()
 * sfs_write_dir()
 *
 */


/* sfsread_dir -
 *
 */
W
sfs_read_dir (struct inode *parentp,
	      W nentry,
	      struct sfs_dir *dirp)
{
  W	size;
  W	errno;
  W	rsize;

#ifdef FMDEBUG
  printk ("sfs_read_dir: start. parent = 0x%x, nentry = %d\n", parentp, nentry);
#endif

  if ((nentry <= 0) || (dirp == NULL))
    {
      return (parentp->i_size / sizeof (struct sfs_dir));
    }
  
  size = ((nentry * sizeof (struct sfs_dir)) <= parentp->i_size) ? 
          nentry * sizeof (struct sfs_dir) :
	  parentp->i_size;

  errno = sfs_i_read (parentp, 0, (char *)dirp, size, &rsize);
  if (errno)
    {
      return (errno);
    }
  return (0);
}


/* sfs_write_dir - ディレクトリに新しい要素を追加
 *
 */
W
sfs_write_dir (struct inode *parentp,
	      W nentry,
	      struct sfs_dir *dirp)
{
  W	errno;
  W	rsize;

  /* 親ディレクトリの netnry 目から後に dirp の内容を追加 */
  errno = sfs_i_write (parentp, nentry*sizeof(struct sfs_dir), (char *)dirp,
		       sizeof(struct sfs_dir), &rsize);
  if (errno)
    {
      return (errno);
    }
  return (0);
}

/*
 * sfs_getdetns()
 */
W sfs_getdents(struct inode *ip, ID caller, W offset,
	       VP buf, UW length, W *rsize, W *fsize)
{
  W nentry, i, s, errno, len;
  struct sfs_dirnet {
    long		d_ino;
    unsigned long	d_off;
    unsigned short	d_reclen;
    char		d_name[SFS_MAXNAMELEN+1];
  } dent;

  *rsize = 0; *fsize = 0;
  dent.d_ino = ip->i_index;
  nentry = sfs_read_dir (ip, 0, NULL);
  s = sizeof(struct sfs_dir);
  if (offset >= nentry*s) return(EP_OK);
  {
    struct sfs_dir dirp[nentry]; /* GCC の拡張機能を使っている */
    errno = sfs_read_dir (ip, nentry, dirp);
    if (errno) return(errno);
    for (i = offset/s; i < nentry; i++) {
      len = sizeof(struct dirent)+strlen(dirp[i].sfs_d_name);
      if ((*rsize) + len >= length) return(EP_OK);
      dent.d_reclen = len;
      dent.d_off = i*s;
      strncpy(dent.d_name, dirp[i].sfs_d_name, SFS_MAXNAMELEN+1);
      errno = vput_reg(caller, buf+(*rsize), len, &dent);
      if (errno) return(errno);
      *rsize += len;
      *fsize += s;
    }
  }
  return(EP_OK);
}
