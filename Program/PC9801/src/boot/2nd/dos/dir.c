/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include "../types.h"
#include "dosfs.h"


/*
 * root ディレクトリの読み取り
 */
void
init_rootdir (struct dos_fsinfo *fs)
{
  fs->rootdir = (struct dos_direntry *)malloc (DOS_ROOTDIRSIZE (fs));
  if (fs->rootdir == NULL)
    {
      printf ("No more memory!\n");
      return;
    }

#ifdef __BOOT__
  read_device (fs->device, DOS_ROOTDIROFFSET(fs), DOS_ROOTDIRSIZE (fs), (fs->rootdir));
#else
  lseek (fs->device, DOS_ROOTDIROFFSET(fs), SEEK_SET);
  read (fs->device, fs->rootdir, DOS_ROOTDIRSIZE (fs));
#endif __BOOT__

#ifdef notdef
  printf ("DOS_ROOTDIROFFSET = %d\n", DOS_ROOTDIROFFSET(fs));
  printf ("DOS_ROOTDIRSIZE   = %d\n", DOS_ROOTDIRSIZE (fs));
  {
    int	i;
    struct dos_direntry *dir;
    char	buf[9];

#ifdef notdef
    for (i = 0; i < DOS_ROOTDIRENTRY(fs); i++)
#else
    for (i = 0; i < 5; i++)
#endif /* notdef */
      {
	dir = get_direntry (DOS_ROOTDIR(fs), i);
	printf ("%d 0x%x\n", i, dir);
	if ((dir->name[0] != 0) && (dir->name[0] != 0xe5))
	  {
#ifdef __BOOT__
	    bzero (buf, sizeof (buf));
	    strncpy (buf, dir->name, 8);
	    printf ("%s.", buf);
	    bzero (buf, sizeof (buf));
	    strncpy (buf, dir->ext, 8);
	    printf ("%s.", buf);
#else
	    printf ("%8.8s.%3.3s\n", dir->name, dir->ext);
#endif /* __BOOT__ */
	  }
      }
  }
#endif /* notdef */  
}

/*
 * ディレクトリ情報の特定のエントリを読み取る
 */
struct dos_direntry *
get_direntry (struct dos_direntry *dir, int entry)
{
  return (&(dir[entry]));
}


/*
 * ディレクトリの情報を読み出す。
 *
 */
struct dos_direntry *
read_dir (struct dos_fsinfo *fs, struct dos_direntry *dir, char *fname, int *count)
{
  int			i;
  struct dos_direntry	*ret_dir;
  int			dirsize;

#ifdef DEBUG
  printf ("read_dir: fatid = %d\n", dir->fatent);
#endif /* DEBUG */

  for (i = 0; dir[i].name[0] != 0; i++)
    {
      unsigned char	dentry[14];
      unsigned char	*p;

      /* ファイル名を正規化して比較 */
      p = strnchr (dir[i].name, ' ', 8);
      if (p)
	{
	  *p = '\0';
	}
      p = strchr (dir[i].ext, ' ');
      if (p)
	{
	*p = '\0';
      }
#ifdef DEBUG
      printf ("read_dir: i = %d, dir[%d].name = <%s>\n", i, i, dir[i].name);
#endif /* DEBUG */

      if (strchr (fname, '.') == NULL)
	{
#ifdef __BOOT__
	  strncpy_with_key (dentry, dir[i].name, 8, ' ');
#else
	  sprintf (dentry, "%s", dir[i].name);
#endif /* __BOOT__ */
	}
      else
	{
#ifdef __BOOT__
	  int	index;

	  index = strncpy (dentry, dir[i].name, 8);
	  if (dir[i].ext[0] != ' ')
	    strncpy (&dentry[index], dir[i].ext, 3);
#else
	  sprintf (dentry, "%.8s.%.3s", dir[i].name, dir[i].ext);
#endif /* __BOOT__ */
	}

      if (strncmp (dentry, fname, 14) == 0)
	{
	  printf ("matched: %s\n", dir[i].name);
	  break;
	}
    }

#ifdef DEBUG
  printf ("read_dir: loopout (line = %d)\n", __LINE__);
#endif /* DEBUG */

  if (dir[i].name[0] == 0)
    {
      *count = 0;
      return (NULL);
    }

  dir = &(dir[i]);
  if (!IS_DIR(*dir))
    {
      printf ("read_dir: not directory.\n");
      return (0);
    }

#ifdef DEBUG
  printf ("dir->name = %s, dir->fatent = %d\n", dir->name, dir->fatent);
#endif /* DEBUG */

  /* FAT エントリのリストを辿って、FAT エントリのカウント数を返す。
   * FAT エントリのリストは 0x0fff で終了するので、そこまで辿ることになる。
   */
  dirsize = DOS_CLUSTER_TO_BYTE (fs, fat_list_count (fs, dir->fatent));
  *count = dirsize / DOS_DIRENTRYSIZE;

  printf ("dirsize = %d\n", dirsize);
  if (dirsize > 0)
    {
#ifdef DEBUG
      printf ("dir->fatent = %d, dirsize = %d\n", ret_dir->fatent, dirsize);
#endif
      ret_dir = (struct dos_direntry *)malloc (dirsize);
      if (ret_dir == NULL)
	{
	  printf ("read_dir: Can not get more memory!\n");
	  return (0);
	}
      bzero (ret_dir, dirsize);
      dos_read (fs, dir->fatent, dirsize, ret_dir);	/* ディレクトリ情報の読み取り */

      return (ret_dir);
    }
  return (NULL);
}

