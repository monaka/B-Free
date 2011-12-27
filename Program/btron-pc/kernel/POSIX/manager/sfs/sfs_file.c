/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_file.c - SFS のファイルに関する処理を行う。
 *
 * $Log: sfs_file.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.30  2000-07-02 04:14:23  naniwa
 * to implement disk block cache
 *
 * Revision 1.29  2000/05/20 11:56:24  naniwa
 * to support chdir
 *
 * Revision 1.28  2000/05/06 03:53:17  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.27  2000/02/20 09:38:07  naniwa
 * to work with fs_register_inode
 *
 * Revision 1.26  2000/02/06 09:14:10  naniwa
 * minor fix
 *
 * Revision 1.25  2000/01/28 10:04:43  naniwa
 * minor fix around strncpy, strncmp
 *
 * Revision 1.24  2000/01/26 08:25:07  naniwa
 * minor fix
 *
 * Revision 1.23  2000/01/08 09:12:17  naniwa
 * fixed sfs_i_unlink
 *
 * Revision 1.22  1999/12/26 11:18:21  naniwa
 * to implement unlink
 *
 * Revision 1.21  1999/07/09 08:13:44  naniwa
 * modified to cache bitmap block
 *
 * Revision 1.20  1999/06/01 09:18:58  naniwa
 * modified to make sfs_i_truncate () work
 *
 * Revision 1.19  1999/05/30 04:05:35  naniwa
 * modified to creat file correctly
 *
 * Revision 1.18  1999/05/29 09:53:41  naniwa
 * removed variable 'sb'
 *
 * Revision 1.17  1999/05/28 15:48:35  naniwa
 * sfs ver 1.1
 *
 * Revision 1.16  1999/05/12 17:19:08  night
 * sfs_i_create() の中身を作成。
 *
 * Revision 1.15  1999/05/10 16:07:07  night
 * alloca を使って一時バッファの領域を確保している処理を、alloca の代わり
 * に動的な配列確保 (GCC の拡張機能) を使うように変更。
 *
 * Revision 1.14  1999/04/13 04:15:27  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.13  1999/03/24 04:52:17  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.12  1997/10/11 16:25:21  night
 * ファイルの write 処理の修正。
 *
 * Revision 1.11  1997/09/23 13:54:29  night
 * デバッグ文の追加。
 *
 * Revision 1.10  1997/09/09 13:51:01  night
 * POSIX のファイルへの書き込み処理(システムコールは、write) を追加。
 *
 * Revision 1.9  1997/08/31 13:36:25  night
 * ファイルの sync 時、ファイルが書き換えられていた時のみ中身を書き出すように
 * 処理を変更した。
 * 以前は、常にファイルの中身を書き込むようにしていた。
 *
 * Revision 1.8  1997/07/07 14:42:31  night
 * sfs_i_close 実行時に EP_OK を返すように修正。
 * (ただし、処理としては何もしていない)
 *
 * Revision 1.7  1997/07/07 12:18:04  night
 * sfs_i_sync 関数の中身を追加。
 *
 * Revision 1.6  1997/07/04 15:07:41  night
 * ・スペシャルファイル - デバイスドライバポートの対応表の関連処理の追加。
 * ・ファイルの読み込み処理の改訂。
 *
 * Revision 1.5  1997/07/03 14:24:35  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.4  1996/11/27 15:42:53  night
 * 中身がまだない関数について EP_NOSUP を返すように修正した。
 *
 * Revision 1.3  1996/11/20 12:13:19  night
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

static char rcsid[] = "$Id: sfs_file.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../posix.h"
#include "../posix_fs.h"
#include "../../../ITRON/kernlib/libkernel.h"
#include "sfs_func.h"

#define MIN(x,y)		((x > y) ? y : x)
extern W strncmp(B *, B *, W);



/* sfs_i_lookup - ファイルの探索
 *
 * 引数 parent (親ディレクトリ) の指すディレクトリからファイルを
 * 探索する。
 *
 */
W
sfs_i_lookup (struct inode *parent,
	      char *fname,
	      W oflag,
	      W mode,
	      struct access_info *acc,
	      struct inode **retip)
{
  W			errno;
  ID			device;
  W			nentry;
  W			i;
  extern		void strncpy(B *, B *, W);
#ifdef FMDEBUG
  printk ("sfs_i_lookup: start. fname = %s\n", fname);		/* */
#endif

  device = parent->i_device;

#ifdef notdef
  if (strcmp (fname, "/") == 0)
    {
      *retip = parent;
      return (EP_OK);
    }
  else if (strcmp (fname, ".") == 0)
    {
      *retip = parent;
      return (EP_OK);
    }
#else
  if (strcmp (fname, ".") == 0) {
    *retip = parent;
    (*retip)->i_refcount++;
    return (EP_OK);
  }
#endif

  nentry = sfs_read_dir (parent, 0, NULL);
  {
#ifdef notdef
    struct sfs_dir	dirp[sizeof (struct sfs_dir) * nentry];
    /* GCC の拡張機能を使っている */
#else
    struct sfs_dir	dirp[nentry];
#endif
    errno = sfs_read_dir (parent, nentry, dirp);
    for (i = 0; i < nentry; i++)
      {
	/* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
	if ((W)strncmp (fname, dirp[i].sfs_d_name, SFS_MAXNAMELEN+1) == 0)
	  {
	    break;
	  }
      }
#ifdef FMDEBUG
    printk ("sfs_i_lookup: called sfs_read_dir(). i = %d, nentry = %d\n", i, nentry);
#endif
    if (i >= nentry)
      {
	return (EP_NOENT);
      }

    *retip = fs_check_inode (parent->i_fs, dirp[i].sfs_d_index);
    if (*retip)
      {
	/* すでにオープンしていたファイルだった
	 */
	(*retip)->i_refcount++;
	if ((*retip)->coverfile)
	  {
	    *retip = (*retip)->coverfile;
	  }
	return (EP_OK);
      }
    
    *retip = alloc_inode ();
    if (*retip == NULL)
      {
	return (EP_NOMEM);
      }

    errno = sfs_read_inode (parent->i_fs, dirp[i].sfs_d_index, *retip);
    if (errno)
      {
	return (errno);
      }
    fs_register_inode (*retip);
  }

  return (EP_OK);
}


W
sfs_i_close ()
{
  return (EP_OK);
}



/*
 * ファイルを作成する。
 *
 * 1) 新しい inode をアロケート。
 * 2) 親ディレクトリにアロケートした新しい inode の情報を追加。
 *
 */
W
sfs_i_create (struct inode *parent,
	      char *fname,
	      W oflag,
	      W mode,
	      struct access_info *acc,
	      struct inode **retip)
{
  struct inode		*newip;
  W			errno;
  struct sfs_dir	dirent;
  W			dirnentry;
  W			i_index;


  /* 引数のチェック */
  newip = alloc_inode();
  if (newip == NULL)
    {
      return (EP_NOMEM);
    }
  *retip = newip;

  /* 新しい sfs_inode をアロケート */
  i_index = sfs_alloc_inode (parent->i_device, parent->i_fs);
  if (i_index <= 0)
    {
      return (EP_NOMEM);
    }

  /* 設定 */
  newip->i_fs = parent->i_fs;
  newip->i_device = parent->i_device;
  newip->i_ops = parent->i_ops;
  newip->i_refcount = 1;
  newip->i_dirty = 1;
  newip->i_mode= mode;
  newip->i_link = 1;
  newip->i_index = i_index;
  newip->i_uid = acc->uid;
  newip->i_gid = acc->gid;
  newip->i_dev = parent->i_dev;
  newip->i_size = 0;
  newip->i_atime = 0;
  newip->i_ctime = 0;
  newip->i_mtime = 0;
  newip->i_size_blk = 0;

  fs_register_inode (newip);

  /* ディレクトリのエントリを作成 */
  dirent.sfs_d_index = newip->i_index;
  /* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
  strncpy (dirent.sfs_d_name, fname, SFS_MAXNAMELEN+1);

  /* ディレクトリにエントリを追加 */
  dirnentry = sfs_read_dir (parent, 0, NULL);
  errno = sfs_write_dir (parent, dirnentry, &dirent);
  if (errno != EP_OK)
    {
      return (errno);
    }

  /* inode 情報の更新 */
  /* 本来は inode の deallocate のところで行う処理のはず */
  errno = sfs_i_sync(newip);
  if (errno != EP_OK)
    {
      return (errno);
    }

  return (EP_OK);
}


/* sfs_i_read -
 *
 */
W
sfs_i_read (struct inode	*ip,
	    W start,
	    B *buf,
	    W length,
	    W *rlength)
{
#ifdef notdef
  B			blockbuf[ip->i_fs->fs_private.sfs_fs.sfs_blocksize];	/* GCC の拡張機能を使っている */
#endif
  W			copysize;
  W			offset;
  B			*bufp;
  ID			fd;
  struct fs *fsp;
  W bn;
  W cn;
  B *cbuf;

#ifdef FMDEBUG
  printk ("sfs_i_read: start. ip = 0x%x, start = %d, length = %d, buf = 0x%x\n", ip, start, length, buf);
#endif

  fd = ip->i_device;
  fsp = ip->i_fs;

  if (start + length > ip->i_private.sfs_inode.sfs_i_size)
    {
      length = ip->i_private.sfs_inode.sfs_i_size - start;
    }
  if (length < 0) length = 0;

  *rlength = length;
  bufp = buf;

  while (length > 0)
    {
#ifdef FMDEBUG
      printk ("read block: %d\n",
	      sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
				 start / fsp->fs_blksize));
#endif
      bn = sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
			      start / fsp->fs_blksize);
      if (bn < 0) {
	return (EP_IO);
      }
#ifdef notdef
      sfs_read_block (fd, bn, fsp->fs_blksize, blockbuf);
#else
      sfs_get_cache(fd, bn, &cn, &cbuf);
#endif
      offset = start % fsp->fs_blksize;
      if (fsp->fs_blksize - offset < length)
	{
	  copysize = fsp->fs_blksize - offset;
	}
      else
	{
	  copysize = length;
	}
#ifdef notdef
      bcopy (&blockbuf[offset], buf, copysize);
#else
      bcopy(&cbuf[offset], buf, copysize);
      sfs_put_cache(cn, 0);
#endif
      buf += copysize;
      start += copysize;
      length -= copysize;
    }
  return (EP_OK);
}


W
sfs_i_write (struct inode	*ip,
	     W			start,
	     B			*buf,
	     W			size,
	     W			*rsize)
{
#ifdef notdef
  B			blockbuf[ip->i_fs->fs_private.sfs_fs.sfs_blocksize];	/* GCC の拡張機能を使っている */
#endif
  int			copysize;
  int			offset;
  int			retsize;
  int			filesize;
  ID			fd;
  struct fs *fsp;
  W bn;
  W cn;
  B *cbuf;

#ifdef FMDEBUG
  printk ("sfs_i_write:(start = %d, size = %d)\n", start, size);		/* */
#endif

  *rsize = 0;
  retsize = size;
  filesize = start + retsize;
  fd = ip->i_device;
  fsp = ip->i_fs;

#ifdef FMDEBUG
  printk ("sfs_i_write: size = %d\n", size);					/* */
#endif

  while (size > 0)
    {
#ifdef FMDEBUG
      printk ("%s\n",
	      (sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
				  start / fsp->fs_blksize) <= 0) ?
	      "allocate block" : "read block");
#endif

      if (sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
			     start / fsp->fs_blksize) <= 0)
	{
	  /* ファイルサイズを越えて書き込む場合には、新しくブロックをアロケートする
	   */
	  bn = sfs_set_block_num (fd, fsp, &(ip->i_private.sfs_inode),
				  start / fsp->fs_blksize,
				  sfs_alloc_block (fd, fsp));
/*
 *   ip->sfs_i_direct[start / fsp->fs_blksize] = alloc_block (fd, fsp);
 */
#ifdef notdef
	  bzero (blockbuf, fsp->fs_blksize);
#else
	  if (bn < 0) {
	    return (EP_IO);
	  }
	  sfs_get_cache(fd, bn, &cn, &cbuf);
#endif
	}
      else
	{
#ifdef FMDEBUG
	  printk ("read block %d\n",
		  sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
				     start / fsp->fs_blksize));
#endif
	  bn = sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
				  start / fsp->fs_blksize);
	  if (bn < 0) {
	    return (EP_IO);
	  }
#ifdef notdef
	  sfs_read_block (fd, bn, fsp->fs_blksize, blockbuf);
#else
	  sfs_get_cache(fd, bn, &cn, &cbuf);
#endif
	}

      /* 読み込んだブロックの内容を更新する
       */
      offset = start % fsp->fs_blksize;
      copysize = MIN (fsp->fs_blksize - offset, size);

#ifdef FMDEBUG
      printk ("*** read block contents ***\n");
      {
	int	i;
	char	tmpbuf[2];

	tmpbuf[1] = '\0';
	printk ("copy size: %d\n", copysize);
	for (i = 0; i < copysize; i++)
	  {
	    tmpbuf[0] = blockbuf[i];
	    printk ("%s", tmpbuf);
	  }
      }
#endif

#ifdef notdef
      bcopy (buf, &blockbuf[offset], copysize);
#else
      bcopy(buf, &cbuf[offset], copysize);
#endif

#ifdef notdef
      printk ("sfs write block: block number = %d\n", 
	      sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
				 start / fsp->fs_blksize));		/* */
#endif

      /* 更新したブロックを書き込む
       */
#ifdef notdef
      bn = sfs_get_block_num (fd, fsp, &(ip->i_private.sfs_inode),
			      start / fsp->fs_blksize);
      if (bn < 0) {
	return (EP_IO);
      }
      sfs_write_block (fd, bn, fsp->fs_blksize, blockbuf);
#else
      sfs_put_cache(cn, 1);
#endif

      buf += copysize;
      start += copysize;
      size -= copysize;
    }

  /* cache の sync をここで行う必要はあるか? */
  sfs_sync_cache(fd);
  
  /* もし、書き込みをおこなった後にファイルのサイズが増えていれば、
   * サイズを更新して inode を書き込む。
   */
  if (filesize > ip->i_private.sfs_inode.sfs_i_size)
    {
      ip->i_size = filesize;
      ip->i_size_blk = 
	ROUNDUP(filesize, fsp->fs_blksize)/fsp->fs_blksize;
      ip->i_dirty = 1;
      /* これは deallocate の中で処理するのが普通 */
      sfs_i_sync(ip);
    }
  else if (filesize < ip->i_private.sfs_inode.sfs_i_size) {
    sfs_i_truncate (ip, filesize);
  }
  *rsize = retsize - size;

#ifdef FMDEBUG
  printk ("write size: %d bytes\n", *rsize);
#endif

  return (EP_OK);
}


W
sfs_i_stat ()
{
  return (EP_NOSUP);
}


W
sfs_i_truncate (struct inode *ip, W newsize)
{
  int	nblock, blockno, inblock, offset, dinblock;
  int	i;
  W	errno;
  W	fd;
  struct fs *fsp;
  struct sfs_inode *sfs_ip;

  fd = ip->i_device;
  fsp = ip->i_fs;
  sfs_ip = &(ip->i_private.sfs_inode);
  nblock = ROUNDUP (newsize, fsp->fs_blksize)/fsp->fs_blksize;
  if (nblock < ip->i_size_blk) {
    /* 余分なブロックを開放 */
    blockno = nblock;
    if (blockno < SFS_DIRECT_BLOCK_ENTRY) {
      /* 直接ブロックの範囲内 */
      for(i = blockno; i < SFS_DIRECT_BLOCK_ENTRY; ++i) {
	if (sfs_ip->sfs_i_direct[i] > 0)
	  sfs_free_block(fd, fsp, sfs_ip->sfs_i_direct[i]);
	sfs_ip->sfs_i_direct[i] = 0;
      }
      sfs_free_indirect(fd, fsp, sfs_ip, 0, 0);
      sfs_free_all_dindirect(fd, fsp, sfs_ip, 0);
    }
    else if (blockno < (SFS_DIRECT_BLOCK_ENTRY 
			+ (SFS_INDIRECT_BLOCK_ENTRY * SFS_INDIRECT_BLOCK))) {
      /* 一重間接ブロックの範囲内 */
      inblock = (blockno - SFS_DIRECT_BLOCK_ENTRY);
      offset = inblock % SFS_INDIRECT_BLOCK;
      inblock = inblock / SFS_INDIRECT_BLOCK;
      sfs_free_indirect(fd, fsp, sfs_ip, offset, inblock);
      sfs_free_all_dindirect(fd, fsp, sfs_ip, 0);
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
      sfs_free_dindirect(fd, fsp, sfs_ip, offset, dinblock, inblock);
      sfs_free_all_dindirect(fd, fsp, sfs_ip, inblock+1);
    }
  }

  ip->i_size = newsize;
  ip->i_size_blk = nblock;
  ip->i_dirty = 1;
  /* これは deallocate の中で処理するのが普通 */
  errno = sfs_i_sync(ip);
  if (errno) {
    return(errno);
  }
  /* ここで fs を sync する必要があるか? */
  errno = sfs_syncfs(fsp);
  if (errno) {
    return(errno);
  }
  return (EP_OK);
}


W
sfs_i_link ()
{
  return (EP_NOSUP);
}


W
sfs_i_unlink (struct inode *parent,
	      char *fname,
	      struct access_info *acc)
{
  int	nentry;
  int	i;
  int	inodeindex;
  struct inode *ip;
  W	rsize, errno;
  
  errno = fs_lookup (parent, fname, O_RDWR, 0, acc, &ip);
  if (errno) {
    printk("[PM] sfs_i_unlink: can't unlink file %s(%d)\n", fname, errno);
    return (errno);
  }
  if ((ip->i_mode & SFS_FMT_DIR) != 0) {
    fs_close_file(ip);
    return (EP_ISDIR);
  }
  if (ip->i_refcount >= 2) {
    fs_close_file(ip);
    return (EP_BUSY);
  }

  nentry = sfs_read_dir (parent, 0, NULL);
  if (nentry <= 0) {
    fs_close_file(ip);
    return (EP_NOENT);
  }
  {
    struct sfs_dir buf[nentry]; /* GCC の拡張機能を使っている */
    if (sfs_read_dir (parent, nentry, buf) != 0) {
      fs_close_file(ip);
      return (EP_IO);
    }

    for (i = 0; i < nentry; i++) {
      /* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
      if (strncmp (fname, buf[i].sfs_d_name, SFS_MAXNAMELEN+1) == 0) {
	inodeindex = buf[i].sfs_d_index;
	break;
      }
    }
    if (i >= nentry) {
      fs_close_file(ip);
      return (EP_NOENT);
    }

    while (i < nentry) {
      buf[i].sfs_d_index = buf[i + 1].sfs_d_index;
      /* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
      strncpy (buf[i].sfs_d_name, buf[i + 1].sfs_d_name, SFS_MAXNAMELEN+1);
      i++;
    }
    i = parent->i_size - sizeof (struct sfs_dir);
    sfs_i_write(parent, 0, (B *)buf, i, &rsize);
    sfs_i_truncate (parent, i);

    ip->i_link--;
    ip->i_dirty = 1;
    if (ip->i_link <= 0) {
      sfs_i_truncate (ip, 0);
      sfs_free_inode(ip->i_fs, inodeindex);
    }
  }
  fs_close_file(ip);
  return (EP_OK);
}


W
sfs_i_symlink ()
{
  return (EP_NOSUP);
}


W
sfs_i_chmod ()
{
  return (EP_NOSUP);
}


W
sfs_i_chown ()
{
  return (EP_NOSUP);
}


W
sfs_i_chgrp ()
{
  return (EP_NOSUP);
}


W
sfs_i_rename ()
{
  return (EP_NOSUP);
}


W
sfs_i_sync (struct inode *ip)
{
  W	err;

#ifdef FMDEBUG
  printk ("sfs_i_sync\n");
#endif
  ip->i_private.sfs_inode.sfs_i_index = ip->i_index;
  ip->i_private.sfs_inode.sfs_i_nlink = ip->i_link;
  ip->i_private.sfs_inode.sfs_i_size = ip->i_size;
  ip->i_private.sfs_inode.sfs_i_size_blk = ip->i_size_blk;
  ip->i_private.sfs_inode.sfs_i_perm = ip->i_mode;
  ip->i_private.sfs_inode.sfs_i_uid = ip->i_uid;
  ip->i_private.sfs_inode.sfs_i_gid = ip->i_gid;
  ip->i_private.sfs_inode.sfs_i_dev = ip->i_dev;
  ip->i_private.sfs_inode.sfs_i_atime = ip->i_atime;
  ip->i_private.sfs_inode.sfs_i_ctime = ip->i_ctime;
  ip->i_private.sfs_inode.sfs_i_mtime = ip->i_mtime;
  
  if (ip->i_dirty)
    {
      err = sfs_write_inode (ip->i_device, ip->i_fs,
			     &(ip->i_private.sfs_inode));
      if (err)
	{
	  return (err);
	}
    }
  ip->i_dirty = 0;

#ifdef FMDEBUG
  printk ("sfs_i_sync: done\n");
#endif
  return (EP_OK);
}

/*
 * ディレクトリを作成する。
 *
 * 1) 新しい inode をアロケート。
 * 2) 親ディレクトリにアロケートした新しい inode の情報を追加。
 *
 */
W
sfs_i_mkdir (struct inode *parent,
	     char *fname,
	     W mode,
	     struct access_info *acc,
	     struct inode **retip)
{
  struct inode		*newip;
  W			errno;
  struct sfs_dir	dirent;
  W			dirnentry;
  W			i_index;
  W			rsize;
  static struct sfs_dir	dir[2] =
    {
      { 0, "." },
      { 0, ".." }
    };


  /* 引数のチェック */
  newip = alloc_inode();
  if (newip == NULL)
    {
      return (EP_NOMEM);
    }
  *retip = newip;

  /* 新しい sfs_inode をアロケート */
  i_index = sfs_alloc_inode (parent->i_device, parent->i_fs);
  if (i_index <= 0)
    {
      return (EP_NOMEM);
    }

  /* 設定 */
  newip->i_fs = parent->i_fs;
  newip->i_device = parent->i_device;
  newip->i_ops = parent->i_ops;
  newip->i_refcount = 1;
  newip->i_dirty = 1;
  newip->i_mode= mode | FS_FMT_DIR;
  newip->i_link = 2;
  newip->i_index = i_index;
  newip->i_uid = acc->uid;
  newip->i_gid = acc->gid;
  newip->i_dev = parent->i_dev;
  newip->i_size = 0;
  newip->i_atime = 0;
  newip->i_ctime = 0;
  newip->i_mtime = 0;
  newip->i_size_blk = 0;

  fs_register_inode (newip);

  dir[0].sfs_d_index = i_index;
  dir[1].sfs_d_index = parent->i_index;
  errno = sfs_i_write (newip, 0, (B*)dir, sizeof (dir), &rsize);
  if (errno) {
    return (errno);
  }

  /* ディレクトリのエントリを作成 */
  dirent.sfs_d_index = newip->i_index;
  /* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
  strncpy (dirent.sfs_d_name, fname, SFS_MAXNAMELEN+1);

  /* ディレクトリにエントリを追加 */
  parent->i_link += 1;
  dirnentry = sfs_read_dir (parent, 0, NULL);
  errno = sfs_write_dir (parent, dirnentry, &dirent);
  if (errno != EP_OK) {
    return (errno);
  }

  return (EP_OK);
}

/*
 * ディレクトリを削除する。
 *
 */
W sfs_i_rmdir (struct inode *parent, char *fname,
	       struct access_info *acc)
{
  int	nentry;
  int	i;
  int	inodeindex;
  struct inode *ip;
  W	rsize, errno;
  
  errno = fs_lookup (parent, fname, O_RDWR, 0, acc, &ip);
  if (errno) {
    printk("[PM] sfs_i_rmdir: can't remove directory %s(%d)\n", fname, errno);
    return (errno);
  }
  if ((ip->i_mode & SFS_FMT_DIR) == 0) {
    fs_close_file(ip);
    return (EP_NOTDIR);
  }
  if (ip->i_refcount >= 2) {
    fs_close_file(ip);
    return (EP_BUSY);
  }
  nentry = sfs_read_dir (ip, 0, NULL);
  if (nentry >= 3) {
    fs_close_file(ip);
    return (EP_NOTEMPTY);
  }

  nentry = sfs_read_dir (parent, 0, NULL);
  if (nentry <= 0) {
    fs_close_file(ip);
    return (EP_NOENT);
  }
  {
    struct sfs_dir buf[nentry]; /* GCC の拡張機能を使っている */
    if (sfs_read_dir (parent, nentry, buf) != 0) {
      fs_close_file(ip);
      return (EP_IO);
    }

    for (i = 0; i < nentry; i++) {
      /* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
      if (strncmp (fname, buf[i].sfs_d_name, SFS_MAXNAMELEN+1) == 0) {
	inodeindex = buf[i].sfs_d_index;
	break;
      }
    }
    if (i >= nentry) {
      fs_close_file(ip);
      return (EP_NOENT);
    }

    while (i < nentry) {
      buf[i].sfs_d_index = buf[i + 1].sfs_d_index;
      /* 表示文字長を SFS_MAXNAMELEN にするため．後に pad があるので大丈夫 */
      strncpy (buf[i].sfs_d_name, buf[i + 1].sfs_d_name, SFS_MAXNAMELEN+1);
      i++;
    }
    i = parent->i_size - sizeof (struct sfs_dir);
    sfs_i_write(parent, 0, (B *)buf, i, &rsize);
    sfs_i_truncate (parent, i);

    ip->i_link--;
    ip->i_dirty = 1;
    if (ip->i_link <= 1) {
      sfs_i_truncate (ip, 0);
      sfs_free_inode(ip->i_fs, inodeindex);
    }
    parent->i_link -= 1;
    parent->i_dirty = 1;
  }
  fs_close_file(ip);
  return (EP_OK);
}
