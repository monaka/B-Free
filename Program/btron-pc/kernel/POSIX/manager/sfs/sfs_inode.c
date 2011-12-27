/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_inode.c - SFS の inode の管理を行う
 *
 * $Log: sfs_inode.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.15  2000-07-02 04:14:26  naniwa
 * to implement disk block cache
 *
 * Revision 1.14  2000/05/06 03:53:20  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.13  2000/02/20 09:38:09  naniwa
 * to work with fs_register_inode
 *
 * Revision 1.12  1999/12/26 11:18:23  naniwa
 * to implement unlink
 *
 * Revision 1.11  1999/12/23 07:03:46  naniwa
 * fixed sfs_alloc_inode
 *
 * Revision 1.10  1999/12/21 10:55:46  naniwa
 * fixed sfs_alloc_inode ()
 *
 * Revision 1.9  1999/05/28 15:48:39  naniwa
 * sfs ver 1.1
 *
 * Revision 1.8  1999/05/10 15:55:02  night
 * inode の書き込み時に、inode のサイズが 1024 バイト以上のときには、1024
 * 毎に書き込むように処理を変更。ただし、2048 以上の場合はサポートしてい
 * ない。
 *
 * Revision 1.7  1997/10/24 14:00:48  night
 * 変数の引数合わせを厳密にした。
 *
 * Revision 1.6  1997/07/07 15:25:33  night
 * ROUNDUP マクロがすでに定義しているならば、再定義しないように修正した。
 *
 * Revision 1.5  1997/07/04 15:07:41  night
 * ・スペシャルファイル - デバイスドライバポートの対応表の関連処理の追加。
 * ・ファイルの読み込み処理の改訂。
 *
 * Revision 1.4  1997/07/03 14:24:37  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.3  1996/11/20  12:13:19  night
 * 細々とした修正。
 *
 * Revision 1.2  1996/11/18  13:46:11  night
 * ファイルの中身を作成。
 *
 * Revision 1.1  1996/11/17  14:52:57  night
 * 最初の登録
 *
 *
 */

static char rcsid[] = "$Id: sfs_inode.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../posix.h"
#include "sfs_func.h"

#ifndef ROUNDUP
#define ROUNDUP(x,align)	(((((int)x) + ((align) - 1))/(align))*(align))
#endif



/* inode に関係している処理
 *
 * sfs_get_inode_offset()
 * sfs_read_inode()
 * sfs_alloc_inode()
 * sfs_write_inode()
 */


/* get_inode_offset -
 *
 */
W
sfs_get_inode_offset (struct fs *fsp, W ino)
{
  W	offset;
  W	nblock;
  W	blocksize;
  struct sfs_superblock *sb;

  sb = &(fsp->fs_private.sfs_fs);
  nblock = sb->sfs_nblock;
  blocksize = sb->sfs_blocksize;
  offset = 1 + 1 + (ROUNDUP (nblock / 8, blocksize) / blocksize);
  offset *= blocksize;
  return (offset + ((ino - 1) * sizeof (struct sfs_inode)));
}


/* sfs_read_inode -
 *
 */
W
sfs_read_inode (struct fs *fsp, W ino, struct inode *ip)
{
  W	offset;
#ifdef notdef
  W	errno;
  W	rsize;
#endif
  ID	fd;
  W cn;
  B *buf;
  
  fd = fsp->fs_device;
  offset = sfs_get_inode_offset (fsp, ino);
#ifdef notdef
  printk ("sfs_read_inode: inode number = %d, offset = %d byte, size = %d\n",
	  ino, offset, sizeof (struct sfs_inode));
#endif
#ifdef notdef
  errno = sfs_read_device (fd, (B*)&(ip->i_private.sfs_inode), offset, sizeof (struct sfs_inode), &rsize);
  if (errno)
    {
      return (errno);
    }
#else
  sfs_get_cache(fd, offset/SFS_BLOCK_SIZE, &cn, &buf);
  bcopy(buf, (B*)&(ip->i_private.sfs_inode), sizeof (struct sfs_inode));
  sfs_put_cache(cn, 0);
#endif

  ip->i_index = ip->i_private.sfs_inode.sfs_i_index;
  ip->i_uid = ip->i_private.sfs_inode.sfs_i_uid;
  ip->i_gid = ip->i_private.sfs_inode.sfs_i_gid;
  ip->i_size = ip->i_private.sfs_inode.sfs_i_size;
  ip->i_atime = ip->i_private.sfs_inode.sfs_i_atime;
  ip->i_ctime = ip->i_private.sfs_inode.sfs_i_ctime;
  ip->i_mtime = ip->i_private.sfs_inode.sfs_i_mtime;
  ip->i_mode = ip->i_private.sfs_inode.sfs_i_perm;
  ip->i_link = ip->i_private.sfs_inode.sfs_i_nlink;
  if (ip->i_private.sfs_inode.sfs_i_size_blk <= 0)
    {
      ip->i_size_blk = fsp->fs_private.sfs_fs.sfs_blocksize;
    }
  else
    {
      ip->i_size_blk = ip->i_private.sfs_inode.sfs_i_size_blk;
    }
  ip->i_ops = &sfs_iops;
  ip->i_refcount = 1;
  ip->i_lock = 0;
#if 0
  ip->i_prev = ip->i_next = NULL;
#endif
  ip->i_fs = fsp;
  ip->i_device = fsp->fs_device;
  if (ip->i_mode & FS_FMT_DEV) {
    /* スペシャルファイルだった */
    ip->i_dev = ip->i_private.sfs_inode.sfs_i_direct[0];
  }
#ifdef notdef
  printk ("sfs_read_inode: index = %d mode = %d size = %d\n",
	  ip->i_index, ip->i_mode, ip->i_size);
#endif
  return (0);
}


/* sfs_alloc_inode -
 *
 */
W
sfs_alloc_inode (ID fd, struct fs *fsp)
{
  W	 		i;
  W			offset;
#ifdef notdef
  struct sfs_inode	ipbuf;
  W			errno;
  W			rsize;
#else
  struct sfs_inode	*ipbufp;
  W cn;
#endif

  if (fsp->fs_freeinode <= 0)
    {
      return (0);
    }

  offset = sfs_get_inode_offset (fsp, fsp->fs_isearch);
  for (i = fsp->fs_isearch; i <= fsp->fs_allinode; i++)
    {
#ifdef notdef
      errno = sfs_read_device (fd, (B*)&ipbuf, offset,
			       sizeof (struct sfs_inode), &rsize);
      if (errno)
	{
	  return (0);
	}
#else
      sfs_get_cache(fd, offset/SFS_BLOCK_SIZE, &cn, (B**) &ipbufp);
#endif


      offset += sizeof (struct sfs_inode);
#ifdef notdef
      if (ipbuf.sfs_i_index != i)
	{
	  bzero ((VP)&ipbuf, sizeof (ipbuf));
	  ipbuf.sfs_i_index = i;
	  sfs_write_device (fd,
			    (B*)&ipbuf,
			    sfs_get_inode_offset (fsp, ipbuf.sfs_i_index),
			    sizeof (ipbuf),
			    &rsize);
	  fsp->fs_freeinode--;
	  fsp->fs_usedinode++;
	  fsp->fs_isearch = (i+1);
	  /* ここで fs の sync を行う必要があるか? */
	  sfs_syncfs(fsp);
	  return (i);
	}
#else
      if (ipbufp->sfs_i_index != i)
	{
	  bzero ((VP)ipbufp, sizeof (struct sfs_inode));
	  ipbufp->sfs_i_index = i;
	  sfs_put_cache(cn, 1);
	  fsp->fs_freeinode--;
	  fsp->fs_usedinode++;
	  fsp->fs_isearch = (i+1);
	  /* ここで fs の sync を行う必要があるか? */
	  sfs_syncfs(fsp);
	  return (i);
	}
      else {
	sfs_put_cache(cn, 0);
      }
#endif
    }

  return (0);
}	


/* sfs_write_inode -
 *
 */
W
sfs_write_inode (W fd, struct fs *fsp, struct sfs_inode *ip)
{
  W	errno;
  W	rlength;
  W cn;
  B *buf;

  if (sizeof (struct sfs_inode) >= MAX_BODY_SIZE)	/* Kludge!! */
    {
      UB	*tmp;

      tmp = (B *)ip;
      errno = sfs_write_device (fd,
				tmp,
				sfs_get_inode_offset (fsp, ip->sfs_i_index),
				MAX_BODY_SIZE,
				&rlength);
      if (errno)
	{
	  return (EP_IO);
	}

      tmp += MAX_BODY_SIZE;
      errno = sfs_write_device (fd,
				tmp,
				sfs_get_inode_offset (fsp, ip->sfs_i_index)
				+ MAX_BODY_SIZE,
				sizeof (struct sfs_inode) - MAX_BODY_SIZE,
				&rlength);

      if (errno)
	{
	  return (EP_IO);
	}
    }
  else
    {
#ifdef notdef
      errno = sfs_write_device (fd,
				(B*)ip,
				sfs_get_inode_offset (fsp, ip->sfs_i_index),
				sizeof (struct sfs_inode),
				&rlength);
      if (errno)
	{
	  return (EP_IO);
	}
#else
      sfs_get_cache(fd,
		    sfs_get_inode_offset (fsp, ip->sfs_i_index)/SFS_BLOCK_SIZE,
		    &cn, &buf);
      bcopy((B*) ip, buf, sizeof(struct sfs_inode));
      sfs_put_cache(cn, 1);
      /* ここで fs の sync を行う必要があるか? */
      sfs_syncfs(fsp);
#endif
    }

  return (EP_OK);
}


/* sfs_free_inode -
 *
 */
W
sfs_free_inode (struct fs *fsp, W inode_index)
{
#ifdef notdef
  struct sfs_inode	ipbuf;
  W rlength, errno;
#else
  W cn;
  B *buf;
#endif
  
#ifdef notdef
  bzero ((B *) &ipbuf, sizeof (struct sfs_inode));
  errno = sfs_write_device (fsp->fs_device,
			    (B*) &ipbuf,
			    sfs_get_inode_offset (fsp, inode_index),
			    sizeof (struct sfs_inode),
			    &rlength);
  if (errno) {
    return (EP_IO);
  }
#else
  sfs_get_cache(fsp->fs_device,
		sfs_get_inode_offset (fsp, inode_index)/SFS_BLOCK_SIZE,
		&cn, &buf);
  bzero(buf, sizeof(struct sfs_inode));
  sfs_put_cache(cn, 1);
#endif

  fsp->fs_freeinode++;
  fsp->fs_usedinode--;
  if (fsp->fs_isearch >= inode_index) fsp->fs_isearch = inode_index;
  /* ここで fs の sync を行う必要があるか? */
  sfs_syncfs(fsp);
  return (EP_OK);
}
