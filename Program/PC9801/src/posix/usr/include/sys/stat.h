/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
/* @(#)$Header: /cvsroot/bfree-info/B-Free/Program/PC9801/src/posix/usr/include/sys/stat.h,v 1.1 2011/12/27 17:13:36 liu1 Exp $ */

/*
 * $Log: stat.h,v $
 * Revision 1.1  2011/12/27 17:13:36  liu1
 * Initial Version.
 *
 * Revision 1.2  1995-09-21 15:52:02  night
 * ソースファイルの先頭に Copyright notice 情報を追加。
 *
 * Revision 1.1  1995/08/27  10:51:26  night
 * 最初の登録
 *
 *
 */

/*
 *
 *
 */

#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__	1

#include <sys/types.h>
#include <time.h>

struct stat
{
  mode_t	st_mode;
  ino_t		st_ino;
  dev_t		st_dev;
  nlink_t	st_nlink;
  uid_t		st_uid;
  gid_t		st_gid;
  off_t		st_size;
  time_t	st_atime;
  time_t	st_mtime;
  time_t	st_ctime;
};

/*
 * ファイルタイプ/モードに関するマクロ定義
 *
 * ファイルのタイプは、struct stat の st_mode によって示される。
 *
 * st_mode の形式は次のようになっている。
 *
 *
 * |        File Type          |   S bit   |   OWNER   |   GROUP   |  OTHERS   |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * |   |   |   |   |   |   |   | SU| SG| SO| R | W | X | R | W | X | R | W | X |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *   |   |   |   |   |   |   |__ Regular File
 *   |   |   |   |   |   |______ Directory  
 *   |   |   |   |   |__________ Character Device
 *   |   |   |   |______________ Block Device
 *   |   |   |__________________ Named Fifo 
 *   |   |______________________ Symbolic File (Unused)
 *   |__________________________ Socket (Unused)
 */

/*
 * ファイルタイプを示すマクロ
 */
#define	S_IFMT		01770000
#define S_IFREG         00010000
#define	S_IFDIR		00020000
#define S_IFCHR         00040000
#define S_IFBLK         00100000
#define S_IFIFO         00200000
#define S_IFLNK         00400000
#define S_IFSOCK        01000000


#define S_ISUID		04000		
#define S_IRWXU		00700
#define S_IRUSR		00400
#define S_IWUSR		00200
#define S_IXUSR		00100

#define S_ISGID		02000
#define S_IRWXG		00070
#define S_IRGRP		00040
#define S_IWGRP		00020
#define S_IXGRP		00010

#define S_IRWXO		00007
#define S_IROTH		00004
#define S_IWOTH		00002
#define S_IXOTH		00001

/*
 * ファイルを識別するためのマクロ
 *
 */
#define S_ISREG(m)	((m & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	((m & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	((m & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	((m & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	((m & S_IFMT) == S_IFFIFO)
#define S_ISLINK(m)	((m & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m)	((m & S_IFMT) == S_IFSOCK)


/*
 * このディレクトリ関係の関数の定義
 *
 */
extern int     chmod (const char *path, mode_t mode);
extern int     mkdir (const char *path, mode_t mode);
extern int     stat (const char *path, struct stat *buf);
extern int     fstat (int fildes, struct stat *buf);
extern int     mkfifo (const char *path, mode_t mode);
extern mode_t  umask (mode_t cmask);


#endif /* __SYS_STAT_H__ */
