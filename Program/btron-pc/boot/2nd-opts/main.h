#ifndef __MAIN_H__
#define __MAIN_H__

extern int load_module (int 			fd,
	     struct sfs_superblock	*sb,
	     struct sfs_inode		*ip,
	     int 			offset,
	     struct module_info		*info, 
	     int			silent);
#endif


