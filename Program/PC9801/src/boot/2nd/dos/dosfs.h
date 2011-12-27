/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/*
 *
 */

#ifndef __DOSFS_H__
#define __DOSFS_H__	1

struct dos_fsinfo
{
  int			device;
  unsigned char		magicid;
  unsigned short	blocksize;
  unsigned short	iplsize;
  unsigned short	fatsize;
  unsigned short	fatcount;
  unsigned short	rootdirsize;
  unsigned short	rootdirentry;
  unsigned short	clustersize;
  unsigned short	*fattable;
  struct dos_direntry	*rootdir;
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


#define SUCCESS		1
#define FAILURE		0

#define DOS_ATTR_NORMAL		0x20
#define DOS_ATTR_READONLY	0x01
#define DOS_ATTR_HIDDEN		0x02
#define DOS_ATTR_SYSFILE	0x04
#define DOS_ATTR_VOLUME		0x08
#define DOS_ATTR_DIR		0x10


#define IS_NORMAL(dir)		(((dir).attr) & DOS_ATTR_NORMAL)
#define IS_READONLY(dir)	(((dir).attr) & DOS_ATTR_READONLY)
#define IS_HIDDEN(dir)		(((dir).attr) & DOS_ATTR_HIDDEN)
#define IS_SYSFILE(dir)		(((dir).attr) & DOS_ATTR_SYSFILE)
#define IS_VOLUME(dir)		(((dir).attr) & DOS_ATTR_VOLUME)
#define IS_DIR(dir)		(((dir).attr) & DOS_ATTR_DIR)

#define LAST_FATID	(0x0fff)
#define BAD_FATID	(0x0ff7)
#define FREE_FATID	(0x000)
#define IS_LAST_FATID(id)	((id >= 0x0ff7) && (id <= 0x0fff))

#define min(a,b)	((a) > (b) ? b : a)

#define DOS_FSIZE(dir)		(((dir)->fsize[2] << 24) | ((dir)->fsize[2] << 16) | ((dir)->fsize[1] << 8 ) | (dir)->fsize[0])
#define DOS_IPLSIZE(fs)	(fs->iplsize * fs->blocksize)
#define DOS_FATENTRY(fs)	\
        ((fs->fatsize * fs->blocksize) * 2 / 3)
#define DOS_FATSIZE(fs)	(fs->fatsize * fs->blocksize)
#define DOS_ROOTDIR(fs)	(fs->rootdir)
#define DOS_ROOTDIROFFSET(fs) \
	(fs->iplsize * fs->blocksize)  \
	+ ((fs->fatsize * fs->blocksize) * fs->fatcount)
#define DOS_DIRENTRYSIZE	(32)
#define DOS_DIRENTRY		(fs->clustersize / DOS_DIRENTRYSIZE)
#define DOS_ROOTDIRSIZE(fs) (fs->rootdirsize)
#define DOS_ROOTDIRENTRY(fs)   (fs->rootdirentry)
#define DOS_FAT_TO_BLOCK(fs,fat)	(dos_fat_to_block(fs, fat))
#define DOS_CLUSTER_TO_BLOCK(fs,cls)	(dos_cluster_to_block(fs,cls))
#define DOS_BLOCK_TO_BYTE(fs,block)	(fs->blocksize * block)
#define DOS_CLUSTER_TO_BYTE(fs,cls)	(fs->clustersize * cls)

extern void   list_dir (struct dos_fsinfo *fs, char *path);
extern void   init_fattable ();
extern void   init_rootdir (struct dos_fsinfo *fs);
extern struct dos_direntry *get_direntry (struct dos_direntry *dir, int entry);
extern struct dos_direntry *read_dir (struct dos_fsinfo *fs, struct dos_direntry *dir, char *fname, int *);

extern char  *get_next_path (char *path, char *entry);
extern void   make_fattable (unsigned char *fat, unsigned short *fattable, int entry);

extern void    cat_file (struct dos_fsinfo *fs, char *path);

extern int	init_filesystem (struct dos_fsinfo *fs, char *devname);
extern int	read_device (int device, int offset, int size, void *buf);

#endif /* __DOSFS_H__ */
