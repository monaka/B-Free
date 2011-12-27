/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include "../init.h"
#include "dosfs.h"


/*
 * ファイルの読み取り (cat コマンド)
 */
void
cat_file (struct dos_fsinfo *fs, char *path)
{
  struct dos_direntry  dir, *dirp, *old_dirp;
  int	i;
  char	*rest;
  char	fname[14];
  int	count;
  
  printf ("list_dir: path = %s\n", path);
  fix_path (path);	/* パス名を DOS の制限に合わせて正規化する */
  if (strcmp (path, "/") == 0)
    {
      print_dir (DOS_ROOTDIR (fs), DOS_ROOTDIRENTRY (fs));
      return;
    }

  if (*path == '/')
    {
      path++;
    }

  dirp = DOS_ROOTDIR(fs);
  rest = path;
  while (*rest != '\0')
    {
      printf ("path: %s  ", rest);
      rest = get_next_path (rest, fname);
      if (*rest == '\0')
	{
	  print_file (fs, dirp, fname);
	}
      else
	{
	  old_dirp = dirp;
	  dirp = read_dir (fs, old_dirp, fname, &count);
	  free (old_dirp);
	}
      if (dirp == NULL)
	{
	  return;
	}
      printf ("fname = %s, rest = %.3s\n", fname, (*rest) ? rest : "END");
    }
}


/*
 * 引数で指定したディレクトリの中にあるこれまた引数で指定した
 * ファイルの内容を表示する。
 */
print_file (struct dos_fsinfo *fs, struct dos_direntry *dir, char *fname)
{
  int	i;
  char	*buf;

  printf ("cat_file: cat = %s\n", fname);
  for (i = 0; dir[i].name[0] != 0; i++)
    {
      unsigned char	dentry[14];
      unsigned char	*p;

      /* ファイル名を正規化して比較 */
      p = strchr (dir[i].name, ' ');
      if (p)
	*p = '\0';
      p = strchr (dir[i].ext, ' ');
      if (p)
	*p = '\0';
      if (strchr (fname, '.') == NULL)
	{
	  bzero (dentry, 14);
	  strncpy (dentry, dir[i].name, 8);
	}
      else
	{
#ifdef __BOOT__	  
	  int	index;

	  bzero (dentry, 14);
	  index = strncpy (dentry, dir[i].name, 8);
	  strncpy (&dentry[index], dir[i].ext, 3);
#else
	  sprintf (dentry, "%.8s.%.3s", dir[i].name, dir[i].ext);
#endif
	}
#ifdef DEBUG
      printf ("dir[%d]: %s (%s), fname: %s\n", i, dir[i].name, dentry, fname);
#endif /* DEBUG */
      if (strncmp (dentry, fname, 14) == 0)
	break;
    }

  if (dir[i].name[0] == '\0')
    {
      printf ("file not found.\n");
      return -1;
    }

  buf = (char *)malloc (DOS_FSIZE (&dir[i]));
  if (buf == NULL)
    {
      printf ("Can not malloc.\n");
      return -1;
    }

  printf ("cat: fname = %-8s.%-3s\tsize = %d\n",
	  dir[i].name, dir[i].ext, DOS_FSIZE (&dir[i]));

  printf ("readed size = %d\n", 
	  dos_read (fs, dir[i].fatent, DOS_FSIZE (&dir[i]), buf));
  put_line (buf, DOS_FSIZE (&dir[i]));
  free (buf);
  return 0;
}

put_line (char *buf, int size)
{
  int	i;

  for (i = 0; i < size; i++)
    {
      putchar (buf[i]);
    }
  return (i);
}
     
