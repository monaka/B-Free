/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* sfs_fs.c - SFS のファイルシステム全体の処理
 *
 *
 * $Log: sfs_fs.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.23  2000-07-02 04:14:24  naniwa
 * to implement disk block cache
 *
 * Revision 1.22  2000/06/01 08:47:25  naniwa
 * to implement getdents
 *
 * Revision 1.21  2000/05/06 03:53:18  naniwa
 * implement mkdir/rmdir, etc.
 *
 * Revision 1.20  1999/12/19 11:03:20  naniwa
 * minor fix of debug message
 *
 * Revision 1.19  1999/11/19 10:10:54  naniwa
 * add debug message
 *
 * Revision 1.18  1999/07/09 08:13:45  naniwa
 * modified to cache bitmap block
 *
 * Revision 1.17  1999/05/28 15:48:36  naniwa
 * sfs ver 1.1
 *
 * Revision 1.16  1999/04/13 04:15:28  monaka
 * MAJOR FIXcvs commit -m 'MAJOR FIX!!! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.'! There are so many changes, modifys, fixes. Sorry but I can't remember all of those. For example, all the manager and driver programmer have got power to access all ITRON systemcall. (My works is just making access route to ITRON. I don't know what happens in the nuclus.
 *
 * Revision 1.15  1999/03/24 04:52:18  monaka
 * Source file cleaning for avoid warnings.
 *
 * Revision 1.14  1997/10/11 16:25:22  night
 * ファイルの write 処理の修正。
 *
 * Revision 1.13  1997/08/31 13:37:05  night
 * root file system の設定時、Magic ID が異なっていた時にエラーを
 * 返すように変更した。
 *
 * Revision 1.12  1997/07/06 11:57:53  night
 * デバッグ文の修正。
 *
 * Revision 1.11  1997/07/03 14:24:36  night
 * mountroot/open 処理のバグを修正。
 *
 * Revision 1.10  1997/07/02 13:26:39  night
 * sfs_mountroot の引数が間違っていたので、修正した
 *
 * Revision 1.9  1997/05/08 15:13:08  night
 * sfs_mountroot() 実行時に、rootfile->i_ops に対して sfs_iops を
 * 設定する処理を有効にした。
 *
 * Revision 1.8  1997/04/28 15:29:53  night
 * デバッグ用の文を追加。
 *
 * Revision 1.7  1997/04/25 13:02:36  night
 * mountroot 実行時にファイルシステム情報の設定処理を追加した。
 *
 * Revision 1.6  1997/04/24 15:43:34  night
 * sfs_mountroot の実行時に正常に終了した場合には EP_OK のエラーコードを
 * 返すように修正。
 *
 * Revision 1.5  1996/11/20  12:13:19  night
 * 細々とした修正。
 *
 * Revision 1.4  1996/11/18  13:46:11  night
 * ファイルの中身を作成。
 *
 * Revision 1.3  1996/11/17  14:51:24  night
 * rcsid の追加。
 *
 * Revision 1.2  1996/11/07  21:12:11  night
 * 文字コードを EUC に変更した。
 *
 * Revision 1.1  1996/11/07  12:50:56  night
 * 最初の登録
 *
 */

static char rcsid[] = "$Id: sfs_fs.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $";

#include "../posix.h"
#include "sfs_func.h"



struct fsops sfs_fsops =
{
  sfs_mount,
  sfs_mountroot,
  sfs_umount,
  sfs_statfs,
  sfs_syncfs,
  NULL,
  NULL,
  NULL,
  NULL,
  sfs_get_inode,
  sfs_put_inode,
};


struct iops sfs_iops = 
{
  sfs_i_lookup,
  sfs_i_create,
  sfs_i_close,
  sfs_i_read,
  sfs_i_write,
  sfs_i_stat,
  sfs_i_truncate,
  sfs_i_link,
  sfs_i_unlink,
  sfs_i_symlink,
  sfs_i_chmod,
  sfs_i_chown,
  sfs_i_chgrp,
  sfs_i_rename,
  sfs_i_sync,
  sfs_i_mkdir,
  sfs_i_rmdir,
  sfs_getdents
};




/* sfs_mount -
 *
 */
W
sfs_mount ()
{
  return (EP_NOSUP);
}


/* sfs_mountroot -
 *
 */
W
sfs_mountroot (ID		device,
	       struct fs	*rootfsp,
	       struct inode	*rootfile)
{
  struct sfs_superblock	sfs_sb;
  W			rlength;
  W			errno;


#ifdef FMDEBUG
  printk ("sfs: mountroot, device = %x\n", device);
#endif
  sfs_init_cache();

  errno = sfs_read_device (device, (B *)&sfs_sb, 512, sizeof (struct sfs_superblock), &rlength);
  if (errno)
    {
#ifdef FMDEBUG
      printk ("Cannot read from device.\n");
#endif
      return (EP_IO);
    }

#ifdef FMDEBUG
  printk ("rootfsp = 0x%x\n", rootfsp);
#endif

  if (sfs_sb.sfs_magic != SFS_MAGIC)
    {
      printk("ERROR: mountroot: magic number %x\n", sfs_sb.sfs_magic);
      return (EP_INVAL);
    }

  rootfsp->fs_typeid =    FS_SFS;
  rootfsp->fs_blksize =   sfs_sb.sfs_blocksize;
  rootfsp->fs_allblock =  sfs_sb.sfs_nblock;
  rootfsp->fs_freeblock = sfs_sb.sfs_freeblock;
  rootfsp->fs_usedblock = sfs_sb.sfs_nblock - sfs_sb.sfs_freeblock;
  rootfsp->fs_allinode =  sfs_sb.sfs_ninode;
  rootfsp->fs_freeinode = sfs_sb.sfs_freeinode;
  rootfsp->fs_usedinode = sfs_sb.sfs_ninode - sfs_sb.sfs_freeinode;
  rootfsp->fs_isearch = sfs_sb.sfs_isearch;
  rootfsp->fs_bsearch = sfs_sb.sfs_bsearch;
  rootfsp->fs_device    = device;
  rootfsp->fs_private.sfs_fs = sfs_sb;

#ifdef FMDEBUG
  /* ファイルシステム情報の出力 ; for FMDEBUG
   */
  printk ("sfs_sb: blocksize = %d\n", rootfsp->fs_blksize);
  printk ("sfs_sb: allblock  = %d\n", rootfsp->fs_allblock);
  printk ("sfs_sb: allinode  = %d\n", rootfsp->fs_allinode);
  printk ("sfs_sb: version  = %d.%d\n", rootfsp->fs_private.sfs_fs.sfs_version_hi, rootfsp->fs_private.sfs_fs.sfs_version_lo);
#endif

  rootfile->i_ops = (struct iops *)&sfs_iops;
  errno = sfs_read_inode (rootfsp, 1, rootfile);		/* root file の読み込み、inode = 1 が root file */
  if (errno)
    {
#ifdef FMDEBUG
      printk ("sfs_mountroot: error = %d\n", errno);
#endif
      return (errno);
    }

#ifdef FMDEBUG
  printk ("root file inode:\n");
  printk ("  UID/GID: %d/%d\n", rootfile->i_uid, rootfile->i_gid);
  printk ("  size:    %d bytes\n", rootfile->i_size);
#endif  

  return (EP_OK);
}


/* sfs_umount -
 *
 */
W
sfs_umount ()
{
  return (EP_NOSUP);
}


/* sfs_statfs -
 *
 */
W
sfs_statfs ()
{
  return (EP_NOSUP);
}


/* sfs_syncfs -
 *
 */
W
sfs_syncfs (struct fs *fsp)
{
  W	errno;
  W	rsize;
  struct sfs_superblock *sb;
  
  fsp->fs_private.sfs_fs.sfs_freeblock = fsp->fs_freeblock;
  fsp->fs_private.sfs_fs.sfs_freeinode = fsp->fs_freeinode;
  fsp->fs_private.sfs_fs.sfs_isearch = fsp->fs_isearch;
  fsp->fs_private.sfs_fs.sfs_bsearch = fsp->fs_bsearch;
  sb = &(fsp->fs_private.sfs_fs);
  errno = sfs_write_device (fsp->fs_device, (B*)sb, 1 * sb->sfs_blocksize,
		    sizeof (struct sfs_superblock), &rsize);
  if (errno) {
    return(errno);
  }
#ifdef notdef
  errno = sfs_sync_bitmap(fsp->fs_device);
  if (errno) {
    return(errno);
  }
#endif
  errno = sfs_sync_cache(fsp->fs_device);
  if (errno) {
    return(errno);
  }
  return (EP_OK);
}


/* sfs_get_inode -
 *
 */
W
sfs_get_inode ()
{
  return (EP_NOSUP);
}


/* sfs_put_inode -
 *
 */
W
sfs_put_inode ()
{
  return (EP_NOSUP);
}






