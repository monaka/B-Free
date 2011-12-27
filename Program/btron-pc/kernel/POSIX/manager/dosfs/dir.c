/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include "../init.h"
#include "dosfs.h"


/*
 * root ディレクトリの読み取り
 */
ER
init_rootdir (struct filesystem *fs)
{
  struct dos_file	*dos;
  struct filedata	*root;
  struct dos_fs_paramater *dosfs = (struct dos_fs_paramater *)(fs->private);

  root = (struct filedata *)malloc (sizeof (struct filedata));
  if (root == NULL)
    {
      printf ("No more memory!\n");
      return (E_NOMEM);
    }

  dos = (struct dos_file *)malloc (sizeof (struct dos_file));
  if (dos == NULL)
    {
      printf ("No more memory!\n");
      free (root);
      return (E_NOMEM);
    }

  dos->fatid = 0;
  root->private = dos;
  root->fs = fs;
  root->child_fs = NULL;
  root->type = directory;
  root->ref_count = 1;
  root->size = DOS_ROOTDIRSIZE (dosfs);

#ifdef notdef
  printf ("DOS_ROOTDIRSIZE (dosfs) = %d, root->fs = 0x%x\n", 
	  DOS_ROOTDIRSIZE (dosfs),
	  root->fs);
#endif
  fs->rootdir = root;

  return (E_OK);
}

/*
 * ディレクトリ情報の特定のエントリを読み取る
 */
struct dos_direntry *
get_direntry (struct dos_direntry *dir, W entry)
{
  return (&(dir[entry]));
}


#ifdef notdef
/*
 * ディレクトリの情報を読み出す。
 *
 */
struct dos_direntry *
read_dir (struct filesystem *fs, struct dos_direntry *dir, char *fname, W *count)
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
	  strncpy_with_key (dentry, dir[i].name, 8, ' ');
	}
      else
	{
	  int	index;

	  index = strncpy (dentry, dir[i].name, 8);
	  if (dir[i].ext[0] != ' ')
	    strncpy (&dentry[index], dir[i].ext, 3);
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

#endif /* notdef */
