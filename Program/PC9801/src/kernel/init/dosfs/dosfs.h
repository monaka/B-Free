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

#define MAXDOSFS	1


struct dos_direntry
{
  UB			name[8];
  UB			ext[3];
  UB			attr;
  UB			reserve[10];
  UB			mtime[2];
  UB			mdate[2];
  UH			fatent;
  UB			fsize[4];
};

/*
 * ファイルシステムパラメータの型紙
 */
struct dos_fs_paramater
{
  W			magicid;		/* マジックナンバ */
  W			iplsize;		/* IPL のブロック数 */
  W			fatsize;		/* FAT のブロック数 */
  W			fatcount;		/* FAT の数 */
  W			rootdirsize;		/* root ディレクトリのサイズ(バイト) */
  W			rootdirentry;		/* root ディレクトリに含まれるエントリ数 */
  W			secsize;		/* セクタサイズ */
  W			blocksize;		/* ブロックサイズ */
  W			clustersize;		/* クラスタサイズ (バイト) */
  W			cluster_sec;		/* クラスタサイズ (セクタ) */
  W			track_sec;
  W			head_disk;
  W			sec_disk;
  W			reserve_sec;
  UH			*fattable;
  struct dos_direntry	*rootdir;
};

struct dosfs
{
  W				index;
  B				device[50];		/* デバイス名 */
  W				port;
  W				dd;			/* device */
  struct dos_fs_paramater	dosparam;
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
#define DOS_IPLSIZE(fs)	(fs->dosparam.iplsize * fs->dosparam.secsize)
#define DOS_FATENTRY(fs,type)	\
((type == 16) ? \
        (fs->dosparam.fatsize * fs->dosparam.secsize / 2) : \
        (((fs->dosparam.fatsize * fs->dosparam.secsize) * 2) / 3))
#define DOS_FATSIZE(fs)	(fs->dosparam.fatsize * fs->dosparam.secsize)
#define DOS_ROOTDIR(fs)	(fs->dosparam.rootdir)
#define DOS_ROOTDIROFFSET(fs) \
	(fs->dosparam.iplsize * fs->dosparam.secsize)  \
	+ ((fs->dosparam.fatsize * fs->dosparam.secsize) * fs->dosparam.fatcount)
#define DOS_DIRENTRYSIZE	(32)
#define DOS_DIRENTRY		(fs->dosparam.clustersize / DOS_DIRENTRYSIZE)
#define DOS_ROOTDIRSIZE(fs) (fs->dosparam.rootdirsize)
#define DOS_ROOTDIRENTRY(fs)   (fs->dosparam.rootdirentry)
#define DOS_FAT_TO_BLOCK(fs,fat)	(dos_fat_to_block(fs, fat))
#define DOS_CLUSTER_TO_BLOCK(fs,cls)	(dos_cluster_to_block(fs,cls))
#define DOS_BLOCK_TO_BYTE(fs,block)	(fs->dosparam.blocksize * block)
#define DOS_CLUSTER_TO_BYTE(fs,cls)	(fs->dosparam.clustersize * cls)


extern struct dosfs	dosfs_table[MAXDOSFS];


extern void   list_dir (struct dosfs *fs, char *path);
extern void   init_fattable ();
extern void   init_rootdir (struct dosfs *fs);
extern struct dos_direntry *get_direntry (struct dos_direntry *dir, W entry);
extern struct dos_direntry *read_dir (struct dosfs *fs, struct dos_direntry *dir, char *fname, W *);

extern char  *get_next_path (char *path, char *entry);
extern void   make_fat12table (UB *fat, UH *fattable, W entry);
extern void   make_fat16table (UB *fat, UH *fattable, W entry);

extern void    cat_file (struct dosfs *fs, char *path);

extern W	init_filesystem (struct dosfs *fs, char *devname);
extern W	read_device (ID device, W offset, UW dd, W size, void *buf);



#endif /* __DOSFS_H__ */
