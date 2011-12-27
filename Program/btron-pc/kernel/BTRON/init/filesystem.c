/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

/* $Id: filesystem.c,v 1.1 2011/12/27 17:13:35 liu1 Exp $
 */

/*
 * �ե����륷���ƥ�δ�����Ԥ�����δؿ���
 *
 * �ʲ��δؿ���������Ƥ��롣
 *
 * init_filesystem ...... �ե����륷���ƥ�����Τ���ν����
 * attach ............... �ե����륷���ƥ�Υޥ����
 * dettach .............. �ե����륷���ƥ�Υ���ޥ����
 * get_root ............. �ե����륷���ƥ�� root �ǥ��쥯�ȥ�μ���
 * lookup ............... ���ꤷ���ե����������������
 * get_attribute ........ �ե�����ξ�������
 * get_directory_list ... �ǥ��쥯�ȥ���Υ���ȥ�������������
 * read ................. �ե���������Ƥ��ɤ߹���
 * write ................ �ե���������Ƥν񤭹���
 * truncate ............. �ե����륵�����ν���
 * make_directory ....... �ǥ��쥯�ȥ�κ���
 * remove_directory ..... �ǥ��쥯�ȥ�κ��
 *
 */

/*
 * $Log: filesystem.c,v $
 * Revision 1.1  2011/12/27 17:13:35  liu1
 * Initial Version.
 *
 * Revision 1.1  1996-07-25 16:02:00  night
 * IBM PC ���Ѥؤκǽ����Ͽ
 *
 */

#include "init.h"


extern struct file_ops	dosfs_ops;

struct filesystem_entry filesystem_table[] =
{
  { "dosfs", &dosfs_ops},	/* DOS �ե����륷���ƥ�Τ���Υ���ȥ� */
};

W	nfilesystem;


/*
 * �ե����륷���ƥ�δ����ѥǡ���
 */
struct filesystem	*root_fs;
struct filedata		*rootdir;
struct filedata		*current_dir;


/* init_filesystem �ե����륷���ƥ�����Τ���ν����
 *
 * root_device	root �ե����륷���ƥ�Ȥʤ�ǥХ���
 * type		�ե����륷���ƥ�μ���
 * root		(�֤���) �ե����륷���ƥ�� root �ǥ��쥯�ȥ�ξ���
 */
ER
init_filesystem (ID root_device, UW part, B *type)
{
  W	i;
  ER	error;

  nfilesystem = sizeof (filesystem_table) / sizeof (struct filesystem_entry);
  for (i = 0; i < nfilesystem; i++)
    {
      if (strcmp (type, filesystem_table[i].fsname) == 0)
	{
	  break;
	}
    }
  if (i >= nfilesystem)
    {
      return (E_OBJ);	/* �����˳�������ե����륷���ƥ�Ϥʤ� */
    }

  
  root_fs = (struct filesystem *)malloc (sizeof (struct filesystem));
  root_fs->fs_type = &(filesystem_table[i]);
  root_fs->mount_file = NULL;
  root_fs->private = NULL;

  error = FOP_ATTACH (root_fs, root_device, part);
  root_fs->device = root_device;
  root_fs->dd = part;

  if (error != E_OK)
    {
      return (error);
    }

/*
  rootdir = (struct filedata *)malloc (sizeof (struct filedata));
  if (rootdir == NULL)
    {
      free (root_fs);
      root_fs = NULL;
      return (E_NOMEM);
    }
*/

  error = FOP_GETROOT(root_fs, &rootdir);
  if (error != E_OK)
    {
      printf ("init_filesystem: cannot getrootdir\n");
      return (error);
    }

/*  printf ("rootdir = 0x%x\n", rootdir); */
  current_dir = rootdir;
  return (E_OK);
}

 
ER
attach (struct filedata *parent, ID device, UW dd)
{
  ER error;

  error = FOP_ATTACH (parent->fs, device, dd);
  return (error);
}


/* get_root ............. �ե����륷���ƥ�� root �ǥ��쥯�ȥ�μ���
 *
 *
 */
ER
get_root (struct filesystem *fs, struct filedata **fp)
{
  return (FOP_GETROOT (fs, fp));
}


B *
fentry_copy (B *fname, B *bp)
{
  while (*fname == '/')
    fname++;

  while ((*fname != '/') && (*fname != '\0'))
    *bp++ = *fname++;

  *bp = '\0';
  return (fname);
}

/* lookup ............... ���ꤷ���ե����������������
 *
 *
 */
ER
lookup (struct filedata *parent, B *fname, struct filedata **ret_fp)
{
  struct filedata *fp;
  B		  bp[MAXFILENAME];
  ER		  error;
  struct filedata	*tmp;

  if (*fname == '\0')
    {
      (*ret_fp) = parent;
      return (E_OK);
    }

  tmp = parent;
  while (*fname != '\0')
    {
      fname = fentry_copy (fname, bp);
      printf ("name: %s\n", fname);	/* */

      printf ("lookup: call lookup for dos. parent = 0x%x, bp = 0x%x, fp = 0x%x\n",
	      parent, bp, &fp);
      error = FOP_LOOKUP (parent, bp, &fp);
      if (error != E_OK)
	{
	  return (error);
	}

      if (parent != tmp)
	free (parent);
      parent = fp;
    }
  
  (*ret_fp) = fp;
  printf ("lookup: return\n");	/* */
  return (E_OK);
}


/* get_attribute ........ �ե�����ξ�������
 *
 *
 */
ER
get_attribute (struct filedata *fp, struct fstat *ret_st)
{
#ifdef notdef
  printf ("get_attribute: %s, %d\n", __FILE__, __LINE__);
  printf ("0x%x, 0x%x\n", fp, ret_st);
  printf ("fp = 0x%x\n", fp);
  printf ("fp->fs = 0x%x\n", fp->fs);
  printf ("fp->fs->fs_type = 0x%x\n", fp->fs->fs_type);
  printf ("fp->fs->fs_type->fop = 0x%x\n", fp->fs->fs_type->fop);
  printf ("fp->fs->fs_type->fop->getattr = 0x%x\n", fp->fs->fs_type->fop->getattr);
#endif
  return (FOP_GETATTR (fp, ret_st));
}


/* get_directory_list ... �ǥ��쥯�ȥ���Υ���ȥ�������������
 *
 *
 */
ER
get_directory_list (struct filedata *dir, struct directory *ret)
{
  return (FOP_GETDIRECTORY (dir, ret));
}


/* truncate ............. �ե����륵�����ν���
 *
 *
 */

/* read ................. �ե���������Ƥ��ɤ߹���
 *
 *
 */
ER
read (struct filedata *fp, W offset, W size, B *buf)
{
  return (FOP_READ (fp, offset, size, buf));
}


/* write ................ �ե���������Ƥν񤭹���
 *
 *
 */
ER
write (struct filedata *fp, W offset, W size, B *buf)
{
  return (FOP_WRITE (fp, offset, size, buf));
}


/* make_directory ....... �ǥ��쥯�ȥ�κ���
 *
 *
 */

/* remove_directory ..... �ǥ��쥯�ȥ�κ��
 *
 *
 */

