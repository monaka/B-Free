/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include "dosfs.h"
#include "../types.h"
#include "../file.h"

/*
 * ファイルシステムパラメータの型紙
 */
struct dos_fs_paramater
{
  char	*devname;		/* デバイス名 */
  int	magicid;		/* マジックナンバ */
  int	blocksize;		/* ブロックサイズ */
  int	iplsize;		/* IPL のブロック数 */
  int	fatsize;		/* FAT のブロック数 */
  int	fatcount;		/* FAT の数 */
  int	rootdirsize;		/* root ディレクトリのサイズ(バイト) */
  int	rootdirentry;		/* root ディレクトリに含まれるエントリ数 */
  int	clustersize;		/* クラスタサイズ (バイト) */
};

static struct dos_fs_paramater paramater[] =
{
  { "0",  0xf9, 512, 1, 7, 2, 7168, 224, 512},
  { "1",  0xf9, 512, 1, 3, 2, 3584, 112, 1024},
  { NULL, 0,    0,   0, 0, 0, 0,    0,      0}
};


/*
 * ファイルシステム情報の初期化
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

  /* ファイルシステム情報の作成 */
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
