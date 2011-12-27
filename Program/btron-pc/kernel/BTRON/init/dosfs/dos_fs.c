/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include "../init.h"
#include "dosfs.h"

/*
 * DOS ファイルシステムと上位層とのインタフェースをとる関数群を定義
 * しているファイル。
 */

/*
 * $Log: dos_fs.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.2  1998-12-14 02:27:00  monaka
 * Minor fix.
 *
 * Revision 1.1  1996/07/25 16:03:30  night
 * IBM PC 版への最初の登録
 *
 */


extern ER  dos_init ();
extern ER  dos_attach ();
extern ER  dos_dettach ();
extern ER  dos_getroot ();
extern ER  dos_lookup  ();
extern ER  dos_getattr ();
extern ER  dos_getdirentry ();
extern ER  dos_getdirsize ();


struct file_ops dosfs_ops =
{
  dos_init,
  dos_attach,
  dos_dettach,
  dos_getroot,
  dos_lookup,
  dos_getattr,
  dos_getdirentry,
  dos_getdirsize,
};


/*
 * dos_init ...... DOS ファイルシステム全体の初期化
 */
ER
dos_init ()
{
  /* 特に何もしない */
  return (E_OK);
}

/*
 * dos_attach ..... DOS ファイルシステムのマウント
 */
ER
dos_attach (struct filesystem *fs, ID dev, UW dd)
{
  struct dos_fs_paramater	*param;
  UB				buf[512];

  param = (struct dos_fs_paramater *)malloc (sizeof (struct dos_fs_paramater));
  if (param == NULL)
    {
      return (E_NOMEM);
    }

  read_device (dev, 0, dd, 512, buf);
  fs->private = param;
  param->secsize = (buf[12] << 8) | buf[11];
  param->cluster_sec  = buf[13];
  param->iplsize  = (buf[15] << 8) | buf[14];
  param->fatcount  = buf[16];
  param->rootdirsize  = (buf[18] << 8) | buf[17];
  param->sec_disk = (buf[20] << 8) | buf[19];
  param->magicid = buf[21];
  param->fatsize  = (buf[23] << 8) | buf[22];
  param->track_sec = (buf[25] << 8) | buf[24];
  param->head_disk = (buf[27] << 8) | buf[26];
  param->reserve_sec = (buf[29] << 8) | buf[28];
  param->clustersize = param->cluster_sec * param->secsize;

#ifdef DEBUG
  printf ("type:    %d\n", param->magicid);
  printf ("cls/sec: %d\n", param->cluster_sec);
  printf ("cls/byte: %d\n", param->clustersize);
  printf ("sec/byte:%d\n", param->secsize);
  printf ("bootsec: %d\n", param->iplsize);
  printf ("nfat:    %d\n", param->fatcount);
  printf ("rootsize:%d\n", param->rootdirsize);
  printf ("fat/sec: %d\n", param->fatsize);
  printf ("head:    %d\n", param->head_disk);
  printf ("nsector: %d\n", param->sec_disk);
  printf ("sec/track:%d\n",param->track_sec);
  printf ("reserve  %d\n", param->reserve_sec);
#endif

  init_fattable (fs);
  init_rootdir (fs);

#ifdef DEBUG
  printf ("dos_attach: fs->rootsize = %d\n", fs->rootdir->size);
  printf ("init_dosfs: done.\n");		/* */
#endif
  return (E_OK);
}

/*
 * dos_dettach ...  DOS ファイルシステムのアンマウント
 */
ER
dos_dettach (struct filesystem *fs)
{
  return (E_OK);
}


ER
dos_getroot (struct filesystem *fs, struct filedata **fp)
{
  *fp = fs->rootdir;
  return (E_OK);
}

ER
dos_lookup (struct filedata *dir, B *fname, struct filedata **ret_fp)
{
  struct dos_direntry *buf;
  struct dos_file     *dosfile;
  W	i;

#ifdef notdef
  printf ("dos_lookup start, %d\n", __LINE__);
#endif
  if (dir->type != directory)
    return (E_OBJ);

  if (*fname == '\0')
    {
      (*ret_fp) = dir;
#ifdef notdef
      printf ("dos_lookup return (%d)\n", __LINE__);
#endif
      return (E_OK);
    }

  buf = (struct dos_direntry *)malloc ((dir->size / DOS_DIRENTRYSIZE) * sizeof (struct dos_direntry));
  if (buf == NULL)
    {
      return (E_NOMEM);
    }

#ifdef notdef
  printf ("call dos_dirread\n");
#endif
  if (dos_dirread (dir, (dir->size / DOS_DIRENTRYSIZE), buf) != E_OK)
    {
      return (E_OBJ);
    }

  for (i = 0; i < ((dir->size) / DOS_DIRENTRYSIZE); i++)
    {
      if ((buf[i].name[0] == '\0') || (buf[i].name[0] == ' '))
	break;
      if (fname_cmp (fname, &buf[i]) == 0)
	{
	  printf ("found file. index = %d, name = %s\n",
		  i, fname);

	  /* ファイルが見つかった */

	  (*ret_fp) = (struct filedata *)malloc (sizeof (struct filedata));
	  bzero ((*ret_fp), sizeof (struct filedata));
	  (*ret_fp)->fs = dir->fs;

	  dosfile = (struct dos_file *)malloc (sizeof (struct dos_file));
	  bzero (dosfile, sizeof (struct dos_file));

	  (struct dos_file *)(*ret_fp)->private = dosfile;
	  dosfile->fatid = buf[i].fatent;

	  /* ...日付情報の設定等を行う... */
	  if (buf[i].attr & 0x20)
	    {
	      (*ret_fp)->type = regular;
	    }
	  else if (buf[i].attr & 0x10)
	    {
	      (*ret_fp)->type = directory;
	    }
	  else
	    {
	      (*ret_fp)->type = invalid;
	    }

	  (*ret_fp)->size = (buf[i].fsize[3] << 24) | 
                            (buf[i].fsize[2] << 16) | 
			    (buf[i].fsize[1] << 8) | 
			    (buf[i].fsize[0]);

	  (*ret_fp)->ref_count = 1;

	  free (buf);
	  printf  ("dos_lookup: return\n");
	  return (E_OK);
	}
    }

#ifdef notdef
  printf  ("dos_lookup: return\n");
#endif
  return (E_NOSPT);
}

W
fname_cmp (B *fname, struct dos_direntry *dir)
{
  B	entry_name[8 + 3 + 2];
  W	i,j;

  for (i = 0, j = 0; i < 8; i++)
    {
      if ((dir->name[i] == ' ') || (dir->name[i] == '\0'))
	{
	  break;
	}
      entry_name[j++] = dir->name[i];
    }

  if ((dir->ext[0] != '\0') || (dir->ext[0] != ' '))
    {
      entry_name[j++] = '.';
    }

  for (i = 0; i < 3; i++)
    {
      if ((dir->ext[i] == ' ') || (dir->ext[i] == '\0'))
	{
	  break;
	}
      entry_name[j++] = dir->ext[i];
    }
  entry_name[j] = '\0';

  printf ("fname_cmp: strncmp (%s, %s)\n", entry_name, fname);	/* */
  if (strncmp (entry_name, fname, 8 + 3 + 1) == 0)
    return (0);

  return (1);
}

ER
dos_dirread (struct filedata *dir, W nentry, struct dos_direntry *dirbuf)
{
  UB	*buf, *p;
  W	i;
  struct dos_fs_paramater	*dosfs;
  ER	error;

#ifdef notdef
  printf ("dos_dirread: %s, %d, dir->fs->private = 0x%x\n",
	  __FILE__, __LINE__, dir->fs->private);
#endif

  dosfs = (struct dos_fs_paramater *)(dir->fs->private);

#ifdef notdef
  printf ("dos_dirread: malloc (%d)\n", 
	  ROUNDUP (nentry * DOS_DIRENTRYSIZE, dosfs->clustersize));
  printf ("dos_dirread: %s, %d\n", __FILE__, __LINE__);
  printf ("dos_dirread: nentry = %d, dosfs->clustersize = %d, %s, %d\n", 
	  nentry, dosfs->clustersize, __FILE__, __LINE__);
#endif

  buf = (UB *)malloc (ROUNDUP (nentry * DOS_DIRENTRYSIZE, dosfs->clustersize));
  if (buf == NULL)
    {
      return (E_NOMEM);
    }

  error = dos_read (dir, 
		    0, 
		    ROUNDUP (nentry * DOS_DIRENTRYSIZE, dosfs->clustersize), 
		    buf);
  if (error != E_OK)
    {
      free (buf);
      return (error);
    }

  p = buf;
  for (i = 0; i < nentry; i++, p += DOS_DIRENTRYSIZE)
    {
      bcopy (&p[0],  dirbuf[i].name, 8);
      bcopy (&p[8], dirbuf[i].ext,  3);
      dirbuf[i].attr = p[11];
      dirbuf[i].mtime[0] = p[22];
      dirbuf[i].mtime[1] = p[23];
      dirbuf[i].mdate[0] = p[24];
      dirbuf[i].mdate[1] = p[25];
      dirbuf[i].fatent   = (p[26] << 8) | p[27];
      dirbuf[i].fsize[0] = p[28];
      dirbuf[i].fsize[1] = p[29];
      dirbuf[i].fsize[2] = p[30];
      dirbuf[i].fsize[3] = p[31];
    }

  free (buf);
  return (E_OK);
}


ER
dos_read_rootdir (struct filedata *fp, W offset, W size, UB *buf)
{
  ER	error;
  struct dos_fs_paramater	*dosfs;

#ifdef notdef
  printf ("dos_read_rootdir: start\n");
#endif

  dosfs = (struct dos_fs_paramater *)(fp->fs->private);
#ifdef notdef
  printf ("device = %d, offset = %d, dd = %d, size = %d, buf = 0x%x\n",
	  fp->fs->device, DOS_ROOTDIROFFSET(dosfs), fp->fs->dd, size, buf);
#endif
  error = read_device (fp->fs->device, DOS_ROOTDIROFFSET(dosfs), fp->fs->dd, size, (void *)buf);
  return (error);
}


ER
dos_read (struct filedata *fp, W offset, W size, UB *buf)
{
  W				fatid;
  W				total;
  ER				error;
  struct filesystem		*fs = fp->fs;
  struct dos_fs_paramater	*dosfs = (struct dos_fs_paramater *)fs->private;
  struct dos_file		*dfile = (struct dos_file *)fp->private;


#ifdef notdef
  printf ("dos_read: %s, %d\n", __FILE__, __LINE__);
#endif

  if (fp == (fp->fs->rootdir))
    {
      return (dos_read_rootdir (fp, offset, size, buf));
    }

  total = 0;
  for (fatid = dfile->fatid;
       total <= size;
       fatid = dosfs->fattable[fatid], total += dosfs->clustersize)
    {
      if ((fatid & (0xffff)) == 0xffff)
	{
	  return (E_PAR);
	}

#ifdef notdef
      printf ("dosread: offset = %d, size = %d\n",
	      dos_cluster_to_byte (dosfs, fatid),
	      dosfs->clustersize);
#endif

      error = read_device (fs->device,
			   dos_cluster_to_byte (dosfs, fatid),
			   fs->dd,
			   dosfs->clustersize,
			   &buf[total]);
      if (error != E_OK)
	return (error);
    }

  return (E_OK);
}


ER
dos_getattr (struct filedata *fp, struct fstat *ret)
{
  if (ret == NULL)
    return E_PAR;

  ret->type = fp->type;
  ret->size = fp->size;
  ret->ctime = ret->mtime = ret->atime = fp->ctime;
  return (E_OK);
}


ER
dos_getdirentry (struct filedata *fp, struct directory *dir)
{
  struct dos_direntry	*p;
  W			i, j;
  W			k;
  W			livecount;

  printf ("dos_getdirentry: start\n");
  bzero (dir, sizeof (struct directory));
  p = (struct dos_direntry *)malloc (fp->size);
  bzero (p, fp->size);
  dos_dirread (fp, fp->size / DOS_DIRENTRYSIZE, p);
  for (i = 0, livecount = 0; i < fp->size / DOS_DIRENTRYSIZE; i++)
    {
      if (p[i].name[0] == '\0')
	break;

      for (k = 0, j = 0; j < 8; j++, k++)
	{
	  if (p[i].name[j] == ' ')
	    break;
	  dir->entry[i].fname[k] = p[i].name[j];
	}

      dir->entry[i].fname[k] = '.';
      k++;

      for (j = 0; j < 3; j++, k++)
	{
	  if (p[i].ext[j] == ' ')
	    break;
	  dir->entry[i].fname[k] = p[i].ext[j];
	}
      dir->entry[i].fname[k] = '\0';
    }
  dir->nentry = i;
#ifdef notdef
  printf ("dos_getdirentry: end %s, %d\n", __FILE__, __LINE__);
#endif
  free (p);
  return (E_OK);
}


ER
dos_getdirsize (struct filedata *dir, W *size)
{
  struct dos_fs_paramater *dosfs;

  dosfs = (struct dos_fs_paramater *)dir->fs->private;
  *size = (dir->size / DOS_DIRENTRYSIZE);
  *size *= sizeof (struct dos_direntry);
  *size = ROUNDUP ((*size), dosfs->secsize);
  return (E_OK);
}


