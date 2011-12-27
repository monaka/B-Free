/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include <stdio.h>
#include <fcntl.h>
#include "dosfs.h"


/*
 * ディレクトリの読み取り (ls コマンド)
 */
void
list_dir (struct dos_fsinfo *fs, char *path)
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
      old_dirp = dirp;
      dirp = read_dir (fs, old_dirp, fname, &count);
      if (dirp == NULL)
	{
	  return;
	}
      free (old_dirp);
#ifdef notdef
      printf ("fname = %s, rest = %.3s\n", fname, (*rest) ? rest : "END");
#endif /* DEBUG */
    }
  print_dir (dirp, count);
}


print_dir (struct dos_direntry *dir, int entry)
{
  int	i;
  char	fname[15];

  for (i = 0; i < entry; i++)
    {
      if ((dir[i].name[0] != 0) && (dir[i].name[0] != 0xe5))
	{
	  bzero (fname, sizeof (fname));
	  bcopy (dir[i].name, fname, 8);
	  cnv_spc_to_null (fname, 8);
	  if ((dir[i].ext[0] != ' ') && (dir[i].ext[0] != 0x00))
	    {
	      strcat (fname, ".");
	      strncat (fname, dir[i].ext, 3);
	      cnv_spc_to_null (fname, 12);
	    }
#ifdef __BOOT__
	  {
	    char	line[15];

	    bzero (line, sizeof (line));
	    strncpy (line, fname, 14);
	    printf ("%s", line);
	  }
#else
	  printf ("%-14.14s", fname);
#endif /* __BOOT__ */
	  if (IS_DIR(dir[i]))
	    {
	      printf ("   <DIR>   ");
	    }
	  else
	    {
	      printf ("           ");
	    }

	  printf ("\t%d\t%5d\n", dir[i].fatent, DOS_FSIZE(&dir[i]));
	}
    }
}

cnv_spc_to_null(char *s, int max)
{
  int	i;

  for (i = 0; i < max; i++)
    {
      if (*s == ' ')
	{
	  *s = '\0';
	}
      s++;
    }
}

     
