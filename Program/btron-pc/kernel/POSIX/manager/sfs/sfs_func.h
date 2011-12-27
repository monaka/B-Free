/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/


#ifndef __SFS_FUNC_H__
#define __SFS_FUNC_H__	1

/* common definitions */
#define SFS_BLOCK_SIZE 512


/* sfs_block.c */
extern W sfs_read_block (ID device, W blockno, W blocksize, B *buf);
extern W sfs_write_block (ID device, W blockno, W blocksize, B *buf);
extern W sfs_alloc_block (W fd, struct fs *fsp);
extern W sfs_get_block_num (W fd, struct fs *fsp,
			    struct sfs_inode *ip, W blockno);
extern W sfs_get_indirect_block_num (W fd, struct fs *fsp,
				     struct sfs_inode *ip, W blockno);
extern W sfs_get_dindirect_block_num (W fd, struct fs *fsp,
				      struct sfs_inode *ip, W blockno);
extern W sfs_set_block_num (W fd, struct fs *fsp,
	       struct sfs_inode *ip, W blockno, W newblock);
extern W sfs_set_indirect_block_num (W fd, struct fs *fsp, 
				     struct sfs_inode *ip, W blockno, W newblock);
extern W sfs_set_dindirect_block_num (W fd, struct fs *fsp,
				      struct sfs_inode *ip, W blockno, W newblock);
extern W sfs_free_block (W fd, struct fs *fsp, W blockno);
extern W sfs_free_indirect(W fd, struct fs *fsp, struct sfs_inode *ip,
			   int offset, int inblock);
extern W sfs_free_dindirect(W fd, struct fs *fsp, struct sfs_inode *ip,
			    int offset, int dinblock, int inblock);
extern W sfs_free_all_dindirect(W fd, struct fs *fsp,
				struct sfs_inode *ip, int inblock);

/* sfs_device.c */
extern W sfs_write_device (ID device, B *buf, W start, W length, W *rlength);
extern W sfs_read_device (ID device, B *buf, W start, W length, W *rlength);
extern W sfs_trans_device (ID device, ID port, ID tskid, B *buf, W start, W length);

/* sfs_dir.c */
extern W sfs_read_dir (struct inode *ip, W nentry, struct sfs_dir *dirp);
extern W sfs_write_dir (struct inode *parentp, W nentry, struct sfs_dir *dirp);
extern W sfs_getdents (struct inode *ip, ID caller, W offset, VP buf,
		       UW length, W *rsize, W *fsize);

/* sfs_inode.c */
extern W	sfs_get_inode_offset (struct fs *fsp, W ino);
extern W	sfs_read_inode (struct fs *fsp, W index, struct inode *ip);
extern W	sfs_alloc_inode (ID fd, struct fs *fsp);
extern W	sfs_write_inode (W fd, struct fs *fsp, struct sfs_inode *ip);
extern W	sfs_free_inode (struct fs *fsp, W inode_index);


/* sfs_file.c */
/* extern W	sfs_i_open (); */
extern W	sfs_i_lookup (struct inode *parent, char *fname, W oflag,
			      W mode, struct access_info *acc,
			      struct inode **retip);
extern W	sfs_i_create (struct inode *parent, char *fname, W oflag,
			      W mode, struct access_info *acc,
			      struct inode **retip);
extern W	sfs_i_close ();
extern W	sfs_i_read (struct inode *ip, W start, B *buf, W length, W *rlength);
extern W	sfs_i_write (struct inode *ip, W start, B *buf, W size,
			     W *rsize);
extern W	sfs_i_stat ();
extern W	sfs_i_truncate (struct inode *ip, W newsize);
extern W	sfs_i_link ();
extern W	sfs_i_unlink (struct inode *parent, char *fname,
			      struct access_info *acc);
extern W	sfs_i_symlink ();
extern W	sfs_i_chmod ();
extern W	sfs_i_chown ();
extern W	sfs_i_chgrp ();
extern W	sfs_i_rename ();
extern W	sfs_i_sync (struct inode *ip);
extern W	sfs_i_mkdir (struct inode *parent, char *fname, W mode,
			     struct access_info *acc, struct inode **retip);
extern W	sfs_i_rmdir (struct inode *parent, char *fname,
			     struct access_info *acc);

/* sfs_fs.c */
extern W sfs_mountroot (ID device, struct fs *rootfsp, struct inode *rootfile);
extern W sfs_syncfs (struct fs *fsp);

/* sfs_bitmap.c */
void sfs_init_bitmap_cache(void);
W sfs_alloc_bitmap(W fd, W blockno, B **ptr, int **dirty);
W sfs_sync_bitmap(W fd);

/* sfs_cache.c */
void sfs_init_cache(void);
void sfs_get_cache(W fd, W blockno, W *cn, B **ptr);
void sfs_put_cache(W cn, W dirty);
W sfs_sync_cache(W fd);

#endif /* __SFS_FUNC_H__ */

