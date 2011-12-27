/*

B-Free Project の生成物は GNU Generic PUBLIC LICENSE に従います。

GNU GENERAL PUBLIC LICENSE
Version 2, June 1991

(C) B-Free Project.

*/

#include "../init.h"
#include "dosfs.h"


static W read_device2 (ID device, W offset, UW dd, W size, void *buf);


/*
 * 指定したクラスタ番号から指定したバイト数分だけ読み込む。
 * もし指定したバイト数がクラスタのサイズを越えていた場合には、
 * 次の FAT エントリを取り出して、先を読み進む。
 *
 */
int
dos_read (struct dosfs *fs, W fatent, W size, B *buf)
{
  /* 引数 fs で指定したデバイスから情報を読み取る */
}



/*
 * 引数で指定した FAT エントリのリストを辿って、
 * いくつのエントリを使用しているかをカウントする。
 */
int
fat_list_count (struct dosfs *fs, int entry)
{
  int	count;

  /*
   * この FAT は使用できない。
   */
  if (next_fatid (fs, entry) == BAD_FATID)
    {
      return (0);
    }

  for (count = 1;
       (!IS_LAST_FATID (next_fatid (fs, entry))) && (next_fatid (fs, entry) != BAD_FATID);
       count++)
    {
      entry = next_fatid (fs, entry);
    }
#ifdef DEBUG
  printf ("fat_list_count: %d\n", count);
#endif /* DEBUG */
  return (count);
}


/*
 *
 */
int
dos_cluster_to_block (struct dosfs *fs, W fat)
{
  int	offset;

  offset =  DOS_ROOTDIROFFSET (fs) + DOS_ROOTDIRSIZE(fs);
  offset += ((fat - 2) * (fs->dosparam.clustersize));
#ifdef DEBUG
  printf ("fat: %d, offset (byte): %d\n", fat, offset);
#endif /* DEBUG */
  return (offset / fs->dosparam.blocksize);
}



/*
 * 指定したデバイスからデータを読み込む
 */
W
read_device (ID device, W offset, UW dd, W size, void *buf)
{
  W	i;
  ER    error;

  for (i = 0; i < size; i += 512)
    {
      error = read_device2 (device, offset + i, dd, 512, buf +i);
      if (error != E_OK)
	break;
    }
  return (error);
}

static W
read_device2 (ID device, W offset, UW dd, W size, void *buf)
{
  DDEV_REQ	req;		/* 要求パケット */
  DDEV_RES	res;		/* 返答パケット */
  ER		error;	
  W		rsize;
  W		i;

/*  printf ("*** READ *** %d\n", device); */
  req.header.mbfid = dev_recv;
  req.header.msgtyp = DEV_REA;
  req.body.rea_req.dd = dd;
  req.body.rea_req.start = offset;
  req.body.rea_req.size = size;
  error = snd_mbf (device, sizeof (req), &req);
  if (error != E_OK)
    {
/*      printf ("cannot send packet. %d\n", error); */
      return (error);
    }
/*  printf ("kernel: snd_mbf\n");	/* */
  rsize = sizeof (res);
  rcv_mbf ((UB *)&res, (INT *)&rsize, dev_recv);
/*  printf ("read_device2: rcv_mbf\n");	/* */
/*  printf ("res.dd      = %d\n", res.body.rea_res.dd); */
/*  printf ("res.error   = %d\n", res.body.rea_res.errcd); */
/*  printf ("res.errinfo = %d\n", res.body.rea_res.errinfo); */
  if (res.body.rea_res.a_size > 0)
    {
/*      printf ("res.a_size  = 0x%x\n",  res.body.rea_res.a_size); */
      bcopy ((B*)res.body.rea_res.dt, buf, size);
    }
#ifdef notdef
  for (i = 0; i < 8; i++)
    {
      printf ("0x%x, ", res.body.rea_res.dt[i]);
    }
#endif
  return (E_OK);
}
