/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: file.h,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */

/*
 *
 */

/*
 * $Log: file.h,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.4  1999-02-17 08:31:01  monaka
 * Minor fix again for comment to avoid warning.
 *
 * Revision 1.3  1998/12/12 05:40:34  monaka
 * Minor fix for comment to avoid warning.
 *
 * Revision 1.2  1998/05/23 15:30:10  night
 * enum の最後の要素から ',' を削除した。
 * (ANSI の文法に従った)
 *
 * Revision 1.1  1996/07/25 16:02:00  night
 * IBM PC 版用への最初の登録
 *
 */

#ifndef __FILE_H__
#define __FILE_H__	1

enum file_type
{
  invalid,
  regular,
  directory,
  char_dev,
  block_dev
};


struct filesystem_entry
{
  B			fsname[MAXFSNAME + 1];
  struct file_ops	*fop;
};


/* マウントポイント毎にもつファイルシステムの情報
 */
struct filesystem
{
  struct filesystem_entry	*fs_type;
  struct filedata		*rootdir;
  struct filedata		*mount_file;
  ID				device;		/* デバイス ID */
  UW				dd;		/* デバイス内の位置を示す情報 */
  void				*private;
  UW				ref_count;
  struct filedata		*opentable;
};



struct filedata
{
  struct filedata	*next;
  struct filesystem	*fs;
  struct filesystem	*child_fs;	/* このディレクトリがマウントポイントの時、
					 * マウントしたファイルシステムを示す.
					 */
  enum file_type	type;
  UW			ref_count;
  void			*private;

  UW			ctime;
  UW			mtime;
  UW			atime;
  UW			size;
};

struct fstat
{
  enum file_type	type;
  UW			ctime;
  UW			mtime;
  UW			atime;

  UW			size;
};

struct direntry
{
  B			fname[MAXFILENAME];
};


struct directory
{
  W			nentry;
  struct direntry	entry[MAXDIRENTRY];
};

struct file_ops
{
  ER	(*init)(void);
  ER	(*attach)(struct filesystem *, ID, UW);
  ER	(*dettach)(struct filesystem *);
  ER	(*getroot)(struct filesystem *, struct filedata **);
  ER	(*lookup)(struct filedata *, B *, struct filedata **);
  ER	(*getattr)(struct filedata *, struct fstat *);
  ER	(*getdirentry)(struct filedata *, struct directory *dir);
  ER	(*getdirsize)(struct filedata *, W *size);
  ER	(*read)(struct filedata *fp, W offset, W size, B *buf);
  ER	(*write)(struct filedata *fp, W offset, W size, B *buf);
  /* ...
   */
};




#define FOP_ATTACH(fs,dev,part)		(fs->fs_type->fop->attach)(fs,dev,part)
#define FOP_DETTACH(fs,dev,part)	(fs->fs_type->fop->attach)(fs,dev,part)
#define FOP_GETROOT(fs,ret)		(fs->fs_type->fop->getroot)(fs,ret)

#define FOP_LOOKUP(dir,fname,ret)	(dir->fs->fs_type->fop->lookup)(parent,fname,ret)
#define FOP_GETATTR(fp,ret)		(fp->fs->fs_type->fop->getattr)(fp,ret)
#define FOP_GETDIRECTORY(fp,ret)	(fp->fs->fs_type->fop->getdirentry)(fp,ret)
#define FOP_GETDIRSIZE(fp,ret)		(fp->fs->fs_type->fop->getdirsize)(fp,ret)
#define FOP_READ(fp,offset,size,buf)	(fp->fs->fs_type->fop->read)(fp,offset,size,buf)
#define FOP_WRITE(fp,offset,size,buf)	(fp->fs->fs_type->fop->write)(fp,offset,size,buf)

/*
 * ファイルシステムの管理用データ
 */
extern struct filesystem	*root_fs;
extern struct filedata		*rootdir;
extern struct filedata		*current_dir;


#endif /* __FILE_H__ */
