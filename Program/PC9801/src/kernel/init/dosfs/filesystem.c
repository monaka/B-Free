/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/
#include "../init.h"
#include "dosfs.h"

static struct dos_fs_paramater paramater[] =
{
  { "fd0a",  0xf9, 512,  1,       7,       2,       7168, 224, 512},
  { "fd0b",  0xf9, 512,  1,       3,       2,       3584, 112, 1024},
  { "scsi",  0xf8, 1024  },
  { NULL, 0,    0,   0, 0, 0, 0,    0,      0}
};

struct boot
{
  UB	ver[8];
  UH	sec_byte;
  UB	cls_sec;
  UH	bootsec;	/* reserve */
  UB	nfat;
  UH	rootsize;
  UH	sec_disk;
  UB	disk_type;
  UH	fat_sec;
  UH	track_sec;
  UH	head_disk;
};

struct dosfs	dosfs_table[1];

/*
 * ファイルシステム情報の初期化
 *
 */
W
init_dosfs (B *devname)
{
  ID fsid;
  W  error;
  UB  buf[512];
  struct boot	bootp;
  W	i, index;
  struct dos_fs_paramater	*param;

  error = find_port (devname, &fsid);
  if (error != E_PORT_OK)
    {
      printf ("cannot find port. error = %d\n", error);
      return (-1);
      /* DO NOT REACHED */
    }

  for (index = 0; index < MAXDOSFS; index++)
    {
      if (dosfs_table[0].port == 0)
	break;
    }
  if (index == MAXDOSFS)
    {
      return (-1);
    }

  strncpy (dosfs_table[index].device, devname, 50);
  dosfs_table[index].index = 0;
  dosfs_table[index].port = fsid;
  param = &(dosfs_table[index].dosparam);

  read_device (fsid, 0, (0x2 << 7) | 1, 512, buf);
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
  dosfs_table[index].dd = ((0x2 << 7) | 1);

  printf ("type:    %d\n", param->magicid);
  printf ("cls/sec: %d\n", param->cluster_sec);
  printf ("sec/byte:%d\n", param->secsize);
  printf ("bootsec: %d\n", param->iplsize);
  printf ("nfat:    %d\n", param->fatcount);
  printf ("rootsize:%d\n", param->rootdirsize);
  printf ("fat/sec: %d\n", param->fatsize);
  printf ("head:    %d\n", param->head_disk);
  printf ("nsector: %d\n", param->sec_disk);
  printf ("sec/track:%d\n",param->track_sec);
  printf ("reserve  %d\n", param->reserve_sec);

  init_fattable (&(dosfs_table[index]));
  init_rootdir (&dosfs_table[index]);
#ifdef notdef  		/* not yet */
#endif

  printf ("init_dosfs: done.\n");		/* */
  return (index);
}

