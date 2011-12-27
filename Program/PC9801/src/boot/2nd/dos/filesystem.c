/*

B-Free Project ������ʪ�� GNU Generic PUBLIC LICENSE �˽����ޤ���

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include "dosfs.h"
#include "../types.h"
#include "../file.h"

/*
 * �ե����륷���ƥ�ѥ�᡼���η���
 */
struct dos_fs_paramater
{
  char	*devname;		/* �ǥХ���̾ */
  int	magicid;		/* �ޥ��å��ʥ�� */
  int	blocksize;		/* �֥�å������� */
  int	iplsize;		/* IPL �Υ֥�å��� */
  int	fatsize;		/* FAT �Υ֥�å��� */
  int	fatcount;		/* FAT �ο� */
  int	rootdirsize;		/* root �ǥ��쥯�ȥ�Υ�����(�Х���) */
  int	rootdirentry;		/* root �ǥ��쥯�ȥ�˴ޤޤ�륨��ȥ�� */
  int	clustersize;		/* ���饹�������� (�Х���) */
};

static struct dos_fs_paramater paramater[] =
{
  { "0",  0xf9, 512, 1, 7, 2, 7168, 224, 512},
  { "1",  0xf9, 512, 1, 3, 2, 3584, 112, 1024},
  { NULL, 0,    0,   0, 0, 0, 0,    0,      0}
};


/*
 * �ե����륷���ƥ����ν����
 *
 */
init_filesystem (struct dos_fsinfo *fs, char *devname)
{
  int	i;
  int	fd;

  for (i = 0; paramater[i].devname != NULL; i++)
    {
      if (strcmp (paramater[i].devname, devname) == 0)
	{
	  break;
	}
    }
  if (paramater[i].devname == NULL)
    {
      return (FAILURE);
    }

#ifdef DEBUG
  printf ("init_filesystem: file system type = 0x%x\n", paramater[i].magicid);
#endif /* DEBUG */

  /* �ե����륷���ƥ����κ��� */
  if (strcmp (devname, "0") == 0)
    fs->device = 0;
  else
    fs->device = 1;
  fs->magicid = paramater[i].magicid;
  fs->blocksize = paramater[i].blocksize;
  fs->iplsize = paramater[i].iplsize;
  fs->fatsize = paramater[i].fatsize;
  fs->fatcount = paramater[i].fatcount;
  fs->rootdirsize = paramater[i].rootdirsize;
  fs->rootdirentry = paramater[i].rootdirentry;
  fs->clustersize = paramater[i].clustersize;

  init_fd ();
  fd_reset ();
  init_fattable (fs);
  init_rootdir (fs);

  return (SUCCESS);
}

dos_test_fd ()
{
  static unsigned short	buf[512];
  unsigned char		*buf2;
  int			i;

  read_device (0, 512, 512, buf);
  printf ("buf[256] = 0x%x\n", buf[0]);

  buf2 = (unsigned short *)malloc (512);
  for (i = 0; i < 512; i++)
    {
      buf2[i] = 0xaa;
    }

  printf ("buf2[512] = 0x%x, buf2 = 0x%x\n", buf2[500], &buf2[500]);
  vir_to_phys (0x80010000);
}
