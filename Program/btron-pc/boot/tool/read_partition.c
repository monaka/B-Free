#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <limits.h>
#include <linux/hdreg.h>
#include <errno.h>



struct hd_partition
{
  unsigned char		boot_flag;
  unsigned char		begin[3];
  unsigned char		type;
  unsigned char		end[3];	
  unsigned long		start;
  unsigned long		length;
};


/* HD のパーティション情報を読み込み、出力する。
 * 使いかたは
 * 
 *	コマンド デバイス名
 *
 * デバイス名には、/dev/hda や /dev/hdb などを指定する。
 *
 */
main (int ac, char **av)
{
  int			fd;
  char			buf[512];
  struct hd_partition	*par;
  int			i;
  unsigned int		c, h, s;
  struct hd_geometry	geo;


  if (ac < 2)
    {
      fprintf (stderr, "usage: %s device\n", av[0]);
      exit (0);
    }

  fd = open (av[1], O_RDONLY);
  if (fd < 0)
    {
      perror ("open");
      exit (1);
    }

  if (ioctl (fd, HDIO_GETGEO, &geo) < 0)
    {
      perror ("ioctl (HDIO_GETGEO)");
      close (fd);
      exit (1);
    }

  if (read (fd, buf, sizeof (buf)) != sizeof (buf))
    {
      perror ("read");
      close (fd);
      exit (1);
    }
  close (fd);


  printf ("device: %s\n", av[1]);
  printf ("geometry: C/H/S = %d/%d/%d\n", geo.cylinders, geo.heads, geo.sectors);

  (unsigned char *)par = &(buf[0x1be]);
  for (i = 0; i < 4; i++)
    {
      c = par->begin[2] | ((par->begin[1] & 0xC0) << 2);
      h = par->begin[0];
      s = par->begin[1] & 0x3F;
      printf ("[%d] start = %7d, c/h/s = %4d/%4d/%4d  ", i, par->start, c, h, s);

      c = par->end[2] | ((par->end[1] & 0xC0) << 2);
      h = par->end[0];
      s = par->end[1] & 0x3F;
      printf ("-  c/h/s = %4d/%4d/%4d\n", c, h, s);
      par++;
    }

  exit (0);
}

